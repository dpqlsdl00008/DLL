#pragma once

#include <Windows.h>
#include "Memory.h"

namespace IMEHook
{
    // 메이플스토리 내부 함수 주소
    DWORD CWndMan__EnableIME_Addr = 0x9E85F3;
    DWORD CUIStatusBar__EndChat_Addr = 0x008D5AAF;
    DWORD CCtrlEdit__OnSetFocus_Addr = 0x004CA043;
    DWORD CCtrlEdit__OnDestroy_Addr = 0x004CA619;
    DWORD CCtrlMLEdit__OnSetFocus_Addr = 0x004D32AB;
    DWORD CCtrlMLEdit__OnDestroy_Addr = 0x004D384D;

    // 함수 타입 정의
    typedef int(__fastcall* CUIStatusBar__EndChat_t)(void* pThis, void* edx);
    static auto CUIStatusBar__EndChat = reinterpret_cast<CUIStatusBar__EndChat_t>(CUIStatusBar__EndChat_Addr);

    typedef int(__fastcall* CWndMan__EnableIME_t)(void* pThis, void* edx, int bEnable);
    static auto CWndMan__EnableIME = reinterpret_cast<CWndMan__EnableIME_t>(CWndMan__EnableIME_Addr);

    typedef int(__fastcall* CCtrlEdit__OnSetFocus_t)(void* pThis, void* edx, int bFocus);
    static auto CCtrlEdit__OnSetFocus = reinterpret_cast<CCtrlEdit__OnSetFocus_t>(CCtrlEdit__OnSetFocus_Addr);
    static auto CCtrlMLEdit__OnSetFocus = reinterpret_cast<CCtrlEdit__OnSetFocus_t>(CCtrlMLEdit__OnSetFocus_Addr);

    typedef int(__fastcall* CCtrlEdit__OnDestroy_t)(void* pThis, void* edx);
    static auto CCtrlEdit__OnDestroy = reinterpret_cast<CCtrlEdit__OnDestroy_t>(CCtrlEdit__OnDestroy_Addr);
    static auto CCtrlMLEdit__OnDestroy = reinterpret_cast<CCtrlEdit__OnDestroy_t>(CCtrlMLEdit__OnDestroy_Addr);

    // 윈도우 매니저 인스턴스 가져오기
    DWORD Get_TSingleton_CWndMan__ms_pInstance() {
        return *(DWORD*)(0x00BEC20C);
    }

    // IME 활성화
    int EnableIME() {
        return CWndMan__EnableIME((void*)Get_TSingleton_CWndMan__ms_pInstance(), nullptr, 1);
    }

    // IME 비활성화
    int DisableIME() {
        return CWndMan__EnableIME((void*)Get_TSingleton_CWndMan__ms_pInstance(), nullptr, 0);
    }

    // CCtrlEdit::OnSetFocus 훅
    bool Detour_CCtrlEdit__OnSetFocus() {
        CCtrlEdit__OnSetFocus_t Hook_Edit = [](void* pThis, void* edx, int bFocus) -> int {

            DWORD pParent = *(DWORD*)((DWORD)pThis + 0x24);
            DWORD bPassword = *(DWORD*)((DWORD)pThis + 0x80);
            DWORD height = *(DWORD*)((DWORD)pThis + 0x20);
            DWORD CCtrlWnd__IGObj__vftable_OnCreate_Addr = *(DWORD*)(*(DWORD*)pThis + 0x3C);

            typedef void(__fastcall* CCtrlWnd__IGObj__vftable_OnCreate_t)(void* pThis, void* edx, void* sStrDefault);
            static auto CCtrlWnd__IGObj__vftable_OnCreate =
                reinterpret_cast<CCtrlWnd__IGObj__vftable_OnCreate_t>(CCtrlWnd__IGObj__vftable_OnCreate_Addr);

            typedef void(__fastcall* CWnd__InvalidateRect_t)(void* pThis, void* edx, void* pRect);
            static auto CWnd__InvalidateRect = reinterpret_cast<CWnd__InvalidateRect_t>(0x009E04C9);

            typedef void(__fastcall* CUserLocal__OnKeyDownSkillEnd_t)(void* pThis, void* edx);
            static auto CUserLocal__OnKeyDownSkillEnd = reinterpret_cast<CUserLocal__OnKeyDownSkillEnd_t>(0x0095BEDF);

            typedef void(__fastcall* CWndMan__ClearComposition_t)(void* pThis, void* edx);
            static auto CWndMan__ClearComposition = reinterpret_cast<CWndMan__ClearComposition_t>(0x009E85E7);

            DWORD TSingleton_CUserLocal__ms_pInstance = *(DWORD*)0x00BEBF98;

            if (!bFocus) {
                // 입력창이 포커스를 잃으면 IME 비활성화
                CCtrlWnd__IGObj__vftable_OnCreate(pThis, nullptr, (void*)0x00AF1734);
                DisableIME();
                CWnd__InvalidateRect((void*)height, nullptr, 0);
                return 1;
            }

            if (!pParent) {
                return 1;
            }

            if (TSingleton_CUserLocal__ms_pInstance && *(DWORD*)(TSingleton_CUserLocal__ms_pInstance + 3149)) {
                CUserLocal__OnKeyDownSkillEnd((void*)TSingleton_CUserLocal__ms_pInstance, nullptr);
            }
            CWndMan__ClearComposition((void*)Get_TSingleton_CWndMan__ms_pInstance(), nullptr);

            if (!bPassword) {
                // 비밀번호 입력창이 아니면 IME 활성화
                EnableIME();
            }
            else {
                // 비밀번호 입력창이면 IME 비활성화
                DisableIME();
            }

            CWnd__InvalidateRect((void*)height, nullptr, 0);
            return 1;
            };

        return Memory::SetHook(true, reinterpret_cast<void**>(&CCtrlEdit__OnSetFocus), Hook_Edit);
    }

    // CCtrlMLEdit::OnSetFocus 훅 (멀티라인 입력창)
    bool Detour_CCtrlMLEdit__OnSetFocus() {
        CCtrlEdit__OnSetFocus_t Hook_MLEdit = [](void* pThis, void* edx, int bFocus) -> int {

            DWORD pParent = *(DWORD*)((DWORD)pThis + 0x24);
            DWORD bPassword = *(DWORD*)((DWORD)pThis + 0x80);
            DWORD height = *(DWORD*)((DWORD)pThis + 0x20);
            DWORD CCtrlWnd__IGObj__vftable_OnCreate_Addr = *(DWORD*)(*(DWORD*)pThis + 0x3C);

            typedef void(__fastcall* CCtrlWnd__IGObj__vftable_OnCreate_t)(void* pThis, void* edx, void* sStrDefault);
            static auto CCtrlWnd__IGObj__vftable_OnCreate =
                reinterpret_cast<CCtrlWnd__IGObj__vftable_OnCreate_t>(CCtrlWnd__IGObj__vftable_OnCreate_Addr);

            typedef void(__fastcall* CWnd__InvalidateRect_t)(void* pThis, void* edx, void* pRect);
            static auto CWnd__InvalidateRect = reinterpret_cast<CWnd__InvalidateRect_t>(0x009E04C9);

            typedef void(__fastcall* CUserLocal__OnKeyDownSkillEnd_t)(void* pThis, void* edx);
            static auto CUserLocal__OnKeyDownSkillEnd = reinterpret_cast<CUserLocal__OnKeyDownSkillEnd_t>(0x0095BEDF);

            typedef void(__fastcall* CWndMan__ClearComposition_t)(void* pThis, void* edx);
            static auto CWndMan__ClearComposition = reinterpret_cast<CWndMan__ClearComposition_t>(0x009E85E7);

            DWORD TSingleton_CUserLocal__ms_pInstance = *(DWORD*)0x00BEBF98;

            if (!bFocus) {
                CCtrlWnd__IGObj__vftable_OnCreate(pThis, nullptr, (void*)0x00AF1734);
                DisableIME();
                CWnd__InvalidateRect((void*)height, nullptr, 0);
                return 1;
            }

            if (!pParent) {
                return 1;
            }

            if (TSingleton_CUserLocal__ms_pInstance && *(DWORD*)(TSingleton_CUserLocal__ms_pInstance + 3149)) {
                CUserLocal__OnKeyDownSkillEnd((void*)TSingleton_CUserLocal__ms_pInstance, nullptr);
            }
            CWndMan__ClearComposition((void*)Get_TSingleton_CWndMan__ms_pInstance(), nullptr);

            if (!bPassword) {
                EnableIME();
            }
            else {
                DisableIME();
            }

            CWnd__InvalidateRect((void*)height, nullptr, 0);
            return 1;
            };

        return Memory::SetHook(true, reinterpret_cast<void**>(&CCtrlMLEdit__OnSetFocus), Hook_MLEdit);
    }

    // 라틴 문자 체크 비활성화 (한글/중국어 입력 허용)
    bool DisableLatinCheck() {
        MessageBoxA(NULL, "DisableLatinCheck 실행됨", "Debug", MB_OK);
        // 캐릭터 이름 유효성 검사 우회
        Memory::PatchNop(0x007A015D, 2);

        // CUIStatusBar::OnKey - IsDBCSLeadByte() 로직 수정
        Memory::PatchNop(0x008D54A8, 7);

        // CUser::OnChat - IsDBCSLeadByte() 로직 수정
        Memory::PatchNop(0x00937227, 7);

        // CField::OnGroupMessage
        Memory::PatchNop(0x00531EEA, 7);

        // CCtrlEdit::CopyClipBoard - 채팅 시 텍스트 복사 문제 해결
        Memory::PatchNop(0x004CAE7D, 2);
        Memory::PatchNop(0x004CAEA4, 2);

        // 한글 줄바꿈 수정 (추가)
        Memory::PatchNop(0x008E4252, 2);

        return true;
    }

    // IME 훅 초기화 함수
    bool InitIMEHooks()
    {
        bool bResult = true;

        bResult &= Detour_CCtrlEdit__OnSetFocus();
        bResult &= Detour_CCtrlMLEdit__OnSetFocus();
        bResult &= DisableLatinCheck();

        return bResult;
    }
}