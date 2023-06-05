#include <Windows.h>
#include "ocrinit.h"

int main(int argv, char** args) {
    bool shiftPressed = false;
    bool altPressed = false;
    bool sPressed = false;

    while (true) {
        if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
            shiftPressed = true;
        }
        else {
            shiftPressed = false;
        }

        if (GetAsyncKeyState(VK_MENU) & 0x8000) {
            altPressed = true;
        }
        else {
            altPressed = false;
        }

        if (GetAsyncKeyState('S') & 0x8000) {
            sPressed = true;
        }
        else {
            sPressed = false;
        }

        if (shiftPressed && altPressed && sPressed) {
            RecognizeText();
        }
    }

    return 0;
}
