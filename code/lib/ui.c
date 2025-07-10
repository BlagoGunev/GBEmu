#include <ui.h>
#include <emu.h>
#include <bus.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
SDL_Texture *sdlTexture;
SDL_Surface *screen;

SDL_Window *sdlDebugWindow;
SDL_Renderer *sdlDebugRenderer;
SDL_Texture *sdlDebugTexture;
SDL_Surface *debugScreen;

static int scale = 6;

void ui_init() {

    SDL_Init(SDL_INIT_VIDEO);
    printf("SDL INIT\n");
    TTF_Init();
    printf("TTF INIT\n");

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &sdlWindow, &sdlRenderer);

    // Debug Window
    SDL_CreateWindowAndRenderer(16 * 8 * scale, 32 * 8 * scale, 0, &sdlDebugWindow, &sdlDebugRenderer);

    debugScreen = SDL_CreateRGBSurface(0, (16 * 8 * scale) + (16 * scale), 
                                        (32 * 8 * scale) + (64 * scale), 32,
                                        0x00FF0000,
                                        0x0000FF00,
                                        0x000000FF,
                                        0xFF000000);

    sdlDebugTexture = SDL_CreateTexture(sdlDebugRenderer, 
                                        SDL_PIXELFORMAT_ABGR8888,
                                        SDL_TEXTUREACCESS_STREAMING,
                                        (16 * 8 * scale) + (16 * scale),
                                        (32 * 8 * scale) + (64 * scale));
    
    int x, y;
    SDL_GetWindowPosition(sdlWindow, &x, &y);
    SDL_SetWindowPosition(sdlDebugWindow, x + SCREEN_WIDTH + 10, y);
}

// Debug palette
static unsigned long tile_colors[4] = { 0xFFFFFFFF, 0xFFAAAAAA, 0xFF555555, 0xFF000000 };

void display_tile(SDL_Surface *surface, u16 startAddress, int tileNum, int x, int y) {
    SDL_Rect rc;

    for (int tileByte = 0; tileByte < 16; tileByte += 2) {
        u8 byte0 = bus_read(startAddress + (tileNum * 16) + tileByte);
        u8 byte1 = bus_read(startAddress + (tileNum * 16) + tileByte + 1);

        for (int pixelShift = 7; pixelShift >= 0; pixelShift--) {
            u8 hi = !!(byte0 & (1 << pixelShift)) << 1;
            u8 lo = !!(byte1 & (1 << pixelShift));

            u8 color = hi | lo;

            rc.x = x + ((7 - pixelShift) * scale);
            rc.y = y + ((tileByte / 2) * scale);
            rc.w = scale;
            rc.h = scale;

            SDL_FillRect(surface, &rc, tile_colors[color]);
        }
    }
}

void update_debug_window() {
    int xDraw = 0;
    int yDraw = 0;
    int tileNum = 0;

    SDL_Rect rc;
    rc.x = 0;
    rc.y = 0;
    rc.w = debugScreen->w;
    rc.h = debugScreen->h;
    SDL_FillRect(debugScreen, &rc, 0xFF111111);

    u16 addr = 0x8000;

    // Total of 384 tiles, 24x16
    for (int y = 0; y < 24; y++) {
        for (int x = 0; x < 16; x++) {
            display_tile(debugScreen, addr, tileNum, xDraw + (x * scale), yDraw + (y * scale));
            xDraw += 8 * scale;
            tileNum++;
        }
        xDraw = 0;
        yDraw += 8 * scale;
    }

    SDL_UpdateTexture(sdlDebugTexture, NULL, debugScreen->pixels, debugScreen->pitch);
    SDL_RenderClear(sdlDebugRenderer);
    SDL_RenderCopy(sdlDebugRenderer, sdlDebugTexture, NULL, NULL);
    SDL_RenderPresent(sdlDebugRenderer);

}

void ui_update() {
    update_debug_window();
}

void ui_handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e) > 0) {
        if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE) {
            emu_get_context()->die = true;
        }
    }
}

void delay(u32 ms) {
    SDL_Delay(ms);
}