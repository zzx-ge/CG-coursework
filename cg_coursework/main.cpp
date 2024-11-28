#include "window.h";
#include "adapter.h"
#include "shader.h"
#include "vertex.h"
#include "Timer.h"
#include "gamecontrol.h"

extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	bool running = true;
	Window win;
	DXcore dx;
	win.create("zzx", 1024, 1024);
	dx.init(1024, 1024, win.hwnd, false); 
	Camera camera(45, 1, 0.1f, 100.f, 20);
	camera.init();
	Shaders shader(&dx);
	shader.init("PS.txt", "VS.txt", camera);
	//GEMModel bunny("Resources/bunny.gem", dx.device);
	Plane plane;
	plane.init(dx);
	shader.updateConstantVS("TransformBuffer", "W", &(plane.world));
	gm gamemanager;
	TIMER timer;
	while (running) {
		gamemanager.dt = timer.getDeltaTime();
		dx.clear();
		win.processMessages();
		shader.apply();
		if (win.keyPressed('Q')) {
			running = false;
		} 
		plane.Draw(dx);

		//HandleMouseInput(win.hwnd, camera, gamemanager.dt, gamemanager.lastX, gamemanager.lastY);
		dx.present();
	}
}