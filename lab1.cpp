#include <windows.h>
#include <string>

// Türk Alfabesi (29 Karakter)
std::wstring alphabet = L"ABCÇDEFGĞHIİJKLMNOÖPRSŞTUÜVYZ";

int getIndex(wchar_t c) {
    for (int i = 0; i < alphabet.length(); i++) {
        if (towupper(c) == alphabet[i]) return i;
    }
    return -1;
}

std::wstring encrypt(std::wstring text, std::wstring key) {
    std::wstring result = L"";
    std::wstring fullKey = key + text; 
    int keyIdx = 0;
    for (int i = 0; i < text.length(); i++) {
        int pIdx = getIndex(text[i]);
        if (pIdx == -1) continue;
        int kIdx = getIndex(fullKey[keyIdx++]);
        result += alphabet[(pIdx + kIdx) % 29];
    }
    return result;
}

std::wstring decrypt(std::wstring cipher, std::wstring key) {
    std::wstring result = L"";
    std::wstring currentKey = key;
    for (int i = 0; i < cipher.length(); i++) {
        int cIdx = getIndex(cipher[i]);
        if (cIdx == -1) continue;
        int kIdx = getIndex(currentKey[i]);
        wchar_t pChar = alphabet[(cIdx - kIdx + 29) % 29];
        result += pChar;
        currentKey += pChar;
    }
    return result;
}

HWND hEditMsg, hEditKey, hEditResEnc, hEditResDec;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            CreateWindowW(L"Static", L"Insert your message here:", WS_VISIBLE | WS_CHILD, 20, 20, 200, 20, hwnd, NULL, NULL, NULL);
            hEditMsg = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 220, 20, 200, 25, hwnd, NULL, NULL, NULL);
            
            CreateWindowW(L"Static", L"Insert your key here:", WS_VISIBLE | WS_CHILD, 20, 60, 200, 20, hwnd, NULL, NULL, NULL);
            hEditKey = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 220, 60, 200, 25, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"Button", L"Press to encrypt your message", WS_VISIBLE | WS_CHILD, 20, 110, 220, 40, hwnd, (HMENU)1, NULL, NULL);
            hEditResEnc = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY, 260, 117, 160, 25, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"Button", L"Press to decrypt your cipher", WS_VISIBLE | WS_CHILD, 20, 170, 220, 40, hwnd, (HMENU)2, NULL, NULL);
            hEditResDec = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY, 260, 177, 160, 25, hwnd, NULL, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            wchar_t msgBuf[256], keyBuf[256];
            GetWindowTextW(hEditMsg, msgBuf, 256);
            GetWindowTextW(hEditKey, keyBuf, 256);
            if (LOWORD(wParam) == 1) SetWindowTextW(hEditResEnc, encrypt(msgBuf, keyBuf).c_str());
            else if (LOWORD(wParam) == 2) SetWindowTextW(hEditResDec, decrypt(msgBuf, keyBuf).c_str());
            break;
        }
        case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"VigenereClass";
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    RegisterClassW(&wc);
    HWND hwnd = CreateWindowExW(0, L"VigenereClass", L"Vigenere Autokey System", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 400, 200, 470, 300, NULL, NULL, hInstance, NULL);
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    return 0;
}