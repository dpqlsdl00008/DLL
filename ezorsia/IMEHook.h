#pragma once

#include <Windows.h>
#include <immdev.h>
#include "Memory.h"

#pragma comment(lib, "imm32.lib")

namespace IMEHook
{
    bool Hook_PeekMessageA()
    {
        static auto pPeekMessageA = decltype(&PeekMessageA)(GetProcAddress(GetModuleHandleA("USER32"),
            "PeekMessageA"));

        decltype(&PeekMessageA) Hook = [](LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT
            wRemoveMsg) -> BOOL {
                return PeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
            };

        return Memory::SetHook(true, reinterpret_cast<void**>(&pPeekMessageA), Hook);
    }

    bool Hook_ImmAssociateContext()
    {
        static auto _ImmAssociateContext = decltype(&ImmAssociateContext)(GetProcAddress(GetModuleHandleA("IMM32"),
            "ImmAssociateContext"));

        decltype(&ImmAssociateContext) Hook = [](HWND hWnd, HIMC hIMC) -> HIMC {
            HIMC overrideHIMC = ImmGetContext(hWnd);
            ImmSetOpenStatus(overrideHIMC, TRUE);
            return _ImmAssociateContext(hWnd, overrideHIMC);
            };

        return Memory::SetHook(true, reinterpret_cast<void**>(&_ImmAssociateContext), Hook);
    }

    bool Hook_GetTextExtentPoint32A()
    {
        static auto pGetTextExtentPoint32A =
            decltype(&GetTextExtentPoint32A)(GetProcAddress(GetModuleHandleA("GDI32"), "GetTextExtentPoint32A"));

        decltype(&GetTextExtentPoint32A) Hook = [](HDC hdc, LPCSTR lpString, int c, LPSIZE psizl) -> BOOL {
            wchar_t* wideStr = new wchar_t[2 * c + 2];
            int len = MultiByteToWideChar(CP_UTF8, 0, lpString, c, wideStr, c);
            wideStr[len] = '\0';
            BOOL result = GetTextExtentPoint32W(hdc, wideStr, len, psizl);
            delete[] wideStr;
            return result;
            };

        return Memory::SetHook(true, reinterpret_cast<void**>(&pGetTextExtentPoint32A), Hook);
    }

    bool Hook_MultiByteToWideChar()
    {
        static auto _MultiByteToWideChar = decltype(&MultiByteToWideChar)(GetProcAddress(GetModuleHandleA("KERNEL32"),
            "MultiByteToWideChar"));

        decltype(&MultiByteToWideChar) Hook = [](UINT CodePage, DWORD dwFlags, LPCCH lpMultiByteStr, int cbMultiByte,
            LPWSTR lpWideCharStr, int cchWideChar) -> int {
                CodePage = CP_UTF8;
                return _MultiByteToWideChar(CodePage, dwFlags, lpMultiByteStr, cbMultiByte, lpWideCharStr, cchWideChar);
            };

        return Memory::SetHook(true, reinterpret_cast<void**>(&_MultiByteToWideChar), Hook);
    }

    bool Hook_WideCharToMultiByte()
    {
        static auto _WideCharToMultiByte = decltype(&WideCharToMultiByte)(GetProcAddress(GetModuleHandleA("KERNEL32"),
            "WideCharToMultiByte"));

        decltype(&WideCharToMultiByte) Hook = [](UINT CodePage, DWORD dwFlags, LPCWCH lpWideCharStr, int cchWideChar,
            LPSTR lpMultiByteStr, int cbMultiByte, PCCH lpDefaultChar, LPBOOL lpUsedDefaultChar) -> int {
                CodePage = CP_UTF8;
                return _WideCharToMultiByte(CodePage, dwFlags, lpWideCharStr, cchWideChar, lpMultiByteStr, cbMultiByte,
                    lpDefaultChar, lpUsedDefaultChar);
            };

        return Memory::SetHook(true, reinterpret_cast<void**>(&_WideCharToMultiByte), Hook);
    }

    bool Hook_TextOutA()
    {
        static auto pTextOutA = decltype(&TextOutA)(GetProcAddress(GetModuleHandleA("GDI32"), "TextOutA"));

        decltype(&TextOutA) Hook = [](HDC hdc, int x, int y, LPCSTR lpString, int c) -> BOOL {
            wchar_t* wideStr = new wchar_t[2 * c + 2];
            int len = MultiByteToWideChar(CP_UTF8, 0, lpString, c, wideStr, c);
            wideStr[len] = '\0';
            BOOL result = TextOutW(hdc, x, y, wideStr, len);
            delete[] wideStr;
            return result;
            };

        return Memory::SetHook(true, reinterpret_cast<void**>(&pTextOutA), Hook);
    }

    bool Hook_CharPrevA()
    {
        static auto pCharPrevA = decltype(&CharPrevA)(GetProcAddress(GetModuleHandleA("USER32"), "CharPrevA"));

        decltype(&CharPrevA) Hook = [](LPCSTR lpszStart, LPCSTR lpszCurrent) -> LPSTR {
            LPCSTR result = lpszCurrent;
            char pCurrentChar;
            do {
                if (result <= lpszStart)
                    break;
                pCurrentChar = *--result;
            } while ((pCurrentChar & 0xC0) == 0x80);
            return (LPSTR)result;
            };

        return Memory::SetHook(true, reinterpret_cast<void**>(&pCharPrevA), Hook);
    }

    bool Hook_CharNextA()
    {
        static auto pCharNextA = decltype(&CharNextA)(GetProcAddress(GetModuleHandleA("USER32"), "CharNextA"));

        decltype(&CharNextA) Hook = [](LPCSTR lpCurrentChar) -> LPSTR {
            LPCSTR result = lpCurrentChar;
            char pCurrentChar;
            if (*result) {
                do {
                    pCurrentChar = *++result;
                } while ((pCurrentChar & 0xC0) == 0x80);
            }
            return (LPSTR)result;
            };

        return Memory::SetHook(true, reinterpret_cast<void**>(&pCharNextA), Hook);
    }

    // IME 후킹 초기화 함수
    bool InitIMEHooks()
    {
        bool bResult = true;

        bResult &= Hook_WideCharToMultiByte();
        bResult &= Hook_MultiByteToWideChar();
        bResult &= Hook_CharNextA();
        bResult &= Hook_CharPrevA();
        bResult &= Hook_TextOutA();
        bResult &= Hook_GetTextExtentPoint32A();
        bResult &= Hook_ImmAssociateContext();
        bResult &= Hook_PeekMessageA();

        return bResult;
    }
}
