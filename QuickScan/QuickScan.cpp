#include "ScreenSelector.h"
#include "ScreenshotProvider.h"
#include <iostream>
#include <Windows.h>  // Do obsługi schowka Windows
#include <fstream>    // Do zapisu obrazu jako pliku tymczasowego
#include <tesseract/baseapi.h>  // Do rozpoznawania tekstu z obrazu
#include <leptonica/allheaders.h>
#include <cstdio>

#pragma comment (lib, "crypt32.lib")

const char* SCREENSHOT_FILENAME = "screenshot.bmp";

void SetTextToClipboard(const std::string& text)
{
    if (!OpenClipboard(NULL))
    {
        // Obsłuż błąd
        return;
    }

    EmptyClipboard();

    int textLength = static_cast<int>(text.length());
    HGLOBAL hText = GlobalAlloc(GMEM_MOVEABLE, (textLength + 1) * sizeof(char));
    if (hText != NULL)
    {
        char* pText = static_cast<char*>(GlobalLock(hText));
        strcpy_s(pText, textLength + 1, text.c_str());
        GlobalUnlock(hText);

        SetClipboardData(CF_TEXT, hText);
    }

    CloseClipboard();
}

int main(int argc, char* argv[]) {

    Cordinates cordinates = getCordinates();

    CreateScreenshot(cordinates.x, cordinates.y, cordinates.width, cordinates.height, SCREENSHOT_FILENAME);
    
    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();


    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "pol")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    Pix* image = pixRead(SCREENSHOT_FILENAME);
    api->SetImage(image);
    
    char* outText = api->GetUTF8Text();
    printf("%s", outText);
    std::string ocrResult(outText);

    api->End();
    delete api;
    delete[] outText;
    pixDestroy(&image);

    SetTextToClipboard(ocrResult);

    remove(SCREENSHOT_FILENAME);

    return 0;
}
