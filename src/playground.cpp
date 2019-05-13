#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stack>

#include <GL/glew.h>

#include <glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Node.h"
#include "mesh_gen.h"
using namespace glm;

#include "../common/shader.hpp"

int main
(
	void
)
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	//glDepthFunc(GL_LESS);

	//Starting here, i kinda know shit
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera matrix
	vec3 camera_pos = glm::vec3(0,0,6);
	glm::mat4 view       = glm::lookAt(
								camera_pos, // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	glm::vec3 light_pos = glm::vec3(0.0f, 0.0f, 6.0f);

	mesh_gen generator;

	Node world(projection, view);
	Node pivot(projection, view);

	//first triangle
	Node node(projection, view);
	node.load_shader("../shaders/light.vertexshader", "../shaders/light.fragmentshader");



	//mesh_data triangle = generator.gen_triangle(1,1);
	mesh_data triangle = node.load_mesh("../res/sphere.obj");
	node.load_to_GPU(triangle);

	glm::vec3 diffuse = glm::vec3(0.7f, 0.2f, 0.4f);
	node.set_attributes(diffuse, triangle.indices.size());

	//second triangle
	Node node2(projection, view);
	node2.load_shader("../shaders/light.vertexshader", "../shaders/light.fragmentshader");

	//mesh_data triangle2 = generator.gen_triangle(2,2);
	mesh_data triangle2 = node.load_mesh("../res/sphere.obj");
	node2.load_to_GPU(triangle2);

	glm::vec3 diffuse2 = glm::vec3(0.2f, 0.8f, 0.2f);
	node2.set_attributes(diffuse2, triangle2.indices.size());


	pivot.add_child(&node);
	//pivot.add_child(&node2);

	world.add_child(&pivot);

	//pre loop tranformations
	node2.trans.set_translate(glm::vec3(2,0,0));
	node2.update(projection, view);
	float time;
	do{
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		time += 0.01f;

		//node.update(projection, view);

		//node2.trans.translate(glm::vec3(0,0.001f,0));
		node2.trans.rotate_y(0.01f);
		node2.update(projection, view);

		//pivot.trans.translate(glm::vec3(0.001f,0.0f,0.0f));
		pivot.trans.rotate_y(0.01f);
		pivot.update(projection, view);

		// Traverse the scene graph and render all the nodes
		auto node_stack = std::stack<Node const*>();
		auto matrix_stack = std::stack<glm::mat4>();
		node_stack.push(&world);
		matrix_stack.push(glm::mat4());
		do {
			auto const* const current_node = node_stack.top();
			node_stack.pop();

			auto const parent_matrix = matrix_stack.top();
			matrix_stack.pop();

			auto const current_node_matrix = current_node->get_model();

			auto const current_node_world_matrix = parent_matrix*current_node_matrix;
			current_node->render(projection, view,light_pos, camera_pos, current_node_world_matrix);

			for (int i = static_cast<int>(current_node->get_children_nb()) - 1; i >= 0; --i) {
				node_stack.push(current_node->get_child(static_cast<size_t>(i)));
				matrix_stack.push(current_node_world_matrix);
			}
		} while (!node_stack.empty());

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
	return 0;
}
