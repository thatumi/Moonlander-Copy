#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>


/**
 * @brief Initialize the SDL library and create a window and renderer.
 *
 * This function initializes the SDL library, creates a window with the specified
 * dimensions, and a software-based renderer. It assigns the created window and
 * renderer to the provided pointers.
 *
 * @param width The width of the window to be created.
 * @param height The height of the window to be created.
 * @param pwindow A pointer to an SDL_Window pointer to store the created window.
 * @param prenderer A pointer to an SDL_Renderer pointer to store the created renderer.
 *
 * @note If SDL initialization or any of the creation steps fail, the function
 * will log an error message and exit the program with a non-zero status code.
 */
void sdl_init(int width, int height, SDL_Window **pwindow, SDL_Renderer **prenderer);

/**
 * @brief Initialize the SDL window and renderer.
 * @param[out] window - Pointer to the SDL window.
 * @param[out] renderer - Pointer to the SDL renderer.
 */
void initializeSDLWindow(SDL_Window **window, SDL_Renderer **renderer);
