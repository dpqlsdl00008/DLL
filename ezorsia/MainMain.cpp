#include "stdafx.h"
#include "INIReader.h"
#include <filesystem>
#include "resource.h"

INIReader reader("config.ini");
MainMain* MainMain::_s_pInstance;
FILE* MainMain::stream;
bool MainMain::ownLoginFrame = false;
bool MainMain::CustomLoginFrame = false;
bool MainMain::bigLoginFrame = false;
bool MainMain::ownCashShopFrame = false;
bool MainMain::EzorsiaV2WzIncluded = false;
SOCKET MainMain::m_GameSock = INVALID_SOCKET;
WSPPROC_TABLE MainMain::m_ProcTable = { 0 };//to set your max level you have to go to the function rewrite
const char* MainMain::m_sRedirectIP = "194.238.29.22";
bool MainMain::usingEzorsiaV2Wz = false;
HANDLE MainMain::mainTHread;

MainMain::MainMain(std::function<void()> pPostMutexFunc)
{
	std::filesystem::path BfilePath("Base.wz"); std::filesystem::path BfilePath2("Data/zmap.img"); std::filesystem::path filePath("config.ini"); //check if needed stuff exists
	if (std::filesystem::exists(filePath) && reader.ParseError()) { Sleep(20); SuspendThread(MainMain::mainTHread); MessageBox(NULL, L"your config.ini file cannot be properly read, go to troubleshooting section of Ezorsia v2 setup guide at https://github.com/444Ro666/MapleEzorsia-v2 for more details, or delete your config.ini to have a new one generated with default settings", L"bad config file", 0); ExitProcess(0); }
	else if (!std::filesystem::exists(filePath)) {
		HANDLE hOrg = CreateFileA("config.ini", (GENERIC_READ | GENERIC_WRITE), NULL, NULL, CREATE_ALWAYS, NULL, NULL); DWORD dw;
		if (hOrg) {
			HMODULE hModule = GetModuleHandle(L"dinput8.dll"); // Get handle to current DLL
			HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_RCDATA1), RT_RCDATA);
			HGLOBAL hResourceData = LoadResource(hModule, hResource);
			DWORD resourceSize = SizeofResource(hModule, hResource);
			LPVOID resourceData = LockResource(hResourceData); Sleep(2); SuspendThread(MainMain::mainTHread);
			if (WriteFile(hOrg, resourceData, resourceSize, &dw, NULL) && dw == resourceSize) { 
				CloseHandle(hOrg);
				ResumeThread(MainMain::mainTHread);
			}
			else { 
				CloseHandle(hOrg);
				MessageBox(NULL, L"your config.ini file doesn't exist, please re-download config.ini from Ezorsia v2 releases at https://github.com/444Ro666/MapleEzorsia-v2", L"bad config file", 0);
				ExitProcess(0);
			}
		}
	}
	if (!std::filesystem::exists(BfilePath) && !std::filesystem::exists(BfilePath2)) { Sleep(20); SuspendThread(MainMain::mainTHread); MessageBox(NULL, L"Either Base.wz is missing from your game directory OR you are loading from .img and zmap.img is not in your Data directory, please reinstall and make sure relevant file(s) exist", L"missing .wz/.img file", 0); ExitProcess(0); }
	MainMain::CustomLoginFrame = reader.GetBoolean("optional", "CustomLoginFrame", false);
	if (MainMain::CustomLoginFrame) { MainMain::ownLoginFrame = true; MainMain::bigLoginFrame = true; } //use own login if true
	std::filesystem::path EfilePath("EzorsiaV2_UI.wz");	//support for other non-big frame users (i.e. ezorsia-like, with login centered, but different frame, isnt currently supported)
	std::filesystem::path EfilePath2("Data/MapleEzorsiaV2wzfiles.img");
	if (std::filesystem::exists(EfilePath)) { 	//only check after "if false" on Client::CustomLoginFrame or things break
		MainMain::EzorsiaV2WzIncluded = true; MainMain::CustomLoginFrame = true; MainMain::usingEzorsiaV2Wz = true; }
	else if(std::filesystem::exists(EfilePath2)){ MainMain::EzorsiaV2WzIncluded = true; MainMain::CustomLoginFrame = true; }
	else {
		if (std::filesystem::exists(BfilePath)) {
			HANDLE hOrg = CreateFileA("EzorsiaV2_UI.wz", (GENERIC_READ | GENERIC_WRITE), NULL, NULL, CREATE_ALWAYS, NULL, NULL); DWORD dw;
			if (hOrg) {
				HMODULE hModule = GetModuleHandle(L"dinput8.dll"); // Get handle to current DLL
				HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(IDR_RCDATA2), RT_RCDATA);
				HGLOBAL hResourceData = LoadResource(hModule, hResource);
				DWORD resourceSize = SizeofResource(hModule, hResource);
				LPVOID resourceData = LockResource(hResourceData); Sleep(2); SuspendThread(MainMain::mainTHread);
				if (WriteFile(hOrg, resourceData, resourceSize, &dw, NULL) && dw == resourceSize) {
					CloseHandle(hOrg);
					ResumeThread(MainMain::mainTHread);
				}
				else {
					CloseHandle(hOrg);
					MessageBox(NULL, L"your EzorsiaV2_UI.wz file doesn't exist, please re-download EzorsiaV2_UI.wz from Ezorsia v2 releases at https://github.com/444Ro666/MapleEzorsia-v2", L"bad WZ file", 0);
					ExitProcess(0);
				}
			}
			MainMain::EzorsiaV2WzIncluded = true; MainMain::CustomLoginFrame = true; MainMain::usingEzorsiaV2Wz = true;
		}
	}
	
	// Read from Config
	Client::m_nGameWidth = reader.GetInteger("general", "width", 1366);
	Client::m_nGameHeight = reader.GetInteger("general", "height", 768);
	unsigned int sleepySleepy = reader.GetInteger("debug", "sleepTime", 0);

	/*
		Sleep is for the unpacked client run //themida needs a slight amount of time to unpack or exe memory will be corrupted and crash. from my 
		tests the amount of time to sleep for ranges from 80 to 300 miliseconds, but may differ depending on the machine; most modern pc are pretty 
		fast so favor lower numbers. if the number is too high the non-system hooks wont reach the main thread in time and the main thread will
		run without them (causing hackshield load that is normally overwritten and hooked in time to block access)
	*/

	if (sleepySleepy != 0) { Sleep(sleepySleepy); }
	pPostMutexFunc(); //can't reach my functions from this class =(
}	//and if i try to #include "ReplacementFuncs.h" here it says a lot of things are defined twice

MainMain::~MainMain()
{
	if (MainMain::m_GameSock != INVALID_SOCKET)
	{//credits to the creators of https://github.com/MapleStory-Archive/MapleClientEditTemplate
		MainMain::m_ProcTable.lpWSPCloseSocket(m_GameSock, nullptr);
		MainMain::m_GameSock = INVALID_SOCKET;
	}
}