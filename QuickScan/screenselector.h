#include <SDL.h>
#include <windows.h>
#include <iostream>

typedef struct {
    int x, y, width, height;
} Cordinates;

void MakeWindowTransparent(COLORREF colorKey)
{
    HWND hwnd = GetActiveWindow();
    DWORD dwExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, dwExStyle | WS_EX_LAYERED);

    BYTE alpha = 75;
    SetLayeredWindowAttributes(hwnd, colorKey, alpha, LWA_COLORKEY | LWA_ALPHA);
}

Cordinates getCordinates() {
    // Get resolution of primary monitor
    int desktopWidth = GetSystemMetrics(SM_CXSCREEN);
    int desktopHeight = GetSystemMetrics(SM_CYSCREEN);

    SDL_Window* window = SDL_CreateWindow("SDL Transparent Window",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        desktopWidth, desktopHeight, SDL_WINDOW_BORDERLESS);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Set background color to black and clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw a semi-transparent overlay rectangle on the entire screen
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128); // Przyciemniony kolor (RGBA: 0, 0, 0, 128)
    SDL_Rect overlayRect = { 0, 0, desktopWidth, desktopHeight };
    SDL_RenderFillRect(renderer, &overlayRect);

    // Making background darker
    SDL_Rect rect1 = { 0, 0, desktopWidth, desktopHeight };
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderFillRect(renderer, &rect1);

    SDL_RenderPresent(renderer);

    MakeWindowTransparent(RGB(0, 0, 0));

    // Event loop
    bool quit = false;
    SDL_Event event;

    SDL_Rect drawingRect = { 0, 0, 0, 0 }; // Pocz¹tkowe wartoœci prostok¹ta
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, desktopWidth, desktopHeight);

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                // Pocz¹tek rysowania prostok¹ta
                drawingRect.x = event.button.x;
                drawingRect.y = event.button.y;
            }
            else if (event.type == SDL_MOUSEMOTION && event.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                // Aktualizacja rozmiaru prostok¹ta podczas ruchu myszy
                drawingRect.w = event.motion.x - drawingRect.x;
                drawingRect.h = event.motion.y - drawingRect.y;
            }
            else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                // Koniec rysowania prostok¹ta
                drawingRect.w = event.button.x - drawingRect.x;
                drawingRect.h = event.button.y - drawingRect.y;
                quit = true;
            }
        }

        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderClear(renderer);

        // Renderowanie przyciemnionego t³a
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_RenderFillRect(renderer, &overlayRect);

        // Renderowanie t³a z ciemniejszym kolorem
        SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
        SDL_RenderFillRect(renderer, &rect1);

        // Renderowanie prostok¹ta rysowanego przez mysz
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &drawingRect);

        // Prze³¹czanie na renderowanie na ekranie
        SDL_SetRenderTarget(renderer, NULL);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    // Zniszcz teksturê i inne zasoby
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return Cordinates{ drawingRect.x, drawingRect.y, drawingRect.w, drawingRect.h };
}