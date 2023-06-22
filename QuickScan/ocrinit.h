#include "screenselector.h"
#include "screenshotprovider.h"
#include "clippboardmanager.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <cstdio>
#include <vector>

#pragma comment (lib, "crypt32.lib")

const char* SCREENSHOT_FILENAME = "screenshot.bmp";

void RecognizeText(tesseract::TessBaseAPI* api) {
    Cordinates cordinates = getCordinates();

    CreateScreenshot(cordinates.x, cordinates.y, cordinates.width, cordinates.height, SCREENSHOT_FILENAME);

    // Open input image with leptonica library
    Pix* image = pixRead(SCREENSHOT_FILENAME);
    api->SetImage(image);

    char* outText = api->GetUTF8Text();
    std::string ocrResult(outText);

    delete[] outText;
    pixDestroy(&image);

    SetTextToClipboard(ocrResult);

    if (remove(SCREENSHOT_FILENAME) != 0) {
        MessageBox(NULL, L"Unable to delete temporary screenshot file, check if QuickScan has all neccesary permissions.", L"QuickScan fatal error", MB_OK);
        exit(4);
    }
}

std::string getLanguageCfgStr() {
    tesseract::TessBaseAPI tess;

    if (tess.Init(nullptr, nullptr)) {
        std::cerr << "Cannot initialize tesseract" << std::endl;
    }

    std::vector<std::string> avaliableLangs;
    tess.GetAvailableLanguagesAsVector(&avaliableLangs);

    std::string languageString;
    for (int i = 0; i < avaliableLangs.size(); i++) {
        languageString += avaliableLangs[i];
        if (i < avaliableLangs.size() - 1) {
            languageString += "+";
        }
    }

    tess.End();
    return languageString;
}