#include "stdafx.h"
#include "ReplacementFuncs.h"
#include "dinput8.h"

// Executed after the client is unpacked
void MainFunc() 
{

#pragma region Ezorsia Prepackaged

	Hook_sub_44E88E(true);//not re-written//unlikely conflict, section virtualized by default //sub_44E88E	end	0044EA61 //int (__stdcall *__stdcall MyGetProcAddress(HINSTANCE__ *hModule, const char *lpProcName))()
	Hook_sub_494CA3(true);//seems to work//unlikely conflict, section virtualized by default //sub_494CA3  end 00494D04 //CClientSocket::Connect(CClientSocket *this, CClientSocket::CONNECTCONTEXT *ctx)
	Hook_sub_494D07(true);//seems to work//unlikely conflict, section virtualized by default //sub_494D07	end	00494D2C //unnamed function, part of CClientSocket::Connect in v83, merged together with connect in v95
	Hook_sub_494D2F(true);//seems to work//unlikely conflict, section virtualized by default //sub_494D2F	end	00494ECE //CClientSocket::Connect(CClientSocket *this, sockaddr_in *pAddr)
	Hook_sub_9F4E54(true);//rewritten friendly CrC non-checker//unlikely conflict, section virtualized by default //sub_9F4E54  end 009F4F08	//likely: unsigned int __cdecl Crc32_GetCrc32_VMTable(unsigned int* pmem, unsigned int size, unsigned int* pcheck, unsigned int *pCrc32) 
	Hook_sub_9F4FDA(true);//seems to work//unlikely conflict, section virtualized by default //sub_9F4FDA end 009F51D3 //void __thiscall CWvsApp::CWvsApp(CWvsApp *this, const char *sCmdLine)
	Hook_sub_9F5239(true);//seems to work//unlikely conflict, section virtualized by default //sub_9F5239 end 009F5C4F //void __thiscall CWvsApp::SetUp(CWvsApp *this)
	Hook_sub_9F5C50(true);//seems to work//unlikely conflict, section virtualized by default //sub_9F5C50  end 009F698D //void __thiscall CWvsApp::Run(CWvsApp *this, int *pbTerminate)
	Hook_sub_9F7CE1(true);//seems to work//unlikely conflict, section virtualized by default //sub_9F7CE1 end 009F821E //void __thiscall CWvsApp::InitializeInput(CWvsApp *this)
	Hook_sub_9F84D0(true);//seems to work//unlikely conflict, section virtualized by default //sub_9F84D0 end 009F8B5E //void __thiscall CWvsApp::CallUpdate(CWvsApp *this, int tCurTime)
	HookCWvsApp__Dir_BackSlashToSlash(true);//rewritten but minor utility //sub_9F95FE	end 009F9620
	Hook_sub_9F7964(true);//re-written for testing//HRESULT __thiscall IWzFileSystem::Init(IWzFileSystem *this, Ztl_bstr_t sPath)
	Hook_sub_9F7159(true);//added on some stuff//void __thiscall CWvsApp::InitializeResMan(CWvsApp *this)//sub_9F7159 end 009F7909 //experimental //ty to all the contributors of the ragezone release: Client load .img instead of .wz v62~v92
	Hook_StringPool__GetString(true);//no conflicts, only modifies return //hook stringpool modification //ty !! popcorn //ty darter
	Hook_CUIStatusBar__ChatLogAdd(false);//@@still crashes	//potential conflicts //sub_8DB070	end 008DB45A //custom Set Any Chat Bar Limit (default is 64) //ty Spiderman
	Hook_sub_425ADD(true);//not re-written, just tracking//void __thiscall Ztl_bstr_t::Ztl_bstr_t(Ztl_bstr_t *this, const char *s)
	Hook_sub_9F51F6(true);//not re-written, just tracking process exit for tests//void __thiscall CWvsApp::~CWvsApp(CWvsApp *this)
	Hook_sub_9FCD88(true);//not re-written, just tracking process exit for tests//void __thiscall <IWzSeekableArchive(IWzSeekableArchive* this, IUnknown* p)
	Hook_sub_5D995B(true);//re-written!!may conflict//Ztl_variant_t *__thiscall IWzNameSpace::Getitem(IWzNameSpace *this, Ztl_variant_t *result, Ztl_bstr_t sPath)
	Hook_sub_4032B2(true);//not re-written, just tracking process exit for tests//IUnknown* __thiscall Ztl_variant_t::GetUnknown(Ztl_variant_t* this, bool fAddRef, bool fTryChangeType)

#pragma endregion

	Client::UpdateGameStartup();
	Client::AdventureMS();
	Client::UpdateResolution();
	dinput8::CreateHook();
}

// Gatekeeper Thread
void MainProc() { MainMain::CreateInstance(MainFunc); }

// DLL Insertion
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	{	
		//MainMain::CreateConsole(MainMain::stream);//console for devs, use this to log stuff if you want
		MainMain::mainTHread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, GetCurrentThreadId());
		Hook_CreateMutexA(true);//Sleep(42);//not in-game edit //multiclient //ty darter, angel, and alias! //new one: credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
		Hook_WSPStartup(true);//allows to set IP for default installed client//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
		Hook_CreateWindowExA(true);//not in-game edit//enables minimize button //default ezorsia
		Hook_FindFirstFileA(true);//kills check for .dll already in dir//ty joo for advice with this, check out their releases: https://github.com/OpenRustMS
		Hook_GetACP(true);//either kills locale checks or feeds the server custom data//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
		Hook_GetModuleFileNameW(true);//not in-game edit//better call _GetModuleFileNameW more flexible//default ezorsia
		Hook_GetLastError(false);//for some reason crashes at hooking, still dunno why//mostly for deving, prints last error to console

		// Things That Can Be Enabled
		//INITWINHOOK("KERNEL32", "OpenProcess", OpenProcess_Original, OpenProcess_t, WinHooks::OpenProcess_Hook); //Used to track what processes Maple opens.
		//INITWINHOOK("KERNEL32", "CreateProcessW", CreateProcessW_Original, CreateProcessW_t, WinHooks::CreateProcessW_Hook); //Used to track what maple is trying to start (mainly for anticheat modules).
		//INITWINHOOK("KERNEL32", "CreateProcessA", CreateProcessA_Original, CreateProcessA_t, WinHooks::CreateProcessA_Hook); //Used same as above and also to kill/redirect some web requests
		//INITWINHOOK("KERNEL32", "OpenMutexA", OpenMutexA_Original, OpenMutexA_t, WinHooks::OpenMutexA_Hook);//In some versions, Maple calls this library function to check if the anticheat has started. We can spoof this and return a fake handle for it to close.
		//INITWINHOOK("KERNEL32", "RegCreateKeyExA", RegCreateKeyExA_Original, RegCreateKeyExA_t, WinHooks::RegCreateKeyExA_Hook); //Maplestory saves registry information (config stuff) for a number of things. This can be used to track that.
		//INITWINHOOK("KERNEL32", "GetProcAddress", GetProcAddress_Original, GetProcAddress_t, WinHooks::GetProcAddress_Hook); //Used to map out imports used by MapleStory
		//INITWINHOOK("NTDLL", "NtTerminateProcess", NtTerminateProcess_Original, NtTerminateProcess_t, WinHooks::NtTerminateProcess_Hook); //We use this function to track what memory addresses are killing the process,There are more ways that Maple kills itself, but this is one of them.
		
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&MainProc, NULL, 0, 0);
		break;
	}
	default: break;
	case DLL_PROCESS_DETACH:
	{
		MainMain::GetInstance()->~MainMain();
		break;
	} }
	return TRUE;
}