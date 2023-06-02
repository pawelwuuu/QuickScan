//
// Created by pawelwuuu on 31.05.2023.
//

#ifndef UNTITLED_SCREENSHOTPROVIDER_H
#define UNTITLED_SCREENSHOTPROVIDER_H

#endif //UNTITLED_SCREENSHOTPROVIDER_H

#include <windows.h>

void GetScreenShot(int x, int y, int width, int height)
{
    // copy screen to bitmap
    HDC     hScreen = GetDC(NULL);
    HDC     hDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width, height);
    HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
    BOOL    bRet = BitBlt(hDC, 0, 0, width, height, hScreen, x, y, SRCCOPY);

    // save bitmap to clipboard
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hBitmap);
    CloseClipboard();

    // clean up
    SelectObject(hDC, old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);
    DeleteObject(hBitmap);
}