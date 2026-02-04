#pragma once
#include "AutoTypes.h"
#include "MapleClientCollectionTypes\ZtlSecure.h"
#include "MapleClientCollectionTypes\ZRef.h"

//notes from my knowledge as i have not used these kinds of codes practically well
//function replacement is when you replace the original function in the client with your own fake function, usually to add some extra functionality
//for more complex applications you would also need to define the client's variables and reinterpret_cast those (no void this time)
//you need the right calling convention (match client's original or use _fastcall, i havent tried it much)
//it would help to know the benefits and drawbacks of "reinterpret_cast", as well as how it is hooking to prevent accidents
//hooking to the original function will replace it at all times when it is called by the client
//i personally have not tried it more because it requires a very thorough understanding of how the client code works, re-making the parts here,
//and using it, all together, in a way that doesnt break anything
//it would be the best way to do it for very extensive client edits and if you need to replace entire functions in that context but
//code caving is generally easier for short term, one-time patchwork fixes	//thanks you teto for helping me on this learning journey

#pragma region AdventureMS

#pragma region Maker Changes

/*
std::unordered_map<int, std::string> stringPool;

bool Hook_StringPool__GetString(bool bEnable)
{
	typedef ZXString<char>* (__fastcall* StringPool__GetString_t)(void* ecx, void* edx, ZXString<char>* result, int nIdx);
	static auto StringPool__GetString = reinterpret_cast<StringPool__GetString_t>(0x00406455);
	StringPool__GetString_t Hook = [](void* ecx, void* edx, ZXString<char>* result, int nIdx) -> ZXString<char>*
	{
		auto ret = StringPool__GetString(ecx, edx, result, nIdx);

		if (stringPool.find(nIdx) != stringPool.end()) 
		{
			result->Assign(stringPool[nIdx].c_str());
		}
			
		return ret;
	};

	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&StringPool__GetString), Hook);
}

void ReplaceStringPool(int id, std::string string)
{
	stringPool[id] = string;
}

void StringPoolStuff()
{
	Hook_StringPool__GetString(true);

	ReplaceStringPool(9999, "Other");
}*/

#pragma endregion


#pragma region Trinket Slot (Shoulder)

/*

// Trinket Slot (Shoulder)
typedef ZXString<char>* (__cdecl* GetItemCategoryNameType)(DWORD* a1, signed int nItemId);
static GetItemCategoryNameType GetItemCategoryName = reinterpret_cast<GetItemCategoryNameType>(0x005C9E61);
typedef int(__fastcall* CUIEquipGetBodyPartFromPointType)(void* pThis, void* edx, int rx, int ry);
static CUIEquipGetBodyPartFromPointType CUIEquipGetBodyPartFromPoint = reinterpret_cast<CUIEquipGetBodyPartFromPointType>(0x007FEC32);
typedef int(__cdecl* GetBodyPartFromItemType)(int nItemID, int nGender, int* pnBodyPart, int bAll);
static GetBodyPartFromItemType GetBodyPartFromItem = reinterpret_cast<GetBodyPartFromItemType>(0x004606A0);
typedef bool(__cdecl* IsCorrentBodyPartType)(int nItemID, int nBodyPart, int nGender);
static IsCorrentBodyPartType IsCorrentBodyPart = reinterpret_cast<IsCorrentBodyPartType>(0x00460358);

void* GetItemCategoryName_Hook(DWORD* result, int nItemId)
{
	auto ret = GetItemCategoryName(result, nItemId);
	if (nItemId / 10000 == 115)
	{
		ret->Assign("Trinket");
		// result = (DWORD*)ret->m_pStr; Testing
	}
	return ret;
}

bool HookTrinketCategory(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&GetItemCategoryName), GetItemCategoryName_Hook);
}

int __cdecl GetBodyPartFromItem_Hook(int nItemID, int nGender, int* pnBodyPart, int bAll)
{
	int type = nItemID / 10000;

	if (type == 115)
	{
		*pnBodyPart = 51;
		return 51;
	}

	else
	{
		return GetBodyPartFromItem(nItemID, nGender, pnBodyPart, bAll);
	}
}
bool __cdecl IsCorrentBodyPart_Hook(int nItemID, int nBodyPart, int nGender)
{
	bool ret = IsCorrentBodyPart(nItemID, nBodyPart, nGender);

	if (nItemID / 10000 == 115)
	{
		ret = nBodyPart == 51;
	}

	return ret;
}

int __fastcall CUIEquip_GetBodyPartFromPoint_Hook(void* pThis, void* edx, int rx, int ry)
{
	int equipSlot = getEquipGridPosition(rx, ry);
	return equipSlot;
}

bool HookTrinketBodyPart(bool bEnable)
{
	Memory::SetHook(bEnable, reinterpret_cast<void**>(&CUIEquipGetBodyPartFromPoint), CUIEquip_GetBodyPartFromPoint_Hook);
	Memory::SetHook(bEnable, reinterpret_cast<void**>(&GetBodyPartFromItem), GetBodyPartFromItem_Hook);
	Memory::SetHook(bEnable, reinterpret_cast<void**>(&IsCorrentBodyPart), IsCorrentBodyPart_Hook);
}

End Trinket Changes (Shoulder)

*/

#pragma endregion

#pragma endregion

#pragma region Ezorsia Prepackaged Hooks

bool HookGetModuleFileName_initialized = true;
bool Hook_GetModuleFileNameW(bool bEnable) {
	static decltype(&GetModuleFileNameW) _GetModuleFileNameW = &GetModuleFileNameW;

	const decltype(&GetModuleFileNameW) GetModuleFileNameW_Hook = [](HMODULE hModule, LPWSTR lpFileName, DWORD dwSize) -> DWORD {
		if (HookGetModuleFileName_initialized)
		{
			std::cout << "HookGetModuleFileName started" << std::endl;
			HookGetModuleFileName_initialized = false;
		}
		auto len = _GetModuleFileNameW(hModule, lpFileName, dwSize);
		// Check to see if the length is invalid (zero)
		if (!len) {
			// Try again without the provided module for a fixed result
			len = _GetModuleFileNameW(nullptr, lpFileName, dwSize);
			std::cout << "HookGetModuleFileName null file name" << std::endl;
		}
		return len;
		};

	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_GetModuleFileNameW), GetModuleFileNameW_Hook);
}

CreateWindowExA_t CreateWindowExA_Original = (CreateWindowExA_t)GetFuncAddress("USER32", "CreateWindowExA");
bool HookCreateWindowExA_initialized = true;
HWND WINAPI CreateWindowExA_Hook(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam) {
	if (HookCreateWindowExA_initialized)//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
	{
		std::cout << "HookCreateWindowExA started" << std::endl;
		HookCreateWindowExA_initialized = false;
	}
	if (strstr(lpClassName, "MapleStoryClass"))
	{
		dwStyle |= WS_MINIMIZEBOX; // enable minimize button
		RECT rectDesktop;
		GetWindowRect(GetDesktopWindow(), &rectDesktop);

		X = (rectDesktop.right / 2) - (Client::m_nGameWidth / 2);
		Y = (rectDesktop.bottom / 2) - (Client::m_nGameHeight / 2);
		HWND ret = CreateWindowExA_Original(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
		return ret;
	}
	else if (strstr(lpClassName, "StartUpDlgClass"))
	{
		return NULL; //kill startup balloon
	}
	return CreateWindowExA_Original(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
};
bool Hook_CreateWindowExA(bool bEnable) {
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&CreateWindowExA_Original), CreateWindowExA_Hook);
}
bool CreateMutexA_initialized = true; ////credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
CreateMutexA_t CreateMutexA_Original = (CreateMutexA_t)GetFuncAddress("KERNEL32", "CreateMutexA");
HANDLE WINAPI CreateMutexA_Hook(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR	lpName) {
	if (CreateMutexA_initialized)
	{
		std::cout << "HookCreateMutexA started" << std::endl;
		CreateMutexA_initialized = false;
	}
	if (!CreateMutexA_Original)
	{
		std::cout << "Original CreateMutex pointer corrupted. Failed to return mutex value to calling function." << std::endl;
		return nullptr;
	}
	else if (lpName && strstr(lpName, "WvsClientMtx"))
	{
		// from https://github.com/pokiuwu/AuthHook-v203.4/blob/AuthHook-v203.4/Client176/WinHook.cpp
		char szMutex[128];	//multiclient
		int nPID = GetCurrentProcessId();
		sprintf_s(szMutex, "%s-%d", lpName, nPID);
		lpName = szMutex;
		return CreateMutexA_Original(lpMutexAttributes, bInitialOwner, lpName);
	}
	return CreateMutexA_Original(lpMutexAttributes, bInitialOwner, lpName);
}
bool Hook_CreateMutexA(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&CreateMutexA_Original), CreateMutexA_Hook);
}

FindFirstFileA_t FindFirstFileA_Original = (FindFirstFileA_t)GetFuncAddress("KERNEL32", "FindFirstFileA");
bool FindFirstFileA_initialized = true; //ty joo for advice with this, check out their releases: https://github.com/OpenRustMS
HANDLE WINAPI FindFirstFileA_Hook(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) {
	if (FindFirstFileA_initialized)
	{
		std::cout << "HookFindFirstFileA started" << std::endl;
	}
	if (!FindFirstFileA_Original)
	{
		std::cout << "Original FindFirstFileA pointer corrupted. Failed to return ??? value to calling function." << std::endl;
		return nullptr;
	}
	else if (lpFileName && strstr(lpFileName, "*") && FindFirstFileA_initialized)
	{
		FindFirstFileA_initialized = false;
		return FindFirstFileA_Original("*.wz", lpFindFileData);
	}

	FindFirstFileA_initialized = false;
	return FindFirstFileA_Original(lpFileName, lpFindFileData);
}
bool Hook_FindFirstFileA(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&FindFirstFileA_Original), FindFirstFileA_Hook);
}
GetLastError_t GetLastError_Original = (GetLastError_t)GetFuncAddress("KERNEL32", "GetLastError");
bool GetLastError_initialized = true;
DWORD WINAPI GetLastError_Hook() {
	if (GetLastError_initialized)
	{
		std::cout << "HookGetLastError started" << std::endl;
		GetLastError_initialized = false;
	}
	DWORD error = GetLastError_Original();
	std::cout << "GetLastError: " << error << std::endl;
	return error;
}
bool Hook_GetLastError(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&GetLastError_Original), GetLastError_Hook);
}
sockaddr_in default_nXXXON_if;
#define WSAAddressToString  WSAAddressToStringA
bool WSPStartup_initialized = true; ////credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
INT WSPAPI WSPConnect_Hook(SOCKET s, const struct sockaddr* name, int namelen, LPWSABUF	lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS, LPINT lpErrno) {
	char szAddr[50];
	DWORD dwLen = 50;
	WSAAddressToString((sockaddr*)name, namelen, NULL, szAddr, &dwLen);

	sockaddr_in* service = (sockaddr_in*)name;
	//hardcoded NXXON IP addies in default client
	if (strstr(szAddr, "63.251.217.2") || strstr(szAddr, "63.251.217.3") || strstr(szAddr, "63.251.217.4"))
	{
		default_nXXXON_if = *service;
		service->sin_addr.S_un.S_addr = inet_addr(MainMain::m_sRedirectIP);
		//service->sin_port = htons(MainMain::porthere);
		MainMain::m_GameSock = s;
	}

	return MainMain::m_ProcTable.lpWSPConnect(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS, lpErrno);
}
INT WSPAPI WSPGetPeerName_Hook(SOCKET s, struct sockaddr* name, LPINT namelen, LPINT lpErrno) {
	int nRet = MainMain::m_ProcTable.lpWSPGetPeerName(s, name, namelen, lpErrno);//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
	if (nRet != SOCKET_ERROR && s == MainMain::m_GameSock)
	{
		sockaddr_in* service = (sockaddr_in*)name; //suspecting this is for checking rather than actually connecting
		service->sin_addr.S_un.S_addr = default_nXXXON_if.sin_addr.S_un.S_addr;//inet_addr(MainMain::m_sRedirectIP)
		//service->sin_port = htons(MainMain::porthere);
	}
	return nRet;
}
INT WSPAPI WSPCloseSocket_Hook(SOCKET s, LPINT lpErrno) {//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
	int nRet = MainMain::m_ProcTable.lpWSPCloseSocket(s, lpErrno);
	if (s == MainMain::m_GameSock)
	{
		MainMain::m_GameSock = INVALID_SOCKET;
	}
	return nRet;
}
WSPStartup_t WSPStartup_Original = (WSPStartup_t)GetFuncAddress("MSWSOCK", "WSPStartup"); /*in this function we'll call the WSP Functions*/					const wchar_t* v42;
INT WSPAPI WSPStartup_Hook(WORD wVersionRequested, LPWSPDATA lpWSPData, LPWSAPROTOCOL_INFOW lpProtocolInfo, WSPUPCALLTABLE UpcallTable, LPWSPPROC_TABLE	lpProcTable) {
	if (WSPStartup_initialized)//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
	{
		std::cout << "HookWSPStartup started" << std::endl;
		WSPStartup_initialized = false;
	}
	int nRet = WSPStartup_Original(wVersionRequested, lpWSPData, lpProtocolInfo, UpcallTable, lpProcTable);

	if (nRet == NO_ERROR)
	{
		MainMain::m_GameSock = INVALID_SOCKET;
		MainMain::m_ProcTable = *lpProcTable;

		lpProcTable->lpWSPConnect = WSPConnect_Hook;
		lpProcTable->lpWSPGetPeerName = WSPGetPeerName_Hook;
		lpProcTable->lpWSPCloseSocket = WSPCloseSocket_Hook;
	}
	else
	{
		std::cout << "WSPStartup Error Code: " + nRet << std::endl;
	}
	return nRet;
}
bool Hook_WSPStartup(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&WSPStartup_Original), WSPStartup_Hook);
}
#define x86CMPEAX 0x3D
bool GetACP_initialized = true; DWORD LocaleSpoofValue = 0;//choose value from https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers
GetACP_t GetACP_Original = (GetACP_t)GetFuncAddress("KERNEL32", "GetACP");
UINT WINAPI GetACP_Hook() { // AOB: FF 15 ?? ?? ?? ?? 3D ?? ?? ?? 00 00 74 <- library call inside winmain func
	if (GetACP_initialized) {//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate	
		std::cout << "HookGetACP started" << std::endl;
		GetACP_initialized = false;	//NOTE: idk what this really does tbh, maybe it is custom locale value but more likely it is to skip a check
	}	//that some clients may have that restricts you based on locale; if it is not a check and instead logged by server feel free to feed bad data by disabling the part below
	UINT uiNewLocale = LocaleSpoofValue;
	if (uiNewLocale == 0) { return GetACP_Original(); } //we do hook in my version!//should not happen cuz we dont hook if value is zero
	DWORD dwRetAddr = reinterpret_cast<DWORD>(_ReturnAddress());
	// return address should be a cmp eax instruction because ret value is stored in eax
	// and nothing else should happen before the cmp
	if (ReadValue<BYTE>(dwRetAddr) == x86CMPEAX) {	//disable this if and else if you wanna always use spoof value (i.e. if server logs it)
		uiNewLocale = ReadValue<DWORD>(dwRetAddr + 1); // check value is always 4 bytes
		std::cout << "[GetACP] Found desired locale: " << uiNewLocale << std::endl;
	}
	else { std::cout << "[GetACP] Unable to automatically determine locale, using stored locale: " << uiNewLocale << std::endl; }
	std::cout << "[GetACP] Locale spoofed to: " << uiNewLocale << " unhooking. Calling address: " << dwRetAddr << std::endl;
	if (Memory::SetHook(FALSE, reinterpret_cast<void**>(&GetACP_Original), GetACP_Hook)) {
		std::cout << "Failed to unhook GetACP." << std::endl;
	}
	return uiNewLocale;
}
bool Hook_GetACP(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&GetACP_Original), GetACP_Hook);
}

bool HookPcCreateObject_IWzResMan(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9FAF55), _PcCreateObject_IWzResMan_Hook);
}
bool HookPcCreateObject_IWzNameSpace(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9FAFBA), _PcCreateObject_IWzNameSpace_Hook);
}
bool HookPcCreateObject_IWzFileSystem(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9FB01F), _PcCreateObject_IWzFileSystem_Hook);
}
bool HookCWvsApp__Dir_BackSlashToSlash(bool bEnable)
{
	BYTE firstval = 0x56;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F95FE;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F95FE), _CWvsApp__Dir_BackSlashToSlash_rewrite);
}
bool HookCWvsApp__Dir_upDir(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F9644), _CWvsApp__Dir_upDir_Hook);
}
bool Hookbstr_ctor(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_406301), _bstr_ctor_Hook);
}
bool HookIWzNameSpace__Mount(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F790A), _IWzNameSpace__Mount_Hook);
}

bool resmanSTARTED = false;
static _CWvsApp__InitializeResMan_t _sub_9F7159_append = [](CWvsApp* pThis, void* edx) {

	resmanSTARTED = true;
	_sub_9F7159(pThis, nullptr);	//comment this out and uncomment below if testing, supposed to load from .img files in folders but i never got to test it
	resmanSTARTED = false;
	};
bool Hook_sub_9F7159(bool bEnable)	//resman hook that does nothing, kept for analysis and referrence //not skilled enough to rewrite to load custom wz files
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F7159;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F7159), _sub_9F7159_append);
}
bool Hook_StringPool__GetString_initialized = true;
_StringPool__GetString_t _StringPool__GetString_rewrite = [](void* pThis, void* edx, ZXString<char>* result, unsigned int nIdx, char formal) ->  ZXString<char>*
	{
		if (Hook_StringPool__GetString_initialized)
		{
			std::cout << "Hook_StringPool__GetString started" << std::endl;
			Hook_StringPool__GetString_initialized = false;
		}
		auto ret = _sub_79E993(pThis, nullptr, result, nIdx, formal);//_StringPool__GetString_t
		switch (nIdx)
		{
		case 1307:	//1307_UI_LOGINIMG_COMMON_FRAME = 51Bh
			if (MainMain::EzorsiaV2WzIncluded && !MainMain::ownLoginFrame) {
				switch (Client::m_nGameWidth)
				{
				case 1280:	//ty teto for the suggestion to use ZXString<char>::Assign and showing me available resources
					*ret = ("MapleEzorsiaV2wzfiles.img/Common/frame1280"); break;
				case 1366:
					*ret = ("MapleEzorsiaV2wzfiles.img/Common/frame1366"); break;
				case 1600:
					*ret = ("MapleEzorsiaV2wzfiles.img/Common/frame1600"); break;
				case 1920:
					*ret = ("MapleEzorsiaV2wzfiles.img/Common/frame1920"); break;
				case 1024:
					*ret = ("MapleEzorsiaV2wzfiles.img/Common/frame1024"); break;
				}
				break;
			}
		case 1301:	//1301_UI_CASHSHOPIMG_BASE_BACKGRND  = 515h
			if (MainMain::EzorsiaV2WzIncluded && !MainMain::ownCashShopFrame) { *ret = ("MapleEzorsiaV2wzfiles.img/Base/backgrnd"); } break;
		case 1302:	//1302_UI_CASHSHOPIMG_BASE_BACKGRND1 = 516h
			if (MainMain::EzorsiaV2WzIncluded && !MainMain::ownCashShopFrame) { *ret = ("MapleEzorsiaV2wzfiles.img/Base/backgrnd1"); } break;
		case 5361:	//5361_UI_CASHSHOPIMG_BASE_BACKGRND2  = 14F1h			
			if (MainMain::EzorsiaV2WzIncluded && !MainMain::ownCashShopFrame) { *ret = ("MapleEzorsiaV2wzfiles.img/Base/backgrnd2"); } break;
		case 5527:
			const char* dFont = Client::dFont.c_str();
			*ret = (dFont); break;
		}
		return ret;
	};
bool Hook_StringPool__GetString(bool bEnable)	//hook stringpool modification //ty !! popcorn //ty darter
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x0079E993;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_79E993), _StringPool__GetString_rewrite);//_StringPool__GetString_t
}
bool HookMyTestHook(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9DE4D2), _CWndCreateWnd_Hook);
}

bool HookDetectLogin(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_5F726D), _CLoginSendSelectCharPacket_Hook);
}

bool Hook_CUIStatusBar__ChatLogAdd(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_8DB070), _CUIStatusBar__ChatLogAdd_Hook);
}
bool Hook_sub_9F9808(bool bEnable)	//1
{
	static _sub_9F9808_t _sub_9F9808_Hook = [](void) {
		return _sub_9F9808();
		};	//2
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F9808), _sub_9F9808_Hook);	//2
}
bool Hook_sub_4959B8(bool bEnable)	//1
{
	static _sub_4959B8_t _sub_4959B8_Hook = [](void* pThis, void* edx, void* pData, unsigned int uSize) {
		return _sub_4959B8(pThis, nullptr, pData, uSize);
		};	//2
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_4959B8), _sub_4959B8_Hook);	//2
}

//bool sub_44E88E_initialized = true; //int(__stdcall* __stdcall MyGetProcAddress(HINSTANCE__* hModule, const char* lpProcName))()
static _sub_44E88E_t _sub_44E88E_rewrite = [](HINSTANCE__* hModule, const char* lpProcName) {
	return _sub_44E88E(hModule, lpProcName);
	};
bool Hook_sub_44E88E(bool bEnable)
{
	BYTE firstval = 0x55;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x0044E88E;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_44E88E), _sub_44E88E_rewrite);
}
//void __cdecl ResetLSP(void)
static _sub_44ED47_t _sub_44ED47_rewrite = []() {
	return _sub_44ED47();
	};	//2
bool Hook_sub_44ED47(bool bEnable)	//1
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_44ED47), _sub_44ED47_rewrite);	//2
}
//void __thiscall CClientSocket::ConnectLogin(CClientSocket *this)
static _sub_494931_t _sub_494931_rewrite = [](void* pThis, void* edx) {
	edx = nullptr;
	return  _sub_494931(pThis, edx);
	};	//2
bool Hook_sub_494931(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_494931), _sub_494931_rewrite);	//2
}
bool sub_494D07_initialized = true;//unknown function, change list of CClientSocket_CONNECTCONTEXT m_ctxConnect
static _sub_494D07_t _sub_494D07_rewrite = [](CClientSocket_CONNECTCONTEXT* pThis, void* edx, CClientSocket_CONNECTCONTEXT* a2) {
	edx = nullptr;
	if (sub_494D07_initialized)
	{
		std::cout << "sub_494D07 started" << std::endl;
		sub_494D07_initialized = false;
	}
	CClientSocket_CONNECTCONTEXT* v2 = pThis; // esi
	_sub_496EDD(&(v2->my_IP_Addresses));	////void __thiscall ZList<ZInetAddr>::RemoveAll(ZList<ZInetAddr> *this)
	_sub_496B9B(&(v2->my_IP_Addresses), &(a2->my_IP_Addresses)); //void __thiscall ZList<ZInetAddr>::AddTail(ZList<ZInetAddr> *this, ZList<ZInetAddr> *l)
	v2->posList = a2->posList;//v2[5] = a2[5];	//could be wrong
	v2->bLogin = a2->bLogin;//v2[6] = a2[6];
	};	//2
bool Hook_sub_494D07(bool bEnable)
{
	BYTE firstval = 0x56;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x00494D07;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_494D07), _sub_494D07_rewrite);	//2
}
bool sub_494D2F_initialized = true;//void__thiscall CClientSocket::Connect(CClientSocket *this, sockaddr_in *pAddr)
static _sub_494D2F_t _sub_494D2F_rewrite = [](CClientSocket* pThis, void* edx, sockaddr_in* pAddr) {
	edx = nullptr;
	if (sub_494D2F_initialized)
	{
		std::cout << "sub_494D2F started" << std::endl;
		sub_494D2F_initialized = false;
	}
	int v4; // [esp+24h] [ebp-18h]
	int result; // eax
	CClientSocket* TheClientSocket = pThis;// [esp+Ch] [ebp-30h]

	_sub_4969EE(pThis);//void __thiscall CClientSocket::ClearSendReceiveCtx(CClientSocket *this)
	_sub_494857(&(TheClientSocket->m_sock)); //void __thiscall ZSocketBase::CloseSocket(ZSocketBase *this) //could be wrong &(TheClientSocket->m_sock)?
	(TheClientSocket->m_sock)._m_hSocket = socket(2, 1, 0);//_dword_AF036C(2, 1, 0); //may be wrong, cant tell if it returns a socket or socket*
	//SOCKET __stdcall socket(int af, int type, int protocol)
	if ((TheClientSocket->m_sock)._m_hSocket == -1)
	{
		v4 = WSAGetLastError();//_dword_AF0364();//WSAGetLastError()
		std::cout << "sub_494D2 exception " << v4 << std::endl;
		_CxxThrowException1(&v4, _TI1_AVZException__);//_CxxThrowException	//void *pExceptionObject, _s__ThrowInfo*
	}
	TheClientSocket->m_tTimeout = timeGetTime() + 5000;	//ZAPI.timeGetTime() //_dword_BF060C
	if (WSAAsyncSelect((TheClientSocket->m_sock)._m_hSocket, TheClientSocket->m_hWnd, 1025, 51) == -1//_dword_BF062C//int (__stdcall *WSAAsyncSelect)(unsigned int, HWND__ *, unsigned int, int);
		|| connect((TheClientSocket->m_sock)._m_hSocket, (sockaddr*)pAddr, 16) != -1	//stdcall *connect//_dword_BF064C
		|| (result = WSAGetLastError(), result != 10035))//(result = _dword_BF0640(), result != 10035))// int (__stdcall *WSAGetLastError)();
	{
		_sub_494ED1(pThis, nullptr, 0);
	}
	};	//2
bool Hook_sub_494D2F(bool bEnable)
{
	BYTE firstval = 0x55;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x00494D2F;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_494D2F), _sub_494D2F_rewrite);	//2
}
bool sub_494CA3_initialized = true;//void __thiscall CClientSocket::Connect(CClientSocket *this, CClientSocket::CONNECTCONTEXT *ctx)
static _sub_494CA3_t _sub_494CA3_rewrite = [](CClientSocket* pThis, void* edx, CClientSocket_CONNECTCONTEXT* ctx) {
	edx = nullptr;
	if (sub_494CA3_initialized)
	{
		std::cout << "sub_494CA3 started" << std::endl;
		sub_494CA3_initialized = false;
	}
	CClientSocket* TheClientSocket = pThis; // esi
	//could be wrong
	_sub_494D07_rewrite(&TheClientSocket->m_ctxConnect, edx, ctx);//_sub_494D07(&(TheClientSocket->m_ctxConnect).my_IP_Addresses, edx, &(*ctx).my_IP_Addresses);
	ZInetAddr* v3 = ((TheClientSocket->m_ctxConnect).my_IP_Addresses).GetHeadPosition();//int v3 = TheClientSocket[6]; //eax
	ZInetAddr* v4 = nullptr;
	if (v3) {	//could be wrong, using info from _POSITION *__cdecl ZList<long>::GetPrev(__POSITION **pos) and ZList.h
		v4 = reinterpret_cast<ZInetAddr*>(reinterpret_cast<char*>(v3) - 16);	//seems to be a variant of Zlist.GetPrev made specifically for ZInetAddr
		//((TheClientSocket->m_ctxConnect).my_IP_Addresses).RemoveAt(v3);
		//v4 = v3;
	}

	if (v4 != nullptr && v4->my_IP_wrapper.sin_addr.S_un.S_addr) {
		(TheClientSocket->m_ctxConnect).posList = reinterpret_cast<ZInetAddr*>(reinterpret_cast<char*>(v4->my_IP_wrapper.sin_addr.S_un.S_addr) + 16);
	}
	else {
		(TheClientSocket->m_ctxConnect).posList = nullptr;
	}

	_sub_494D2F_rewrite(TheClientSocket, edx, (sockaddr_in*)v3);
	};	//2
bool Hook_sub_494CA3(bool bEnable)
{
	BYTE firstval = 0x55;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x00494CA3;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_494CA3), _sub_494CA3_rewrite);	//2
}
//int __thiscall CClientSocket::OnConnect(CClientSocket * this, int bSuccess) 	//1//will try this again later, seems it's not require to rewrite this to run from default client
static _sub_494ED1_t _sub_494ED1_rewrite = [](CClientSocket* pThis, void* edx, int bSuccess) {
	return _sub_494ED1(pThis, nullptr, bSuccess);	//will try this again later, seems it's not require to rewrite this to run from default client
	};
bool Hook_sub_494ED1(bool bEnable)
{
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_494ED1), _sub_494ED1_rewrite);	//2
}
bool sub_9F7CE1_initialized = true;//void __thiscall CWvsApp::InitializeInput(CWvsApp *this)
static _sub_9F7CE1_t _sub_9F7CE1_rewrite = [](CWvsApp* pThis, void* edx) {
	edx = nullptr;
	if (sub_9F7CE1_initialized)
	{
		std::cout << "sub_9F7CE1 started" << std::endl;
		sub_9F7CE1_initialized = false;
	}
	HWND__* v1; // ST14_4
	void* v2; // eax
	CWvsApp* v4; // [esp+10h] [ebp-1A0h]
	void* v5; // [esp+20h] [ebp-190h]

	v4 = pThis;
	//std::cout << _unk_BF0B00 << std::endl;
	v5 = _sub_403065(_unk_BF0B00, 0x9D0u);//void *__thiscall ZAllocEx<ZAllocAnonSelector>::Alloc(ZAllocEx<ZAllocAnonSelector> *this, unsigned int uSize)
	if (v5)//at _unk_BF0B00 = ZAllocEx<ZAllocAnonSelector> ZAllocEx<ZAllocAnonSelector>::_s_alloc
	{
		//std::cout << "CInputSystem::CInputSystem" << std::endl;
		_sub_9F821F(v5);//void __thiscall CInputSystem::CInputSystem(CInputSystem *this)
	}
	v1 = v4->m_hWnd; //4
	v2 = _sub_9F9A6A();//CInputSystem *__cdecl TSingleton<CInputSystem>::GetInstance()
	_sub_599EBF(v2, v1, v4->m_ahInput); //84 //104//void __thiscall CInputSystem::Init(CInputSystem *this, HWND__ *hWnd, void **ahEvent)
	};
bool Hook_sub_9F7CE1(bool bEnable)
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F7CE1;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F7CE1), _sub_9F7CE1_rewrite);
}
bool sub_9F84D0_initialized = true;	//void __thiscall CWvsApp::CallUpdate(CWvsApp *this, int tCurTime)
static _sub_9F84D0_t _sub_9F84D0_rewrite = [](CWvsApp* pThis, void* edx, int tCurTime) {
	edx = nullptr;
	if (sub_9F84D0_initialized)
	{
		std::cout << "sub_9F84D0 started" << std::endl;
		sub_9F84D0_initialized = false;
	}
	CWvsApp* v3 = pThis;	//// [esp+20h] [ebp-204h]
	if (pThis->m_bFirstUpdate)//if (this[7]) //28 //48
	{
		pThis->m_tUpdateTime = tCurTime;//this[6] = a2; //24 //44
		pThis->m_tLastServerIPCheck = tCurTime;//this[17] = a2; //v95 88
		pThis->m_tLastServerIPCheck2 = tCurTime;//this[18] = a2; //v95 92
		pThis->m_tLastGGHookingAPICheck = tCurTime;//this[19] = a2; //v95 96
		pThis->m_tLastSecurityCheck = tCurTime;//this[20] = a2; //v95 100
		pThis->m_bFirstUpdate = 0;//this[7] = 0; //28 //48
	}
	while (tCurTime - v3->m_tUpdateTime > 0)
	{
		auto  v9 = (void(__thiscall***)(void*))((DWORD)(*_dword_BEDED4));//fuck NXXXON and their stupid recursive function. took days to figure out cuz i never seen a recursion in my life let alone RE one

		//v10 = 0;	//stack frame counter of sorts for errors
		if (v9) {		//hard to define unknown function, likely wrong//(*_dword_BEDED4)->p
			(*(*v9))(v9);	//(*_dword_BEDED4)->p ////void __thiscall CLogin::Update(CLogin *this)//_sub_5F4C16<- only at first step!
		}	//fuck NXXXON and their stupid recursive function. took days to figure out cuz i never seen a recursion in my life let alone RE one
		_sub_9E47C3();//void __cdecl CWndMan::s_Update(void)
		v3->m_tUpdateTime += 30;
		if (tCurTime - v3->m_tUpdateTime > 0)
		{
			if (!(*_dword_BF14EC).m_pInterface)//_com_ptr_t<_com_IIID<IWzGr2D,&_GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9> > g_gr
			{
				_com_issue_error(-2147467261);//_sub_A5FDE4(-2147467261);//void __stdcall _com_issue_error(HRESULT hr)
			}
			auto v7 = *(int(__stdcall**)(IWzGr2D*, int))(*(int*)((*_dword_BF14EC).m_pInterface) + 24);
			v7((*_dword_BF14EC).m_pInterface, v3->m_tUpdateTime);//unknown function//((int (__stdcall *)(IWzGr2D *, int))v2->vfptr[2].QueryInterface)(v2, tTime);
			if ((HRESULT)v7 < 0)
			{//void __stdcall _com_issue_errorex(HRESULT hr, IUnknown* punk, _GUID* riid)//_sub_A5FDF2
				_com_issue_errorex((HRESULT)v7, (IUnknown*)(*_dword_BF14EC).m_pInterface, *_unk_BD83B0);//GUID _GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9
			}
		}
		//v10 = -1; //stack frame counter of sorts for errors
	}
	if (!(*_dword_BF14EC).m_pInterface)//_com_ptr_t<_com_IIID<IWzGr2D,&_GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9> > g_gr
	{
		_com_issue_error(-2147467261);//_sub_A5FDE4(-2147467261);//void __stdcall _com_issue_error(HRESULT hr)
	}
	auto v5 = *(int(__stdcall**)(IWzGr2D*, int))(*(int*)((*_dword_BF14EC).m_pInterface) + 24); //*(_DWORD *)dword_BF14EC + 24)
	v5((*_dword_BF14EC).m_pInterface, tCurTime);//unknown function//((int (__stdcall *)(IWzGr2D *, int))v2->vfptr[2].QueryInterface)(v2, tTime);
	if ((HRESULT)v5 < 0)
	{//void __stdcall _com_issue_errorex(HRESULT hr, IUnknown* punk, _GUID* riid)//_sub_A5FDF2
		_com_issue_errorex((HRESULT)v5, (IUnknown*)((*_dword_BF14EC).m_pInterface), *_unk_BD83B0);//GUID _GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9
	}//void __thiscall CActionMan::SweepCache(CActionMan* this)
	_sub_411BBB(*_dword_BE78D4);//CActionMan *TSingleton<CActionMan>::ms_pInstance
	};	const wchar_t* v13;
void fixWnd() 
{
	STARTUPINFOA siMaple;
	PROCESS_INFORMATION piMaple;

	ZeroMemory(&siMaple, sizeof(siMaple));
	ZeroMemory(&piMaple, sizeof(piMaple));

	char gameName[MAX_PATH];
	GetModuleFileNameA(NULL, gameName, MAX_PATH);

	char MapleStartupArgs[MAX_PATH]{}; // AdventureMS Custom
	strcat(MapleStartupArgs, " GameLaunching");

	// Create the child process
	CreateProcessA(
		gameName,
		const_cast<LPSTR>(MapleStartupArgs),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&siMaple,
		&piMaple
	);

	// Close process and thread handles
	CloseHandle(piMaple.hProcess);
	CloseHandle(piMaple.hThread);
}
bool Hook_sub_9F84D0(bool bEnable)
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F84D0;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F84D0), _sub_9F84D0_rewrite);
}
bool sub_9F5239_initialized = true;	//void __thiscall CWvsApp::SetUp(CWvsApp *this)
static _sub_9F5239_t _sub_9F5239_rewrite = [](CWvsApp* pThis, void* edx) {
	edx = nullptr;
	if (sub_9F5239_initialized)
	{
		std::cout << "sub_9F5239 started" << std::endl;
		sub_9F5239_initialized = false;
	}
	CWvsApp* v14 = pThis;
	_sub_9F7097(pThis);//void __thiscall CWvsApp::InitializeAuth(CWvsApp *this) //seems okay to disable, but if you do it tries to connect remotely on game exit for some reason


	(*_dword_BF1AC8) = 16;//TSingleton<CConfig>::GetInstance()->m_sysOpt.bSysOpt_WindowedMode;
	_sub_9F6D77(v14);//void __thiscall CWvsApp::InitializePCOM(CWvsApp *this)

	//void __thiscall CWvsApp::CreateMainWindow(CWvsApp *this) //a bit broken...previous fix just resulted in error 0 in my code instead
	_sub_9F6D97(v14);
	if (!v14->m_hWnd)
	{
		std::cout << "failed to create game window...trying again..." << std::endl;//Sleep(2000);
		fixWnd(); ExitProcess(0);
	}
	_sub_9F9E53();//CClientSocket *__cdecl TSingleton<CClientSocket>::CreateInstance()
	_sub_9F6F27(v14);//void __thiscall CWvsApp::ConnectLogin(CWvsApp *this)
	_sub_9F9E98();//CFuncKeyMappedMan *__cdecl TSingleton<CFuncKeyMappedMan>::CreateInstance()
	_sub_9FA0CB();//CQuickslotKeyMappedMan *__cdecl TSingleton<CQuickslotKeyMappedMan>::CreateInstance()
	_sub_9F9EEE();//CMacroSysMan *__cdecl TSingleton<CMacroSysMan>::CreateInstance()
	_sub_9F7159_append(v14, nullptr);//void __thiscall CWvsApp::InitializeResMan(CWvsApp *this)

	_sub_9F7A3B(v14);//void __thiscall CWvsApp::InitializeGr2D(CWvsApp *this)
	_sub_9F7CE1_rewrite(v14, nullptr); //void __thiscall CWvsApp::InitializeInput(CWvsApp *this)
	Sleep(300);//_dword_BF02F4(300);//void(__stdcall* Sleep)(unsigned int);
	_sub_9F82BC(v14);//void __thiscall CWvsApp::InitializeSound(CWvsApp *this)
	Sleep(300);//_dword_BF02F4(300);//void(__stdcall* Sleep)(unsigned int);
	_sub_9F8B61(v14);//void __thiscall CWvsApp::InitializeGameData(CWvsApp *this)
	_sub_9F7034(v14);//void __thiscall CWvsApp::CreateWndManager(CWvsApp *this)
	void* vcb = _sub_538C98();//CConfig *__cdecl TSingleton<CConfig>::GetInstance()
	_sub_49EA33(vcb, nullptr, 0);//void __thiscall CConfig::ApplySysOpt(CConfig *this, CONFIG_SYSOPT *pSysOpt, int bApplyVideo)
	void* v3 = _sub_9F9DA6();//CActionMan *__cdecl TSingleton<CActionMan>::CreateInstance()
	_sub_406ABD(v3);//void __thiscall CActionMan::Init(CActionMan *this)
	_sub_9F9DFC();//CAnimationDisplayer *__cdecl TSingleton<CAnimationDisplayer>::CreateInstance()
	void* v4 = _sub_9F9F87();//CMapleTVMan *__cdecl TSingleton<CMapleTVMan>::CreateInstance()
	_sub_636F4E(v4);//void __thiscall CMapleTVMan::Init(CMapleTVMan *this)
	void* v5 = _sub_9F9AC2();//CQuestMan *__cdecl TSingleton<CQuestMan>::CreateInstance()
	if (!_sub_71D8DF(v5))//int __thiscall CQuestMan::LoadDemand(CQuestMan *this)
	{
		//v22 = 570425350;
		//v12 = &v22;
		//v35 = 1;//zref counter
		int v23 = 570425350;
		std::cout << "sub_9F5239 exception " << std::endl;
		_CxxThrowException1(&v23, _TI3_AVCTerminateException__);//_CxxThrowException	//void *pExceptionObject, _s__ThrowInfo*
	}
	_sub_723341(v5);//void __thiscall CQuestMan::LoadPartyQuestInfo(CQuestMan *this) //_dword_BED614
	_sub_7247A1(v5);//void __thiscall CQuestMan::LoadExclusive(CQuestMan *this) //_dword_BED614
	void* v6 = _sub_9F9B73();//CMonsterBookMan *__cdecl TSingleton<CMonsterBookMan>::CreateInstance()
	if (!_sub_68487C(v6))//int __thiscall CMonsterBookMan::LoadBook(CMonsterBookMan *this)
	{
		int v21 = 570425350;
		std::cout << "sub_9F5239 exception " << std::endl;
		_CxxThrowException1(&v21, _TI3_AVCTerminateException__);//_CxxThrowException	//void *pExceptionObject, _s__ThrowInfo*
	}
	_sub_9FA078();//CRadioManager *__cdecl TSingleton<CRadioManager>::CreateInstance()

	char v34[MAX_PATH];//char sStartPath[MAX_PATH];
	GetModuleFileNameA(NULL, v34, MAX_PATH);//_dword_BF028C(0, &v34, 260);//GetModuleFileNameA(NULL, sStartPath, MAX_PATH);
	_CWvsApp__Dir_BackSlashToSlash_rewrite(v34);//_CWvsApp__Dir_BackSlashToSlash_rewrite(v34);//_CWvsApp__Dir_BackSlashToSlash//_sub_9F95FE
	_sub_9F9644(v34);//_CWvsApp__Dir_upDir
	_sub_9F9621(v34);//void __cdecl CWvsApp::Dir_SlashToBackSlash(char *sDir) //fast way to define functions
	//v19 = &v8;
	//v15 = &v8;
	ZXString<char> v8;
	_sub_414617(&v8, v34, -1);//void __thiscall ZXString<char>::Assign(ZXString<char> *this, const char *s, int n)
	_sub_49CCF3(vcb, v8);//void __thiscall CConfig::CheckExecPathReg(CConfig *this, ZXString<char> sModulePath)
	void* v17 = _sub_403065(_unk_BF0B00, 0x38u);//void *__thiscall ZAllocEx<ZAllocAnonSelector>::Alloc(ZAllocEx<ZAllocAnonSelector> *this, unsigned int uSize)
	//v35 = 4;//zref counter
	if (v17)
	{
		_sub_62ECE2(v17);//void __thiscall CLogo::CLogo(CLogo *this)
		_sub_777347((CStage*)v17, nullptr);//void __cdecl set_stage(CStage *pStage, void *pParam)
	}
	else
	{
		_sub_777347(nullptr, nullptr);//void __cdecl set_stage(CStage *pStage, void *pParam)
	}
	SetFocus(v14->m_hWnd);
	SetForegroundWindow(v14->m_hWnd);
	};
bool Hook_sub_9F5239(bool bEnable)
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F5239;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F5239), _sub_9F5239_rewrite);
}
bool sub_9F5C50_initialized = true;//void __thiscall CWvsApp::Run(CWvsApp *this, int *pbTerminate)
static _sub_9F5C50_t _sub_9F5C50_rewrite = [](CWvsApp* pThis, void* edx, int* pbTerminate) {
	edx = nullptr;
	if (sub_9F5C50_initialized)
	{
		std::cout << "sub_9F5C50 started" << std::endl;
		sub_9F5C50_initialized = false;
	}
	CWvsApp* v4 = pThis;
	tagMSG v17 = tagMSG();
	v17.hwnd = nullptr; //0
	v17.message = 0; //4 //memset(&v18, 0, 0x18u);
	v17.wParam = 0; //8
	v17.lParam = 0; //12
	v17.time = 0; //16
	v17.pt.x = 0; //20 //size 8, 2 longs
	v17.pt.y = 0;
	ISMSG v21 = ISMSG();
	v21.message = 0;
	v21.wParam = 0; //v22
	v21.lParam = 0; //v23
	if (*_dword_BE7914)//CClientSocket *TSingleton<CClientSocket>::ms_pInstance //C64064
	{//void __thiscall CClientSocket::ManipulatePacket(CClientSocket *this)
		_sub_49651D(*_dword_BE7914);//CClientSocket* TSingleton<CClientSocket>::ms_pInstance //C64064
	}
	//v20 = 0; unknown variable //hendi's instructions say to skip it, but for some reason it wasnt skipped in v83, skipping it
	do
	{//unsigned int (__stdcall *MsgWaitForMultipleObjects)(unsigned int, void **, int, unsigned int, unsigned int);
		DWORD v16 = MsgWaitForMultipleObjects(3, v4->m_ahInput, false, 0, 255); //C6D9C4 v9
		if (v16 <= 2)
		{//dword_BEC33C=TSingleton_CInputSystem___ms_pInstance dd ? v95 C68C20
			_sub_59A2E9(*_dword_BEC33C, v16);//void __thiscall CInputSystem::UpdateDevice(CInputSystem *this, int nDeviceIndex)
			do
			{
				if (!_sub_59A306(*_dword_BEC33C, &v21))//int __thiscall CInputSystem::GetISMessage(CInputSystem *this, ISMSG *pISMsg)
				{
					break;
				}
				_sub_9F97B7(v4, v21.message, v21.wParam, v21.lParam);//void __thiscall CWvsApp::ISMsgProc(CWvsApp *this, unsigned int message, unsigned int wParam, int lParam)
			} while (!*pbTerminate);
		}
		else if (v16 == 3)
		{
			do
			{
				if (!PeekMessageA(LPMSG(&v17), nullptr, 0, 0, 1))//_dword_BF04E8//int (__stdcall *PeekMessageA)(tagMSG *, HWND__ *, unsigned int, unsigned int, unsigned int);
				{
					break;
				}
				TranslateMessage((MSG*)(&v17));//_dword_BF0430//int (__stdcall *TranslateMessage)(tagMSG *);
				DispatchMessageA((MSG*)(&v17));//_dword_BF042C//int (__stdcall *DispatchMessageA)(tagMSG *);
				HRESULT v15 = 0;
				if (FAILED(v4->m_hrComErrorCode))//(v4[14])
				{
					v15 = v4->m_hrComErrorCode;//v15 = v4[14];
					v4->m_hrComErrorCode = 0;//v4[14] = 0;
					v4->m_hrZExceptionCode = 0;//v4[13] = 0;
					_com_raise_error(v15, nullptr);//_sub_A605C3(v15, nullptr);//void __stdcall _com_raise_error(HRESULT hr, IErrorInfo *perrinfo)
				}
				if (FAILED(v4->m_hrZExceptionCode))//if (v4[13])
				{
					v15 = v4->m_hrZExceptionCode;//v15 = v4[13];
					v4->m_hrComErrorCode = 0;//v4[14] = 0;
					v4->m_hrZExceptionCode = 0;//v4[13] = 0;
					if (v15 == 0x20000000)
					{//create custom error here from struct ZException { const HRESULT m_hr; }; so it doesnt break
						CPatchException v12 = CPatchException();
						//void* v2 = change return to void* if trying other way
						_sub_51E834(&v12, v4->m_nTargetVersion);//v4[16]//void __thiscall CPatchException::CPatchException(CPatchException *this, int nTargetVersion)
						std::cout << "sub_9F5C50 exception" << std::endl;
						_CxxThrowException1(&v12, _TI3_AVCPatchException__);//&v13
					}
					if (v15 >= 553648128 && v15 <= 553648134)
					{
						//v10 = v15;
						//v24 = 1;//address of one frame up but idk what it's for
						int v11 = v15;
						std::cout << "sub_9F5C50 exception" << std::endl;
						_CxxThrowException1(&v11, _TI3_AVCDisconnectException__);
					}
					if (v15 >= 570425344 && v15 <= 570425357)
					{
						//v8 = v15;
						//v24 = 2;//address of 2 frames up but idk what it's for
						int v9 = v15;
						std::cout << "sub_9F5C50 exception " << v9 << _TI3_AVCTerminateException__ << std::endl;
						_CxxThrowException1(&v9, _TI3_AVCTerminateException__);
					}
					int v7 = v15;
					std::cout << "sub_9F5C50 exception " << v7 << _TI1_AVZException__ << std::endl;
					_CxxThrowException1(&v7, _TI1_AVZException__);
				}
			} while (!*pbTerminate && v17.message != 18);
		}
		else
		{//int __thiscall CInputSystem::GenerateAutoKeyDown(CInputSystem *this, ISMSG *pISMsg)
			if (_sub_59B2D2(*_dword_BEC33C, &v21))//dword_BEC33C=TSingleton_CInputSystem___ms_pInstance dd ? v95 C68C20
			{
				_sub_9F97B7(v4, v21.message, v21.wParam, v21.lParam);//void __thiscall CWvsApp::ISMsgProc(CWvsApp *this, unsigned int message, unsigned int wParam, int lParam)
			}
			//std::cout << "_sub_9F5C50 @ _dword_BF14EC error check" << std::endl;
			if ((*_dword_BF14EC).m_pInterface)//_com_ptr_t<_com_IIID<IWzGr2D,&_GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9> > g_gr
			{
				int v14 = _sub_9F6990((*_dword_BF14EC).m_pInterface);//int __thiscall IWzGr2D::GetnextRenderTime(IWzGr2D *this)
				_sub_9F84D0_rewrite(v4, nullptr, v14);//void __thiscall CWvsApp::CallUpdate(CWvsApp *this, int tCurTime)//_rewrite
				_sub_9E4547();//void __cdecl CWndMan::RedrawInvalidatedWindows(void)
				if (!(*_dword_BF14EC).m_pInterface)//_com_ptr_t<_com_IIID<IWzGr2D,&_GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9> > g_gr
				{
					_com_issue_error(-2147467261);//_sub_A5FDE4(-2147467261);//void __stdcall _com_issue_error(HRESULT hr)
				}//not sure if still needed since the return isnt used
				HRESULT unused_result_vv = _sub_777326((*_dword_BF14EC).m_pInterface);//HRESULT __thiscall IWzGr2D::RenderFrame(IWzGr2D *this)
			}
			Sleep(1);//_dword_BF02F4(1);//void(__stdcall* Sleep)(unsigned int);
		}
	} while (!*pbTerminate && v17.message != 18);
	//_sub_A61DF2(lpMem);//void __cdecl free(void *) //hendi's instructions say to skip it, but for some reason it wasnt skipped in v83, skipping it
	if (v17.message == 18)
	{
		PostQuitMessage(0);//_dword_BF041C(0);//void (__stdcall *PostQuitMessage)(int);
	}
	};
bool Hook_sub_9F5C50(bool bEnable)
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F5C50;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F5C50), _sub_9F5C50_rewrite);
}
bool sub_9F4FDA_initialized = true;
static _sub_9F4FDA_t _sub_9F4FDA_rewrite = [](CWvsApp* pThis, void* edx, const char* sCmdLine) {
	if (sub_9F4FDA_initialized)//void __thiscall CWvsApp::CWvsApp(CWvsApp *this, const char *sCmdLine)
	{
		std::cout << "sub_9F4FDA started" << std::endl;
		sub_9F4FDA_initialized = false;
	}
	edx = nullptr;
	CWvsApp* v3 = pThis;//function=void __thiscall CWvsApp::CWvsApp(CWvsApp *this, const char *sCmdLine)
	*_dword_BE7B38 = pThis->m_hWnd != nullptr ? pThis : nullptr;//protected: static class CWvsApp * TSingleton<class CWvsApp>::ms_pInstance
	pThis->m_hWnd = nullptr;//pThis[1] = 0; //unlikely to be wrong because [3] is assigned a unsigned int further down that matches
	pThis->m_bPCOMInitialized = 0;//pThis[2] = 0; //could be wrong
	pThis->m_hHook = 0;//pThis[4] = 0; //could be wrong
	pThis->m_tUpdateTime = 0;//pThis[6] = 0;
	pThis->m_bFirstUpdate = 1;//pThis[7] = 1;
	//v19 = 0; //probly ref counter or stack frame counter//[esp+B4h] [ebp-4h]
	pThis->m_sCmdLine = ZXString<char>();//pThis[8] = 0;
	//LOBYTE(v19) = 1;
	pThis->m_nGameStartMode = 0;//pThis[9] = 0;
	pThis->m_bAutoConnect = 1;//pThis[10] = 1;
	pThis->m_bShowAdBalloon = 0;//pThis[11] = 0;
	pThis->m_bExitByTitleEscape = 0;//pThis[12] = 0;
	pThis->m_hrZExceptionCode = 0;//pThis[13] = 0;
	pThis->m_hrComErrorCode = 0;//pThis[14] = 0;
	pThis->vfptr = _off_B3F3E8;//const CWvsApp::`vftable'
	_sub_414617(&(pThis->m_sCmdLine), sCmdLine, -1);//void __thiscall ZXString<char>::Assign(ZXString<char> *this, const char *s, int n)
	ZXString<char>* v4 = _sub_474414(&(v3->m_sCmdLine), "\" ");//ZXString<char> *__thiscall ZXString<char>::TrimRight(ZXString<char> *this, const char *sWhiteSpaceSet)
	_sub_4744C9(v4, "\" ");//ZXString<char> *__thiscall ZXString<char>::TrimLeft(ZXString<char> *this, const char *sWhiteSpaceSet)
	v3->m_nGameStartMode = 2;	//unlikely to be wrong, matches type and name of m_dwMainThreadId
	v3->m_dwMainThreadId = GetCurrentThreadId();//_dword_BF02B4();//unsigned int (__stdcall *GetCurrentThreadId)();
	OSVERSIONINFOA v13 = OSVERSIONINFOA();
	v13.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	GetVersionExA((LPOSVERSIONINFOA)(&v13));//_dword_BF03E4(&v13);//int (__stdcall *GetVersionExA)(_OSVERSIONINFOA *);
	v3->m_bWin9x = v13.dwPlatformId == 1; //at memory byte 20 of v3 struct //could also be wrong
	if (v13.dwMajorVersion > 6 && !v3->m_nGameStartMode)
	{
		v3->m_nGameStartMode = 2;
	}
	if (v13.dwMajorVersion < 5)
	{
		*_dword_BE2EBC = 1996;//unsigned int g_dwTargetOS
	} v42 = L"EzorsiaV2_UI.wz";
	HMODULE v9 = GetModuleHandleA("kernel32.dll");//sub_44E88E=//int (__stdcall *__stdcall MyGetProcAddress(HINSTANCE__ *hModule, const char *lpProcName))()
	auto v10 = (void(__stdcall*)(HANDLE, int*))_sub_44E88E_rewrite(v9, "IsWow64Process"); //tough definition, one of a kind
	int v18 = 0;
	if (v10)
	{
		HANDLE v11 = GetCurrentProcess();
		v10(v11, &v18);
		if (v18 != 0)
		{
			*_dword_BE2EBC = 1996;
		}
	}
	if (v13.dwMajorVersion >= 6 && v18 == 0)
	{
		_sub_44ED47_rewrite();//void __cdecl ResetLSP(void)
	}
	};	//2 //^part of part to skip
bool Hook_sub_9F4FDA(bool bEnable)	//1
{
	BYTE firstval = 0x7D;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F4FDC;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F4FDA), _sub_9F4FDA_rewrite);	//2
}
static _sub_9F51F6_t _sub_9F51F6_Hook = [](CWvsApp* pThis, void* edx) {
	std::cout << "sub_9F51F6 started: CWvsapp dieing" << std::endl;
	_sub_9F51F6(pThis, nullptr);
	};
bool Hook_sub_9F51F6(bool bEnable)	//1
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F51F6;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F51F6), _sub_9F51F6_Hook);	//2
}

static _IWzFileSystem__Init_t _sub_9F7964_Hook = [](void* pThis, void* edx, Ztl_bstr_t sPath) {

	edx = nullptr;	//function does nothing.., can replace return with S_OK and it works
	void* v2 = pThis; // esi
	wchar_t* v3 = 0; // eax
	HRESULT v5; // edi
	v13 = v42; //ebp

	if (sPath.m_Data)
	{
		v3 = sPath.m_Data->m_wstr;
	}
	auto v4 = (*(int(__stdcall**)(void*, wchar_t*))(*(DWORD*)pThis + 52));//overloaded unknown funct at offset 52 of IWzFileSystem
	v4(pThis, v3);//seems to do nothing and just check the input, works if not run
	v5 = (HRESULT)v4;
	if ((HRESULT)v4 < 0)
	{
		_com_issue_errorex((HRESULT)v4, (IUnknown*)v2, *_unk_BE2EC0);//GUID _GUID_352d8655_51e4_4668_8ce4_0866e2b6a5b5
	}
	if (sPath.m_Data)
	{
		_sub_402EA5(sPath.m_Data);
	}
	return v5;
	//return _sub_9F7964(pThis, nullptr, sPath);//HRESULT __thiscall IWzFileSystem::Init(IWzFileSystem *this, Ztl_bstr_t sPath)
	};
bool Hook_sub_9F7964(bool bEnable)	//1
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F7964;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F7964), _sub_9F7964_Hook);	//2
}
static _sub_9FCD88_t _sub_9FCD88_Hook = [](void* pThis, void* edx, IUnknown* p) {
	_sub_9FCD88(pThis, nullptr, p);//void __thiscall <IWzSeekableArchive(IWzSeekableArchive* this, IUnknown* p)
	};
bool Hook_sub_9FCD88(bool bEnable)	//1
{
	BYTE firstval = 0x55;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009FCD88;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9FCD88), _sub_9FCD88_Hook);	//2
}
bool ZSecureCrypt_Init = false;
static _sub_5D995B_t _sub_5D995B_Hook = [](void* pThis, void* edx, Ztl_variant_t* result, Ztl_bstr_t sPath) {
	edx = nullptr;
	void* v3 = pThis; // esi
	const wchar_t* v4 = 0; // eax
	Ztl_variant_t pvarg = Ztl_variant_t(); // [esp+8h] [ebp-20h]

	VariantInit(&pvarg);
	if (sPath.m_Data)
	{
		v4 = sPath.m_Data->m_wstr;
	}
	auto v5 = (*(int(__stdcall**)(void*, const wchar_t*, Ztl_variant_t*))(*(DWORD*)v3 + 12));//unknown virtual function at offset 12 of IWzNameSpace
	if (!ZSecureCrypt_Init && MainMain::usingEzorsiaV2Wz)
	{
		ZSecureCrypt_Init = true; v4 = v13;
	}
	v5(v3, v4, &pvarg);
	if ((HRESULT)v5 < 0)
	{
		_com_issue_errorex((HRESULT)v5, (IUnknown*)v3, *_unk_BD8F28); ///GUID _GUID_2aeeeb36_a4e1_4e2b_8f6f_2e7bdec5c53d
	}
	_sub_4039AC(result, &pvarg, 0);//non-existent func in v95//int __thiscall sub_4039AC(VARIANTARG *pvargDest, VARIANTARG *pvargSrc, char) //works with v95 overwrite//memcpy_s(result, 0x10u, &pvarg, 0x10u);//_sub_4039AC(result, &pvarg, 0); //works with v95 overwrite
	pvarg.vt = 0;
	if (sPath.m_Data)
	{
		_sub_402EA5(sPath.m_Data);//unsigned int __thiscall _bstr_t::Data_t::Release(_bstr_t::Data_t *this)
	}
	return result;
	//return _sub_5D995B(pThis, nullptr, result, sPath);
	};
//bool sub_9F4E54_initialized = true; //unsigned int __cdecl Crc32_GetCrc32_VMTable(unsigned int* pmem, unsigned int size, unsigned int* pcheck, unsigned int *pCrc32) 
static _sub_9F4E54_t _sub_9F4E54_Hook = [](unsigned int* pmem, void* edx, unsigned int size, unsigned int* pcheck, unsigned int* pCrc32) {
	//if (sub_9F4E54_initialized)
	//{
	std::cout << "!!!WARNING!!! _sub_9F4E54 anonymously called !!!WARNING!!!" << std::endl;
	//sub_9F4E54_initialized = false;
//}
	edx = nullptr;
	unsigned int result = *pCrc32;
	return result;
	};
bool Hook_sub_9F4E54(bool bEnable)	//1
{
	BYTE firstval = 0x55;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x009F4E54;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_9F4E54), _sub_9F4E54_Hook);	//2
}
bool Hook_sub_5D995B(bool bEnable)	//1
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x005D995B;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_5D995B), _sub_5D995B_Hook);	//2
}
static _sub_4032B2_t _sub_4032B2_Hook = [](Ztl_variant_t* pThis, void* edx, bool fAddRef, bool fTryChangeType) {
	return _sub_4032B2(pThis, nullptr, fAddRef, fTryChangeType);//IUnknown* __thiscall Ztl_variant_t::GetUnknown(Ztl_variant_t* this, bool fAddRef, bool fTryChangeType)
	};
bool Hook_sub_4032B2(bool bEnable)	//1
{
	BYTE firstval = 0xB8;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x004032B2;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_4032B2), _sub_4032B2_Hook);	//2
}
static _sub_425ADD_t _sub_425ADD_Hook = [](Ztl_bstr_t* pThis, void* edx, const char* str) {
	return _sub_425ADD(pThis, nullptr, str); //HRESULT __thiscall IWzPackage::Init(IWzPackage *this, Ztl_bstr_t sKey, Ztl_bstr_t sBaseUOL, IWzSeekableArchive *pArchive)
	};
bool Hook_sub_425ADD(bool bEnable)	//1
{
	BYTE firstval = 0x56;  //this part is necessary for hooking a client that is themida packed
	DWORD dwRetAddr = 0x00425ADD;	//will crash if you hook to early, so you gotta check the byte to see
	while (1) {						//if it matches that of an unpacked client
		if (ReadValue<BYTE>(dwRetAddr) != firstval) { Sleep(1); } //figured this out myself =)
		else { break; }
	}//void __thiscall Ztl_bstr_t::Ztl_bstr_t(Ztl_bstr_t *this, const char *s)
	return Memory::SetHook(bEnable, reinterpret_cast<void**>(&_sub_425ADD), _sub_425ADD_Hook);	//2
}

#pragma endregion