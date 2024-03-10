// main.c

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "vector.h"

#define M_PI_F ((float)(M_PI))
#define M_PI_2_F ((float)(M_PI * 2.0))

#define FPS (60)
#define FRAME_TARGET_TIME (1000 / 60)

// Globals
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
uint32_t* pixels;
int pixels_w, pixels_h;
SDL_Rect window_rect;
bool is_running;
int animation_mode;

// Global state for rendering
uint32_t frame_index;

// Globals for cube
#define CUBE_POINT_COUNT (9*9*9)
vec3_t cube_model[CUBE_POINT_COUNT];
vec2_t projected_points[CUBE_POINT_COUNT];
mat3_t transform_2d;
mat4_t transform_3d;
float angle;

// Momemtum
float pitch, roll, yaw; // how many degrees per second of rotation

// Camera
vec3_t camera_position = { 0.0f, 0.0f, -5.0f };

// Functions

void build_cube_model(void) {
    int i = 0;
    for (float x = -1; x <= 1; x += 0.25f) {
        for (float y = -1; y <= 1; y += 0.25f) {
            for (float z = -1; z <= 1; z += 0.25f) {
                vec3_t pt = { .x = x, .y = y, .z = z };
                cube_model [i++] = pt;
            }
        }
    }
    
    mat3_get_identity(transform_2d);
    mat4_get_identity(transform_3d);
}

vec2_t orthographic_project_point(vec3_t pt3d, float scale2d) {
    // Apply scaling and drop z
    vec2_t pt2d = { .x = pt3d.x * scale2d, .y = pt3d.y * scale2d };

    // Apply 2d matrix transform
    pt2d = vec2_mat3_multiply(pt2d, transform_2d);
    return pt2d;
}

vec2_t perspective_project_point(vec3_t pt3d, float scale2d) {
    // Apply 3d transform
    pt3d = vec3_mat4_multiply(pt3d, transform_3d);

    // Apply camera position
    pt3d = vec3_subtract(pt3d, camera_position);

    vec2_t pt2d = { .x = pt3d.x / pt3d.z, .y = pt3d.y / pt3d.z };

    // Apply 2d transform
    pt2d = vec2_mat3_multiply(pt2d, transform_2d);

    // Scale for screen
    pt2d.x = pt2d.x * scale2d;
    pt2d.y = pt2d.y * scale2d;

    return pt2d;
}

void project_model(void) {
    // Project the 3d model into 2d space
    for (int i = 0; i < CUBE_POINT_COUNT; i++) {
        vec3_t pt3d = cube_model[i];
        //vec2_t pt2d = orthographic_project_point(pt3d, 240);
        vec2_t pt2d = perspective_project_point(pt3d, 640);
        projected_points[i] = pt2d;
    }
}

void clear_screen(uint32_t color) {
    for (int i = 0; i < pixels_w * pixels_h; i++) {
        pixels[i] = color;
    }
}

void set_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= pixels_w) return;
    if (y < 0 || y >= pixels_h) return;
    pixels[x + y * pixels_w] = color;
}

void draw_centered_rect(int x, int y, int w, int h, uint32_t color) {
    int top = y - h / 2;
    int left = x - w / 2;
    for (int y1 = 0; y1 < h; y1++) {
        for (int x1 = 0; x1 < w; x1++) {
            set_pixel(left + x1, top + y1, color);
        }
    }
}

void update_state(void) {
    // Clear frame buffer
    clear_screen(0x000000FF);

    // Time variable
    float t = (float)(frame_index % 480) / 480.0f;

    // Variable for camera movement
    switch (animation_mode) {
    case 1:
        // Translate camera in a circle;
        camera_position.x = sinf(t * M_PI_2_F);
        camera_position.y = cosf(t * M_PI_2_F);
        break;
    case 2:
        // Automatic rotation
        angle = t * M_PI_2_F;
        mat3_get_identity(transform_2d);
        mat3_rotate(transform_2d, angle);
        break;
    default:
        // Static camera, but transform stays the same
        camera_position.x = 0.0f;
        camera_position.y = 0.0f;
        break;
    }

    // Update rotation
    float increment = (M_PI_F / 180.f) / 60.0f; // 1 deg/sec divided by 60 fps
    mat4_pitch(transform_3d, pitch * increment);
    mat4_roll(transform_3d, roll * increment);
    mat4_yaw(transform_3d, yaw * increment);

    // Update frame index
    frame_index++;
}

bool initialize_windowing_system(void) {
    // Set up SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL_Init() failed!\n");
        return false;
    }

    // Full screen
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_rect.x = 0;
    window_rect.y = 0;
    window_rect.w = display_mode.w;
    window_rect.h = display_mode.h;

    // Window
    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_rect.w, window_rect.h, SDL_WINDOW_BORDERLESS);
    // options: SDL_WINDOWPOS_CENTERED, SDL_WINDOW_BORDERLESS
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow() failed!\n");
        return false;
    }

    // Renderer
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer() failed!\n");
        return false;
    }

    // Texture
    pixels_w = window_rect.w;
    pixels_h = window_rect.h;
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING, pixels_w, pixels_h);
    if (!texture) {
        fprintf(stderr, "SDL_CreateTexture() failed!\n");
        return false;
    }

    // Allocate frame buffer
    pixels = (uint32_t*)malloc(pixels_w * pixels_h * sizeof(uint32_t));
    if (!pixels) {
        fprintf(stderr, "malloc() failed!\n");
        return false;
    }
    clear_screen(0x000000FF);

    // Set up the renderer
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0); // Use no interpolation
    // Clear the window
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    // Reset the frame index
    frame_index = 0;

    return true;
}

void clean_up_windowing_system(void) {
    free(pixels);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void process_keyboard_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);


    // Keyboard interaction
    switch (event.type) {
    case SDL_QUIT: // when 'X' button is pressed in window titlebar
        // Exit program
        is_running = false;
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                // Exit program
                is_running = false;
                break;
            case SDLK_0:
                // Reset all state variables and positions
                animation_mode = 0;
                mat3_get_identity(transform_2d);
                mat4_get_identity(transform_3d);
                angle = 0;
                pitch = 0;
                roll = 0;
                yaw = 0;
                break;
            case SDLK_1:
                animation_mode = 1;
                break;
            case SDLK_2:
                animation_mode = 2;
                break;
            case SDLK_e:
                // Roll right
                roll += 1.0f;
                break;
            case SDLK_q:
                // Roll left
                roll -= 1.0f;
                break;
            case SDLK_w:
                // Pitch down
                pitch += 1.0f;
                break;
            case SDLK_s:
                // Pitch up
                pitch -= 1.0f;
                break;
            case SDLK_a:
                // Yaw left
                yaw += 1.0f;
                break;
            case SDLK_d:
                // Yaw right
                yaw -= 1.0f;
                break;
        }
        break;
    }
}

void run_render_pipeline(void) {
    // Draw a 5x5 rect at every projected point
    project_model();
    for (int i = 0; i < CUBE_POINT_COUNT; i++) {
        vec2_t pt = projected_points[i];
        int cx = pixels_w / 2;
        int cy = pixels_h / 2;
        uint32_t color = 0xFFFFFFFF; // white color
        draw_centered_rect(cx + (int)pt.x, cy + (int)pt.y, 5, 5, color);
    }

    // Render frame buffer
    SDL_UpdateTexture(texture, NULL, pixels, pixels_w * sizeof(uint32_t));
    SDL_RenderCopy(renderer, texture, NULL, &window_rect);
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    printf("Started program.\n");

    if (!initialize_windowing_system()) return 0;

    // Create the cube model
    build_cube_model();

    animation_mode = 0;
    angle = 0;
    pitch = 0;
    roll = 0;
    yaw = 0;

    // Game loop
    const bool wasteCycles = false;
    uint32_t startTime, ticks;
    is_running = true;
    while (is_running) {
        // Use SDL_GetTicks() to get how long each frame takes
        // SDL_Ticks_Passed()

        startTime = SDL_GetTicks();
        process_keyboard_input();
        update_state();
        run_render_pipeline();

        if (wasteCycles) {
            while (SDL_GetTicks() - startTime < FRAME_TARGET_TIME) {
                // waste CPU cycles
                // Explanation: Task Manager shows about 24% CPU usage because I have a 4-core CPU,
                // and this code is only a single thread, only one core is running close to 100%.
            }
        } else {
            // Delay to get to target frame time
            ticks = SDL_GetTicks() - startTime;
            if (ticks < FRAME_TARGET_TIME) {
                SDL_Delay(FRAME_TARGET_TIME - ticks);
            }
            // Explanation: Task Manager shows about 14% CPU usage, indicating that the delay is more
            // efficient for CPU usage, but not dramatically so. This code counts the number of ticks
            // that the code takes to do everything in its game loop, and subtracts that from the
            // target frame time, so that the total time between each frame is about the same.
        }
    }

    clean_up_windowing_system();

    return 0;
}
