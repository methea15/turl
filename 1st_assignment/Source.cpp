#include<GL/glew.h>

using namespace std;


enum eVertexArrayObject {
	VAOCircleData,
	VAOCount
};
enum eBufferObject {
	VBOCircleVertices,
	VBOCircleColor,
	BOCount
};
enum eProgram {
	CircleShaderProgram,
	ProgramCount
};
enum eTexture {
	NoTexture,		// fixes 0 sized array problem
	TextureCount
};
#include<common.cpp>
GLint		winHeight = 700;
GLint		winWidth = 700;
GLchar		winTitle[] = "Bouncing Circle";
GLFWwindow* win = nullptr;

const int segment = 50;
const float radius = 0.5f;

std::array<vec2, segment + 2> vertice;
std::array<vec3, segment + 2> color;

void createCircle() {
	//red center
	vertice[0] = vec2(0.0f, 0.0f);
	color[0] = vec3(1.0f, 0.0f, 0.0f);
	//green border
	for (int i = 0; i <= segment; i++) {
		double angle = (2.0f * M_PI * i) / segment;
		//position
		double x = cos(angle) * radius;
		double y = sin(angle) * radius;

		vertice[i] = vec2(x, y);
		//transition
		double transit = sqrt(x * x + y * y) / radius;
		color[i] = vec3(1.0f - transit, transit, 0.0f);
	}	
}

//animation circle
float posX = 0.0f, posY = 0.0f;//position
float vecX = 0.01f, vecY = 0.007f; //speed
bool isMoving = false;
void updatePosition() {
	if (isMoving){
		posX += vecX;
		posY += vecY;
		if (posX + 0.1f > 1.0f || posX - 0.1f < -1.0f)
			vecX = -vecX;
		if (posY + 0.1f > 1.0f || posY - 0.1f < -1.0f)
			vecY = -vecY;
	}
}
GLint colorSwapLocation;
// draw blue line
float lineY = 0.0f;
void blueLine() {
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f( -0.25, lineY);
	glVertex2f(0.25, lineY);
	glEnd();
}
void startProgram() {
	ShaderInfo shader_info[] = {
		{GL_FRAGMENT_SHADER, "./fragmentShader.glsl"},
		{GL_VERTEX_SHADER, "./vertexShader.glsl"},
		{GL_NONE, nullptr}
	};
	program[CircleShaderProgram] = LoadShaders(shader_info);

	glBindBuffer(GL_ARRAY_BUFFER, BO[VBOCircleVertices]);
	glBufferData(GL_ARRAY_BUFFER, vertice.size() * sizeof(vec2), vertice.data(), GL_STATIC_DRAW);
	
	glBindVertexArray(VAO[VAOCircleData]);
	//position attribute
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, 2 * sizeof(GLfloat),(GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, BO[VBOCircleColor]);
	glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(vec3), color.data(), GL_STATIC_DRAW);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glUseProgram(program[CircleShaderProgram]);
	//intersect to swap]
	colorSwapLocation = glGetUniformLocation(program[CircleShaderProgram], "colorSwap");

	if (posY > lineY - 0.1f && posY < lineY + 1.0f)	{
		glUniform1i(colorSwapLocation, GL_TRUE);
	}


}




void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAO[VAOCircleData]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, segment + 2);
}

void closeProgram(int returnCode) {
	glfwTerminate();
	exit(returnCode);
}

void handleKey(GLFWwindow* win, int keys, int scancode, int act, int mods){
	if ((act == GLFW_PRESS) && (keys == GLFW_KEY_BACKSPACE))
		glfwSetWindowShouldClose(win, GLFW_TRUE);
	if (act == GLFW_PRESS)
		keyboard[keys] = GL_TRUE;
	else if (act == GLFW_RELEASE)
		keyboard[keys] = GL_FALSE;
	//set up key
	if (act == GLFW_PRESS && keys == GLFW_KEY_P)
		isMoving = true;
	if (act == GLFW_PRESS && keys == GLFW_KEY_UP)
		lineY += 0.05f;
	if (act == GLFW_PRESS && keys == GLFW_KEY_DOWN)
		lineY -= 0.05f;
}

int main(void) {

	if (!glfwInit())
		closeProgram(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//create window
	glfwCreateWindow(winWidth, winHeight, winTitle, nullptr, nullptr);
	if (!win)	{
		cerr << "Can't create window.";
		closeProgram(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(win);
	
	
	if (glewInit() != GLEW_OK) {
		cerr << "GLEW system is failed to load.";
		closeProgram(EXIT_FAILURE);
	
	}//yellow window
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

	glfwSetKeyCallback(win, handleKey);
	setlocale(LC_ALL, "");

	cout << "Keyboard control" << endl;
	cout << "Up\tmove line Up" << endl;
	cout << "Down\tmove line Down" << endl;
	cout << "P\tPlay animation" << endl;

	while (!glfwWindowShouldClose(win)) {
		display();
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	closeProgram(EXIT_SUCCESS);
	return EXIT_SUCCESS;
}