#include <GL/glut.h>
#include <GL/freeglut.h>

float anglex = -20, angley = 0, anglez = 0;
float posx = 0, posz = 1, posy = 10;
int val_tr = 1;
int val_rot = 1;
int coord = 1;

void drawText(const char *string, int length, int x, int y)
{
    const unsigned char* text = reinterpret_cast<const unsigned char *>(string);

    glColor3f(255, 255, 255);
    glMatrixMode(GL_PROJECTION);
    double *matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0, 800, 0, 600, -5, 5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2i(x, y);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, text);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}

void drawCube() {
    //Face
	glColor3f(1, 0.5, 0.5);
	glBegin(GL_POLYGON);
        glVertex3f(-1, -1, -1);
        glVertex3f(1, -1, -1);
        glVertex3f(1, 1, -1);
        glVertex3f(-1, 1, -1);
	glEnd();

	//Left
	glColor3f(0.5, 0.5, 1);
	glBegin(GL_POLYGON);
        glVertex3f(1, -1, 1);
        glVertex3f(1, -1, -1);
        glVertex3f(1, 1, -1);
        glVertex3f(1, 1, 1);
	glEnd();

	//Right
	glColor3f(0.5, 1, 0.5);
	glBegin(GL_POLYGON);
        glVertex3f(-1, -1, 1);
        glVertex3f(-1, -1, -1);
        glVertex3f(-1, 1, -1);
        glVertex3f(-1, 1, 1);
	glEnd();

	//Back
    glColor3f(1, 1, 0.5);
	glBegin(GL_POLYGON);
        glVertex3f(-1, -1, 1);
        glVertex3f(1, -1, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(-1, 1, 1);
	glEnd();

	//Top
    glColor3f(1, 0.5, 1);
	glBegin(GL_POLYGON);
        glVertex3f(-1, 1, 1);
        glVertex3f(1, 1, 1);
        glVertex3f(1, 1, -1);
        glVertex3f(-1, 1, -1);
	glEnd();

	//Down
	glColor3f(0.5, 1, 1);
	glBegin(GL_POLYGON);
        glVertex3f(-1, -1, 1);
        glVertex3f(1, -1, 1);
        glVertex3f(1, -1, -1);
        glVertex3f(-1, -1, -1);
	glEnd();

}

void drawCoord()
{
    //x axis
    glColor3f(1, 0, 0); //red
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(2, 0, 0);

        glVertex3f(2, 0, 0);
        glVertex3f(1.85, 0.075, 0);

        glVertex3f(2, 0, 0);
        glVertex3f(1.85, -0.075, 0);
    glEnd();
    glFlush();

    glRasterPos3f(1.95, 0.15, 0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"x");

    //y axis
    glColor3f(0, 1, 0); //green
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, -2);

        glVertex3f(0, 0, -2);
        glVertex3f(0, 0.075, -1.85);

        glVertex3f(0, 0, -2);
        glVertex3f(0, -0.075, -1.85);
    glEnd();
    glFlush();

    glRasterPos3f(0, 0.15, -1.95);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"y");

    //z axis
    glColor3f(0, 0, 1); //blue
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 2, 0);

        glVertex3f(0, 2, 0);
        glVertex3f(0.075, 1.85, 0);

        glVertex3f(0, 2, 0);
        glVertex3f(-0.075, 1.85, 0);
    glEnd();
    glFlush();

    glRasterPos3f(0.15, 1.95, 0);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)"z");
}

void resizeMe(int w, int h)
{
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45 ,ratio, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void draw_info()
{
    float drawVar[3];
    float var[3];
    char ax[3];
    int textx = 0, texty = 580;

    if(val_tr == 1)
    {
        drawText("Translation:", 13, textx, texty);

        drawVar[0] = abs(posx);
        drawVar[1] = abs(posy - 10);
        drawVar[2] = abs(posz - 1);

        var[0] = posx;
        var[1] = posy - 10;
        var[2] = posz - 1;

        ax[0] = 'x';
        ax[1] = 'y';
        ax[2] = 'z';

        for(int i = 0; i < 3; i++)
        {
            char drText[8];

            drText[0] = ax[i];
            drText[1] = ':';
            if(var[i] < 0)
                drText[2] = '-';
            else
                drText[2] = ' ';
            drText[3] = (int)drawVar[i]%10 + '0';
            drText[4] = ',';
            drText[5] = (int)(drawVar[i]*10)%10 + '0';
            drText[6] = (int)(drawVar[i]*100)%10 + '0';
            drText[7] = '\0';
            drawText(drText, 7, textx, texty - 20*(i + 1));
        }
    }

    if(val_rot == 1)
    {
        if(val_tr == 1)
            textx += 160;
        drawText("Rotation:", 9, textx, texty);

        drawVar[0] = abs(anglex);
        drawVar[1] = abs(angley);
        drawVar[2] = abs(anglez);

        var[0] = anglex;
        var[1] = angley;
        var[2] = anglez;

        ax[0] = 'x';
        ax[1] = 'y';
        ax[2] = 'z';

        for(int i = 0; i < 3; i++)
        {
            char drText[8];

            drText[0] = ax[i];
            drText[1] = ':';
            if(var[i] < 0)
                drText[2] = '-';
            else
                drText[2] = ' ';
            drText[3] = (int)(drawVar[i]/10)%10 + '0';
            drText[4] = (int)(drawVar[i])%10 + '0';
            drText[5] = ',';
            drText[6] = (int)(drawVar[i]*10)%10 + '0';
            drText[7] = '\0';
            drawText(drText, 7, textx, texty - 20*(i + 1));
        }
    }
}

void renderMe(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set camera
	glLoadIdentity();
	gluLookAt(
           0, 1, 4,
           0, 1, 5,
           0, 1, 0);

    //Draw Ground
	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_QUADS);
		glVertex3f(-100, 0, -100);
		glVertex3f(-100, 0,  100);
		glVertex3f( 100, 0,  100);
		glVertex3f( 100, 0, -100);
	glEnd();

	//Draw and move cube
	glPushMatrix();
	glTranslatef(posx, posz, posy);
    if(coord == 1)
        drawCoord();
    glRotatef(anglez, 0, 1, 0);
    glRotatef(angley, 0, 0, 1);
    glRotatef(anglex, 1, 0, 0);
    drawCube();
    glPopMatrix();

    draw_info();

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'a':
        posx += 0.01;
        break;

    case 'd':
        posx -= 0.01;
        break;

    case 'w':
        posy += 0.01;
        break;

    case 's':
        posy -= 0.01;
        break;

    case 'q':
        anglez -= 0.5;
        break;

    case 'e':
        anglez += 0.5;
        break;
    }

}

void menu(int value)
{
    switch (value)
    {
    case 0:
        exit(0);
        break;
    case 1:
        val_tr = 1;
        break;
    case 2:
        val_tr = 0;
        break;
    case 3:
        val_rot = 1;
        break;
    case 4:
        val_rot = 0;
        break;
    case 5:
        coord = 1;
        break;
    case 6:
        coord = 0;
        break;
    }
}

void createMenu()
{
    int show_tr;
    int show_rot;
    int show_coord;

    show_tr = glutCreateMenu(menu);

	glutAddMenuEntry("Show", 1);
	glutAddMenuEntry("Hide", 2);

	show_rot = glutCreateMenu(menu);

	glutAddMenuEntry("Show", 3);
	glutAddMenuEntry("Hide", 4);

	show_coord = glutCreateMenu(menu);

	glutAddMenuEntry("Show", 5);
	glutAddMenuEntry("Hide", 6);

	glutCreateMenu(menu);

	glutAddSubMenu("Cube Translation", show_tr);
	glutAddSubMenu("Cube Rotation", show_rot);
	glutAddSubMenu("Coordinate System", show_coord);

	glutAddMenuEntry("Exit", 0);


	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(800, 200);
	glutInitWindowSize(700, 700);
	glutCreateWindow("SR App");

	createMenu();

	glutDisplayFunc(renderMe);
	glutReshapeFunc(resizeMe);
	glutIdleFunc(renderMe);

	glutKeyboardFunc(processNormalKeys);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	return 1;
}
