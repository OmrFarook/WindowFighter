#include <GL/glut.h>
#include<bits/stdc++.h>
using namespace std;

int windowLocationX=960, windowLocationY=600, WindowSizeX=500, WindowSizeY=500;

int incX = 0, incY = 0;

int score = 0;

int locationX=250, locationY=250;

void shape(int w, int h) {
    WindowSizeX = w;
    WindowSizeY = h;

    glViewport(0, 0, WindowSizeX, WindowSizeY);
}

void glDrawCircle(double x=0, double y=0,int degrees=360, double r=1, bool solid = true) {

    glVertex2f(x, y);

    glBegin(solid?GL_POLYGON:GL_LINE_LOOP);

    for (int i = 0; i <= degrees; i += 1) {
        double rad = i * 3.141 / 180.0;
        glVertex2f(r * cos(rad)+x, r * sin(rad)+y);
    }

    glEnd();
}


class tl2a;
class Rectangles;
vector<tl2a> vt;
vector<Rectangles> vr;


class tl2a {
public:
    float PlayerX;
    float PlayerY;

    float MouseX;
    float MouseY;

    float dirx;
    float diry;


    float scale=4.0;
    float speed = 10;

    bool operator== (tl2a *rhs)
    {
        return PlayerX == rhs->PlayerX && PlayerY == rhs->PlayerY &&
            MouseX == rhs->MouseX && MouseY == rhs->MouseY && dirx == rhs->dirx && diry && rhs->diry;
    }

    tl2a(int PlayerX, int PlayerY, int MouseX,int MouseY) {
        this->PlayerX = PlayerX;
        this->PlayerY = PlayerY;

        this->MouseX = MouseX;
        this->MouseY = MouseY;

        dirx = PlayerX;
        diry = PlayerY;
    }


    void draw() {
        glDrawCircle(dirx, diry, 360, scale, true);
    }

    void update() {

        float directionX = MouseX - PlayerX;
        float directionY = MouseY - PlayerY;


        float length = sqrt(directionX * directionX + directionY * directionY);
        if (length != 0) {
            directionX /= length;
            directionY /= length;
        }


        dirx += directionX * speed;
        diry += directionY * speed;


        if (dirx >= WindowSizeX) {

            for (int i = 0; i < vt.size(); i++) {
                if (vt[i] == this) {
                    vt.erase(vt.begin()+i);
                    break;

                }
            }

            incX += 25;


        }

        if (diry > WindowSizeY) {

            for (int i = 0; i < vt.size(); i++) {
                if (vt[i] == this) {
                    vt.erase(vt.begin() + i);
                    break;

                }
            }

            incY += 25;
        }




    }



};


class Rectangles {
public:

    float x;
    float y;

    float length;
    float width;
    float speed;
    int angle = 0;
    float rotateSpeed;
    int health = 2;

    Rectangles(float x, float y, float length, float width, float speed, float rotateSpeed) {
        this->x = x;
        this->y = y;
        this->length = length;
        this->width = width;
        this->speed = speed;
        this->rotateSpeed = rotateSpeed;
    }

    bool operator== (Rectangles* rhs)
    {
        return x == rhs->x && y == rhs->y &&
            length == rhs->length && width == rhs->width && speed == rhs->speed
            && angle == rhs->angle && rotateSpeed == rhs->rotateSpeed && health == rhs->health;
    }


    void draw() {

        glPushMatrix();
        glTranslatef(x, y, 0);
        glRotatef(angle, 0, 0, 1.0);
        glTranslatef(-x, -y, 0);

        glBegin(GL_QUADS);

        if(health ==2)
            glColor3ub(255, 0, 0);
        else
            glColor3ub(255, 255, 0);

        glVertex2f(x - width / 2, y + length / 2);
        glVertex2f(x + width / 2, y + length / 2);
        glVertex2f(x + width / 2, y - length / 2);
        glVertex2f(x - width / 2, y - length / 2);
        glColor3ub(0, 0, 0);

        glVertex2f(x - (width - 5) / 2, y + (length - 5) / 2);
        glVertex2f(x + (width - 5) / 2, y + (length - 5) / 2);
        glVertex2f(x + (width - 5) / 2, y - (length - 5) / 2);
        glVertex2f(x - (width - 5) / 2, y - (length - 5) / 2);
        glEnd();
        glPopMatrix();

    }

    void update() {

        angle += rotateSpeed;
        angle %= 360;


        float directionX = x - locationX;
        float directionY = y - locationY;


        float length = sqrt(directionX * directionX + directionY * directionY);
        if (length != 0) {
            directionX /= length;
            directionY /= length;
        }


        x -= directionX * speed;
        y -= directionY * speed;


        for (int i = 0; i < vt.size(); i++) {
            if (is_inside(vt[i].dirx, vt[i].diry,vt[i].scale)) {
                health -= 1;
                vt.erase(vt.begin() + i);
            }
        }

       if (health == 0) {
            score++;

            for (int i = 0; i < vr.size(); i++) {
                if (vr[i] == this) {
                    vr.erase(vr.begin() + i);
                    break;
                }
            }
            for (int i = 0; i <= rand() % 2; i++) {
                respawn();
            }

        }


    }

    void respawn() {
        int x = rand() % 250, y = rand() % 250;

        if (x > y) {
            if (x > WindowSizeX/4) {
                x = WindowSizeX+10;
            }
            else {
                x = -10;
            }
        }
        else {

            if (y > WindowSizeY/4) {
                y = WindowSizeX+10;
            }
            else {
                y = -10;
            }
        }


        auto rect = Rectangles(x, y, length, width, speed, rotateSpeed);

        vr.emplace_back(rect);


    }


    bool is_inside(int x, int y,int r) {

        int circleDistanceX = abs(x - this->x);
        int circleDistanceY = abs(y - this->y);

        if(circleDistanceX > (width/2 + r)) { return false; }
        if (circleDistanceY > (length / 2 + r)) { return false; }

        if (circleDistanceX <= (width / 2)) { return true; }
        if (circleDistanceY <= (length / 2)) { return true; }

        double cornerDistance_sq = (circleDistanceX - width / 2) * (circleDistanceX - width / 2) +
            (circleDistanceY - length / 2)* (circleDistanceY - length / 2);

        return (cornerDistance_sq <= (r *r));
    }


};




void display() {

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    glutReshapeWindow(WindowSizeX, WindowSizeY);

    glOrtho(0.0,WindowSizeX,WindowSizeY,0.0,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    glColor3ub(255, 255, 255);
    glDrawCircle(locationX, locationY, 360, 15, true);
    glColor3ub(0, 0, 0);
    glDrawCircle(locationX, locationY, 360, 10, true);
    glColor3ub(255, 255, 255);


    for (int i = 0; i < vt.size(); i++) {
        vt[i].draw();
    }





    for (int i = 0; i < vr.size(); i++) {
        vr[i].draw();
    }
    glColor3ub(255, 255, 255);


    glutSwapBuffers();




}

bool flag = false;

bool keys[] = { false,false,false,false };

void timer(int v) {

    for (int i = 0; i < vt.size(); i++) {
        vt[i].update();
    }
    for (int i = 0; i < vr.size(); i++) {
        vr[i].update();
    }

    if (keys[0]) {
        locationY -= 3;

    }
    if (keys[1]) {
        locationX -= 3;
    }

    if (keys[2]) {
        locationY += 3;
    }
    if (keys[3]) {
        locationX += 3;
    }


    if (incX > 0) {
        WindowSizeX += 2;
        incX -= 2;
    }
    else {
        WindowSizeX -= 1;
    }


    if (incY > 0) {
        WindowSizeY += 2;
        incY -= 2;
    }
    else {
        WindowSizeY -= 1;
    }


    bool died = false;

    for (int i = 0; i < vr.size(); i++) {
        if (vr[i].is_inside(locationX, locationY, 10)) {
            died = true;
        }
    }


    if (locationX + 15 > WindowSizeX || locationX - 15 < 0 || locationY+15 > WindowSizeY || locationY -15 < 0 || died) {
        char _score[10];
        itoa(score,_score,10);
        char text[50]="Your Score: ";
        strcat(text,_score);
        MessageBox(NULL,text,"Game Over",0);
        exit(0);
    }




    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, v);
}


void mouseHandle(int button,int state,int x,int y) {
    if (state != GLUT_DOWN)
        return;

    tl2a t = tl2a(locationX, locationY, x, y);
    vt.emplace_back(t);
}





void keyboardDown(unsigned char key, int x, int y) {

    switch (key)
    {
    case 'w':
        keys[0] = true;
        break;

    case 's':
        keys[2] = true;
        break;

    case 'a':
        keys[1] = true;
        break;

    case 'd':
        keys[3] = true;
        break;

    default:
        break;
    }

}

void keyboardup(unsigned char key, int x, int y) {

    switch (key)
    {
    case 'w':
        keys[0] = false;
        break;

    case 's':
        keys[2] = false;
        break;

    case 'a':
        keys[1] = false;
        break;

    case 'd':
        keys[3] = false;
        break;

    default:
        break;
    }


}




int main(int argc, char** argv) {
    glutInit(&argc, argv);
    srand(time(NULL));
    pair<int, int> pairs[] = { {-10,-10},{-10,510},{510,510}, {510,-1},{250,-10},{-10,250} };

    int rnd = rand() % 6;

    auto rt1 = Rectangles(pairs[rnd].first, pairs[rnd].second, 30, 30, 1, 1);

    glutInitWindowSize(WindowSizeX, WindowSizeY);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("OpenGl");
    vr.emplace_back(rt1);
    glutDisplayFunc(display);
    glutIgnoreKeyRepeat(3);
    glutReshapeFunc(shape);
    glutMouseFunc(mouseHandle);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardup);

    timer(0);
    glutMainLoop();


    return 0;
}

