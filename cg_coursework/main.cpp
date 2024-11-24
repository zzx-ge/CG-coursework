#include "window.h";
#include "adapter.h"
#include "shader.h"
#include "vertex.h"
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	DXcore dx;
	win.create("zzx", 1024, 1024);
	dx.init(1024, 1024, win.hwnd, false); 
	Shaders shader;
	shader.init("PS.txt", "VS.txt", dx);
	Triangle tr1;
	tr1.init(dx);
	while (true) {
		dx.clear();
		win.processMessages();
		shader.apply(dx);
		tr1.draw(dx);
		dx.present();
	}
}