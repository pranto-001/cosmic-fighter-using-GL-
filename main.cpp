#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#define PI 3.1416



#define MAX_BULLETS 50

float bulletX[MAX_BULLETS];
float bulletY[MAX_BULLETS];
bool bulletActive[MAX_BULLETS];



      // Player bullets
float pBulletX[MAX_BULLETS];
float pBulletY[MAX_BULLETS];
bool pBulletActive[MAX_BULLETS];


#define MAX_ASTEROIDS 5
float asteroidX[MAX_ASTEROIDS];
float asteroidY[MAX_ASTEROIDS];
float asteroidR[MAX_ASTEROIDS];
bool asteroidActive[MAX_ASTEROIDS];
float asteroidAngle[MAX_ASTEROIDS];


#define MAX_BLUEOBJ 6

float blueX[MAX_BLUEOBJ];
float blueY[MAX_BLUEOBJ];
float blueAngle[MAX_BLUEOBJ];


//THEME
bool dayMode = false;

// cloud positions
float cloudX[5] = {-0.8f, -0.4f, 0.0f, 0.4f, 0.8f};
float cloudY[5] = {0.7f, 0.6f, 0.75f, 0.65f, 0.8f};



//Show GAME OVER on screen
bool gameOver = false;
bool win = false;


//Enemy Life Variable
int enemyLife = 15;   // enemy needs 5 hits to die
int enemyShootCounter = 0;


int enemyShootDelay = 0;
int playerShootCounter = 0;









   // Rocket positions
float normalX = 0.0f;
float normalY = -0.75f;

  // Enemy rocket position
float enemyX = 0.0f;
float enemyY = 0.80f;

   // Enemy movement direction
float enemyDX = 0.01f;
float enemyDY = 0.005f;



   //Player Life System
int playerLife = 3;



   // Star positions
float starX[100];
float starY[100];


   // Initialize stars
void initStars()
{
    for(int i=0;i<100;i++)
    {
        starX[i] = ((rand()%200)-100)/100.0f;
        starY[i] = ((rand()%200)-100)/100.0f;
    }
}



    // ASTEROID INIT FUNCTION

void initAsteroids()
{
    for(int i=0;i<MAX_ASTEROIDS;i++)
    {
        asteroidActive[i] = true;

        asteroidX[i] = (rand()%200 - 100) / 100.0f;
        asteroidY[i] = 1.0f + (rand()%100)/100.0f;
        asteroidR[i] = 0.05f + (rand()%10)/100.0f;
    }
}

void initBlueObjects()
{
    for(int i=0; i<MAX_BLUEOBJ; i++)
    {
        blueX[i] = (rand()%200 - 100) / 100.0f;
        blueY[i] = 1.0f + (rand()%100)/100.0f;

        blueAngle[i] = rand()%360;
    }
}



// Draw Circle
    void drawCircle(float x, float y, float r)
    {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);

        for(int i=0;i<=100;i++)
        {
            float angle = 2*PI*i/100;
            glVertex2f(
                x + cos(angle)*r,
                y + sin(angle)*r
            );
        }
        glEnd();
    }



      // for day theme( Sky +  Sun + Clouds)
    void drawCircleFilled(float x, float y, float r)
    {
        glBegin(GL_TRIANGLE_FAN);

        glVertex2f(x, y);

        for(int i=0;i<=100;i++)
        {
            float angle = 2 * PI * i / 100;
            glVertex2f(x + cos(angle)*r, y + sin(angle)*r);
        }

        glEnd();
    }
    void drawCloud(float x, float y)
        {
            glColor3f(1.0, 1.0, 1.0);

            drawCircleFilled(x, y, 0.05f);
            drawCircleFilled(x+0.05f, y+0.02f, 0.06f);
            drawCircleFilled(x-0.05f, y+0.02f, 0.05f);
        }

        void drawSun(float x, float y, float r)
{
    int segments = 100;

    glBegin(GL_TRIANGLE_FAN);

    // center
    glColor3f(1.0f, 1.0f, 0.6f); // brightlight
    glVertex2f(x, y);

    for(int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * 3.1416f * i / segments;

        float px = x + cos(angle) * r;
        float py = y + sin(angle) * r;

        // edge
        glColor3f(1.0f, 0.5f, 0.0f); // darkgreen

        glVertex2f(px, py);
    }

    glEnd();
}
    void drawDayBackground()
    {
        glBegin(GL_QUADS);

        glColor3f(0.4f, 0.7f, 1.0f);
        glVertex2f(-1, -1);

        glColor3f(0.6f, 0.85f, 1.0f);
        glVertex2f(1, -1);

        glColor3f(0.2f, 0.6f, 1.0f);
        glVertex2f(1, 1);

        glColor3f(0.4f, 0.8f, 1.0f);
        glVertex2f(-1, 1);

        glEnd();


        drawSun(0.7f, 0.75f, 0.12f);



         // Clouds transition using Translate & Scale
        for(int i = 0; i < 5; i++)
        {
            glPushMatrix();

            // Move cloud position
            glTranslatef(cloudX[i], cloudY[i], 0.0f);


            // Optional scaling
            glScalef(1.0f, 1.0f, 1.0f);

            // Draw cloud at translated position
            drawCloud(0.0f, 0.0f);

            glPopMatrix();

            // Animation movement
            cloudX[i] += 0.001f;

            // Reset position
            if(cloudX[i] > 1.2f)
                cloudX[i] = -1.2f;
        }
    }


//Fire bullet from enemy
    void fireEnemyBullet()
    {
        for(int i=0;i<MAX_BULLETS;i++)
        {
            if(!bulletActive[i])
            {
                bulletActive[i] = true;
                bulletX[i] = enemyX;
                bulletY[i] = enemyY;
                break;
            }
        }
    }



// Draw bullets
    void drawBullets()
    {
        glColor3f(1.0, 0.3, 0.3);

        for(int i=0;i<MAX_BULLETS;i++)
        {
            if(bulletActive[i])
            {
                drawCircle(bulletX[i], bulletY[i], 0.02);
            }
        }
    }


      //PLAYER Fire function:
    void fireTripleBullet()
    {
        for(int i=0;i<MAX_BULLETS;i++)
        {
            if(!pBulletActive[i])
            {
                // CENTER bullet
                pBulletActive[i] = true;
                pBulletX[i] = normalX;
                pBulletY[i] = normalY + 0.2f;

                // LEFT bullet
                if(i+1 < MAX_BULLETS)
                {
                    pBulletActive[i+1] = true;
                    pBulletX[i+1] = normalX - 0.08f;
                    pBulletY[i+1] = normalY + 0.2f;
                }

                // RIGHT bullet
                if(i+2 < MAX_BULLETS)
                {
                    pBulletActive[i+2] = true;
                    pBulletX[i+2] = normalX + 0.08f;
                    pBulletY[i+2] = normalY + 0.2f;
                }

                break;
            }
        }
    }


//Keyboard
    void keyboard(unsigned char key, int x, int y)
    {

        if(key == ' ')
        {
            fireTripleBullet();
        }

        if(key == 't' || key == 'T')
        {
            dayMode = !dayMode;   // toggle day/night
        }


        if(key == 'r' || key == 'R')
        {
            gameOver = false;
            playerLife = 3;
            enemyLife = 15;
            enemyX = 0.0f;
            enemyY = 0.8f;
        }
    }

//  DRAW STAR
void drawStars()
{
    glColor3f(1,1,1);
    glPointSize(2);

    for(int i = 0; i < 100; i++)
    {
        glPushMatrix();

        // Move star position
        glTranslatef(starX[i], starY[i], 0.0f);

        // Scale star size
        glScalef(1.0f, 1.0f, 1.0f);

        // Draw star at origin
        glBegin(GL_POINTS);

        glVertex2f(0.0f, 0.0f);

        glEnd();

        glPopMatrix();

        // Move stars downward
        starY[i] -= 0.01f;

        // Reset star position
        if(starY[i] < -1.0f)
            starY[i] = 1.0f;
    }
}

void drawPlanet(float x, float y, float r)
{
    int segments = 100;

    glBegin(GL_TRIANGLE_FAN);

    // center (bright color)
    glColor3f(0.4f, 1.0f, 0.4f);
    glVertex2f(x, y);

    for(int i = 0; i <= segments; i++)
    {
        float angle = 2.0f * 3.1416f * i / segments;

        float px = x + cos(angle) * r;
        float py = y + sin(angle) * r;

        // (dark green)
        glColor3f(0.1f, 0.4f, 0.1f);
        glVertex2f(px, py);
    }

    glEnd();
}

void drawBlueObjects()
{
    for(int i=0; i<MAX_BLUEOBJ; i++)
    {
        glPushMatrix();

        // move object
        glTranslatef(blueX[i], blueY[i], 0.0f);

        // rotate object
        glRotatef(blueAngle[i], 0, 0, 1);

        // scale
        glScalef(0.12f, 0.12f, 1.0f);

        // glowing blue color
        glColor3f(0.0f, 0.8f, 1.0f);

        // diamond shape
        glBegin(GL_POLYGON);

            glVertex2f( 0.0f,  0.5f);
            glVertex2f( 0.3f,  0.0f);
            glVertex2f( 0.0f, -0.5f);
            glVertex2f(-0.3f,  0.0f);

        glEnd();

        // center glow
        glColor3f(0.7f, 1.0f, 1.0f);
        drawCircle(0.0f, 0.0f, 0.4f);

        glPopMatrix();
    }
}

    void drawNightBackground()
    {
        // dark sky
        glClearColor(0.0, 0.1, 0.15, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        drawStars();
        drawBlueObjects();
       drawPlanet(-0.75f, 0.75f, 0.15f);
       drawPlanet( 0.75f,-0.50f, 0.18f);
    }



    void background()
    {
        if(dayMode)
            drawDayBackground();
        else
            drawNightBackground();
    }


        // NORMAL ROCKET (BOTTOM → UP)
    void drawNormalRocket()
    {
    glPushMatrix();

    // translation code.
    glTranslatef(normalX, normalY, 0.0);
    glScalef(0.35, 0.35, 1.0);// slightly bigger

    // MAIN BODY (triangle)
    glColor3f(0.55f, 0.0f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0, 1.2);
        glVertex2f(-0.35, 0.0);
        glVertex2f(0.35, 0.0);
    glEnd();

    // INNER BODY
    glColor3f(0.9f, 0.2f, 0.4f);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0, 1.0);
        glVertex2f(-0.20, 0.0);
        glVertex2f(0.20, 0.0);
    glEnd();

    // SIDE BOOSTERS (LEFT)
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(-0.45, 0.1);
        glVertex2f(-0.30, 0.1);
        glVertex2f(-0.30, 0.8);
        glVertex2f(-0.45, 0.8);
    glEnd();

    // SIDE BOOSTERS (RIGHT)
    glBegin(GL_QUADS);
        glVertex2f(0.30, 0.1);
        glVertex2f(0.45, 0.1);
        glVertex2f(0.45, 0.8);
        glVertex2f(0.30, 0.8);
    glEnd();

    // WINGS
    glColor3f(0.7f, 0.1f, 0.3f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.35, 0.2);
        glVertex2f(-0.65, 0.0);
        glVertex2f(-0.35, 0.6);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex2f(0.35, 0.2);
        glVertex2f(0.65, 0.0);
        glVertex2f(0.35, 0.6);
    glEnd();

    // CENTER LIGHT
    glColor3f(0.6, 1.0, 1.0);
    drawCircle(0.0, 0.5, 0.10);

    // FIRE
    glColor3f(1.0f, 0.2f, 0.3f);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.12, 0.0);
        glVertex2f(0.12, 0.0);
        glVertex2f(0.0, -0.45);
    glEnd();

    glPopMatrix();
}


          // ENEMY ROCKET(TOP → DOWN)
    void drawEnemyRocket()
    {
    glPushMatrix();

    glTranslatef(enemyX, enemyY, 0.0);
    glScalef(0.42, -0.42, 1.0);

    // MAIN BODY
    glColor3f(0.0, 0.9, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0, 1.3);
        glVertex2f(-0.40, 0.0);
        glVertex2f(0.40, 0.0);
    glEnd();

    // INNER BODY
    glColor3f(0.0, 0.6, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex2f(0.0, 1.1);
        glVertex2f(-0.25, 0.0);
        glVertex2f(0.25, 0.0);
    glEnd();

    // SIDE BOOSTERS
    glColor3f(0.8, 0.8, 0.9);
    glBegin(GL_QUADS);
        glVertex2f(-0.55, 0.1);
        glVertex2f(-0.35, 0.1);
        glVertex2f(-0.35, 0.9);
        glVertex2f(-0.55, 0.9);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(0.35, 0.1);
        glVertex2f(0.55, 0.1);
        glVertex2f(0.55, 0.9);
        glVertex2f(0.35, 0.9);
    glEnd();

    // BIG WINGS
    glColor3f(0.0, 0.5, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.40, 0.25);
        glVertex2f(-0.80, 0.0);
        glVertex2f(-0.40, 0.7);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex2f(0.40, 0.25);
        glVertex2f(0.80, 0.0);
        glVertex2f(0.40, 0.7);
    glEnd();

    // CENTER LIGHT
    glColor3f(0.6, 1.0, 1.0);
    drawCircle(0.0, 0.6, 0.12);

    // FIRE
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex2f(-0.15, 0.0);
        glVertex2f(0.15, 0.0);
        glVertex2f(0.0, -0.50);
    glEnd();

    glPopMatrix();
}




        //DRAW player bullets
    void drawPlayerBullets()
    {
        glColor3f(0.3, 1.0, 0.3);

        for(int i=0;i<MAX_BULLETS;i++)
        {
            if(pBulletActive[i])
            {
                drawCircle(pBulletX[i], pBulletY[i], 0.015);
            }
        }
    }

        // Asteroid Draw Function (with rotation)
        void drawAsteroids()
        {
            glColor3f(0.5, 0.5, 0.5);

            for(int i = 0; i < MAX_ASTEROIDS; i++)
            {
                if(asteroidActive[i])
                {
                    glPushMatrix();

                    // Move asteroid
                    glTranslatef(asteroidX[i], asteroidY[i], 0.0f);

                    // Rotate asteroid
                    glRotatef(asteroidAngle[i], 0.0f, 0.0f, 1.0f);

                    // Scale asteroid
                    glScalef(0.5f, 0.5f, 1.0f);

                    glBegin(GL_POLYGON);

                    for(int j = 0; j < 10; j++)
                    {
                        float angle = 2 * PI * j / 10;

                        float offset = (rand() % 30) / 100.0f;

                        float radius = 0.1f + offset * 0.05f;

                        glVertex2f(
                            cos(angle) * radius,
                            sin(angle) * radius
                        );
                    }

                    glEnd();

                    glPopMatrix();
                }
            }
        }

         //Create a text drawing function
    void drawText(float x, float y, const char* text)
    {
        glRasterPos2f(x, y);

        for(int i = 0; text[i] != '\0'; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
        }
    }


// DISPLAY
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);



    background();
    drawAsteroids();
    drawEnemyRocket();
    drawNormalRocket();


    if(!gameOver)
    {
        drawBullets();
        drawPlayerBullets();
    }



    //Show player life on screen
    char lifeText[20];
    sprintf(lifeText, "Life: %d", playerLife);

    glColor3f(1.0, 1.0, 1.0); // white color
    drawText(-0.95f, 0.90f, lifeText);


    char enemyText[20];
    sprintf(enemyText, "Enemy: %d", enemyLife);

    glColor3f(1.0, 0.5, 0.5);
    drawText(0.60f, 0.90f, enemyText);


    if(gameOver)
{
    if(enemyLife <= 0)
    {
        glColor3f(0.0, 1.0, 0.0);
        drawText(-0.15f, 0.0f, "YOU ARE ROCK BUDDY,YOU WIN!");
    }
    else
    {
        glColor3f(1.0, 0.0, 0.0);
        drawText(-0.15f, 0.0f, "GAME OVER");
    }
}


    glFlush();
}

//////////////////////////////////////////////////

    void specialKeys(int key, int x, int y)
    {
        float step = 0.05f;

        if(key == GLUT_KEY_LEFT)  normalX -= step;
        if(key == GLUT_KEY_RIGHT) normalX += step;
        if(key == GLUT_KEY_UP)    normalY += step;
        if(key == GLUT_KEY_DOWN)  normalY -= step;

        // Clamp position inside screen
        if(normalX < -0.75f) normalX = -0.75f;
        if(normalX > 0.75f) normalX = 0.75f;

        if(normalY < -0.80f) normalY = -0.80f;
        if(normalY > 0.80f) normalY = 0.80f;
        glutPostRedisplay();
    }

    void firePlayerBullet()
    {
        for(int i=0;i<MAX_BULLETS;i++)
        {
            if(!pBulletActive[i])
            {
                pBulletActive[i] = true;
                pBulletX[i] = normalX;
                pBulletY[i] = normalY + 0.2f;
                break;
            }
        }
    }


//    UPDATE
void update(int value)
{

    // BLUE OBJECT animation
for(int i=0; i<MAX_BLUEOBJ; i++)
{
    // move downward
    blueY[i] -= 0.01f;

    // rotate
    blueAngle[i] += 0.0f;

    // reset position
    if(blueY[i] < -1.2f)
    {
        blueY[i] = 1.2f;

        blueX[i] =
        (rand()%200 - 100) / 100.0f;
    }
}

    if(gameOver)
    {
        glutPostRedisplay();
        glutTimerFunc(30, update, 0);
        return;
    }


    // PLAYER CONTINUOUS SHOOTING
    playerShootCounter++;

    if(playerShootCounter >= 15)   // smaller = faster shooting
    {
        firePlayerBullet();   // auto shooting
        playerShootCounter = 0;
    }

        //asteroid
    for(int i = 0; i < MAX_ASTEROIDS; i++)
{
    if(asteroidActive[i])
    {
        // Move asteroid downward
        asteroidY[i] -= 0.01f;

        // Rotate asteroid
        asteroidAngle[i] += 0.0f;

        // Respawn asteroid
        if(asteroidY[i] < -1.2f)
        {
            asteroidY[i] = 1.0f;

            asteroidX[i] =
            (rand() % 200 - 100) / 100.0f;
        }
    }
}

    // ASTEROID HIT PLAYER
    for(int i=0;i<MAX_ASTEROIDS;i++)
    {
        if(asteroidActive[i])
        {
            float dx = asteroidX[i] - normalX;
            float dy = asteroidY[i] - normalY;

            float dist = sqrt(dx*dx + dy*dy);

            if(dist < asteroidR[i] + 0.10f)   // 0.10 ≈ player size
            {
                // reset asteroid
                asteroidY[i] = 1.0f;
                asteroidX[i] = (rand()%200 - 100) / 100.0f;

                // reduce player life
                playerLife--;

                if(playerLife <= 0)
                {
                    gameOver = true;
                }
            }
        }
    }



    //COLLISION (BULLET vs ASTEROID)

    for(int i=0;i<MAX_BULLETS;i++)
{
    if(pBulletActive[i])
    {
        for(int j=0;j<MAX_ASTEROIDS;j++)
        {
            if(asteroidActive[j])
            {
                float dx = pBulletX[i] - asteroidX[j];
                float dy = pBulletY[i] - asteroidY[j];

                float dist = sqrt(dx*dx + dy*dy);

                if(dist < asteroidR[j])
                {
                    // destroy asteroid
                    asteroidActive[j] = true;
                    asteroidY[j] = 1.0f;
                    asteroidX[j] = (rand()%200 - 100) / 100.0f;

                    // remove bullet
                    pBulletActive[i] = false;

                    break;
                }
            }
        }
    }
}


    //enemy movement
    enemyX += enemyDX;
    enemyY += enemyDY;

    if(enemyX > 0.6f || enemyX < -0.6f)
        enemyDX = -enemyDX;

    if(enemyY > 0.9f || enemyY < 0.2f)
        enemyDY = -enemyDY;

    glutPostRedisplay();
    glutTimerFunc(30, update, 0);


    // enemy shooting (controlled timing)
    enemyShootDelay--;

    if(enemyShootDelay <= 0)
    {
        fireEnemyBullet();
        enemyShootDelay = (rand() % 20) + 5;
    }


    // MOVE ENEMY bullets
    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i])
        {
            bulletY[i] -= 0.02f;

            if(bulletY[i] < -1.0f)
                bulletActive[i] = false;
        }
    }


//     Collision Detection (CORE GAME LOGIC)
//     Detect hit on enemy:
for(int i=0;i<MAX_BULLETS;i++)
{
    if(pBulletActive[i])
    {
        float dx = pBulletX[i] - enemyX;
        float dy = pBulletY[i] - enemyY;

        float dist = sqrt(dx*dx + dy*dy);

        if(dist < 0.15f)
          {
            pBulletActive[i] = false;

            enemyLife--;

            if(enemyLife <= 0)
            {
                    win = true;
                    gameOver = true;
            }
         }
    }
}


    //  Move Player Bullets
    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(pBulletActive[i])
        {
            pBulletY[i] += 0.03f;

            if(pBulletY[i] > 1.0f)
                pBulletActive[i] = false;
        }
    }


        // Enemy bullet hit player
for(int i=0;i<MAX_BULLETS;i++)
{
    if(bulletActive[i])
    {
        float dx = bulletX[i] - normalX;
        float dy = bulletY[i] - normalY;

        float dist = sqrt(dx*dx + dy*dy);

        if(dist < 0.12f)
        {
            bulletActive[i] = false;
            playerLife--;

                if(playerLife <= 0)
                {
                    gameOver = true;
                }
            }
        }
    }

}



void init()
{
    glClearColor(0.0, 0.0, 0.08, 1.0);

    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-1, 1, -1, 1);

    initStars();
    initAsteroids();
    initBlueObjects();

    // Initialize bullets
    for(int i=0;i<MAX_BULLETS;i++)
    {
        bulletActive[i] = false;
        pBulletActive[i] = false;


    }

}

         //MAIN FUNCTION
    int main(int argc, char** argv)
    {
        glutInit(&argc, argv);
        glutInitWindowSize(600, 600);
        glutCreateWindow("Rocket Game With Background");

        init();


        glutDisplayFunc(display);
        glutSpecialFunc(specialKeys);
        glutTimerFunc(90, update, 0);

        glutKeyboardFunc(keyboard);

        glutMainLoop();

        return 0;
    }
