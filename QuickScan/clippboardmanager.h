#include <Windows.h>

bool SetTextToClipboard(const std::string& text)
{
    if (!OpenClipboard(NULL))
    {
        return false;
    }

    EmptyClipboard();

    int textLength = static_cast<int>(text.length());
    int bufferSize = (textLength + 1) * sizeof(wchar_t);

    HGLOBAL hText = GlobalAlloc(GMEM_MOVEABLE, bufferSize);
    if (hText != NULL)
    {
        wchar_t* pText = static_cast<wchar_t*>(GlobalLock(hText));
        if (pText != NULL)
        {
            MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, pText, textLength);
            GlobalUnlock(hText);

            SetClipboardData(CF_UNICODETEXT, hText);
        }
    }

    CloseClipboard();

    return true;
}