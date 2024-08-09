#include "raylib.h"
#include <math.h>

#define ANT_HOR_SPD 100
#define ALPHA_INITIAL_CNT 64
#define BETA_INITIAL_CNT 64
const int screenWidth = 1280;
const int screenHeight = 720;


enum SpeciesType
{
    ALPHA_SPECIES,
    BETA_SPECIES
};

typedef struct 
{
    int type;
    Vector2 position;
    Color color;
    float size;
    int speed;
    int dps;
    int health;
} Species;


void UpdateSpecies(Species allSpecies[], Species closestEnemySpecies[], int speciesCount, float delta)
{
    for (int i = 0; i < speciesCount; i++)
    {   
        if (allSpecies[i].health > 0)
        {
            int currSpeciesX = allSpecies[i].position.x;
            int currSpeciesY = allSpecies[i].position.y;
            int targetSpeciesX = closestEnemySpecies[i].position.x;
            int targetSpeciesY = closestEnemySpecies[i].position.y;
            int distanceX = targetSpeciesX - currSpeciesX;
            int distanceY = targetSpeciesY - currSpeciesY;
            bool collided = false;

            for (int j = 0; j < speciesCount; j++)
            {   
                if ((i == j) || (allSpecies[j].health <= 0)) { continue; }

                collided = CheckCollisionCircles(allSpecies[i].position, allSpecies[i].size, allSpecies[j].position, allSpecies[j].size); 
                if (collided) 
                {
                    int collidedSpeciesX = allSpecies[j].position.x;
                    int collidedSpeciesY = allSpecies[j].position.y;
                    int cDistanceX = collidedSpeciesX - currSpeciesX;
                    int cDistanceY = collidedSpeciesY - currSpeciesY;   
                    if (allSpecies[i].type != allSpecies[j].type)
                    { 
                        allSpecies[i].health -= allSpecies[j].dps; 
                    }
                    else 
                    { 
                        allSpecies[i].position.x -= GetRandomValue(-1, 3)*(cDistanceX/abs(cDistanceX))*allSpecies[i].speed*delta;
                        allSpecies[i].position.y -= GetRandomValue(-1, 3)*(cDistanceY/abs(cDistanceY))*allSpecies[i].speed*delta;
                    }
                }
            } 

            if (!collided)
            {
                allSpecies[i].position.x += GetRandomValue(-1, 3)*(distanceX/abs(distanceX))*allSpecies[i].speed*delta;
                allSpecies[i].position.y += GetRandomValue(-1, 3)*(distanceY/abs(distanceY))*allSpecies[i].speed*delta;
            }

            if (allSpecies[i].position.x >= screenWidth - allSpecies[i].size) { allSpecies[i].position.x -= allSpecies[i].speed*delta; }
            else if (allSpecies[i].position.x <= allSpecies[i].size) { allSpecies[i].position.x += allSpecies[i].speed*delta; }
            if (allSpecies[i].position.y >= screenHeight - allSpecies[i].size) { allSpecies[i].position.y -= allSpecies[i].speed*delta; }
            else if (allSpecies[i].position.y <= allSpecies[i].size) { allSpecies[i].position.y += allSpecies[i].speed*delta; }
        }
    }
}

void FindClosestEnemySpecies(Species species[], Species closestEnemySpecies[], int totalSpeciesCount)
{
    Species closestSpecies = { 0 };

    for (int i = 0; i < totalSpeciesCount; i++)
    {
        Species currSpecies = species[i];
        if (currSpecies.health > 0)
        {
            for (int j = 0; j < totalSpeciesCount; j++)
            {   
                int closestDistance = 999999;
                Species targetSpecies = species[j];
                if ((currSpecies.type != targetSpecies.type) && (targetSpecies.health > 0))
                {
                    int distance = sqrt(pow(targetSpecies.position.x - currSpecies.position.x, 2) + 
                                        pow(targetSpecies.position.y - currSpecies.position.y, 2)) - 
                                        (targetSpecies.size/2 + currSpecies.size/2);
                    if (distance < closestDistance) 
                    {
                        closestEnemySpecies[i] = targetSpecies;
                        closestDistance = distance;
                    }
                    
                } else { continue; }
            }
        }
    }
};

int main(void)
{

    InitWindow(screenWidth, screenHeight, "Species Battle Demo");

    int totalSpeciesCount = ALPHA_INITIAL_CNT + BETA_INITIAL_CNT;    
    Species allSpecies[ALPHA_INITIAL_CNT+BETA_INITIAL_CNT] = { 0 };
    Species closestEnemySpecies[ALPHA_INITIAL_CNT+BETA_INITIAL_CNT] = { 0 };

    for (int i = 0; i<ALPHA_INITIAL_CNT; i++)
    {
        int randomX = GetRandomValue(0, screenWidth/2);   
        int randomY = GetRandomValue(0, screenHeight/2);
        int randomSize = GetRandomValue(5, 10); 
        Species species = { 0 };
        species.type = ALPHA_SPECIES;
        species.position = (Vector2){ randomX, randomY };
        species.color = BLUE;
        species.size = randomSize;
        species.speed = 40;
        species.dps = 8;
        species.health = 1000;
        allSpecies[i] = species; 
    }

    for (int i = ALPHA_INITIAL_CNT; i < ALPHA_INITIAL_CNT + BETA_INITIAL_CNT; i++)
    {
        int randomX = GetRandomValue(screenWidth/2, screenWidth);   
        int randomY = GetRandomValue(screenHeight/2, screenHeight);
        int randomSize = GetRandomValue(15, 25); 
        Species species = { 0 };
        species.type = BETA_SPECIES;
        species.position = (Vector2){ randomX, randomY };
        species.color = RED;
        species.size = randomSize;
        species.speed = 16;
        species.dps = 4;
        species.health = 1500;
        allSpecies[i] = species; 
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {       
        float deltaTime = GetFrameTime();
        
        BeginDrawing();
        ClearBackground(BLACK);

        FindClosestEnemySpecies(allSpecies, closestEnemySpecies, totalSpeciesCount);
        UpdateSpecies(allSpecies, closestEnemySpecies, totalSpeciesCount, deltaTime);

        for (int i = 0; i<totalSpeciesCount; i++)
        {
            if (allSpecies[i].health > 0) { DrawCircle(allSpecies[i].position.x, allSpecies[i].position.y, allSpecies[i].size, allSpecies[i].color); }
        }
        
        DrawText("Species Battle Demo", 20, 20, 10, WHITE);

        EndDrawing();
    }
    
    CloseWindow();
}