#include "ScreenSelector.h"
#include "ScreenshotProvider.h"

using namespace std;



int main(int argc, char* argv[]) {

    Cordinates cordinates = getCordinates();
    cout << "Cordinates are: " << endl;
    cout << cordinates.x << endl;
    cout << cordinates.y << endl;
    cout << cordinates.width << endl;
    cout << cordinates.height << endl;

    GetScreenShot(cordinates.x, cordinates.y, cordinates.width, cordinates.height);


    return 0;
}
