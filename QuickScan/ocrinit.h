#include "screenselector.h"
#include "screenshotprovider.h"
#include "clippboardmanager.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <cstdio>

#pragma comment (lib, "crypt32.lib")

const char* SCREENSHOT_FILENAME = "screenshot.bmp";

void RecognizeText() {
    Cordinates cordinates = getCordinates();

    CreateScreenshot(cordinates.x, cordinates.y, cordinates.width, cordinates.height, SCREENSHOT_FILENAME);

    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();


    // Initialize tesseract-ocr with Polish, without specifying tessdata path
    if (api->Init(NULL, "pol+eng")) {
        exit(1);
    }

    // Open input image with leptonica library
    Pix* image = pixRead(SCREENSHOT_FILENAME);
    api->SetImage(image);

    char* outText = api->GetUTF8Text();
    std::string ocrResult(outText);

    api->End();
    delete api;
    delete[] outText;
    pixDestroy(&image);

    SetTextToClipboard(ocrResult);

    if (remove(SCREENSHOT_FILENAME) != 0) {
        exit(2);
    }
}