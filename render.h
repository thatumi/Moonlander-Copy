#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Render the spaceship at a new position while clearing its previous location.
 *
 * This function clears the previous location of the spaceship by drawing a black box,
 * and then renders the spaceship at its new position using the provided renderer
 * and texture. The spaceship is drawn as a 32x34 rectangle at the specified coordinates.
 *
 * @param renderer   The SDL renderer to use for drawing.
 * @param spaceShip  The SDL texture representing the spaceship image.
 * @param newX       The new X-coordinate of the spaceship.
 * @param newY       The new Y-coordinate of the spaceship.
 * @param prevX      The previous X-coordinate of the spaceship.
 * @param prevY      The previous Y-coordinate of the spaceship.
 */
void spaceship_render(SDL_Renderer *renderer, SDL_Texture *spaceShip, float newX, float newY, float prevX, float prevY);

/**
 * @brief Render text on the screen using a specified font and position.
 *
 * This function renders text on the SDL renderer at the specified coordinates (x, y)
 * using the provided font. The rendered text is obtained from the given text string.
 * The resulting texture is stored in the 'capture_t' pointer for future use.
 *
 * @param renderer   The SDL renderer for rendering the text.
 * @param x          The X-coordinate for the text rendering position.
 * @param y          The Y-coordinate for the text rendering position.
 * @param text       The text to be rendered on the screen.
 * @param font       The TTF font used for rendering the text.
 * @param capture_t  A pointer to an SDL texture where the rendered text will be stored.
 * @param where      A pointer to an SDL rectangle specifying the rendering location and size.
 */
void text_render(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **capture_t, SDL_Rect *where);

/**
 * @brief Render the main menu and wait for user input.
 */
void renderMainMenu(SDL_Renderer *renderer, TTF_Font *fontMenu, SDL_Texture **text_tMenu, SDL_Rect *whereMenu);

/**
 * @brief Render HUD text and convert floats to char.
 *
 * @param renderer - The SDL renderer.
 * @param fontHud - The HUD font.
 * @param fuel - Fuel value.
 * @param thrusters - Thrusters state.
 * @param velocityX - X velocity.
 * @param velocityY - Y velocity.
 * @param spaceshipY - Y coordinate of the spaceship.
 * @param text_tHud - Pointer to the HUD text texture.
 * @param text_tHudValue - Pointer to the HUD value texture.
 * @param whereHud - Pointer to the destination rectangle for HUD text.
 * @param whereHudValue - Pointer to the destination rectangle for HUD value.
 */
void renderHUDText(SDL_Renderer *renderer, TTF_Font *fontHud, float fuel, bool thrusters,
    float velocityX, float velocityY, float spaceshipY,
    SDL_Texture **text_tHud, SDL_Texture **text_tHudValue,
    SDL_Rect *whereHud, SDL_Rect *whereHudValue);

/**
 * @brief Create a starry background for the menu.
 */
void createStarryBackground(SDL_Renderer *renderer);

/**
 * @brief Load the spaceship texture.
 * @param[out] spaceShip - Pointer to the spaceship texture.
 */
void loadSpaceShipTexture(SDL_Renderer *renderer, SDL_Texture **spaceShip);
