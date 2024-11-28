#pragma once
#include <Windows.h>
#include <iostream>

class Window
{
public:
	HWND hwnd; //Where to store the window
	HINSTANCE hinstance; 
	std::string name;
	int width;
	int height;
	bool keys[256];
	int mousex;
	int mousey;
	bool mouseButtons[3];
	int mouseWheel;

	//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void create(std::string window_name, int _window_width, int _window_height, int window_x = 0, int window_y = 0);
	void processMessages();
	void updateMouse(int x, int y) {
		mousex = x;
		mousey = y;
	}
	bool keyPressed(int key)
	{
		return keys[key];
	}
};
