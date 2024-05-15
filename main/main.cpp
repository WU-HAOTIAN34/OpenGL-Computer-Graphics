#include <glad.h>
#include <GLFW/glfw3.h>

#include <typeinfo>
#include <stdexcept>
#include <stb_image.h>
#include <cstdio>
#include <cstdlib>

#include "button.hpp"
#include "../support/error.hpp"
#include "../support/program.hpp"
#include "../support/checkpoint.hpp"
#include "../support/debug_output.hpp"

#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"
#include "../vmlib/mat33.hpp"

#include "defaults.hpp"
#include "model.hpp"
#include "simple_mesh.hpp"
#include "loadobj.hpp"
#include "texture.hpp"
#include "particle.hpp"
#include <iostream>


namespace
{
	constexpr char const* kWindowTitle = "COMP3811 - CW2";

	constexpr float kPi_ = 3.1415926f;

	constexpr float kMovementPerSecond_ = 5.f; // units per second
	constexpr float kMouseSensitivity_ = 0.01f; // radians per pixel

	struct State_
	{
		ShaderProgram* prog;

		struct CamCtrl_
		{
			bool cameraActive;
			bool actionZoomIn, actionZoomOut, actionLeft, actionRight, speedUP, slowDown;
			bool cPressed, shiftP, forword, backword, animation, split;
			float phi, theta;
			float radius, x, y, time;
			
			int cState;
			int cState2;
			float lastX, lastY;

		} camControl;

		int buttonState1, buttonState2;
		std::vector<Particle> particles;
	};

	void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	void glfw_callback_motion_(GLFWwindow*, double, double);

	void glfw_callback_error_( int, char const* );

	void glfw_callback_key_( GLFWwindow*, int, int, int, int );

	struct GLFWCleanupHelper
	{
		~GLFWCleanupHelper();
	};
	struct GLFWWindowDeleter
	{
		~GLFWWindowDeleter();
		GLFWwindow* window;
	};
}


int main() try
{
	// Initialize GLFW
	if( GLFW_TRUE != glfwInit() )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwInit() failed with '%s' (%d)", msg, ecode );
	}

	// Ensure that we call glfwTerminate() at the end of the program.
	GLFWCleanupHelper cleanupHelper;

	// Configure GLFW and create window
	glfwSetErrorCallback( &glfw_callback_error_ );

	glfwWindowHint( GLFW_SRGB_CAPABLE, GLFW_TRUE );
	glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

	//glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	glfwWindowHint( GLFW_DEPTH_BITS, 24 );

#	if !defined(NDEBUG)
	// When building in debug mode, request an OpenGL debug context. This
	// enables additional debugging features. However, this can carry extra
	// overheads. We therefore do not do this for release builds.
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#	endif // ~ !NDEBUG

	GLFWwindow* window = glfwCreateWindow(
		1280,
		720,
		kWindowTitle,
		nullptr, nullptr
	);

	if( !window )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwCreateWindow() failed with '%s' (%d)", msg, ecode );
	}

	GLFWWindowDeleter windowDeleter{ window };

	// Set up event handling
	// TODO: Additional event handling setup
	State_ state{};
	glfwSetWindowUserPointer(window, &state);
	glfwSetKeyCallback( window, &glfw_callback_key_ );
	glfwSetCursorPosCallback(window, &glfw_callback_motion_);
	glfwSetMouseButtonCallback(window, &glfw_mouse_button_callback);

	// Set up drawing stuff
	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 ); // V-Sync is on.

	// Initialize GLAD
	// This will load the OpenGL API. We mustn't make any OpenGL calls before this!
	if( !gladLoadGLLoader( (GLADloadproc)&glfwGetProcAddress ) )
		throw Error( "gladLoaDGLLoader() failed - cannot load GL API!" );

	std::printf( "RENDERER %s\n", glGetString( GL_RENDERER ) );
	std::printf( "VENDOR %s\n", glGetString( GL_VENDOR ) );
	std::printf( "VERSION %s\n", glGetString( GL_VERSION ) );
	std::printf( "SHADING_LANGUAGE_VERSION %s\n", glGetString( GL_SHADING_LANGUAGE_VERSION ) );

	// Ddebug output
#	if !defined(NDEBUG)
	setup_gl_debug_output();
#	endif // ~ !NDEBUG

	// Global GL state
	OGL_CHECKPOINT_ALWAYS();

	// TODO: global GL setup goes here
	glEnable(GL_FRAMEBUFFER_SRGB);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	OGL_CHECKPOINT_ALWAYS();

	// Get actual framebuffer size.
	// This can be different from the window size, as standard window
	// decorations (title bar, borders, ...) may be included in the window size
	// but not be part of the drawable surface area.
	int iwidth, iheight;
	glfwGetFramebufferSize( window, &iwidth, &iheight );

	glViewport( 0, 0, iwidth, iheight );

	// Other initialization & loading
	ShaderProgram prog({
		{ GL_VERTEX_SHADER, "assets/default.vert" },
		{ GL_FRAGMENT_SHADER, "assets/default.frag" }
	});
	ShaderProgram prog2({
		{ GL_VERTEX_SHADER, "assets/vert2.vert" },
		{ GL_FRAGMENT_SHADER, "assets/frag2.frag" }
		});
	ShaderProgram prog3({
		{ GL_VERTEX_SHADER, "assets/vert3.vert" },
		{ GL_FRAGMENT_SHADER, "assets/frag3.frag" }
		});
	ShaderProgram prog4({
		{ GL_VERTEX_SHADER, "assets/vert4.vert" },
		{ GL_FRAGMENT_SHADER, "assets/frag4.frag" }
		});
	state.prog = &prog;
	state.camControl.radius = 10.f;
	state.camControl.cState = 0;
	state.camControl.cState2 = 0;
	state.camControl.x = 0.0f;
	state.camControl.y = 0.0f;
	state.buttonState1 = 0;
	state.buttonState2 = 0;
	state.camControl.cPressed = false;
	state.camControl.shiftP = false;

	auto last = Clock::now();
	float angle = 0.f;
	OGL_CHECKPOINT_ALWAYS();
	
	// TODO: global GL setup goes here

	SimpleMeshData armadilloMesh;
	armadilloMesh = load_wavefront_obj("assets/parlahti.obj");
	GLuint vao = create_vao(armadilloMesh);
	std::size_t vertexCount = armadilloMesh.positions.size();

	SimpleMeshData armadilloMesh2;
	armadilloMesh2 = load_wavefront_obj("assets/landingpad.obj");
	GLuint vao2 = create_vao(armadilloMesh2);
	std::size_t vertexCount2 = armadilloMesh2.positions.size();


	auto cyl =  make_cylinder(true, 5, { 0.f, 0.4f, 0.f },
		make_rotation_z(3.141592f / 2.f) *
		make_scaling(0.3f, 0.3f, 0.3f) 
	);
	auto cy2 = make_cylinder(true, 4, { 0.3f, 0.3f, 0.3f },
		make_rotation_z(3.141592f / 2.f)* 
		make_scaling(1.f, 0.2f, 0.2f) * 
		make_translation({ 0.3f, 0.f, 0.f })
	);
	auto cy3 = make_cylinder(true, 255, { 0.3f, 0.2f, 0.6f },
		make_rotation_y(3.141592f / 4.f)*
		make_scaling(0.4f, 0.1f, 0.1f) *
		make_translation({ 0.35f, 10.f, 0.f })
	);
	auto cy4 = make_cylinder(true, 255, { 0.3f, 0.2f, 0.6f },
		make_rotation_y(3.141592f / 4.f * 3.f) *
		make_scaling(0.4f, 0.1f, 0.1f) *
		make_translation({ 0.35f, 10.f, 0.f })
	);
	auto cy5 = make_cylinder(true, 255, { 0.3f, 0.2f, 0.6f },
		make_rotation_y(-3.141592f / 4.f) *
		make_scaling(0.4f, 0.1f, 0.1f) *
		make_translation({ 0.35f, 10.f, 0.f })
	);
	auto cy6 = make_cylinder(true, 255, { 0.3f, 0.2f, 0.6f },
		make_rotation_y(-3.141592f / 4.f * 3.f) *
		make_scaling(0.4f, 0.1f, 0.1f) *
		make_translation({ 0.35f, 10.f, 0.f })
	);
	auto cy7 = make_cylinder(true, 3, { 0.f, 0.f, 0.6f },
		make_rotation_z(3.141592f / 2.f)*
		make_scaling(0.5f, 0.05f, 0.04f)*
		make_translation({ 2.6f, 0.f, 0.f })
	);
	auto ship1 = concatenate( std::move(cyl), cy2 );
	auto ship2 = concatenate(std::move(ship1), cy3);
	auto ship3 = concatenate(std::move(ship2), cy4);
	auto ship4 = concatenate(std::move(ship3), cy5);
	auto ship5 = concatenate(std::move(ship4), cy6);
	auto ship = concatenate(std::move(ship5), cy7);
	GLuint vao3 = create_vao(ship);
	std::size_t vertexCount3 = ship.positions.size();

	auto test = make_cylinder(true, 2, { 0.9f, 0.1f, 0.1f },
		make_scaling(0.25f, 0.08f, 0.08f) *
		make_translation({ -1.2f, -10.f, 0.f })
	);
	GLuint vao4 = create_vao(test);
	std::size_t vertexCount4 = test.positions.size();

	auto test1 = make_cylinder(true, 2, { 0.1f, 0.9f, 0.1f },
		make_scaling(0.25f, 0.08f, 0.08f) *
		make_translation({ 0.2f, -10.f, 0.f })
	);
	GLuint vao5 = create_vao(test1);

	GLuint tex = load_texture_2d("assets/L4343A-4k.jpeg");

	OGL_CHECKPOINT_ALWAYS();
	
	GLuint queryID[14];
	glGenQueries(14, queryID);

	auto frameStartTime = std::chrono::high_resolution_clock::now();
	auto frameEndTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> frameDuration;

	auto renderingStartTime = std::chrono::high_resolution_clock::now();
	auto renderingEndTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> renderingDuration;

	// Main loop
	while( !glfwWindowShouldClose( window ) )
	{
		// Let GLFW process events
		glfwPollEvents();
		int nwidth, nheight;
		// Check if window was resized.
		float fbwidth, fbheight;
		{
			
			glfwGetFramebufferSize( window, &nwidth, &nheight );

			fbwidth = float(nwidth);
			fbheight = float(nheight);
			
			if( 0 == nwidth || 0 == nheight )
			{
				// Window minimized? Pause until it is unminimized.
				// This is a bit of a hack.
				do
				{
					glfwWaitEvents();
					glfwGetFramebufferSize( window, &nwidth, &nheight );
				} while( 0 == nwidth || 0 == nheight );
			}

		}

		// Update state
		//TODO: update state
		auto const now = Clock::now();
		float dt = std::chrono::duration_cast<Secondsf>(now - last).count();
		last = now;

		angle += 0;
		if (angle >= 2.f * kPi_)
			angle -= 2.f * kPi_;

		float sp = kMovementPerSecond_;

		Mat44f Rx;
		Mat44f Ry;
		Mat44f T;

		float y = 0.f;
		float x = 0.f;
		float y1 = 0.f;
		float x1 = 0.f;
		Mat44f T1 = make_translation({ 0.f, 0.0f, 0.0f });
		Mat44f T2 = make_rotation_z(0.f);
		if (state.camControl.animation) {
			if (state.camControl.time < 75.0f) {
				state.camControl.time += 1.f;
			}
			float th = 0.025f * kPi_ * state.camControl.time * state.camControl.time / 180.0f;
			float c = std::cos(th);
			float s = std::sin(th);
			y = 10.0f * s;
			y1 = 9.80f * s;
			x = 10.f - 10.f * c;
			x1 = 9.8f - 9.8f * c;
			T1 = make_translation({ x, y, 0.0f });
			T2 = make_rotation_z(kPi_ / 2.f - th - 3.141592f / 2.f);
		}

		if (state.camControl.time > 0.f) {
			for (GLuint i = 0; i < 20; ++i)
			{
				int tem = findParticle(state.particles);
				resetParticle(state.particles, tem, { x1+0.f, y1-0.5f, 0.0f }, { -y, x-10.f, 0.0f });
			}
			for (GLuint i = 0; i < 500; ++i)
			{
				Particle& p = state.particles[i];
				p.lifeTime -= 1.f; 
				if (p.lifeTime > 0.0f)
				{   
					p.position += p.velocity * dt;
				}
			}
		}


		if (state.camControl.cState == 0) {
			if (state.camControl.speedUP)
				sp = sp * 4;
			else if (state.camControl.slowDown)
				sp = sp / 4;

			if (state.camControl.actionZoomIn)
				state.camControl.radius -= sp * dt;
			else if (state.camControl.actionZoomOut)
				state.camControl.radius += sp * dt;
			else if (state.camControl.actionLeft)
				state.camControl.x -= sp * dt;
			else if (state.camControl.actionRight)
				state.camControl.x += sp * dt;
			else if (state.camControl.forword)
				state.camControl.y += sp * dt;
			else if (state.camControl.backword)
				state.camControl.y -= sp * dt;

			if (state.camControl.radius <= 0.1f)
				state.camControl.radius = 0.1f;

			Rx = make_rotation_x(state.camControl.theta);
			Ry = make_rotation_y(state.camControl.phi);
			T = make_translation({ -state.camControl.x, -state.camControl.y, -state.camControl.radius });

		}
		else if (state.camControl.cState == 1) {

			Rx = make_rotation_x(0.0f);
			Ry = make_rotation_y(0.f);
			T = make_translation({ -x, -y + 0.95f, -5.f });
		}
		else if (state.camControl.cState == 2) {
			float th = 0.025f * kPi_ * state.camControl.time * state.camControl.time / 180.0f;
			Ry = make_rotation_y( th * 45.f / 140.f);
			Rx = make_rotation_x(0.0f);
			T = make_translation({ 0.f  - 20.f * std::sin(th * 45.f / 140.f), 0.f, -20.f * std::cos(th * 45.f / 140.f) });
		}
	
		// Draw scene
		OGL_CHECKPOINT_DEBUG();

		//TODO: draw frame
		
		Mat44f model2world = make_rotation_y(angle);
		Mat44f world2camera = T * Rx * Ry;
		Mat44f projection; 
		if (state.camControl.split) {
			projection = make_perspective_projection(
				60.f * 3.1415926f / 180.f,
				fbwidth / 2 / float(fbheight),
				0.1f, 100.0f
			);
		}
		else {
			projection = make_perspective_projection(
				60.f * 3.1415926f / 180.f,
				fbwidth / float(fbheight),
				0.1f, 100.0f
			);
		}
		Mat44f projCameraWorld = projection * world2camera;

		Mat33f normalMatrix = mat44_to_mat33(transpose(invert(model2world)));

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		glViewport(0, 0, nwidth, nheight);

		if (state.camControl.split) {
			glViewport(0, 0, nwidth / 2.f, nheight);
		}
		else {
			glViewport(0, 0, nwidth, nheight);
		}

		
		glUseProgram(prog.programId());
		
		glUniformMatrix4fv(0, 1, GL_TRUE, projCameraWorld.v);
		glUniformMatrix3fv(1, 1, GL_TRUE, normalMatrix.v);

		Vec3f lightDir = normalize(Vec3f{ 0.0f, 1.f, -1.0f });
		glUniform3fv(2, 1, &lightDir.x);
		glUniform3f(3, 0.9f, 0.9f, 0.6f);
		glUniform3f(4, 0.05f, 0.05f, 0.05f);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		glBindVertexArray(vao);
		
		glQueryCounter(queryID[0], GL_TIMESTAMP);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		glQueryCounter(queryID[1], GL_TIMESTAMP);

		if (state.camControl.time > 0.f) {
			glDepthMask(GL_FALSE);

			glUseProgram(0);
			glUseProgram(prog3.programId());
			glUniformMatrix4fv(0, 1, GL_TRUE, projCameraWorld.v);

			GLuint VAO = create_vao_particles(state.particles);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle)* state.particles.size(), state.particles.data());
			glBindVertexArray(VAO);

			glPointSize(20.0f);
			glQueryCounter(queryID[8], GL_TIMESTAMP);
			glDrawArrays(GL_POINTS, 0, state.particles.size());
			glQueryCounter(queryID[9], GL_TIMESTAMP);
			glDepthMask(GL_TRUE);
		}
		
		glUseProgram(0);
		
		glUseProgram(prog2.programId());

		renderingStartTime = std::chrono::high_resolution_clock::now();
		glUniformMatrix3fv(1, 1, GL_TRUE, normalMatrix.v);
		glUniform3f(2, 0.3f + x, -0.6f + y, 0.3f);
		glUniform3f(3, 0.05f, 0.05f, 0.05f);
		glUniform3f(4, 0.1f, 0.5f, 0.1f);
		glUniform3f(5, state.camControl.x, state.camControl.y, state.camControl.radius);
		glUniform1f(6, 0.5f);
		glUniform3f(7, -0.3f + x, -0.3f + y, -0.3f);
		glUniform3f(8, 0.6f, 0.6f, 0.1f);
		glUniform3f(9, 0.f + x, 0.4f + y, 0.f);
		glUniform3f(10, 0.7f, 0.f, 0.f);

		
		T = make_translation({ 25.f, -0.97f, 15.0f });
		glUniformMatrix4fv(0, 1, GL_TRUE, (projCameraWorld* T).v);
		glUniformMatrix3fv(1, 1, GL_TRUE, normalMatrix.v);
		glUniformMatrix4fv(11, 1, GL_TRUE, T.v);
		renderingEndTime = std::chrono::high_resolution_clock::now();

		glBindVertexArray(vao2);

		glQueryCounter(queryID[2], GL_TIMESTAMP);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount2);
		glQueryCounter(queryID[3], GL_TIMESTAMP);

		T = make_translation({ 0.f, -0.97f, 0.0f });
		glUniformMatrix4fv(0, 1, GL_TRUE, (projCameraWorld* T).v);
		glUniformMatrix3fv(1, 1, GL_TRUE, normalMatrix.v);
		glUniformMatrix4fv(11, 1, GL_TRUE, T.v);
		glBindVertexArray(vao2);
		glQueryCounter(queryID[4], GL_TIMESTAMP);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount2);
		glQueryCounter(queryID[5], GL_TIMESTAMP);

		glUseProgram(0);
		glUseProgram(prog2.programId());
		glUniformMatrix3fv(1, 1, GL_TRUE, normalMatrix.v);
		glUniform3f(2, 0.3f, -0.6f, 0.3f);
		glUniform3f(3, 0.05f, 0.05f, 0.05f);
		glUniform3f(4, 0.1f, 0.5f, 0.1f);
		glUniform3f(5, state.camControl.x, state.camControl.y, state.camControl.radius);
		glUniform1f(6, 0.5f);
		glUniform3f(7, -0.3f, -0.3f, -0.3f);
		glUniform3f(8, 0.6f, 0.6f, 0.1f);
		glUniform3f(9, 0.f, 0.4f, 0.f);
		glUniform3f(10, 0.7f, 0.f, 0.f);

		T = make_translation({ 0.f, -0.95f, 0.0f });
		glUniformMatrix4fv(0, 1, GL_TRUE, (projCameraWorld* T * T1 * T2).v);
		glUniformMatrix3fv(1, 1, GL_TRUE, normalMatrix.v);
		glUniformMatrix4fv(11, 1, GL_TRUE, T.v);
		glBindVertexArray(vao3);

		glQueryCounter(queryID[6], GL_TIMESTAMP);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount3);
		glQueryCounter(queryID[7], GL_TIMESTAMP);

		glUseProgram(0);
		glUseProgram(prog4.programId());

		glDepthMask(GL_FALSE);

		if (state.buttonState1 == 0) {
			static float const baseColor[] = { 1.f, 1.f, 1.f };
			glUniform3fv(1, 1, baseColor);
			glUniform1f(0, 0.3f);
		}
		else if (state.buttonState1 == 1) {
			static float const baseColor[] = { 1.f, 1.f, 1.f };
			glUniform3fv(1, 1, baseColor);
			glUniform1f(0, 0.9f);
		}
		else if (state.buttonState1 == 2){
			static float const baseColor[] = { 0.2f, 1.f, 1.f };
			glUniform3fv(1, 1, baseColor);
			glUniform1f(0, 0.9f);
		}
		glBindVertexArray(vao4);
		glQueryCounter(queryID[10], GL_TIMESTAMP);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount4);
		glQueryCounter(queryID[11], GL_TIMESTAMP);
		if (state.buttonState2 == 0) {
			static float const baseColor[] = { 1.f, 1.f, 1.f };
			glUniform3fv(1, 1, baseColor);
			glUniform1f(0, 0.3f);
		}
		else if (state.buttonState2 == 1) {
			static float const baseColor[] = { 1.f, 1.f, 1.f };
			glUniform3fv(1, 1, baseColor);
			glUniform1f(0, 0.9f);
		}
		else if (state.buttonState2 == 2) {
			static float const baseColor[] = { 0.1f, 0.2f, 1.f };
			glUniform3fv(1, 1, baseColor);
			glUniform1f(0, 0.9f);
		}
		glBindVertexArray(vao5);
		glQueryCounter(queryID[12], GL_TIMESTAMP);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount4);
		glQueryCounter(queryID[13], GL_TIMESTAMP);
		glDepthMask(GL_TRUE);

		glBindVertexArray(0);
		glUseProgram(0);
		OGL_CHECKPOINT_DEBUG();

		
		if (state.camControl.split) {

			if (state.camControl.cState2 == 0) {
				if (state.camControl.speedUP)
					sp = sp * 4;
				else if (state.camControl.slowDown)
					sp = sp / 4;

				if (state.camControl.actionZoomIn)
					state.camControl.radius -= sp * dt;
				else if (state.camControl.actionZoomOut)
					state.camControl.radius += sp * dt;
				else if (state.camControl.actionLeft)
					state.camControl.x -= sp * dt;
				else if (state.camControl.actionRight)
					state.camControl.x += sp * dt;
				else if (state.camControl.forword)
					state.camControl.y += sp * dt;
				else if (state.camControl.backword)
					state.camControl.y -= sp * dt;

				if (state.camControl.radius <= 0.1f)
					state.camControl.radius = 0.1f;

				Rx = make_rotation_x(state.camControl.theta);
				Ry = make_rotation_y(state.camControl.phi);
				T = make_translation({ -state.camControl.x, -state.camControl.y, -state.camControl.radius });

			}
			else if (state.camControl.cState2 == 1) {

				Rx = make_rotation_x(0.0f);
				Ry = make_rotation_y(0.f);
				T = make_translation({ -x, -y + 0.95f, -5.f });
			}
			else if (state.camControl.cState2 == 2) {
				float th = 0.025f * kPi_ * state.camControl.time * state.camControl.time / 180.0f;
				Ry = make_rotation_y(th * 45.f / 140.f);
				Rx = make_rotation_x(0.0f);
				T = make_translation({ 0.f - 20.f * std::sin(th * 45.f / 140.f), 0.f, -20.f * std::cos(th * 45.f / 140.f) });
			}

			world2camera = T * Rx * Ry;
			projCameraWorld = projection * world2camera;

			glViewport(nwidth / 2.f, 0, nwidth / 2.f, nheight);

			glUseProgram(prog.programId());

			glUniformMatrix4fv(0, 1, GL_TRUE, projCameraWorld.v);
			glUniformMatrix3fv(1, 1, GL_TRUE, normalMatrix.v);

			Vec3f lightDir = normalize(Vec3f{ 0.0f, 1.f, -1.0f });
			glUniform3fv(2, 1, &lightDir.x);
			glUniform3f(3, 0.9f, 0.9f, 0.6f);
			glUniform3f(4, 0.05f, 0.05f, 0.05f);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);

			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);

			glUseProgram(0);

			if (state.camControl.time > 0.f) {
				glDepthMask(GL_FALSE);

				glUseProgram(0);
				glUseProgram(prog3.programId());
				glUniformMatrix4fv(0, 1, GL_TRUE, projCameraWorld.v);

				GLuint VAO = create_vao_particles(state.particles);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle) * state.particles.size(), state.particles.data());
				glBindVertexArray(VAO);

				glPointSize(15.0f);
				glDrawArrays(GL_POINTS, 0, state.particles.size());

				glDepthMask(GL_TRUE);
			}

			glUseProgram(prog2.programId());

			glUniformMatrix3fv(1, 1, GL_TRUE, normalMatrix.v);
			glUniform3f(2, 0.3f + x, -0.6f + y, 0.3f);
			glUniform3f(3, 0.05f, 0.05f, 0.05f);
			glUniform3f(4, 0.1f, 0.5f, 0.1f);
			glUniform3f(5, state.camControl.x, state.camControl.y, state.camControl.radius);
			glUniform1f(6, 0.5f);
			glUniform3f(7, -0.3f + x, -0.3f + y, -0.3f);
			glUniform3f(8, 0.6f, 0.6f, 0.1f);
			glUniform3f(9, 0.f + x, 0.4f + y, 0.f);
			glUniform3f(10, 0.7f, 0.f, 0.f);


			T = make_translation({ 25.f, -0.97f, 15.0f });
			glUniformMatrix4fv(0, 1, GL_TRUE, (projCameraWorld * T).v);
			glUniformMatrix3fv(1, 1, GL_TRUE, normalMatrix.v);
			glUniformMatrix4fv(11, 1, GL_TRUE, T.v);
			glBindVertexArray(vao2);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount2);

			T = make_translation({ 0.f, -0.97f, 0.0f });
			glUniformMatrix4fv(0, 1, GL_TRUE, (projCameraWorld * T).v);
			glUniformMatrix3fv(1, 1, GL_TRUE, normalMatrix.v);
			glUniformMatrix4fv(11, 1, GL_TRUE, T.v);
			glBindVertexArray(vao2);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount2);

			glUseProgram(0);
			glUseProgram(prog2.programId());
			glUniformMatrix3fv(1, 1, GL_TRUE, normalMatrix.v);
			glUniform3f(2, 0.3f, -0.6f, 0.3f);
			glUniform3f(3, 0.05f, 0.05f, 0.05f);
			glUniform3f(4, 0.1f, 0.5f, 0.1f);
			glUniform3f(5, state.camControl.x, state.camControl.y, state.camControl.radius);
			glUniform1f(6, 0.5f);
			glUniform3f(7, -0.3f, -0.3f, -0.3f);
			glUniform3f(8, 0.6f, 0.6f, 0.1f);
			glUniform3f(9, 0.f, 0.4f, 0.f);
			glUniform3f(10, 0.7f, 0.f, 0.f);

			T = make_translation({ 0.f, -0.95f, 0.0f });
			glUniformMatrix4fv(0, 1, GL_TRUE, (projCameraWorld * T * T1 * T2).v);
			glUniformMatrix3fv(1, 1, GL_TRUE, normalMatrix.v);
			glUniformMatrix4fv(11, 1, GL_TRUE, T.v);
			glBindVertexArray(vao3);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount3);


			glUseProgram(0);
			glUseProgram(prog4.programId());

			glDepthMask(GL_FALSE);

			if (state.buttonState1 == 0) {
				static float const baseColor[] = { 1.f, 1.f, 1.f };
				glUniform3fv(1, 1, baseColor);
				glUniform1f(0, 0.3f);
			}
			else if (state.buttonState1 == 1) {
				static float const baseColor[] = { 1.f, 1.f, 1.f };
				glUniform3fv(1, 1, baseColor);
				glUniform1f(0, 0.9f);
			}
			else if (state.buttonState1 == 2) {
				static float const baseColor[] = { 0.2f, 1.f, 1.f };
				glUniform3fv(1, 1, baseColor);
				glUniform1f(0, 0.9f);
			}
			glBindVertexArray(vao4);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount4);

			if (state.buttonState2 == 0) {
				static float const baseColor[] = { 1.f, 1.f, 1.f };
				glUniform3fv(1, 1, baseColor);
				glUniform1f(0, 0.3f);
			}
			else if (state.buttonState2 == 1) {
				static float const baseColor[] = { 1.f, 1.f, 1.f };
				glUniform3fv(1, 1, baseColor);
				glUniform1f(0, 0.9f);
			}
			else if (state.buttonState2 == 2) {
				static float const baseColor[] = { 0.1f, 0.2f, 1.f };
				glUniform3fv(1, 1, baseColor);
				glUniform1f(0, 0.9f);
			}
			glBindVertexArray(vao5);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount4);

			glDepthMask(GL_TRUE);



			glBindVertexArray(0);
			glUseProgram(0);
			OGL_CHECKPOINT_DEBUG();

		}


		glBindVertexArray(0);
		glUseProgram(0);
		// Display results

		GLuint64 startTime[7], endTime[7];
		glGetQueryObjectui64v(queryID[0], GL_QUERY_RESULT, &startTime[0]);
		glGetQueryObjectui64v(queryID[1], GL_QUERY_RESULT, &endTime[0]);
		glGetQueryObjectui64v(queryID[2], GL_QUERY_RESULT, &startTime[1]);
		glGetQueryObjectui64v(queryID[3], GL_QUERY_RESULT, &endTime[1]);
		glGetQueryObjectui64v(queryID[4], GL_QUERY_RESULT, &startTime[2]);
		glGetQueryObjectui64v(queryID[5], GL_QUERY_RESULT, &endTime[2]);
		glGetQueryObjectui64v(queryID[6], GL_QUERY_RESULT, &startTime[3]);
		glGetQueryObjectui64v(queryID[7], GL_QUERY_RESULT, &endTime[3]);
		glGetQueryObjectui64v(queryID[10], GL_QUERY_RESULT, &startTime[4]);
		glGetQueryObjectui64v(queryID[11], GL_QUERY_RESULT, &endTime[4]);
		glGetQueryObjectui64v(queryID[12], GL_QUERY_RESULT, &startTime[5]);
		glGetQueryObjectui64v(queryID[13], GL_QUERY_RESULT, &endTime[5]);

		GLuint64 timeElapsed[7];
		for (int num_ = 0; num_ < 6; num_++) {
			timeElapsed[num_] = endTime[num_] - startTime[num_];
		}

		double section12Time = static_cast<double>(timeElapsed[0]);
		double section14Time = static_cast<double>(timeElapsed[1]) + static_cast<double>(timeElapsed[2]);
		double section15Time = static_cast<double>(timeElapsed[3]);
		double fullRenderingTime = static_cast<double>(timeElapsed[4]) + static_cast<double>(timeElapsed[5]) + 
			+ section12Time + section14Time + section15Time;

		if (state.camControl.time > 0.f) {
			glGetQueryObjectui64v(queryID[8], GL_QUERY_RESULT, &startTime[6]);
			glGetQueryObjectui64v(queryID[9], GL_QUERY_RESULT, &endTime[6]);
			timeElapsed[6] = endTime[6] - startTime[6];
			fullRenderingTime += static_cast<double>(timeElapsed[6]);
		}

		fullRenderingTime *= 1.0e-9;
		section12Time *= 1.0e-9;
		section14Time *= 1.0e-9;
		section15Time *= 1.0e-9;

		std::cout << "Full Rendering Time: " << fullRenderingTime << " seconds\n";
		std::cout << "Section 1.2 Time: " << section12Time << " seconds\n";
		std::cout << "Section 1.4 Time: " << section14Time << " seconds\n";
		std::cout << "Section 1.5 Time: " << section15Time << " seconds\n";


		frameEndTime = std::chrono::high_resolution_clock::now();
		frameDuration = frameEndTime - frameStartTime;
		frameStartTime = frameEndTime;

		

		
		renderingDuration = renderingEndTime - renderingStartTime;

		std::cout << "Frame-to-frame time: " << frameDuration.count() << " seconds" << std::endl;
		std::cout << "Rendering command submission time: " << renderingDuration.count() << " seconds" << std::endl;


		glfwSwapBuffers( window );
	}

	// Cleanup.
	//TODO: additional cleanup
	
	return 0;
}
catch( std::exception const& eErr )
{
	std::fprintf( stderr, "Top-level Exception (%s):\n", typeid(eErr).name() );
	std::fprintf( stderr, "%s\n", eErr.what() );
	std::fprintf( stderr, "Bye.\n" );
	return 1;
}


namespace
{
	void glfw_callback_error_( int aErrNum, char const* aErrDesc )
	{
		std::fprintf( stderr, "GLFW error: %s (%d)\n", aErrDesc, aErrNum );
	}

	void glfw_callback_key_(GLFWwindow* aWindow, int aKey, int, int aAction, int)
	{
		if (GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction)
		{
			glfwSetWindowShouldClose(aWindow, GLFW_TRUE);
			return;
		}

		if (auto* state = static_cast<State_*>(glfwGetWindowUserPointer(aWindow)))
		{
			
			if (GLFW_KEY_R == aKey && GLFW_PRESS == aAction)
			{
				if (state->prog)
				{
					try
					{
						state->prog->reload();
						std::fprintf(stderr, "Shaders reloaded and recompiled.\n");
					}
					catch (std::exception const& eErr)
					{
						std::fprintf(stderr, "Error when reloading shader:\n");
						std::fprintf(stderr, "%s\n", eErr.what());
						std::fprintf(stderr, "Keeping old shader.\n");
					}
				}
			}

			if (GLFW_KEY_SPACE == aKey && GLFW_PRESS == aAction)
			{
				state->camControl.cameraActive = !state->camControl.cameraActive;

				if (state->camControl.cameraActive)
					glfwSetInputMode(aWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				else
					glfwSetInputMode(aWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}

			if (state->camControl.cameraActive)
			{
				if (GLFW_KEY_W == aKey)
				{
					if (GLFW_PRESS == aAction)
						state->camControl.actionZoomIn = true;
					else if (GLFW_RELEASE == aAction)
						state->camControl.actionZoomIn = false;
				}
				else if (GLFW_KEY_S == aKey)
				{
					if (GLFW_PRESS == aAction)
						state->camControl.actionZoomOut = true;
					else if (GLFW_RELEASE == aAction)
						state->camControl.actionZoomOut = false;
				}
				else if (GLFW_KEY_A == aKey)
				{
					if (GLFW_PRESS == aAction)
						state->camControl.actionLeft = true;
					else if (GLFW_RELEASE == aAction)
						state->camControl.actionLeft = false;
				}
				else if (GLFW_KEY_D == aKey)
				{
					if (GLFW_PRESS == aAction)
						state->camControl.actionRight = true;
					else if (GLFW_RELEASE == aAction)
						state->camControl.actionRight = false;
				}
				else if (GLFW_KEY_Q == aKey)
				{
					if (GLFW_PRESS == aAction)
						state->camControl.forword = true;
					else if (GLFW_RELEASE == aAction)
						state->camControl.forword = false;
				}
				else if (GLFW_KEY_E == aKey)
				{
					if (GLFW_PRESS == aAction)
						state->camControl.backword = true;
					else if (GLFW_RELEASE == aAction)
						state->camControl.backword = false;
				}
				else if (GLFW_KEY_LEFT_SHIFT == aKey)
				{
					if (GLFW_PRESS == aAction) {
						state->camControl.speedUP = true;
						if (state->camControl.split) {
							state->camControl.shiftP = true;
							if (state->camControl.cPressed) {
								if (state->camControl.cState2 == 2) {
									state->camControl.cState2 = 0;
								}
								else {
									state->camControl.cState2 += 1;
								}
							}
						}
					}
					else if (GLFW_RELEASE == aAction) {
						state->camControl.speedUP = false;
						state->camControl.shiftP = false;
					}

				}
				else if (GLFW_KEY_LEFT_CONTROL == aKey)
				{
					if (GLFW_PRESS == aAction)
						state->camControl.slowDown = true;
					else if (GLFW_RELEASE == aAction)
						state->camControl.slowDown = false;
				}
				else if (GLFW_KEY_F == aKey)
				{
					if (GLFW_PRESS == aAction)
						if (!state->camControl.animation) {
							for (GLuint i = 0; i < 500; ++i)
								state->particles.push_back(Particle());
							state->camControl.animation = true;
						}		
				}
				else if (GLFW_KEY_R == aKey)
				{
					if (GLFW_PRESS == aAction) {
						state->particles.clear();
						state->camControl.animation = false;
						state->camControl.time = 0.0f;
					}			
				}
				else if (GLFW_KEY_C == aKey)
				{
					if (GLFW_PRESS == aAction) {
						
						if (state->camControl.split) {
							state->camControl.cPressed = true;
							if (state->camControl.shiftP) {
								if (state->camControl.cState2 == 2) {
									state->camControl.cState2 = 0;
								}
								else {
									state->camControl.cState2 += 1;
								}
							}
							else {
								if (state->camControl.cState == 2) {
									state->camControl.cState = 0;
								}
								else {
									state->camControl.cState += 1;
								}
							}
						}
						else {
							if (state->camControl.cState == 2) {
								state->camControl.cState = 0;
							}
							else {
								state->camControl.cState += 1;
							}
						}
					}
					else if (GLFW_RELEASE == aAction)
						state->camControl.cPressed = false;
				}
				else if (GLFW_KEY_V == aKey)
				{
					if (GLFW_PRESS == aAction)
						state->camControl.split = !state->camControl.split;
				}
			}
		}
	}

	void glfw_callback_motion_(GLFWwindow* aWindow, double aX, double aY)
	{
		if (auto* state = static_cast<State_*>(glfwGetWindowUserPointer(aWindow)))
		{
			if (state->camControl.cameraActive)
			{
				auto const dx = float(aX - state->camControl.lastX);
				auto const dy = float(aY - state->camControl.lastY);

				state->camControl.phi += dx * kMouseSensitivity_;

				state->camControl.theta += dy * kMouseSensitivity_;
				if (state->camControl.theta > kPi_ / 2.f)
					state->camControl.theta = kPi_ / 2.f;
				else if (state->camControl.theta < -kPi_ / 2.f)
					state->camControl.theta = -kPi_ / 2.f;
			}
			else
			{
				double xpos, ypos;
				glfwGetCursorPos(aWindow, &xpos, &ypos);
				int width, height;
				glfwGetWindowSize(aWindow, &width, &height);
				int x = buttonState(static_cast<float>(width), static_cast<float>(height), static_cast<float>(xpos), static_cast<float>(ypos), state->camControl.split);
				
				if (x == 1) {
					state->buttonState1 = 1;
				}
				else if (x == 2) {
					state->buttonState2 = 1;
				}
				else {
					state->buttonState1 = 0;
					state->buttonState2 = 0;
				}

			}

			state->camControl.lastX = float(aX);
			state->camControl.lastY = float(aY);
		}
	}

	void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

		if (auto* state = static_cast<State_*>(glfwGetWindowUserPointer(window))) {
			if (button == GLFW_MOUSE_BUTTON_LEFT) {

				if (action == GLFW_PRESS) {

					double xpos, ypos;
					glfwGetCursorPos(window, &xpos, &ypos);
					int width, height;
					glfwGetWindowSize(window, &width, &height);
					int x = buttonState(static_cast<float>(width), static_cast<float>(height), static_cast<float>(xpos), static_cast<float>(ypos), state->camControl.split);

					if (x == 1) {
						for (GLuint i = 0; i < 500; ++i)
							state->particles.push_back(Particle());
						state->camControl.animation = true;
						state->buttonState1 = 2;
					}
					else if (x == 2) {
						state->particles.clear();
						state->camControl.animation = false;
						state->camControl.time = 0.0f;
						state->buttonState2 = 2;
					}

				}
				else if (action == GLFW_RELEASE) {
					double xpos, ypos;
					glfwGetCursorPos(window, &xpos, &ypos);
					int width, height;
					glfwGetWindowSize(window, &width, &height);
					int x = buttonState(static_cast<float>(width), static_cast<float>(height), static_cast<float>(xpos), static_cast<float>(ypos), state->camControl.split);

					if (x == 1) {
						state->buttonState1 = 1;
					}
					else if (x == 2) {
						state->buttonState2 = 1;
					}
				}
			}
		}

	}

}


namespace
{
	GLFWCleanupHelper::~GLFWCleanupHelper()
	{
		glfwTerminate();
	}

	GLFWWindowDeleter::~GLFWWindowDeleter()
	{
		if( window )
			glfwDestroyWindow( window );
	}
}