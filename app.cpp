#include <GL/glut.h>

float angle = 0;
float posx = 0;
float posy = 0;

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

void renderMe(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(
           0, 0, 10,
           0, 0, 0,
           0, 1, 0);
    glRotatef( angle, 0, 0, 1);

	glBegin(GL_POLYGON);
		glVertex3f(0 + posx, 2 + posy, 0);
		glVertex3f( 1 + posx, -2 + posy, 0);
		glVertex3f( -1 + posx, -2 + posy, 0);
	glEnd();

	//angle += 1;

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y)
{
    if(key == 'q')
        angle++;
    if(key == 'e')
        angle--;

    if(key == 'd')
        posx += 0.1;
    if(key == 'a')
        posx -= 0.1;

    if(key == 'w')
        posy += 0.1;
    if(key == 's')
        posy -= 0.1;
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(700, 700);
	glutCreateWindow("GLUT Tutorial");

	glutDisplayFunc(renderMe);
	glutReshapeFunc(resizeMe);

	glutIdleFunc(renderMe);

	glutKeyboardFunc(processNormalKeys);

	glutMainLoop();

	return 1;
}
