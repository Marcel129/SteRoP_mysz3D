#include <GL/glut.h>
#include <GL/freeglut.h>

float anglex = 0, angley = 0, anglez = 0;
float posx = 0, posz = 1, posy = 10;
static int val_tr = 1;
static int val_rot = 1;

void drawCube() {
	glColor3f(1, 0.5 , 0.5);
	glutSolidCube(2);
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
    /*
    float drawx = abs(posx);
    float drawy = abs(posy);
    char c1[8], c2[8];
    c1[0] = 'x';
    c1[1] = ':';
    if(posx < 0)
        c1[2] = '-';
    else
        c1[2] = ' ';
    c1[3] = (int)drawx%10 + '0';
    c1[4] = ',';
    c1[5] = (int)(drawx*10)%10 + '0';
    c1[6] = (int)(drawx*100)%10 + '0';
    c1[7] = '\0';

    c2[0] = 'y';
    c2[1] = ':';
    c2[2] = (int)(drawy/10)%10 + '0';
    c2[3] = (int)drawy%10 + '0';
    c2[4] = ',';
    c2[5] = (int)(drawy*10)%10 + '0';
    c2[6] = (int)(drawy*100)%10 + '0';
    c2[7] = '\0';
    */

    /*
    if(val_tr == 1)
    {
        drawText("Trans", 5, 0, 580);
    }
    if(val_rot == 1)
    {
        drawText("Rot", 3, 0, 560);
    }
    */
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
    glRotatef(anglez, 0, 1, 0);
    glRotatef(angley, 1, 0, 0);
    glRotatef(anglex, 0, 0, 1);
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

void menu(int value){
	if(value == 0)
    {
		exit(0);
	}
	else
    {
		val_tr = value;
	}
}
void menu2(int value){
	val_rot = value;
}

void createMenu()
{
    static int show_tr;
    static int show_rot;

    show_tr = glutCreateMenu(menu);

	glutAddMenuEntry("Show", 1);
	glutAddMenuEntry("Hide", 2);

	show_rot = glutCreateMenu(menu2);

	glutAddMenuEntry("Show", 1);
	glutAddMenuEntry("Hide", 2);

	glutCreateMenu(menu);

	glutAddSubMenu("Cube TRanslation", show_tr);
	glutAddSubMenu("Cube Rotation", show_rot);

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
