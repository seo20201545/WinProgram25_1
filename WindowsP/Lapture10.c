#pragma comment(lib, "Opengl32.lib")
#include <GLFW/glfw3.h>

/* NuGet 패키지 매니저에서 GLFW를 설치후 사용 */

int main() {
	if (glfwInit()) return -1;

	GLFWwindow* window = glfwCreateWindow(500, 500, "GLFW: 삼각형", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.5f, -0.5f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(0.5f, -0.5f);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.5f, 0.5f);
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}