//
// Created by pawelwuuu on 31.05.2023.
//

#ifndef UNTITLED_SCREENSHOTPROVIDER_H
#define UNTITLED_SCREENSHOTPROVIDER_H

#endif //UNTITLED_SCREENSHOTPROVIDER_H

void SaveHBITMAPToFile(HBITMAP hBitmap, int width, int height, const char* filename)
{
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24; // Assuming 24-bit RGB format
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;

    // Create file
    HANDLE file = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
    {
        // Error handling
        return;
    }

    // Write BITMAPFILEHEADER
    BITMAPFILEHEADER bmfHeader;
    bmfHeader.bfType = 0x4D42; // "BM"
    DWORD dwSizeImage = ((width * 24 + 31) & ~31) / 8 * height; // Calculate image size
    bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwSizeImage;
    bmfHeader.bfReserved1 = 0;
    bmfHeader.bfReserved2 = 0;
    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    DWORD dwBytesWritten;
    WriteFile(file, &bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(file, &(bmi.bmiHeader), sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);

    // Get bitmap data
    BYTE* pBits = new BYTE[dwSizeImage];
    HDC hDC = CreateCompatibleDC(NULL);
    HGDIOBJ hOld = SelectObject(hDC, hBitmap);
    GetDIBits(hDC, hBitmap, 0, height, pBits, &bmi, DIB_RGB_COLORS);

    // Write bitmap data
    WriteFile(file, pBits, dwSizeImage, &dwBytesWritten, NULL);

    // Clean up
    SelectObject(hDC, hOld);
    DeleteDC(hDC);
    CloseHandle(file);
    delete[] pBits;
}

void CreateScreenshot(int x, int y, int width, int height, const char* filename)
{
    // Copy screen to bitmap
    HDC hScreen = GetDC(NULL);
    HDC hDC = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, width, height);
    HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
    BOOL bRet = BitBlt(hDC, 0, 0, width, height, hScreen, x, y, SRCCOPY);

    // Save bitmap to file
    SaveHBITMAPToFile(hBitmap, width, height, filename);

    // Clean up
    SelectObject(hDC, old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);
    DeleteObject(hBitmap);
}