build: src/main.cpp
	if [ ! -d "./dist" ]; then \
        mkdir ./dist; \
    fi
	if [ ! -f "./dist/clockmaker" ]; then \
	    touch "./dist/clockmaker"; \
	fi

	clang++ src/*.cpp -o dist/clockmaker -DNDEBUG -v -ldl -lX11 -lvulkan -lGL -lGLU -lglut -lGLEW -lglfw -lpthread -lpng -lXrandr -lXi -lstdc++fs -std=c++17

build-debug: src/main.cpp
	if [ ! -d "./dist" ]; then \
        mkdir ./dist; \
    fi
	if [ ! -f "./dist/clockmaker" ]; then \
	    touch "./dist/clockmaker"; \
	fi

	clang++ src/*.cpp -o dist/clockmaker -g -v -ldl -lX11 -lvulkan -lGL -lGLU -lglut -lGLEW -lglfw -lglslang -lpthread -lpng -lXrandr -lXi -lstdc++fs -std=c++17

build-vk-debug: src/main.cpp
	if [ ! -d "./dist" ]; then \
        mkdir ./dist; \
    fi
	if [ ! -f "./dist/clockmaker" ]; then \
	    touch "./dist/clockmaker"; \
	fi

	clang++ src/*.cpp -o dist/clockmaker -g -DVK_DEBUG_FEATURES -v -ldl -lX11 -lvulkan -lGL -lGLU -lglut -lGLEW  -lglfw -lshaderc -lglslang -lOSDependent -lOGLCompiler -lSPIRV -lHLSL -lSPIRV-Tools -lpthread -lpng -lXrandr -lXi -lstdc++fs -std=c++17

run: dist/clockmaker
	./dist/clockmaker


clean: dist/clockmaker
	rm -f ./*.o ./*.gch **/*.gch **/*.o
