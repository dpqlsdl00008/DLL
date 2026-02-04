#pragma once
#include "MapleClientCollectionTypes/ZXString.h"	//also using AutoTypes.h to store unused portions of code since it's a smol file
#include "MapleClientCollectionTypes/ZRef.h"
#include "MapleClientCollectionTypes/ZArray.h"
#include "MapleClientCollectionTypes/ZList.h"
#include "MapleClientCollectionTypes/TSecType.h"
#include "MapleClientCollectionTypes/ZFatalSection.h"
#include "MapleClientCollectionTypes/winhook_types.h"
#include "winsock2.h"
#include "timeapi.h"
#include "comdef.h"

#pragma region Ezorsia Prepackaged

template <typename TType> TType ReadValue(DWORD dwAddr) { return *((TType*)dwAddr); }
struct _bstr_t__Data_t
{
	wchar_t* m_wstr;
	char* m_str;
	unsigned int m_RefCount;
};
struct _bstr_t2 { _bstr_t__Data_t* m_Data; };

//struct ZFatalSection { ZFatalSectionData FatalData; }; //size 8
struct ZInetAddr { sockaddr_in my_IP_wrapper; };	//10 bytes
//struct _WSABUF { unsigned int len; char* buf; };
__declspec(align(4))struct ZSocketBuffer : ZRefCounted, _WSABUF, ZRefCountedAccessorBase { ZRef<ZSocketBuffer> _m_pParent; };//__unaligned __declspec
struct ZSocketBase { SOCKET _m_hSocket; };
struct ZException { HRESULT m_hr; };
struct _variant_t2 : tagVARIANT { };
//section start: ty to bubbling/Bia10 for the definitions/classes in this section; their release is at https://github.com/MapleDevelopers/MaplestoryV95.1
struct Ztl_variant_t : _variant_t2 { };
struct Ztl_bstr_t : _bstr_t2 { };
class ZSyncAutoUnlock
{
public:
	ZSyncAutoUnlock(ZFatalSection* lock) : m_pLock(lock)
	{
	}

	~ZSyncAutoUnlock()
	{
		if (m_pLock)
		{
			if (--m_pLock->_m_nRef == 1)
			{
				m_pLock->_m_pTIB = 0;
			}
		}
	}
private:
	ZFatalSection* m_pLock;
};
//end of section from bubbling's release

struct CInPacket	////size 24
{
	int m_bLoopback;	//size 4
	int m_nState;	//size 4
	ZArray<unsigned char> m_aRecvBuff;	//size 1 //pad3
	unsigned __int16 m_uLength;	//size 2
	unsigned __int16 m_uRawSeq;	//size 2
	unsigned __int16 m_uDataLen;	//size 2 //pad2
	unsigned int m_uOffset;	//size 4
};
struct ISMSG //size 12
{
	unsigned int message; //0
	unsigned int wParam; //4
	int lParam; //8
};
struct IUnknownVtbl { //size 12
	HRESULT(__stdcall* QueryInterface)(IUnknown* pThis, _GUID*, void**);
	unsigned int(__stdcall* AddRef)(IUnknown* pThis);
	unsigned int(__stdcall* Release)(IUnknown* pThis);
	//void* QueryInterface;
	//void* AddRef;
	//void* Release;
};
struct ILUnknown { IUnknownVtbl* vfptr; }; //size 4
struct IWzGr2D : ILUnknown { }; //size 4
struct _com_ptr_t_com_IIID_IWzGr2D { IWzGr2D* m_pInterface; };
struct _TypeDescriptor2
{
	const void* pVFTable;
	void* spare;
	char name[];
};
struct _PMD2
{
	int mdisp;
	int pdisp;
	int vdisp;
};
struct _s__CatchableType2
{
	unsigned int properties;
	_TypeDescriptor2* pType;
	_PMD2 thisDisplacement;
	int sizeOrOffset;
	void(__cdecl* copyFunction)();
};
struct _s__CatchableTypeArray2
{
	int nCatchableTypes;
	_s__CatchableType2* arrayOfCatchableTypes[];
};
struct _s__ThrowInfo2
{
	unsigned int attributes;
	void(__cdecl* pmfnUnwind)();
	int (*pForwardCompat)(...);
	_s__CatchableTypeArray2* pCatchableTypeArray;
};
struct CMSException : ZException {};
struct CPatchException__PATCHINFO
{
	unsigned __int16 nCurrentVersion;
	unsigned __int16 nTargetVersion;
	char sReserved[768];
	char sCommandLine[256];
	char sRootPath[256];
};
struct CPatchException : CMSException
{
	CPatchException__PATCHINFO m_pi;
};
struct CRTTI { CRTTI* m_pPrev; };
struct __declspec(align(1)) FUNCKEY_MAPPED { char nType; int nID; };
struct IWzSerialize : ILUnknown { };
struct IWzShape2D : IWzSerialize { };
struct IWzVector2D : IWzShape2D { };
struct IWzGr2DLayer : IWzVector2D { };
struct _com_ptr_t_IWzGr2DLayer_GUID_6dc8c7ce_8e81_4420_b4f6_4b60b7d5fcdf { IWzGr2DLayer* m_pInterface; };
struct __declspec(align(4)) IDraggable : ZRefCounted
{
	_com_ptr_t_IWzGr2DLayer_GUID_6dc8c7ce_8e81_4420_b4f6_4b60b7d5fcdf m_pLayer;
	FUNCKEY_MAPPED m_OldIcon;
};
struct INetMsgHandler { void* vfptr; }; //INetMsgHandlerVtbl*
struct INetMsgHandlerVtbl { void(__thiscall* OnPacket)(INetMsgHandler* pThis, int, CInPacket*); };
struct IUIMsgHandler { void* vfptr; }; //IUIMsgHandlerVtbl*
struct IUIMsgHandlerVtbl
{
	void(__thiscall* OnKey)(IUIMsgHandler* pThis, unsigned int, unsigned int);
	BYTE gap4[8];
	int(__thiscall* OnMouseMove)(IUIMsgHandler* pThis, int, int);
	int(__thiscall* OnMouseWheel)(IUIMsgHandler* pThis, int, int, int);
	void(__thiscall* OnMouseEnter)(IUIMsgHandler* pThis, int);
	void(__thiscall* OnDraggableMove)(IUIMsgHandler* pThis, int, IDraggable*, int, int);
	void(__thiscall* SetEnable)(IUIMsgHandler* pThis, int);
	int(__thiscall* IsEnabled)(IUIMsgHandler* pThis);
	void(__thiscall* SetShow)(IUIMsgHandler* pThis, int);
	int(__thiscall* IsShown)(IUIMsgHandler* pThis);
	int(__thiscall* GetAbsLeft)(IUIMsgHandler* pThis);
	int(__thiscall* GetAbsTop)(IUIMsgHandler* pThis);
	void(__thiscall* ClearToolTip)(IUIMsgHandler* pThis);
	void(__thiscall* OnIMEModeChange)(IUIMsgHandler* pThis, char);
	void(__thiscall* OnIMEResult)(IUIMsgHandler* pThis, const char*);
	void(__thiscall* OnIMEComp)(IUIMsgHandler* pThis, const char*, ZArray<unsigned long>*, unsigned int, int, ZList<ZXString<char> >*, int, int, int);
	CRTTI* (__thiscall* GetRTTI)(IUIMsgHandler* pThis);
	int(__thiscall* IsKindOf)(IUIMsgHandler* pThis, CRTTI*);
};
struct IGObj { void* vfptr; }; //IGObjVtbl*
struct IGObjVtbl { void(__thiscall* Update)(IGObj* pThis); };
struct CStage : IGObj, IUIMsgHandler, INetMsgHandler, ZRefCounted { }; //#3072
struct CClientSocket_CONNECTCONTEXT { ZList<ZInetAddr> my_IP_Addresses; void* posList; int bLogin; }; //size 28 //4 bytes dd for voids //__POSITION*
//struct CClientSocketVtbl { void*(__thiscall* __vecDelDtor)(CClientSocket* pThis, unsigned int); }; //clientsocket definition error? switching to void
struct CClientSocket {	//singleton		//size 148
	void* vfptr;//00000000 vfptr dd ? ; offset //size 4
	HWND__* m_hWnd;//00000004 m_hWnd dd ? ; offset		//size 4
	ZSocketBase m_sock;//00000008 m_sock ZSocketBase ?	//size 4
	CClientSocket_CONNECTCONTEXT m_ctxConnect;	//0000000C m_ctxConnect CClientSocket::CONNECTCONTEXT ? //size 28
	ZInetAddr m_addr;	//00000028 m_addr ZInetAddr ?	//size 16 //4 ints, for IP address
	DWORD m_tTimeout;//00000038 m_tTimeout dd ?	//size 4
	ZList<ZRef<ZSocketBuffer>> m_lpRecvBuff;//0000003C m_lpRecvBuff ZList<ZRef<ZSocketBuffer> > ? //size 20
	ZList<ZRef<ZSocketBuffer>> m_lpSendBuff;//00000050 m_lpSendBuff ZList<ZRef<ZSocketBuffer> > ? //size 20
	CInPacket m_packetRecv;//00000064 m_packetRecv CInPacket ? //size 24
	ZFatalSection m_lockSend;//0000007C m_lockSend ZFatalSection ? //size 8
	unsigned int m_uSeqSnd;//00000084 m_uSeqSnd dd ?	//size 4
	unsigned int m_uSeqRcv;//00000088 m_uSeqRcv dd ?	//size 4
	ZXString<char> m_URLGuestIDRegistration;	//0000008C m_URLGuestIDRegistration ZXString<char> ? //size 4
	int m_bIsGuestID;//00000090 m_bIsGuestID dd ?	//size 4
};//1713	CClientSocket	00000094	Auto	struct __cppobj : TSingleton<CClientSocket> {CClientSocketVtbl *vfptr;HWND__ *m_hWnd;
struct CUIStatusBar {
	struct CChatLog : ZRefCounted {
		ZXString<unsigned short> m_sChat;
		int _ZtlSecureTear_m_nType[2];
		unsigned int _ZtlSecureTear_m_nType_CS;
		int m_nBack;
		int m_nChannelID;
		int m_bWhisperIcon;
		int m_bFirstLine;
		ZRef<void>* m_pItem;//ZRef<GW_ItemSlotBase> m_pItem;
	};
};	//fix this //fixed?try?
struct CWvsApp { //singleton
	void* vfptr; //0
	HWND__* m_hWnd; //4
	int m_bPCOMInitialized; //8
	unsigned int m_dwMainThreadId; //12
	HHOOK__* m_hHook; //16
	int m_bWin9x; //20
	int m_tUpdateTime; //44
	int m_bFirstUpdate; //48
	ZXString<char> m_sCmdLine; //52
	int m_nGameStartMode; //56
	int m_bAutoConnect; //60
	int m_bShowAdBalloon; //64
	int m_bExitByTitleEscape; //68
	HRESULT m_hrZExceptionCode; //72
	HRESULT m_hrComErrorCode; //76
	unsigned int m_dwSecurityErrorCode; //80
	int m_nTargetVersion; //84
	int m_tLastServerIPCheck; //88
	int m_tLastServerIPCheck2; //92
	int m_tLastGGHookingAPICheck; //96
	int m_tLastSecurityCheck; //100
	void* m_ahInput[3]; //size 12 //104 //84
	int m_tNextSecurityCheck; //116 //96
	bool m_bEnabledDX9; //size 1, pad 3 //120 //100
	ZArray<unsigned char> m_pBackupBuffer; //124 //104
	unsigned int m_dwBackupBufferSize; //128 //108
	unsigned int m_dwClearStackLog; //132 //112
	int m_bWindowActive; //136 //116
};

DWORD GetFuncAddress(LPCSTR lpModule, LPCSTR lpFunc) {	//utility function also found in mapleclient template
	HMODULE mod = LoadLibraryA(lpModule); //ty alias! for sharing their version	//ps i made some changes
	if (!mod)
	{
		std::cout << "GetFuncAddress failed at " << lpModule << std::endl;
		return 0;
	}
	DWORD address = (DWORD)GetProcAddress(mod, lpFunc);
	if (!address)
	{
		std::cout << "GetFuncAddress failed at " << lpFunc << std::endl;
		return 0;
	}
	return address;
}

//sub_9DE438    end 009DE7F8	//returns int but if hooking to run and run other stuff after, dont actually return
typedef void(__fastcall* _CWndCreateWnd_t)(void* pThis, void* edx, int nLeft, int nTop, int nWidth, int nHeight, int z, int bScreenCoord, void* esi, int bSetFocus);
static auto _sub_9DE4D2 = reinterpret_cast<_CWndCreateWnd_t>(0x009DE4D2); //thanks you teto for helping me on this learning journey
static _CWndCreateWnd_t _CWndCreateWnd_Hook = [](void* pThis, void* edx, int nLeft, int nTop, int nWidth, int nHeight, int z, int bScreenCoord, void* esi, int bSetFocus)
-> void {_sub_9DE4D2(pThis, edx, nLeft, nTop, nWidth, nHeight, z, bScreenCoord, esi, bSetFocus); //changing the values here will modify the result of the function
//usually put changes here //this only runs after the original execution
	};
typedef void(__fastcall* _CLoginSendSelectCharPacket_t)(void* pThis, void* edx);    //sub_5F726D    end 005F7690
static auto _sub_5F726D = reinterpret_cast<_CLoginSendSelectCharPacket_t>(0x005F726D);
static _CLoginSendSelectCharPacket_t _CLoginSendSelectCharPacket_Hook = [](void* pThis, void* edx)
-> void {_sub_5F726D(pThis, edx); //changing the values here will modify the result of the function
	};

enum RESMAN_PARAM {
	RC_AUTO_SERIALIZE = 0x1,
	RC_AUTO_SERIALIZE_NO_CACHE = 0x2,
	RC_NO_AUTO_SERIALIZE = 0x4,
	RC_DEFAULT_AUTO_SERIALIZE = 0x0,
	RC_AUTO_SERIALIZE_MASK = 0x7,
	RC_AUTO_REPARSE = 0x10,
	RC_NO_AUTO_REPARSE = 0x20,
	RC_DEFAULT_AUTO_REPARSE = 0x0,
	RC_AUTO_REPARSE_MASK = 0x30,
};

// DWORD Address
auto g_rm = (void*)0x00BF14E8; //static?	//ptr to
auto g_root = (void**)0x00BF14E0; //ptr to	//_com_ptr_t<_com_IIID<IWzNameSpace,&_GUID_2aeeeb36_a4e1_4e2b_8f6f_2e7bdec5c53d> > g_root
auto pNameSpace = 0x00BF0CD0;//converted to int ptr then deref to get val

// Generic
void* pUnkOuter = 0;
void* nPriority = 0;
//NOTE: resman rewrites do not match native typing in IDA. this may have been done purposely for the rewrite
typedef void(__fastcall* _CWvsApp__InitializeResMan_t)(CWvsApp* pThis, void* edx);	//ty to all the contributors of the ragezone release: Client load .img instead of .wz v62~v92 //ty y785	//char* ecx, char* edx
static auto _sub_9F7159 = reinterpret_cast<_CWvsApp__InitializeResMan_t>(0x009F7159);    //sub_9F7159    end 009F7909 //void __thiscall CWvsApp::InitializeResMan(CWvsApp *this)

typedef void(__cdecl* _PcCreateObject_IWzResMan_t)(const wchar_t* sUOL, void* pObj, void* pUnkOuter);   //sub_9FAF55    end 009FAFB9
static auto _sub_9FAF55 = reinterpret_cast<_PcCreateObject_IWzResMan_t>(0x009FAF55);
static _PcCreateObject_IWzResMan_t _PcCreateObject_IWzResMan_Hook = [](const wchar_t* sUOL, void* pObj, void* pUnkOuters) {
	//-> void {_PcCreateObject_IWzResMan(sUOL, pObj, pUnkOuter); //remove -> part and redefine to replace parts of execution code proper
	_sub_9FAF55(sUOL, pObj, pUnkOuter);
	};

typedef void(__cdecl* _PcCreateObject_IWzNameSpace_t)(const wchar_t* sUOL, void* pObj, void* pUnkOuter);    //sub_9FAFBA    end 009FB01E
static auto _sub_9FAFBA = reinterpret_cast<_PcCreateObject_IWzNameSpace_t>(0x009FAFBA);//void __cdecl PcCreateObject(const wchar_t* sUOL, _com_ptr_t<_com_IIID<IWzNameSpace, &_GUID_2aeeeb36_a4e1_4e2b_8f6f_2e7bdec5c53d> > *pObj, IUnknown * pUnkOuter)
static _PcCreateObject_IWzNameSpace_t _PcCreateObject_IWzNameSpace_Hook = [](const wchar_t* sUOL, void* pObj, void* pUnkOuters) {
	//-> void {_PcCreateObject_IWzNameSpace(sUOL, pObj, pUnkOuter);
	_sub_9FAFBA(sUOL, pObj, pUnkOuter);
	};

typedef void(__cdecl* _PcCreateObject_IWzFileSystem_t)(const wchar_t* sUOL, void* pObj, void* pUnkOuter);   //sub_9FB01F    end 009FB083
static auto _sub_9FB01F = reinterpret_cast<_PcCreateObject_IWzFileSystem_t>(0x009FB01F);//void __cdecl PcCreateObject(const wchar_t *sUOL, _com_ptr_t<_com_IIID<IWzFileSystem,&_GUID_352d8655_51e4_4668_8ce4_0866e2b6a5b5> > *pObj, IUnknown *pUnkOuter)
static _PcCreateObject_IWzFileSystem_t _PcCreateObject_IWzFileSystem_Hook = [](const wchar_t* sUOL, void* pObj, void* pUnkOuters) {
	//-> void {_PcCreateObject_IWzFileSystem(sUOL, pObj, pUnkOuter);
	_sub_9FB01F(sUOL, pObj, pUnkOuter);
	};

typedef void(__cdecl* _sub_9FB084_t)(const wchar_t* sUOL, void* pObj, void* pUnkOuter);   //sub_9FB01F    end 009FB083
static auto _sub_9FB084 = reinterpret_cast<_sub_9FB084_t>(0x009FB084);//9FB084 //void __cdecl PcCreateObject_IWzPackage(const wchar_t *sUOL, ??? *pObj, IUnknown *pUnkOuter)

typedef void(__cdecl* _CWvsApp__Dir_BackSlashToSlash_t)(char* sDir);
static auto _sub_9F95FE = reinterpret_cast<_CWvsApp__Dir_BackSlashToSlash_t>(0x009F95FE);    //sub_9F95FE	end 009F9620
bool _CWvsApp__Dir_BackSlashToSlash_rewrite_initialized_2 = true;
static _CWvsApp__Dir_BackSlashToSlash_t _CWvsApp__Dir_BackSlashToSlash_rewrite = [](char* sDir) {
	//-> void {_CWvsApp__Dir_BackSlashToSlash(sDir); 
	if (_CWvsApp__Dir_BackSlashToSlash_rewrite_initialized_2)
	{
		std::cout << "Hook_CWvsApp__Dir_BackSlashToSlash started" << std::endl;
		_CWvsApp__Dir_BackSlashToSlash_rewrite_initialized_2 = false;
	}
	int myLength = strlen(sDir);
	for (int i = 0; i < myLength; ++i) { if (sDir[i] == '\\') { sDir[i] = '/'; } }
	};

typedef void(__cdecl* _CWvsApp__Dir_upDir_t)(char* sDir);
static auto _sub_9F9644 = reinterpret_cast<_CWvsApp__Dir_upDir_t>(0x009F9644);  //sub_9F9644	end 009F9679
static _CWvsApp__Dir_upDir_t _CWvsApp__Dir_upDir_Hook = [](char* sDir) {
	//-> void {_CWvsApp__Dir_upDir(sDir); 
	_sub_9F9644(sDir); };

typedef void* (__fastcall* _bstr_ctor_t)(void* pThis, void* edx, const char* str); //dont use comutil.h bstr_t, nXXXon's is different   //sub_406301	end	00406356
static auto _sub_406301 = reinterpret_cast<_bstr_ctor_t>(0x00406301);	//void __thiscall _bstr_t::_bstr_t(_bstr_t *this, const char *s)
static _bstr_ctor_t _bstr_ctor_Hook = [](void* pThis, void* edx, const char* str) { //can make it return the ptr of inserted val
	return _sub_406301(pThis, nullptr, str); };

typedef void* (__fastcall* _sub_425ADD_t)(Ztl_bstr_t* pThis, void* edx, const char* str); //can make it return the ptr of inserted val   //sub_425ADD
static auto _sub_425ADD = reinterpret_cast<_sub_425ADD_t>(0x00425ADD);//void __thiscall Ztl_bstr_t::Ztl_bstr_t(Ztl_bstr_t *this, const char *s) //Ztl_bstr_t ctor

typedef HRESULT(__fastcall* _IWzFileSystem__Init_t)(void* pThis, void* edx, Ztl_bstr_t sPath); //sub_9F7964	//HRESULT
static auto _sub_9F7964 = reinterpret_cast<_IWzFileSystem__Init_t>(0x009F7964);//HRESULT __thiscall IWzFileSystem::Init(IWzFileSystem *this, Ztl_bstr_t sPath)

typedef unsigned int(__thiscall* _sub_402EA5_t)(_bstr_t__Data_t* pThis);
static auto _sub_402EA5 = reinterpret_cast<_sub_402EA5_t>(0x00402EA5);//unsigned int __thiscall _bstr_t::Data_t::Release(_bstr_t::Data_t *this)

typedef void* (__thiscall* _sub_4039AC_t)(Ztl_variant_t* pvargDest, Ztl_variant_t* pvargSrc, char a3);//could be wrong
static auto _sub_4039AC = reinterpret_cast<_sub_4039AC_t>(0x004039AC);//non-existent func in v95//int __thiscall sub_4039AC(VARIANTARG *pvargDest, VARIANTARG *pvargSrc, char)

auto _unk_BE2EC0 = (_GUID*)0x00BE2EC0;//GUID _GUID_352d8655_51e4_4668_8ce4_0866e2b6a5b5
auto _unk_BD8F28 = (_GUID*)0x00BD8F28;//GUID _GUID_2aeeeb36_a4e1_4e2b_8f6f_2e7bdec5c53d

typedef HRESULT(__fastcall* _IWzNameSpace__Mount_t)(void* pThis, void* edx, Ztl_bstr_t sPath, void* pDown, int nPriority); //HRESULT
static auto _sub_9F790A = reinterpret_cast<_IWzNameSpace__Mount_t>(0x009F790A);    //sub_9F790A	//HRESULT __thiscall IWzNameSpace::Mount(IWzNameSpace *this, Ztl_bstr_t sPath, IWzNameSpace *pDown, int nPriority)
static _IWzNameSpace__Mount_t _IWzNameSpace__Mount_Hook = [](void* pThis, void* edx, Ztl_bstr_t sPath, void* pDown, int nPriority) {
	//-> HRESULT {_IWzNameSpace__Mount(pThis, edx, sPath, pDown, nPriority); //HRESULT //return _IWzNameSpace__Mount(pThis, edx, sPath, pDown, nPriority);
	return _sub_9F790A(pThis, nullptr, sPath, pDown, nPriority);
	};

typedef Ztl_variant_t* (__fastcall* _sub_5D995B_t)(void* pThis, void* edx, Ztl_variant_t* result, Ztl_bstr_t sPath);
static auto _sub_5D995B = reinterpret_cast<_sub_5D995B_t>(0x005D995B);//_sub_5D995B//Ztl_variant_t *__thiscall IWzNameSpace::Getitem(IWzNameSpace *this, Ztl_variant_t *result, Ztl_bstr_t sPath)

typedef IUnknown* (__fastcall* _sub_4032B2_t)(Ztl_variant_t* pThis, void* edx, bool fAddRef, bool fTryChangeType);
static auto _sub_4032B2 = reinterpret_cast<_sub_4032B2_t>(0x004032B2);//IUnknown* __thiscall Ztl_variant_t::GetUnknown(Ztl_variant_t* this, bool fAddRef, bool fTryChangeType)

typedef void(__fastcall* _sub_9FCD88_t)(void* pThis, void* edx, IUnknown* p);
static auto _sub_9FCD88 = reinterpret_cast<_sub_9FCD88_t>(0x009FCD88);//sub_9FCD88 //void __thiscall <IWzSeekableArchive(IWzSeekableArchive* this, IUnknown* p)

typedef ZXString<char>* (__fastcall* _StringPool__GetString_t)(void* pThis, void* edx, ZXString<char>* result, unsigned int nIdx, char formal); //also ty to the creators of MapleClientEditTemplate
static auto _sub_79E993 = reinterpret_cast<_StringPool__GetString_t>(0x0079E993);//hook stringpool modification //ty !! popcorn //ty darter //ty teto

typedef unsigned int* (__fastcall* _sub_78C8A6_t)(unsigned int[], void* edx);  //sub_78C8A6	end 0078D165	//custom return
static auto _sub_78C8A6 = reinterpret_cast<_sub_78C8A6_t>(0x0078C8A6); //void __thiscall NEXTLEVEL::NEXTLEVEL(NEXTLEVEL *this)

//__thiscall CUIStatusBar::ChatLogAdd(CUIStatusBar *this, const char *sChat, int lType, int nChannelID, int bWhisperIcon, ZRef<GW_ItemSlotBase> pItem)
typedef void(__fastcall* _CUIStatusBar__ChatLogAdd_t)(void* pThis, void* edx, const char* sChat, int lType, int nChannelID, int bWhisperIcon, ZRef<void>* pItem);
static auto _sub_8DB070 = reinterpret_cast<_CUIStatusBar__ChatLogAdd_t>(0x008DB070);  //sub_8DB070	end 008DB45A
bool Hook_CUIStatusBar__ChatLogAdd_initialized = true;
static _CUIStatusBar__ChatLogAdd_t _CUIStatusBar__ChatLogAdd_Hook = [](void* pThis, void* edx, const char* sChat, int lType, int nChannelID, int bWhisperIcon, ZRef<void>* pItem) {
	if (Hook_CUIStatusBar__ChatLogAdd_initialized)
	{
		std::cout << "Hook_CUIStatusBar__ChatLogAdd started" << std::endl;
		Hook_CUIStatusBar__ChatLogAdd_initialized = false;
	}
	ZArray<ZRef<CUIStatusBar::CChatLog>>* m_aChatLog = reinterpret_cast<ZArray<ZRef<CUIStatusBar::CChatLog>>*>(0x00BF1100);
	while (m_aChatLog->GetCount() > 1000) { // you can set your custom limit here //not working, crashes
		auto idx = m_aChatLog->IndexOf(0);	//ty to releaser of forum.ragezone.com/threads/set-any-chat-bar-limit-default-is-64.1209514/
		m_aChatLog->RemoveAt(idx);
	}
	_sub_8DB070(pThis, nullptr, sChat, lType, nChannelID, bWhisperIcon, pItem);
	};

typedef int(__cdecl* _sub_44E6C3_t)(int a1, unsigned int a2);
static auto _sub_44E6C3 = reinterpret_cast<_sub_44E6C3_t>(0x0044E6C3);

typedef unsigned int(__cdecl* _sub_44E546_t)(unsigned __int8* a1, int a2);	//sub_44E546	end 0044E5D4
static auto _sub_44E546 = reinterpret_cast<_sub_44E546_t>(0x0044E546);

typedef unsigned int(__cdecl* _sub_44E5D5_t)(int a1, void* a2, size_t a3);	//1	//sub_44E5D5	end	0044E6C2
static auto _sub_44E5D5 = reinterpret_cast<_sub_44E5D5_t>(0x0044E5D5);	//3

typedef void(__cdecl* _sub_A61DF2_t)(void* lpMem);//void __cdecl free(void *)
static auto _sub_A61DF2 = reinterpret_cast<_sub_A61DF2_t>(0x00A61DF2);

typedef unsigned int(__cdecl* _sub_44E716_t)(int a1, void* a2, size_t a3);	//sub_44E716	end 0044E822
static auto _sub_44E716 = reinterpret_cast<_sub_44E716_t>(0x0044E716);

typedef int(__stdcall* (__stdcall* _sub_44E88E_t)(HINSTANCE__* hModule, const char* lpProcName))();	//sub_44E88E	end	0044EA61	//GetProcAddress function (libloaderapi.h), not sure if nXXXon changed it
static auto _sub_44E88E = reinterpret_cast<_sub_44E88E_t>(0x0044E88E);//int (__stdcall *__stdcall MyGetProcAddress(HINSTANCE__ *hModule, const char *lpProcName))()

typedef HMODULE(__fastcall* _sub_44EA64_t)(void* pThis, void* edx);	//sub_44EA64	end	0044EBEB
static auto _sub_44EA64 = reinterpret_cast<_sub_44EA64_t>(0x0044EA64);

typedef DWORD* (__cdecl* _sub_44EC9C_t)(int a1);	//1	//sub_44EC9C	end	0044ED46
static auto _sub_44EC9C = reinterpret_cast<_sub_44EC9C_t>(0x0044EC9C);	//3	//HideDll

typedef void* (__thiscall* _sub_403065_t)(ZAllocEx<ZAllocAnonSelector>* pThis, unsigned int uSize);
static auto _sub_403065 = reinterpret_cast<_sub_403065_t>(0x00403065);	//void *__thiscall ZAllocEx<ZAllocAnonSelector>::Alloc(ZAllocEx<ZAllocAnonSelector> *this, unsigned int uSize)

typedef void(__thiscall* _sub_4031ED_t)(ZAllocEx<ZAllocAnonSelector>* pThis, DWORD* a2);	//ZAllocEx<ZAllocAnonSelector>::Free
static auto _sub_4031ED = reinterpret_cast<_sub_4031ED_t>(0x004031ED);	//3

typedef void(__cdecl* _sub_44ED47_t)();//sub_44ED47	end	0044EEE6
static auto _sub_44ED47 = reinterpret_cast<_sub_44ED47_t>(0x0044ED47);//void __cdecl ResetLSP(void)

//ZAllocEx<ZAllocAnonSelector>* _unk_BF0B00 = reinterpret_cast<ZAllocEx<ZAllocAnonSelector>*>(0x00BF0B00);	//ZAllocEx<class ZAllocAnonSelector> ZAllocEx<class ZAllocAnonSelector>::_s_alloc
auto _unk_BF0B00 = (ZAllocEx<ZAllocAnonSelector>*)0x00BF0B00; //_unk_BF0B00 = ZAllocEx<ZAllocAnonSelector> ZAllocEx<ZAllocAnonSelector>::_s_alloc

typedef DWORD* (__thiscall* _sub_496ADF_t)(DWORD* pThis);
static auto _sub_496ADF = reinterpret_cast<_sub_496ADF_t>(0x00496ADF);	//3

typedef ZXString<char>* (__thiscall* _sub_9F94A1_t)(CWvsApp* pThis, ZXString<char>* result, int nArg);
static auto _sub_9F94A1 = reinterpret_cast<_sub_9F94A1_t>(0x009F94A1);//ZXString<char> *__thiscall CWvsApp::GetCmdLine(CWvsApp *this, ZXString<char> *result, int nArg)

typedef int(__thiscall* _sub_496E9F_t)(DWORD* pThis);
static auto _sub_496E9F = reinterpret_cast<_sub_496E9F_t>(0x00496E9F);	//3

typedef DWORD* (__thiscall* _sub_494C1A_t)(DWORD* pThis, char* cp, unsigned short netshort);
static auto _sub_494C1A = reinterpret_cast<_sub_494C1A_t>(0x00494C1A);	//3

typedef DWORD* (__thiscall* _sub_494BE9_t)(void* pThis, int a2);
static auto _sub_494BE9 = reinterpret_cast<_sub_494BE9_t>(0x00494BE9);	//3

//sub_00474C2E = ZArray<long>::InsertBefore(_DWORD * this, int a2)

typedef void(__thiscall* _sub_496EDD_t)(ZList<ZInetAddr>* pThis);	//void __thiscall ZList<ZInetAddr>::RemoveAll(ZList<ZInetAddr> *this)
static auto _sub_496EDD = reinterpret_cast<_sub_496EDD_t>(0x00496EDD);	//3

//sub_457245 = __thiscall ZArray<long>::RemoveAll(_DWORD * this)

typedef int(__thiscall* _sub_496E6B_t)(int* pThis, void* a2);
static auto _sub_496E6B = reinterpret_cast<_sub_496E6B_t>(0x00496E6B);	//3

typedef int(__thiscall* _sub_494C7A_t)(void* pThis, __int16 a2, int a3);
static auto _sub_494C7A = reinterpret_cast<_sub_494C7A_t>(0x00494C7A);	//3

typedef void(__thiscall* _sub_496B9B_t)(ZList<ZInetAddr>* pThis, ZList<ZInetAddr>* l);	//void __thiscall ZList<ZInetAddr>::AddTail(ZList<ZInetAddr> *this, ZList<ZInetAddr> *l)
static auto _sub_496B9B = reinterpret_cast<_sub_496B9B_t>(0x00496B9B);	//3

typedef void(__fastcall* _sub_494D07_t)(CClientSocket_CONNECTCONTEXT* pThis, void* edx, CClientSocket_CONNECTCONTEXT* a2); //sub_494D07	end	00494D2C
static auto _sub_494D07 = reinterpret_cast<_sub_494D07_t>(0x00494D07);	//3 //unknown function, part of connect

typedef void(__thiscall* _sub_4969EE_t)(CClientSocket* pThis); //void __thiscall CClientSocket::ClearSendReceiveCtx(CClientSocket *this)
static auto _sub_4969EE = reinterpret_cast<_sub_4969EE_t>(0x004969EE);	//3

typedef void(__thiscall* _sub_494857_t)(ZSocketBase* pThis);	//void __thiscall ZSocketBase::CloseSocket(ZSocketBase* this)
static auto _sub_494857 = reinterpret_cast<_sub_494857_t>(0x00494857);	//switching to void* instead of set up Z smart pointer

typedef void(__thiscall* _sub_496369_t)(CClientSocket* pThis);  //CClientSocket::Close	//CClientSocket* pThis
static auto _sub_496369 = reinterpret_cast<_sub_496369_t>(0x00496369);	//3

typedef ZSocketBuffer* (__cdecl* _sub_495FD2_t)(unsigned int u);	//ZSocketBuffer::Alloc(unsigned int u)
static auto _sub_495FD2 = reinterpret_cast<_sub_495FD2_t>(0x00495FD2);	//3

typedef void(__thiscall* _sub_496C2B_t)(void* pThis);	//__thiscall ZRef<ZSocketBuffer>::~ZRef<ZSocketBuffer>(ZRef<ZSocketBuffer> *this)
static auto _sub_496C2B = reinterpret_cast<_sub_496C2B_t>(0x00496C2B);

typedef void(__cdecl* _sub_4062DF_t)(void* pData); //void __cdecl ZXString<char>::_Release(ZXString<char>::_ZXStringData *pData)  //use ZXString<char>::~ZXString as destructor
static auto _sub_4062DF = reinterpret_cast<_sub_4062DF_t>(0x004062DF);	//_ZXStringData *pData

typedef int(__cdecl* _sub_46F37B_t)(ZXString<char>* s, const char* pSrc, unsigned int uRemain);	//CIOBufferManipulator::DecodeStr
static auto _sub_46F37B = reinterpret_cast<_sub_46F37B_t>(0x0046F37B);

typedef int(__thiscall* _sub_4065A6_t)(DWORD* pThis, int a2);	//COutPacket::Encode4
static auto _sub_4065A6 = reinterpret_cast<_sub_4065A6_t>(0x004065A6);

typedef int(__thiscall* _sub_406549_t)(DWORD* pThis, char a2);
static auto _sub_406549 = reinterpret_cast<_sub_406549_t>(0x00406549);	//COutPacket::Encode1

typedef void(__thiscall* _sub_51E834_t)(CPatchException* pThis, int nTargetVersion);//void __thiscall CPatchException::CPatchException(CPatchException* this, int nTargetVersion)
static auto _sub_51E834 = reinterpret_cast<_sub_51E834_t>(0x0051E834);

char* _byte_BF1AD0 = reinterpret_cast<char*>(0x00BF0B00); //??
char* _byte_BF1ACF = reinterpret_cast<char*>(0x00BF1ACF); //??
auto _sub_9F9621 = (void(__cdecl*)(char*))0x009F9621;//void __cdecl CWvsApp::Dir_SlashToBackSlash(char *sDir)
auto _sub_406455 = (void* (__thiscall*)(void* pThis, ZXString<char>*result, unsigned int nIdx))0x00406455;
auto _sub_79E805 = (int(__fastcall*)())0x0079E805; //??
auto _dword_BF03BC = (int(__stdcall*)(DWORD))0x00BF03BC;//redefine/change to microsoft version
auto _dword_BF039C = (int(__stdcall*)(DWORD, DWORD, DWORD))0x00BF039C;//redefine/change to microsoft version


const DWORD dw_sub_9F9808 = 0x009F9808;
typedef const char* (__cdecl* _sub_9F9808_t)(void);	//const char* __cdecl CWvsApp::GetExceptionFileName()
static auto _sub_9F9808 = reinterpret_cast<_sub_9F9808_t>(dw_sub_9F9808);//sub_9F9808 end 009F9892
//__stdcall func_hook_payload(int a, for__usercall func<eax>(int a<eax>,

typedef int(__thiscall* _sub_495704_t)(DWORD* pThis, int a2, int a3, int a4, int a5, int a6, int a7, int a8);
static auto _sub_495704 = reinterpret_cast<_sub_495704_t>(0x00495704);

typedef int(__thiscall* _sub_49588D_t)(void* pThis);
static auto _sub_49588D = reinterpret_cast<_sub_49588D_t>(0x0049588D);

typedef void(__thiscall* _sub_49637B_t)(CClientSocket* pThis, void* oPacket);	//CClientSocket::SendPacket	//const struct COutPacket* a2
static auto _sub_49637B = reinterpret_cast<_sub_49637B_t>(0x0049637B);	////CClientSocket* pThis //checks the return address, might not be able to be used in rewrites

typedef void* (__thiscall* _sub_46C00C_t)(DWORD* pThis, void* a2, size_t a3);	//COutPacket::EncodeBuffer
static auto _sub_46C00C = reinterpret_cast<_sub_46C00C_t>(0x0046C00C);

typedef signed int(__thiscall* _sub_4956A6_t)(DWORD* pThis);
static auto _sub_4956A6 = reinterpret_cast<_sub_4956A6_t>(0x004956A6);
const DWORD dw_sub_4959B8 = 0x004959B8;
__declspec(naked) size_t _sub_4959B8_asm(DWORD a, DWORD b, void* a2, size_t a3) {
	__asm {
		push a3
		push a2
		push ecx
		mov ecx, b
		push ebx
		mov ebx, a
		//return call dw_sub_4959B8	//returns in eax //inline assembler syntax error in 'opcode'; found 'opcode'
	}
}
typedef          char   int8;
typedef   signed char   sint8;
typedef unsigned char   uint8;
typedef          short  int16;
typedef   signed short  sint16;
typedef unsigned short  uint16;
typedef          int    int32;
typedef   signed int    sint32;
typedef unsigned int    uint32;
typedef long long           int64;
typedef long long           sint64;
typedef unsigned long long  uint64;
// carry flag of addition (x+y)
template<class T, class U> int8 __CFADD__(T x, U y)
{
	int size = sizeof(T) > sizeof(U) ? sizeof(T) : sizeof(U);
	if (size == 1)
		return uint8(x) > uint8(x + y);
	if (size == 2)
		return uint16(x) > uint16(x + y);
	if (size == 4)
		return uint32(x) > uint32(x + y);
	return uint64(x) > uint64(x + y);
}
typedef unsigned int(__fastcall* _sub_4959B8_t)(void* pThis, void* edx, void* pData, unsigned int uSize);	////sub_4959B8 end 00495A86
static auto _sub_4959B8 = reinterpret_cast<_sub_4959B8_t>(0x004959B8);	//unsigned int __thiscall ZFileStream::Read(ZFileStream *this, void *pData, unsigned int uSize)

auto _sub_49583A = (int(__cdecl*)(int a1, int a2, int a3, int a4))0x0049583A;
auto _dword_BF03A4 = (int(__cdecl*)(DWORD))0x00BF03A4;// GetLastError Windows API call
auto _dword_BF0358 = (int(__thiscall*)(DWORD, DWORD, DWORD, DWORD, void*, DWORD))0x00BF0358;

typedef char* (__thiscall* _sub_496CA9_t)(ZArray<unsigned char>* pThis, unsigned int u);	//char *__thiscall ZArray<unsigned char>::_Alloc(ZArray<unsigned char> *this, unsigned int u, ZAllocHelper *_ALLOC)
static auto _sub_496CA9 = reinterpret_cast<_sub_496CA9_t>(0x00496CA9);

auto _sub_6EC9CE = (DWORD * (__thiscall*)(DWORD * pThis, int a2))0x006EC9CE;	//COutPacket *__thiscall COutPacket::COutPacket(COutPacket *this, int a2)
auto _sub_427F74 = (int(__thiscall*)(DWORD * pThis, unsigned __int16 a2))0x00427F74;	//int __thiscall COutPacket::Encode2(COutPacket *this, unsigned __int16 a2)
auto _sub_428CF1 = (void(__thiscall*)(DWORD * pThis))0x00428CF1;	//void __thiscall ZArray<unsigned char>::RemoveAll(_DWORD *this)
auto _dword_BF0370 = (int(__stdcall*)(DWORD))0x00BF0370;//redefine/change to microsoft version
auto _dword_BE7918 = (ZXString<char>*)0x00BE7918; //ZXString<char> TSingleton<CWvsContext>::ms_pInstance
auto _dword_BE7B38 = (void**)0x00BE7B38; //protected: static class CWvsApp * TSingleton<class CWvsApp>::ms_pInstance
auto _off_AF2664 = (int(__stdcall*)(char))0x00AF2664; //???//redefine/change to microsoft version

typedef int(__thiscall* _Concurrency_details_AutoDeleter_t)(int(__stdcall**** pThis)(signed int));   //@ 00496A0F ///???
static auto _sub_496A0F = reinterpret_cast<_Concurrency_details_AutoDeleter_t>(0x00496A0F);
//Concurrency::details::_AutoDeleter<Concurrency::details::_TaskProcHandle>::~_AutoDeleter<Concurrency::details::_TaskProcHandle>

typedef void(__stdcall* _sub_A60BB7_t)(void* pExceptionObject, _s__ThrowInfo2* pThrowInfo);//_CxxThrowException	//void *pExceptionObject, _s__ThrowInfo*
static auto _CxxThrowException1 = reinterpret_cast<_sub_A60BB7_t>(0x00A60BB7);	//using this for now, cant get microsoft version to work
auto _TI1_AVZException__ = (_s__ThrowInfo2*)0x00B44EE0;	//_s__ThrowInfo
auto _TI3_AVCTerminateException__ = (_s__ThrowInfo2*)0x00B44760;
auto _TI3_AVCDisconnectException__ = (_s__ThrowInfo2*)0x00B48858;
auto _TI3_AVCPatchException__ = (_s__ThrowInfo2*)0x00B52FC8;

typedef unsigned int(__stdcall* _dword_BF060C_t)();	// unsigned int (__stdcall *timeGetTime)(); //IDB wrong
static auto _dword_BF060C = reinterpret_cast<_dword_BF060C_t>(0x00BF060C);//!!use microsoft version

typedef SOCKET(__stdcall* _dword_AF036C_t)(int, int, int);	//SOCKET __stdcall socket(int af, int type, int protocol)
static auto _dword_AF036C = reinterpret_cast<_dword_AF036C_t>(0x00AF036C);//!!use microsoft version

typedef int(__stdcall* _dword_AF0364_t)(void);	//ZAPI.WSAGetLastError()
static auto _dword_AF0364 = reinterpret_cast<_dword_AF0364_t>(0x00AF0364);//!!use microsoft version

typedef int(__stdcall* _dword_BF0640_t)(void);	//ZAPI.WSAGetLastError() 2?	//C6DB18
static auto _dword_BF0640 = reinterpret_cast<_dword_BF0640_t>(0x00BF0640);//!!use microsoft version

typedef HWND__* (__stdcall* _dword_BF0448_t)(void);	// HWND__ *(__stdcall *GetDesktopWindow)();
static auto _dword_BF0448 = reinterpret_cast<_dword_BF0448_t>(0x00BF0448);//!!use microsoft version

typedef int(__stdcall* _dword_BF0444_t)(HWND__*);	//int (__stdcall *GetWindowRect)(HWND__ *, tagRECT *);
static auto _dword_BF0444 = reinterpret_cast<_dword_BF0444_t>(0x00BF0444);//!!use microsoft version

typedef void(__stdcall* _dword_BF02F4_t)(unsigned int);//void(__stdcall* Sleep)(unsigned int);	//C6D7CC
static auto _dword_BF02F4 = reinterpret_cast<_dword_BF02F4_t>(0x00BF02F4);//!!use microsoft version

typedef unsigned int(__stdcall* _dword_BF04EC_t)(unsigned int, void**, int, unsigned int, unsigned int);//unsigned int (__stdcall *MsgWaitForMultipleObjects)(unsigned int, void **, int, unsigned int, unsigned int);
static auto _dword_BF04EC = reinterpret_cast<_dword_BF04EC_t*>(0x00BF04EC);//!!use microsoft version

typedef int(__stdcall* _dword_BF04E8_t)(tagMSG*, HWND__*, unsigned int, unsigned int, unsigned int);//int (__stdcall *PeekMessageA)(tagMSG *, HWND__ *, unsigned int, unsigned int, unsigned int);
static auto _dword_BF04E8 = reinterpret_cast<_dword_BF04E8_t>(0x00BF04E8);//!!use microsoft version

typedef int(__stdcall* _dword_BF0430_t)(tagMSG*);//int (__stdcall *TranslateMessage)(tagMSG *);
static auto _dword_BF0430 = reinterpret_cast<_dword_BF0430_t>(0x00BF0430);//!!use microsoft version

typedef int(__stdcall* _dword_BF042C_t)(tagMSG*);//int (__stdcall *DispatchMessageA)(tagMSG *);
static auto _dword_BF042C = reinterpret_cast<_dword_BF042C_t>(0x00BF042C);//!!use microsoft version

typedef void(__stdcall* _dword_BF041C_t)(int);//void (__stdcall *PostQuitMessage)(int);
static auto _dword_BF041C = reinterpret_cast<_dword_BF041C_t>(0x00BF041C);//!!use microsoft version

typedef int(__stdcall* _dword_BF064C_t)(unsigned int, const void*, int);//  int (__stdcall *connect)(unsigned int, const void *, int);
static auto _dword_BF064C = reinterpret_cast<_dword_BF064C_t>(0x00BF064C);//!!use microsoft version
//probably a easier way of writing it if dont need the type def		////ZAPI.recv	//!!use microsoft version
int(__stdcall* _dword_BF0674)(unsigned int, const char*, int, int) = reinterpret_cast<int(__stdcall*)(unsigned int, const char*, int, int)>(0x00BF0674);
//change back to * from **. it does look to be pointers from uninitialized functions, but those should "probably" work like normal functions on runtime
typedef int(__stdcall* _dword_BF062C_t)(unsigned int, HWND__*, unsigned int, int);//  int (__stdcall *WSAAsyncSelect)(unsigned int, HWND__ *, unsigned int, int);
static auto _dword_BF062C = reinterpret_cast<_dword_BF062C_t>(0x00BF062C);//!!use microsoft version

typedef unsigned __int64      QWORD;
#define MAKELONGLONG(a, b)    ((QWORD)(((DWORD)(((DWORD_PTR)(a)) & 0xffffffff)) | ((DWORD)((DWORD)(((DWORD_PTR)(b)) & 0xffffffff))) << 32))
#define LODWORD(ll)           ((DWORD)(((DWORD_PTR)(ll)) & 0xffffffff))
#define HIDWORD(ll)           ((DWORD)((((DWORD_PTR)(ll)) >> 32) & 0xffffffff))
#define SHIDWORD(x)			  (((int32)&(x)+1))

typedef int(__fastcall* _sub_494ED1_t)(CClientSocket* pThis, void* edx, int bSuccess);    //sub_494ED1    //end 004954C4
static auto _sub_494ED1 = reinterpret_cast<_sub_494ED1_t>(0x00494ED1);	//int __thiscall CClientSocket::OnConnect(CClientSocket *this, int bSuccess)

typedef void(__fastcall* _sub_494D2F_t)(CClientSocket* pThis, void* edx, sockaddr_in* pAddr);    //sub_494D2F	end	00494ECE
static auto _sub_494D2F = reinterpret_cast<_sub_494D2F_t>(0x00494D2F);	//void__thiscall CClientSocket::Connect(CClientSocket *this, sockaddr_in *pAddr)

typedef void(__fastcall* _sub_494CA3_t)(CClientSocket* pThis, void* edx, CClientSocket_CONNECTCONTEXT* ctx);   //sub_494CA3	end	00494D04
static auto _sub_494CA3 = reinterpret_cast<_sub_494CA3_t>(0x00494CA3);	//void __thiscall CClientSocket::Connect(CClientSocket *this, CClientSocket::CONNECTCONTEXT *ctx)

typedef int(__fastcall* _sub_494931_t)(void* pThis, void* edx);	//1	//sub_494931	end	00494BE8
static auto _sub_494931 = reinterpret_cast<_sub_494931_t>(0x00494931);	//void __thiscall CClientSocket::ConnectLogin(CClientSocket *this)

typedef void(__fastcall* _sub_9F7CE1_t)(CWvsApp* pThis, void* edx);	//sub_9F7CE1 end 009F821E
static auto _sub_9F7CE1 = reinterpret_cast<_sub_9F7CE1_t>(0x009F7CE1); //void __thiscall CWvsApp::InitializeInput(CWvsApp *this)

typedef void(__thiscall* _sub_9F821F_t)(void* pThis);	 //void __thiscall CInputSystem::CInputSystem(CInputSystem *this)
static auto _sub_9F821F = reinterpret_cast<_sub_9F821F_t>(0x009F821F); //CInputSystem* pThis

typedef void(__thiscall* _sub_599EBF_t)(void* pThis, HWND__* hWnd, void** ahEvent);//void __thiscall CInputSystem::Init(CInputSystem *this, HWND__ *hWnd, void **ahEvent)
static auto _sub_599EBF = reinterpret_cast<_sub_599EBF_t>(0x00599EBF); //CInputSystem* pThis //void** ahEvent

typedef void* (__cdecl* _sub_9F9A6A_t)();//CInputSystem *__cdecl TSingleton<CInputSystem>::GetInstance()
static auto _sub_9F9A6A = reinterpret_cast<_sub_9F9A6A_t>(0x009F9A6A); //CInputSystem* pThis

typedef void(__fastcall* _sub_9F84D0_t)(CWvsApp* pThis, void* edx, int tCurTime);	//sub_9F84D0 end 009F8B5E
static auto _sub_9F84D0 = reinterpret_cast<_sub_9F84D0_t>(0x009F84D0); //void __thiscall CWvsApp::CallUpdate(CWvsApp *this, int tCurTime)

auto _dword_BEDED4 = (ZRef<CStage>**)0x00BEDED4;//ZRef<CStage> g_pStage

auto _dword_BF14EC = (_com_ptr_t_com_IIID_IWzGr2D*)0x00BF14EC;//_com_ptr_t<_com_IIID<IWzGr2D,&_GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9> > g_gr
auto _unk_BD83B0 = (_GUID*)0x00BD83B0;//GUID _GUID_e576ea33_d465_4f08_aab1_e78df73ee6d9
auto _dword_BE78D4 = (void**)0x00BE78D4; //CActionMan *TSingleton<CActionMan>::ms_pInstance

typedef void(__thiscall* _sub_496B68_t)(ZRef<CStage>* pThis, ZRef<CStage>* r); //ZRef<CStage> *this
static auto _sub_496B68 = reinterpret_cast<_sub_496B68_t>(0x00496B68); //void __thiscall ZRef<CStage>::ZRef<CStage>(ZRef<CStage> *this, ZRef<CStage> *r)

typedef void(__thiscall* _sub_496B85_t)(ZRef<CStage>* pThis); //void __thiscall ZRef<CStage>::~ZRef<CStage>(ZRef<CStage> *this)
static auto _sub_496B85 = reinterpret_cast<_sub_496B85_t>(0x00496B85);

typedef void(__thiscall* _sub_5F4C16_t)(void* pThis);//void __thiscall CLogin::Update(CLogin *this)
static auto _sub_5F4C16 = reinterpret_cast<_sub_5F4C16_t>(0x005F4C16);

typedef void(__cdecl* _sub_9E47C3_t)();
static auto _sub_9E47C3 = reinterpret_cast<_sub_9E47C3_t>(0x009E47C3);//void __cdecl CWndMan::s_Update(void)

typedef void(__stdcall* _sub_A5FDE4_t)(HRESULT hr);//void __stdcall _com_issue_error(HRESULT hr)
static auto _sub_A5FDE4 = reinterpret_cast<_sub_A5FDE4_t>(0x00A5FDE4);//!!use microsoft version

typedef void(__stdcall* _sub_A5FDF2_t)(HRESULT hr, IUnknown* punk, _GUID* riid);//void __stdcall _com_issue_errorex(HRESULT hr, IUnknown* punk, _GUID* riid)
static auto _sub_A5FDF2 = reinterpret_cast<_sub_A5FDF2_t>(0x00A5FDF2);//!!use microsoft version

typedef void(__thiscall* _sub_411BBB_t)(void* pThis);//void __thiscall CActionMan::SweepCache(CActionMan* this)
static auto _sub_411BBB = reinterpret_cast<_sub_411BBB_t>(0x00411BBB);

typedef void(__fastcall* _sub_9F5239_t)(CWvsApp* pThis, void* edx);	//sub_9F5239 end 009F5C4F 
static auto _sub_9F5239 = reinterpret_cast<_sub_9F5239_t>(0x009F5239);//void __thiscall CWvsApp::SetUp(CWvsApp *this)

typedef void(__thiscall* _sub_9F7097_t)(CWvsApp* pThis);//void __thiscall CWvsApp::InitializeAuth(CWvsApp *this)
static auto _sub_9F7097 = reinterpret_cast<_sub_9F7097_t>(0x009F7097);

typedef void(__cdecl* _sub_A61C60_t)(DWORD);//void __cdecl srand(unsigned int)
static auto _sub_A61C60 = reinterpret_cast<_sub_A61C60_t>(0x00A61C60);

typedef void(__cdecl* _sub_44E824_t)();//void __cdecl GetSEPrivilege(void)
static auto _sub_44E824 = reinterpret_cast<_sub_44E824_t>(0x0044E824);

auto _dword_BEC3A8 = (void*)0x00BEC3A8; //CSecurityClient* TSingleton<CSecurityClient>::ms_pInstance
auto _dword_BF1AC8 = (int*)0x00BF1AC8; //TSingleton<CConfig>::GetInstance()->m_sysOpt.bSysOpt_WindowedMode;

typedef void* (__cdecl* _sub_9F9F42_t)();//CSecurityClient *__cdecl TSingleton<CSecurityClient>::CreateInstance()
static auto _sub_9F9F42 = reinterpret_cast<_sub_9F9F42_t>(0x009F9F42);

typedef void(__thiscall* _sub_9F6D77_t)(CWvsApp* pThis);//void __thiscall CWvsApp::InitializePCOM(CWvsApp *this)
static auto _sub_9F6D77 = reinterpret_cast<_sub_9F6D77_t>(0x009F6D77);

typedef void(__thiscall* _sub_9F6D97_t)(CWvsApp* pThis);//void __thiscall CWvsApp::CreateMainWindow(CWvsApp *this)
static auto _sub_9F6D97 = reinterpret_cast<_sub_9F6D97_t>(0x009F6D97);

typedef CClientSocket* (__cdecl* _sub_9F9E53_t)();//CClientSocket *__cdecl TSingleton<CClientSocket>::CreateInstance()
static auto _sub_9F9E53 = reinterpret_cast<_sub_9F9E53_t>(0x009F9E53);

typedef void(__thiscall* _sub_9F6F27_t)(CWvsApp* pThis);//void __thiscall CWvsApp::ConnectLogin(CWvsApp *this)
static auto _sub_9F6F27 = reinterpret_cast<_sub_9F6F27_t>(0x009F6F27);

typedef void* (__cdecl* _sub_9F9E98_t)();//CFuncKeyMappedMan *__cdecl TSingleton<CFuncKeyMappedMan>::CreateInstance()
static auto _sub_9F9E98 = reinterpret_cast<_sub_9F9E98_t>(0x009F9E98);

typedef void* (__cdecl* _sub_9FA0CB_t)();//CQuickslotKeyMappedMan *__cdecl TSingleton<CQuickslotKeyMappedMan>::CreateInstance()
static auto _sub_9FA0CB = reinterpret_cast<_sub_9FA0CB_t>(0x009FA0CB);

typedef void* (__cdecl* _sub_9F9EEE_t)();//CMacroSysMan *__cdecl TSingleton<CMacroSysMan>::CreateInstance()
static auto _sub_9F9EEE = reinterpret_cast<_sub_9F9EEE_t>(0x009F9EEE);

typedef void(__thiscall* _sub_9F7159_t)(CWvsApp* pThis);//void __thiscall CWvsApp::InitializeResMan(CWvsApp *this)
static auto _sub_9sssF7159 = reinterpret_cast<_sub_9F7159_t>(0x009F7159);

typedef void(__thiscall* _sub_9F7A3B_t)(CWvsApp* pThis);//void __thiscall CWvsApp::InitializeGr2D(CWvsApp *this)
static auto _sub_9F7A3B = reinterpret_cast<_sub_9F7A3B_t>(0x009F7A3B);

typedef void(__thiscall* _sub_9F82BC_t)(CWvsApp* pThis);//void __thiscall CWvsApp::InitializeSound(CWvsApp *this)
static auto _sub_9F82BC = reinterpret_cast<_sub_9F82BC_t>(0x009F82BC);

typedef void(__thiscall* _sub_9F8B61_t)(CWvsApp* pThis);//void __thiscall CWvsApp::InitializeGameData(CWvsApp *this)
static auto _sub_9F8B61 = reinterpret_cast<_sub_9F8B61_t>(0x009F8B61);

typedef void(__thiscall* _sub_9F7034_t)(CWvsApp* pThis);//void __thiscall CWvsApp::CreateWndManager(CWvsApp *this)
static auto _sub_9F7034 = reinterpret_cast<_sub_9F7034_t>(0x009F7034);

typedef void* (__cdecl* _sub_538C98_t)();//CConfig *__cdecl TSingleton<CConfig>::GetInstance()
static auto _sub_538C98 = reinterpret_cast<_sub_538C98_t>(0x00538C98);

typedef void(__thiscall* _sub_49EA33_t)(void* pThis, void* pSysOpt, int bApplyVideo);//void __thiscall CConfig::ApplySysOpt(CConfig *this, CONFIG_SYSOPT *pSysOpt, int bApplyVideo)
static auto _sub_49EA33 = reinterpret_cast<_sub_49EA33_t>(0x0049EA33);

typedef void* (__cdecl* _sub_9F9DA6_t)();//CActionMan *__cdecl TSingleton<CActionMan>::CreateInstance()
static auto _sub_9F9DA6 = reinterpret_cast<_sub_9F9DA6_t>(0x009F9DA6);

typedef void(__thiscall* _sub_406ABD_t)(void* pThis);//void __thiscall CActionMan::Init(CActionMan *this)
static auto _sub_406ABD = reinterpret_cast<_sub_406ABD_t>(0x00406ABD);

typedef void* (__cdecl* _sub_9F9DFC_t)();//CAnimationDisplayer *__cdecl TSingleton<CAnimationDisplayer>::CreateInstance()
static auto _sub_9F9DFC = reinterpret_cast<_sub_9F9DFC_t>(0x009F9DFC);

typedef void* (__cdecl* _sub_9F9F87_t)();//CMapleTVMan *__cdecl TSingleton<CMapleTVMan>::CreateInstance()
static auto _sub_9F9F87 = reinterpret_cast<_sub_9F9F87_t>(0x009F9F87);

typedef void(__thiscall* _sub_636F4E_t)(void* pThis);//void __thiscall CMapleTVMan::Init(CMapleTVMan *this)
static auto _sub_636F4E = reinterpret_cast<_sub_636F4E_t>(0x00636F4E);

typedef void* (__cdecl* _sub_9F9AC2_t)();//CQuestMan *__cdecl TSingleton<CQuestMan>::CreateInstance()
static auto _sub_9F9AC2 = reinterpret_cast<_sub_9F9AC2_t>(0x009F9AC2);

typedef int(__thiscall* _sub_71D8DF_t)(void* pThis);//int __thiscall CQuestMan::LoadDemand(CQuestMan *this)
static auto _sub_71D8DF = reinterpret_cast<_sub_71D8DF_t>(0x0071D8DF);

typedef void(__thiscall* _sub_723341_t)(void* pThis);//void __thiscall CQuestMan::LoadPartyQuestInfo(CQuestMan *this)
static auto _sub_723341 = reinterpret_cast<_sub_723341_t>(0x00723341);

typedef void(__thiscall* _sub_7247A1_t)(void* pThis);//void __thiscall CQuestMan::LoadExclusive(CQuestMan *this)
static auto _sub_7247A1 = reinterpret_cast<_sub_7247A1_t>(0x007247A1);

typedef void* (__cdecl* _sub_9F9B73_t)();//CMonsterBookMan *__cdecl TSingleton<CMonsterBookMan>::CreateInstance()
static auto _sub_9F9B73 = reinterpret_cast<_sub_9F9B73_t>(0x009F9B73);

typedef int(__thiscall* _sub_68487C_t)(void* pThis);//int __thiscall CMonsterBookMan::LoadBook(CMonsterBookMan *this)
static auto _sub_68487C = reinterpret_cast<_sub_68487C_t>(0x0068487C);

typedef void* (__cdecl* _sub_9FA078_t)();//CRadioManager *__cdecl TSingleton<CRadioManager>::CreateInstance()
static auto _sub_9FA078 = reinterpret_cast<_sub_9FA078_t>(0x009FA078);

typedef void(__thiscall* _sub_414617_t)(ZXString<char>* pThis, const char* s, int n);//void __thiscall ZXString<char>::Assign(ZXString<char> *this, const char *s, int n)
static auto _sub_414617 = reinterpret_cast<_sub_414617_t>(0x00414617);

typedef void(__thiscall* _sub_49CCF3_t)(void* pThis, ZXString<char> sModulePath);//void __thiscall CConfig::CheckExecPathReg(CConfig *this, ZXString<char> sModulePath)
static auto _sub_49CCF3 = reinterpret_cast<_sub_49CCF3_t>(0x0049CCF3);

typedef void(__thiscall* _sub_62ECE2_t)(void* pThis);//void __thiscall CLogo::CLogo(CLogo *this)
static auto _sub_62ECE2 = reinterpret_cast<_sub_62ECE2_t>(0x0062ECE2);

typedef void(__cdecl* _sub_777347_t)(CStage* pStage, void* pParam);//void __cdecl set_stage(CStage *pStage, void *pParam)
static auto _sub_777347 = reinterpret_cast<_sub_777347_t>(0x00777347);

typedef void(__fastcall* _sub_9F5C50_t)(CWvsApp* pThis, void* edx, int* pbTerminate);	//sub_9F5C50  end 009F698D  
static auto _sub_9F5C50 = reinterpret_cast<_sub_9F5C50_t>(0x009F5C50);//void __thiscall CWvsApp::Run(CWvsApp *this, int *pbTerminate)

auto _dword_BE7914 = (void**)0x00BE7914;//CClientSocket *TSingleton<CClientSocket>::ms_pInstance
auto _dword_BEC33C = (void**)0x00BEC33C;//TSingleton_CInputSystem___ms_pInstance dd ? v95 C68C20

typedef void(__thiscall* _sub_49651D_t)(void* pThis);//void __thiscall CClientSocket::ManipulatePacket(CClientSocket *this)
static auto _sub_49651D = reinterpret_cast<_sub_49651D_t>(0x0049651D);

typedef void(__thiscall* _sub_59A2E9_t)(void* pThis, DWORD nDeviceIndex);//void __thiscall CInputSystem::UpdateDevice(CInputSystem *this, int nDeviceIndex)
static auto _sub_59A2E9 = reinterpret_cast<_sub_59A2E9_t>(0x0059A2E9);

typedef int(__thiscall* _sub_59A306_t)(void* pThis, void* pISMsg);//int __thiscall CInputSystem::GetISMessage(CInputSystem *this, ISMSG *pISMsg)
static auto _sub_59A306 = reinterpret_cast<_sub_59A306_t>(0x0059A306);

typedef void(__thiscall* _sub_9F97B7_t)(CWvsApp* pThis, unsigned int message, unsigned int wParam, int lParam);//void __thiscall CWvsApp::ISMsgProc(CWvsApp *this, unsigned int message, unsigned int wParam, int lParam)
static auto _sub_9F97B7 = reinterpret_cast<_sub_9F97B7_t>(0x009F97B7);

typedef void(__stdcall* _sub_A605C3_t)(HRESULT hr, void* perrinfo);//void __stdcall _com_raise_error(HRESULT hr, IErrorInfo *perrinfo)
static auto _sub_A605C3 = reinterpret_cast<_sub_A605C3_t>(0x00A605C3);//!!use microsoft version

typedef int(__thiscall* _sub_59B2D2_t)(void* pThis, void* pISMsg);//int __thiscall CInputSystem::GenerateAutoKeyDown(CInputSystem *this, ISMSG *pISMsg)
static auto _sub_59B2D2 = reinterpret_cast<_sub_59B2D2_t>(0x0059B2D2);

typedef int(__thiscall* _sub_9F6990_t)(IWzGr2D* pThis);//int __thiscall IWzGr2D::GetnextRenderTime(IWzGr2D *this)
static auto _sub_9F6990 = reinterpret_cast<_sub_9F6990_t>(0x009F6990);

typedef void(__cdecl* _sub_9E4547_t)();//void __cdecl CWndMan::RedrawInvalidatedWindows(void)
static auto _sub_9E4547 = reinterpret_cast<_sub_9E4547_t>(0x009E4547);

typedef HRESULT(__thiscall* _sub_777326_t)(IWzGr2D* pThis);//HRESULT __thiscall IWzGr2D::RenderFrame(IWzGr2D *this)
static auto _sub_777326 = reinterpret_cast<_sub_777326_t>(0x00777326);

typedef void(__fastcall* _sub_9F4FDA_t)(CWvsApp* pThis, void* edx, const char* sCmdLine);	//sub_9F4FDA end 009F51D3
static auto _sub_9F4FDA = reinterpret_cast<_sub_9F4FDA_t>(0x009F4FDA);//void __thiscall CWvsApp::CWvsApp(CWvsApp *this, const char *sCmdLine)

auto _off_B3F3E8 = (void*)0x00B3F3E8;//const CWvsApp::`vftable'
auto _dword_BE2EBC = (unsigned int*)0x00BE2EBC;//unsigned int g_dwTargetOS
auto _string_B3F3D8 = (char**)0x00B3F3D8;//char aWebstart[]
auto _dword_BF6B44 = (ZXString<char>*)0x00BF6B44;//ZXString<char> ZXString<char>::_s_sEmpty
auto _dword_BF02B4 = (unsigned int(__stdcall*)())0x00BF02B4;//!!use microsoft version//unsigned int (__stdcall *GetCurrentThreadId)();
auto _dword_BF03E4 = (int(__stdcall*)(_OSVERSIONINFOA*))0x00BF03E4;//!!use microsoft version//int (__stdcall *GetVersionExA)(_OSVERSIONINFOA *);

typedef ZXString<char>* (__thiscall* _sub_4744C9_t)(ZXString<char>* pThis, const char* sWhiteSpaceSet);//ZXString<char> *__thiscall ZXString<char>::TrimLeft(ZXString<char> *this, const char *sWhiteSpaceSet)
static auto _sub_4744C9 = reinterpret_cast<_sub_4744C9_t>(0x004744C9);

typedef ZXString<char>* (__thiscall* _sub_474414_t)(ZXString<char>* pThis, const char* sWhiteSpaceSet);//ZXString<char> *__thiscall ZXString<char>::TrimRight(ZXString<char> *this, const char *sWhiteSpaceSet)
static auto _sub_474414 = reinterpret_cast<_sub_474414_t>(0x00474414);

typedef HANDLE(WINAPI* FindFirstFileA_t)(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData);
extern FindFirstFileA_t FindFirstFileA_Original; //HANDLE FindFirstFileA([in] LPCSTR lpFileName,[out] LPWIN32_FIND_DATAA lpFindFileData);

typedef DWORD(WINAPI* GetLastError_t)(); extern GetLastError_t GetLastError_Original;

typedef void(__fastcall* _sub_9F51F6_t)(CWvsApp* pThis, void* edx);
static auto _sub_9F51F6 = reinterpret_cast<_sub_9F51F6_t>(0x009F51F6);//void __thiscall CWvsApp::~CWvsApp(CWvsApp *this)

typedef unsigned int(__fastcall* _sub_9F4E54_t)(unsigned int* pmem, void* edx, unsigned int size, unsigned int* pcheck, unsigned int* pCrc32);
static auto _sub_9F4E54 = reinterpret_cast<_sub_9F4E54_t>(0x009F4E54);//unsigned int __cdecl Crc32_GetCrc32_VMTable(unsigned int pmem, unsigned int size, unsigned intpcheck, unsigned int *pCrc32) 

#pragma endregion