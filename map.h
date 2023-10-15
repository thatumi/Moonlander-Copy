#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL2_gfxPrimitives.h>


/**
 * @brief Structure for storing coordinates of points on a map.
 *
 * This structure is used to represent the coordinates of points on a map.
 * It includes the X and Y coordinates, as well as a boolean flag 'landable'
 * which indicates if it's a landing point.
 */
typedef struct mapCoordinates {
    int x;
    int y;
    bool landable; /**< Indicates if this point is a landing point. */
} mapCoordinates;

/**
 * @brief Structure representing a point in a linked list and its associated functions.
 *
 * This structure defines a point in a linked list, which includes the coordinates of the
 * point and a pointer to the next element in the list. It is used for creating linked lists
 * of map points.
 */
typedef struct mapPoint {
    struct mapCoordinates;
    struct mapPoint* next;
} mapPoint;

/**
 * @brief Insert a new map point into a linked list.
 *
 * This function inserts a new map point, defined by its X and Y coordinates, into a linked
 * list of map points. If the list is empty (start is NULL), a new list is created. If the list
 * is not empty, the new point is added to the end of the list.
 *
 * @param start Pointer to the start of the linked list of map points.
 * @param landableX Pointer to the X-coordinate of the new map point.
 * @param landableY Pointer to the Y-coordinate of the new map point.
 *
 * @return Pointer to the updated start of the linked list after insertion.
 */
mapPoint *insert(mapPoint *start, int *landableX, int *landableY);

/**
 * @brief Clear and deallocate memory for a linked list of map points.
 *
 * This function clears and deallocates the memory used by a linked list of map points. It iterates
 * through the list, freeing the memory for each node, and sets the start pointer to NULL.
 *
 * @param start A pointer to a pointer to the start of the linked list of map points.
 *
 * @return NULL to indicate that the linked list has been cleared.
 */
mapPoint *clear(mapPoint **start);

/**
 * @brief Create and draw map segments and landing points.
 *
 * This function creates map segments and landing points for a game. It takes the previous
 * coordinate as input, generates a new map segment, and draws it on the provided SDL renderer.
 * It also updates a linked list of map points to keep track of the map's structure.
 *
 * @param coordinate The previous map coordinate to start from.
 * @param renderer A pointer to the SDL_Renderer where the map segments will be drawn.
 * @param start A pointer to the start of the linked list of map points.
 *
 * @return A structure containing the newly created map coordinate, including X and Y coordinates
 *         and a flag indicating whether it's a landing point.
 */
mapCoordinates createMap(struct mapCoordinates coordinate, SDL_Renderer *renderer, mapPoint *start);

/**
 * @brief Initialize the linked list for map points.
 * @param[out] mapStart - Pointer to the starting point of the map.
 */
void initializeMap(mapPoint **mapStart);

/**
 * @brief Save map points to a text file.
 *
 * This function saves the coordinates of map points from a linked list to a text file.
 * Each map point's X and Y coordinates are written to the file, one below the other.
 *
 * @param start A pointer to a pointer to the start of the linked list of map points.
 */
void mapSave(mapPoint **start);

/**
 * @brief Load map points from a text file and draw them on the renderer.
 *
 * This function loads map points' coordinates from a text file and draws them on the
 * provided SDL renderer. The map points are connected by lines on the renderer.
 *
 * @param renderer A pointer to the SDL_Renderer where the map points will be drawn.
 * @param start A pointer to a pointer to the start of the linked list of map points.
 */
void mapLoad(SDL_Renderer *renderer,  mapPoint **start);
