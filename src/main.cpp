// ProjectFilter (test)

#include "cm_precompile.hpp"
#include "cm_commons.hpp"
#include "clockmaker.hpp"

int main()
{
	CM_WINDOW_NAME((char*)"Clockmaker");
	std::cout << "Runmode: " << RUNMODE_STR() << std::endl;
	Clockmaker* clockmaker = new Clockmaker();

	try 
	{
		clockmaker->Run();
	}
	catch (const std::exception& aException)
	{
		std::cerr << aException.what() << std::endl;
		return -1;
	}

	return 0;
}
