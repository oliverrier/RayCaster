#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

/// <summary>
/// Draw a one pixel width vertical line in the specified image.
/// </summary>
/// <param name="tex"></param>
/// <param name="x">Abscisse of the line  </param>
/// <param name="y1">Starting height of the line </param>
/// <param name="y2">Ending height of the line  </param>
/// <param name="c"></param>
void drawVerticalLine(sf::Image& img, int x, int y1, int y2, const sf::Color& c)
{
    for (int h = y1; h < y2; h++) {
        img.setPixel(x, h, c);
    }
}


/// <summary>
/// Draw a empty or filled rectangle in the specified image
/// </summary>
/// <param name="img"></param>
/// <param name="origin">top left point of the rectangle </param>
/// <param name="size">width and height of the rectangle </param>
/// <param name="c"></param>
/// <param name="filled">if true rect will be filled with color </param>
void drawRect(sf::Image& img, const sf::Vector2i& origin, const sf::Vector2i& size, const sf::Color& c)
{
    for (int h = origin.y; h < origin.y + size.y; h++) {
        for (int w = origin.x; w < origin.x + size.x; w++) {
            img.setPixel(w, h, c);
        }
    }
}


/// <summary>
///  The World map
/// >0 is a wall
/// </summary>
static const int g_mapWidth = 24;
static const int g_mapHeight = 24;

int g_worldMap[g_mapWidth][g_mapHeight] =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


/// <summary>
/// Return a unnormalized vector perpendicular to the direction
/// </summary>
/// <param name="dir"></param>
/// <returns></returns>
inline sf::Vector2f Perpendicular(const sf::Vector2f& dir)
{
    sf::Vector2f perp = sf::Vector2f(dir.y, -dir.x);
    return perp;
}

static const float PI = 3.14159265359f;
static const float INF = std::numeric_limits<float>::infinity();


/// <summary>
/// Return dot product between a and b
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <returns></returns>
inline float Dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return a.x * b.x + a.y * b.y;
}

inline float Dist(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return sqrtf((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

/// <summary>
/// Return length of a vector2f
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
inline float Length(const sf::Vector2f& a)
{
    return sqrtf(Dot(a, a));
}

/// <summary>
/// Normalize a 2D vector
/// </summary>
/// <param name="a">Vector to normalize</param>
/// <returns>normalized vector</returns>
inline sf::Vector2f Normalize(const sf::Vector2f& a)
{
    return a / Length(a);
}
/// <summary>
/// Rotate a 2D Direction vector with an angle in degree
/// </summary>
/// <param name="dir"></param>
/// <param name="angleInDegree">rotation angle</param>
/// <returns></returns>
inline sf::Vector2f RotateVector(const sf::Vector2f& dir, float angleInDegree)
{
    sf::Transform rotation;
    rotation.rotate(angleInDegree);
    return rotation.transformPoint(dir);
}

//calculate ray direction from abscisse in normalized 0-1 and camera direction
inline sf::Vector2f GetRayDir(float x,const sf::Vector2f& camDir)
{
    sf::Vector2f rightVec = Perpendicular(camDir);
    //x-coordinate in camera space -1.0f to 1.0f
    float camX = (2.0f * x) - 1.0f; 
    sf::Vector2f rayDir = camDir + rightVec * 0.66f*camX;
    return Normalize(rayDir);
}



/// <summary>
/// Clamp a sf::vector2f between min and max
/// </summary>
/// <param name="pos"></param>
/// <param name="min"></param>
/// <param name="max"></param>
/// <returns>clamped vector</returns>
inline sf::Vector2f ClampVector(const sf::Vector2f& pos, const sf::Vector2f& min, const sf::Vector2f& max)
{
    sf::Vector2f res;

    res.x = (pos.x > max.x) ? max.x : (pos.x < min.x) ? min.x : pos.x;
    res.y = (pos.y > max.y) ? max.y : (pos.y < min.y) ? min.y : pos.y;

    return res;
}
/// <summary>
/// Return rotation sign (-1 right, 1 left, 0 no rotation) and position delta vector based on current inputs
/// </summary>
/// <param name="deltaPos">(Out) position delta vector</param>
/// <returns>rotation sign</returns>
float GetCameraMovementFromInput(float& deltaPos)
{
    float angle = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        // move left...
        angle+=1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        // move right...
        angle -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        deltaPos += 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        deltaPos -= 1;
    }
    return angle;
}

// Solution start 
// ------------------------------------------------------------------
void RasterizeScene(const sf::Vector2f& camPos, const sf::Vector2f& camDir, sf::Image& raster, const int* worldMap, int W, int H)
{

    sf::Vector2f sideDistance, deltaDistance;


    for (int x = 0; x < 1920; x++) {
        sf::Vector2i mapPosition = { (int)camPos.x, (int)camPos.y };

        sf::Vector2f ray = GetRayDir((float)x / 1920, camDir);
        deltaDistance.x = (ray.x == 0) ? INF : std::abs(1 / ray.x);
        deltaDistance.y = (ray.y == 0) ? INF : std::abs(1 / ray.y);
        float perpWallDistance;

        int stepX;
        int stepY;

        int hit = 0;
        int side;

        if (ray.x < 0)
        {
            stepX = -1;
            sideDistance.x = (camPos.x - mapPosition.x) * deltaDistance.x;
        }
        else
        {
            stepX = 1;
            sideDistance.x = (mapPosition.x + 1.0 - camPos.x) * deltaDistance.x;
        }
        if (ray.y < 0)
        {
            stepY = -1;
            sideDistance.y = (camPos.y - mapPosition.y) * deltaDistance.y;
        }
        else
        {
            stepY = 1;
            sideDistance.y = (mapPosition.y + 1.0 - camPos.y) * deltaDistance.y;
        }

        while (hit == 0)
        {
            if (sideDistance.x < sideDistance.y)
            {
                sideDistance.x += deltaDistance.x;
                mapPosition.x += stepX;
                side = 0;
            }
            else
            {
                sideDistance.y += deltaDistance.y;
                mapPosition.y += stepY;
                side = 1;
            }
            if (worldMap[mapPosition.x + mapPosition.y * W] > 0) hit = 1;
            if (side == 0) perpWallDistance = (sideDistance.x - deltaDistance.x);
            else perpWallDistance = (sideDistance.y - deltaDistance.y);

            int wallHeight = (int)(1080 / perpWallDistance);

            int wallStart = -wallHeight / 2 + 1080 / 2;
            if (wallStart < 0) wallStart = 0;
            int wallEnd = wallHeight / 2 + 1080 / 2;
            if (wallEnd >= 1080) wallEnd = 1080 - 1;

            //choose wall color
            sf::Color color;
            switch (worldMap[mapPosition.x + mapPosition.y * W])
            {
            case 1:  color = sf::Color::Red;  break; //red
            case 2:  color = sf::Color::Green;  break; //green
            case 3:  color = sf::Color::Blue;   break; //blue
            case 4:  color = sf::Color::White;  break; //white
            default: color = sf::Color(30,30,30); break; //grey
            }

            //give x and y sides different brightness
            if (side == 1) { (color == sf::Color(30, 30, 30)) || color == sf::Color::Red ? color.a = 255 : color.a = 125; }

            //draw the pixels of the stripe as a vertical line
            drawVerticalLine(raster, x, wallStart, wallEnd, color);
        }
    }

}
// -------------------------------------------------------------------
// Solution End 


void RasterizeScene1stTry(const sf::Vector2f& camPos, const sf::Vector2f& camDir, sf::Image& raster, const int* worldMap, int W, int H)
{
    // 3D Scene
    const int cellSize = 64;


    int ray, depthOfField, worldMapArrayPosition; float rayAngle;
    sf::Vector2i mapPosition; sf::Vector2f rayPosition, offset;
    rayAngle = -1.0f;

    float finalDistance = INF;

    for (ray = 0; ray < 1920; ray++)
    {
        // --- Check Horizontal Lines ---

        depthOfField = 0;
        float horizontalDistance = INF;
        sf::Vector2f horizontalPosition{ camPos.x, camPos.y };
        float aTan = -1 / tanf(acos(Dot(sf::Vector2f(0.0f, -1.0f), camDir)));

        // Player looking down (please check on him)
        if (rayAngle > PI) {
            rayPosition.y = (((int)camPos.y * 64) / 64) - 0.0001;
            rayPosition.x = (camPos.y - rayPosition.y) * aTan + camPos.x;

            offset.y = -64;
            offset.x = -offset.y * aTan;
        }

        // Player looking up (way better thanks)
        if (rayAngle < PI) {
            rayPosition.y = (((int)camPos.y * 64) / 64) + 64;
            rayPosition.x = (camPos.y - rayPosition.y) * aTan + camPos.x;

            offset.y = 64;
            offset.x = -offset.y * aTan;
        }

        // Player looking straight left or right
        if (rayAngle == 0 || rayAngle == PI) {
            rayPosition.y = camPos.y;
            rayPosition.x = camPos.x;

            depthOfField = 8;
        }

        while (depthOfField < 8)
        {
            mapPosition.x = (int)rayPosition.x / 64;
            mapPosition.y = (int)rayPosition.y / 64;
            worldMapArrayPosition = mapPosition.y * W + mapPosition.x;

            // Wall hit
            if (worldMapArrayPosition > 0 && worldMapArrayPosition < W * H && worldMap[worldMapArrayPosition] > 0)
            {
                depthOfField = 8;
                horizontalPosition = rayPosition;
                horizontalDistance = Dist(camPos, horizontalPosition);
            }
            else
            {
                rayPosition.x += offset.x;
                rayPosition.y += offset.y;
                depthOfField += 1;
            }
        }

        // --- Check Vertical Lines ---

        depthOfField = 0;
        float verticalDistance = INF;
        sf::Vector2f verticalPosition{ camPos.x, camPos.y };

        float nTan = -tanf(acos(Dot(sf::Vector2f(0.0f, -1.0f), camDir)));

        // Player looking left (probably needs some company)
        if (rayAngle > PI / 2 && rayAngle < 3 * PI / 2) {
            rayPosition.x = (((int)camPos.x * 64) / 64) - 0.0001;
            rayPosition.y = (camPos.x - rayPosition.x) * nTan + camPos.y;

            offset.x = -64;
            offset.y = -offset.x * nTan;
        }

        // Player looking right (way better thanks)
        if (rayAngle < PI / 2 || rayAngle < 3 * PI / 2) {
            rayPosition.x = (((int)camPos.x * 64) / 64) + 64;
            rayPosition.y = (camPos.x - rayPosition.x) * nTan + camPos.y;

            offset.x = 64;
            offset.y = -offset.x * nTan;
        }

        // Player looking straight up or down
        if (rayAngle == 0 || rayAngle == PI) {
            rayPosition.y = camPos.y;
            rayPosition.x = camPos.x;

            depthOfField = 8;
        }

        while (depthOfField < 8)
        {
            mapPosition.x = (int)rayPosition.x / 64;
            mapPosition.y = (int)rayPosition.y / 64;
            worldMapArrayPosition = mapPosition.y * W + mapPosition.x;

            // Wall hit
            if (worldMapArrayPosition > 0 && worldMapArrayPosition < W * H && worldMap[worldMapArrayPosition] > 0)
            {
                depthOfField = 8;
                verticalPosition = rayPosition;
                verticalDistance = Dist(camPos, verticalPosition);
            }
            else
            {
                rayPosition.x += offset.x;
                rayPosition.y += offset.y;
                depthOfField += 1;
            }
        }
        if (verticalDistance < horizontalDistance) { rayPosition = verticalPosition; finalDistance = verticalDistance; };
        if (horizontalDistance < verticalDistance) { rayPosition = horizontalPosition; finalDistance = horizontalDistance; };

        // --- Draw 3DWalls ---

        float wallHeight = (cellSize * 1080) / finalDistance;
        wallHeight > 1080 ? wallHeight = 1080 : wallHeight;

        float lineOffset = 1080 - wallHeight / 2;

        drawVerticalLine(raster, rayPosition.x, lineOffset, rayPosition.y + lineOffset, sf::Color::Red);

        rayAngle += 0.033333f;


    }
}


int main()
{
    sf::Vector2i resolutionScreen = sf::Vector2i(1920, 1080);
    sf::RenderWindow window(sf::VideoMode(resolutionScreen.x, resolutionScreen.y), "SFML Raycaster");
    // Vertical sync to don't waste energy running over 60 FPS
    window.setVerticalSyncEnabled(true);

    // select the font
    //sf::Font font;
    //if (!font.loadFromFile("../../../arialbi.ttf"))
    //{
    //    return 0;
    //}
    //sf::Text text;
    //text.setFont(font); // font is a sf::Font
    //text.setCharacterSize(20); // in pixels, not points!
    //text.setFillColor(sf::Color::Black);

    // Image
    sf::Image raster;
    raster.create(window.getSize().x, window.getSize().y, sf::Color::Black);

    sf::Texture rasterTex;
    rasterTex.loadFromImage(raster);
    sf::Sprite test(rasterTex);

    // Initial Camera position
    sf::Vector2f camPos(22, 12);
    // Initial camera direction vector
    sf::Vector2f camDir(-1, 0);
    // Camera rotation in speed in degree per second
    const float rotationSpeed = 45.0f;

    // Camera movement speed in m per second
    const float movementSpeed = 1.0f;

    // Main loop    
    sf::Clock clock;
    sf::Time fixedPhysic = sf::microseconds(16666);
    sf::Time updateTime;
    while (window.isOpen())
    {
        // Update delta time
        sf::Time elapsed = clock.restart();
        if (elapsed.asSeconds() > 0.1f)
            elapsed = sf::seconds(0.1f);
        updateTime += elapsed;

        // Grab inputs events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                }
            }
        }

        // Get inputs
        float deltaPos = 0.0f;;
        float rotationSign = GetCameraMovementFromInput(deltaPos);

        // Update Logic and physics at fixed timesteps
        while (updateTime >= fixedPhysic)
        {
            // Update physics
            camDir = RotateVector(camDir, rotationSign * rotationSpeed * fixedPhysic.asSeconds());
            camDir = Normalize(camDir);
            camPos += camDir * deltaPos * movementSpeed * fixedPhysic.asSeconds();

            // Clamp world position
            camPos = ClampVector(camPos, sf::Vector2f(0, 0), sf::Vector2f(g_mapWidth, g_mapHeight));

            updateTime -= fixedPhysic;
        }
       
        // Rendering
        window.clear(sf::Color(0, 128, 0));

        {
            // Fill image with blue color
            drawRect(raster, sf::Vector2i(0, 0), resolutionScreen, sf::Color::Blue);

            RasterizeScene(camPos, camDir, raster, reinterpret_cast<int*>(g_worldMap), g_mapWidth, g_mapHeight);
            // Update texture from image
            rasterTex.loadFromImage(raster);
        }

        window.draw(test);
        // UI
        window.setView(window.getDefaultView());

        window.display();
    }

    return 0;
}