﻿#include <Windows.h>
#include <iostream>
#include "ocrinit.h"


int main(int argc, char* argv[]) {
	if (RegisterHotKey(NULL, 1, MOD_ALT | MOD_SHIFT, 0x53) == 0 || RegisterHotKey(NULL, 2, MOD_ALT | MOD_SHIFT, 0x51) == 0) {
		MessageBox(NULL, L"Cannot register hotkeys.", L"QuickScan fatal error", MB_OK);
		exit(1);
	}

	MSG msg;
	bool running = true;

	//tesseract initialization
	tesseract::TessBaseAPI* tess = new tesseract::TessBaseAPI();
	if (tess->Init(NULL, getLanguageCfgStr().c_str())) {
		MessageBox(NULL, L"Tesseract initialization failed.", L"QuickScan fatal error", MB_OK);
		exit(2);
	}

	while (running) {
		if (GetMessage(&msg, NULL, 0, 0) > 0) {
			if (msg.message == WM_HOTKEY) {
				switch (msg.wParam)
				{
				case 1:
					RecognizeText(tess);
					break;
				case 2:
					running = false;
					MessageBox(NULL, L"QuickScan has been stopped.", L"QuickScan", MB_OK);
					break;

				default:
					break;
				}
			}
		}
	}
}