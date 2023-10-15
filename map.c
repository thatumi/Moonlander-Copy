#include "map.h"

mapPoint *clear(mapPoint **start){
    mapPoint *iter = *start;
    while (iter != NULL) {
        mapPoint *next = iter->next;
        free(iter);
        iter = next;
    }
}


mapPoint *insert(mapPoint *start, int *landableX, int *landableY){
    mapPoint *landable;
    landable = (mapPoint*) malloc(sizeof(mapPoint));
    landable->x = landableX;
    landable->y = landableY;
    landable->next = NULL;

    if (start == NULL) {
       start = landable;
    } else {
       mapPoint *mover = start;
       while (mover->next != NULL)
          mover = mover->next;
       mover->next = landable;
    }
    return start;
}

mapCoordinates createMap(struct mapCoordinates coordinate, SDL_Renderer *renderer, mapPoint *start) {
    mapCoordinates landable;

    // Generate X and Y coordinates for the new map segment
    landable.x = coordinate.x + rand() % 40;
    landable.y = coordinate.y - rand() % 20;

    // If there's no landing strip, draw one with a certain probability, otherwise, draw to the end
    if (!landable.landable && (rand() % 33) + 1 == 1) {
        landable.y = coordinate.y;
        landable.x = coordinate.x + 40;
        landable.landable = true;
        lineRGBA(renderer, coordinate.x, coordinate.y, landable.x, landable.y, 219, 139, 182, 255);
    } else {
        // If there's no landing strip even at the end, draw one
        if (!landable.landable && coordinate.x > 600) {
            landable.x = 640;
            landable.y = coordinate.y;
            lineRGBA(renderer, coordinate.x, coordinate.y, landable.x, landable.y, 219, 139, 182, 255);
            landable.landable = true;
        } else {
            // Check if the new segment is not too high or too low. If it is, draw in the opposite direction.
            // If it's not too high or too low, draw the next segment with a 50-50 chance.
            if (landable.y < 400) {
                landable.y = coordinate.y + rand() % 20;
            } else if (landable.y > 450) {
                landable.y = coordinate.y - rand() % 20;
            } else {
                if ((rand() % 2) + 1 == 2) {
                    landable.y = coordinate.y + rand() % 20;
                }
            }
            lineRGBA(renderer, coordinate.x, coordinate.y, landable.x, landable.y, 239, 239, 232, 255);
        }
    }

    // Add the new map point to the linked list
    insert(start, landable.x, landable.y);

    return landable;
}

void initializeMap(mapPoint **mapStart) {
    *mapStart = (mapPoint*) malloc(sizeof(mapPoint));
    (*mapStart)->x = 0;
    (*mapStart)->y = 470;
    (*mapStart)->next = NULL;
}

/**
 * @brief Save map points to a text file.
 *
 * This function saves the coordinates of map points from a linked list to a text file.
 * Each map point's X and Y coordinates are written to the file, one below the other.
 *
 * @param start A pointer to a pointer to the start of the linked list of map points.
 */
void mapSave(mapPoint **start){
    FILE* mapFile;
    mapFile = fopen("map.txt", "w");
    if (mapFile != NULL) {
        mapPoint *mover;
        for (mover = *start; mover != NULL; mover = mover->next){
            fprintf(mapFile,"%d\n", mover->x);
            fprintf(mapFile, "%d\n", mover->y);
        }
        fclose(mapFile);
        printf("map saved");
    }
    else {
        perror("File couldn't be opened");
    }
}

/**
 * @brief Load map points from a text file and draw them on the renderer.
 *
 * This function loads map points' coordinates from a text file and draws them on the
 * provided SDL renderer. The map points are connected by lines on the renderer.
 *
 * @param renderer A pointer to the SDL_Renderer where the map points will be drawn.
 * @param start A pointer to a pointer to the start of the linked list of map points.
 */
void mapLoad(SDL_Renderer *renderer,  mapPoint **start){
    clear(*start);
    mapPoint *landable;
    landable = ( mapPoint*) malloc(sizeof( mapPoint));
    landable->x = 0;
    landable->y = 470;
    landable->next = NULL;
    *start = landable;
    SDL_RenderClear(renderer);
    FILE *mapFile;
    mapFile = fopen("map.txt", "r");

    int a = 0;
    int temp = 0;
    int oldX = 0;
    int oldY = 470;
    for(int i = 0; fscanf(mapFile, "%d", &a) == 1; i++) {
        if((i+1) % 2 == 0){
            insert(*start, temp, a);
            if( i >= 2){
                lineRGBA(renderer,  oldX, oldY , temp, a, 239, 239, 232, 255);
            }
            oldY = a;
            oldX = temp;
        }else{
            temp = a;
        }
    }

    fclose(mapFile);
    SDL_RenderPresent(renderer);
}


