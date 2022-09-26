// ProjectFilter(glfw)

#include "cm_precompile.hpp"
#include "cc_glfw.hpp"

CC_WindowManager::CC_WindowManager(Clockmaker& aClockmakerInstance)
	: theClockmaker(aClockmakerInstance)
	, WIDTH(800)
	, HEIGHT(600)
{}

void CC_WindowManager::InitWindow()
{
	if( !glfwInit() )
	{
        exit( EXIT_FAILURE );
    }

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    myWindow = glfwCreateWindow(WIDTH, HEIGHT, CM_WIN_NAME, nullptr, nullptr);
	if( !myWindow )
	{
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	glfwMakeContextCurrent( myWindow );
}

bool CC_WindowManager::ShouldCloseWindow()
{
	return glfwWindowShouldClose(myWindow);
}

void CC_WindowManager::PollEvents()
{
	glfwPollEvents();
}

GLFWwindow* CC_WindowManager::GetWindow()
{
	return myWindow;
}

void CC_WindowManager::Cleanup()
{
	glfwDestroyWindow(myWindow);
    glfwTerminate();

#if RUNMODE == 1
	std::cout << "GLFW cleanup complete" << std::endl;
#endif
}
