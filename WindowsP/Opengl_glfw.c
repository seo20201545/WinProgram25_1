#pragma comment(lib, "Opengl32.lib")
#include <GLFW/glfw3.h>
#include <math.h>

#define PI 3.14159265358979323846f

int main() {
	if (!glfwInit()) return -1;

	GLFWwindow* window = glfwCreateWindow(500, 500, "GLFW: ��", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		// �� �׸���
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(1.0f, 1.0f, 0.0f);  // �߽� �����
		glVertex2f(0.0f, 0.0f);       // �߽�

		int segments = 100;
		float radius = 0.5f;
		for (int i = 0; i <= segments; i++) {
			float angle = 2.0f * PI * i / segments;
			float x = cosf(angle) * radius;
			float y = sinf(angle) * radius;
			glColor3f(1.0f - i / (float)segments, 0.5f, i / (float)segments);  // ���� ��ȭ
			glVertex2f(x, y);
		}
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
