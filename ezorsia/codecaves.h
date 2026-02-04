#pragma once
#include "Memory.h"
#include "signal.h"

#pragma region Ezorsia Prepackaged

const char* resmanLoadOrder[] = {  //suspecting resman load list is an array of const char* rather than a vector
"Character", //"Character"
"Mob", //"Mob"
"Skill", //"Skill"
"Reactor", //"Reactor"
"Npc", //"Npc"
"UI", //"UI"
"Quest", //"Quest"
"Item", //"Item"
"Effect", //"Effect"
"String", //"String"
"Etc", //"Etc"
"Morph", //"Morph"
"TamingMob", //"TamingMob"
"Sound", //"Sound"
"Map",  //Map"
"EzorsiaV2_UI"	//sneakily insert our wz file here and walk away
};

unsigned char resmanLoadAMNT = sizeof(resmanLoadOrder) / sizeof(resmanLoadOrder[0]) - 1;
__declspec(naked) void LoadUItwice() {
	__asm {
		mov     ebx, [eax * 4 + resmanLoadOrder]//mov     ebx, MainMain::resmanLoadOrder //mov     ebx, [ebp+eax*4+
		jmp dword ptr[dwLoadUItwiceRetn]
	}
}

int nStatusBarY = 0;
__declspec(naked) void AdjustStatusBar() {
	__asm {
		push nStatusBarY
		push ebx // horizontal position; 0
		mov ecx, esi
		jmp dword ptr[dwStatusBarPosRetn]
	}
}

__declspec(naked) void AdjustStatusBarBG() {
	__asm {
		push nStatusBarY
		movsd
		push 0
		jmp dword ptr[dwStatusBarBackgroundPosRetn]
	}
}

__declspec(naked) void AdjustStatusBarInput() {
	__asm {
		push nStatusBarY
		push edi
		lea ecx, [esi + 0x0CD0]
		jmp dword ptr[dwStatusBarInputPosRetn]
	}
}

__declspec(naked) void PositionLoginDlg() {
	__asm {
		push 0x000000B4
		push 400
		push - 48	// y
		push - 185	// x
		jmp dword ptr[dwLoginCreateDlgRtn]
	}
}
__declspec(naked) void PositionLoginUsername() {
	__asm {
		push 0x0F
		push 0x00000084
		push 127	// y
		push 0		// x
		jmp dword ptr[dwLoginUsernameRtn]
	}
}
__declspec(naked) void PositionLoginPassword() {
	__asm {
		push 0x0F
		push 0x78
		push 127	// y
		push 272	// x
		jmp dword ptr[dwLoginPasswordRtn]
	}
}
__declspec(naked) void PositionBossBarY() {
	__asm {	//finally working!, originally posted by Angxl
		//push 22	//modification
		push edi	//part of original memory
		push dword ptr ss : [ebp - 68]	//part of original memory
		lea eax, dword ptr ss : [ebp - 32]	//part of original memory
		jmp dword ptr[dwBossBarRetn]
	}
}
__declspec(naked) void PositionBossBarY1() {
	__asm {	//finally working!, originally posted by Angxl
		push 22	//modification
		//push edi	//part of original memory
		push dword ptr ss : [ebp - 68]	//part of original memory
		lea eax, dword ptr ss : [ebp - 32]	//part of original memory
		jmp dword ptr[dwBossBarRetn]
	}
}

int serverMessageExists;
__declspec(naked) void PositionBossBarY2() {
	__asm {
		mov esi, eax
		and dword ptr ss : [ebp - 4] , 0
		mov serverMessageExists, esi
		jmp dword ptr[dwBossBarRetn2]
	}
}

int myHeight = -(Client::m_nGameHeight - 600) / 2;
int myWidth = -(Client::m_nGameWidth - 800) / 2;

__declspec(naked) void CashShopFix() {
	__asm {
		push    eax //vCanvas //originally posted by shavitash		//fixed
		push    ebx //nZ
		push    ebx //uHeight
		push    ebx //uWidth
		push	myHeight//84//myHeight //nTop - do the math yourself, this wont compile obviously
		push	myWidth//283//myWidth //nLeft - same as above
		jmp dword ptr[dwCashFixRtm]
	}
}

int nHeightOfsetted1 = 0; int nWidthOfsetted1 = 0; int nTopOfsetted1 = 0; int nLeftOfsetted1 = 0;
int nHeightOfsetted2 = 0; int nWidthOfsetted2 = 0; int nTopOfsetted2 = 0; int nLeftOfsetted2 = 0;
int nHeightOfsetted3 = 0; int nWidthOfsetted3 = 0; int nTopOfsetted3 = 0; int nLeftOfsetted3 = 0;
int nHeightOfsetted4 = 0; int nWidthOfsetted4 = 0; int nTopOfsetted4 = 0; int nLeftOfsetted4 = 0;
int nHeightOfsetted5 = 0; int nWidthOfsetted5 = 0; int nTopOfsetted5 = 0; int nLeftOfsetted5 = 0;
int nHeightOfsetted6 = 0; int nWidthOfsetted6 = 0; int nTopOfsetted6 = 0;
int nHeightOfsetted7 = 0; int nWidthOfsetted7 = 0; int nTopOfsetted7 = 0; int nLeftOfsetted7 = 0;
int nHeightOfsetted8 = 0; int nWidthOfsetted8 = 0; int nTopOfsetted8 = 0; int nLeftOfsetted8 = 0;

__declspec(naked) void CashShopFix1() {
	__asm {
		push	nHeightOfsetted1
		push	nWidthOfsetted1
		push	nTopOfsetted1
		push	nLeftOfsetted1
		jmp dword ptr[dwCashFix1Rtm]
	}
}

__declspec(naked) void CashShopFix2() {
	__asm {
		push	nHeightOfsetted2
		push	nWidthOfsetted2
		push	nTopOfsetted2
		push	nLeftOfsetted2
		jmp dword ptr[dwCashFix2Rtm]
	}
}

__declspec(naked) void CashShopFix3() {
	__asm {
		push	nHeightOfsetted3
		push	nWidthOfsetted3
		push	nTopOfsetted3
		push	nLeftOfsetted3
		jmp dword ptr[dwCashFix3Rtm]
	}
}

__declspec(naked) void CashShopFix4() {
	__asm {
		push	nHeightOfsetted4
		push	nWidthOfsetted4
		push	nTopOfsetted4
		push	nLeftOfsetted4
		jmp dword ptr[dwCashFix4Rtm]
	}
}

__declspec(naked) void CashShopFix5() {
	__asm {
		push	nHeightOfsetted5
		push	nWidthOfsetted5
		push	nTopOfsetted5
		push	nLeftOfsetted5
		jmp dword ptr[dwCashFix5Rtm]
	}
}

__declspec(naked) void CashShopFix6() {
	__asm {
		push	nHeightOfsetted6
		push	nWidthOfsetted6
		push	nTopOfsetted6
		jmp dword ptr[dwCashFix6Rtm]
	}
}

__declspec(naked) void CashShopFix7() {
	__asm {
		push	nHeightOfsetted7
		push	nWidthOfsetted7
		push	nTopOfsetted7
		push	nLeftOfsetted7
		jmp dword ptr[dwCashFix7Rtm]
	}
}

__declspec(naked) void CashShopFix8() {
	__asm {
		push	nHeightOfsetted8
		push	nWidthOfsetted8
		push	nTopOfsetted8
		push	nLeftOfsetted8
		jmp dword ptr[dwCashFix8Rtm]
	}
}

__declspec(naked) void CashShopFixOnOff() {	//could be improved upon because idk if it's the right way to do it or if it might cause issues
	__asm {
		pop	ebx
		push ecx
		mov ecx, dword ptr[dwCashFixOnOffCall]
		call ecx
		add esp, 4
		leave
		retn    4
	}
}

int nHeightOfsettedPrev = 0; int nWidthOfsettedPrev = 0; int nTopOfsettedPrev = 0; int nLeftOfsettedPrev = 0;

__declspec(naked) void CashShopFixPrev() {
	__asm {
		push	nHeightOfsettedPrev
		push	nWidthOfsettedPrev
		push	nTopOfsettedPrev
		push	nLeftOfsettedPrev
		jmp dword ptr[dwCashFixPrevRtm]
	}
}

int nTopOfsettedVerFix = 0; int nLeftOfsettedVerFix = 0;

__declspec(naked) void VersionNumberFix() {
	__asm {
		mov    eax, nLeftOfsettedVerFix
		sub    eax, DWORD PTR[ebp - 0x1c]
		push	nTopOfsettedVerFix
		jmp dword ptr[dwVersionNumberFixRtm]
	}
}

int myAlwaysViewRestoreFixOffset = 0;

__declspec(naked) void AlwaysViewRestoreFix() {
	__asm {
		test	eax, eax
		jnz C_Dest
		mov ecx, myAlwaysViewRestoreFixOffset
		push myAlwaysViewRestoreFixOffset
		jmp dword ptr[dwAlwaysViewRestorerFixRtm]
		C_Dest:
		mov ecx, DWORD PTR[eax]
			push eax
			jmp dword ptr[dwAlwaysViewRestorerFixRtm]
	}
}

int nHeightOfsettedLoginBackCanvasFix = 0; int nWidthOfsettedLoginBackCanvasFix = 0;
int nTopOfsettedLoginBackCanvasFix = 0; int nLeftOfsettedLoginBackCanvasFix = 0;

__declspec(naked) void ccLoginBackCanvasFix() {
	__asm {
		push	nHeightOfsettedLoginBackCanvasFix
		push	nWidthOfsettedLoginBackCanvasFix
		push	nTopOfsettedLoginBackCanvasFix
		push	nLeftOfsettedLoginBackCanvasFix
		jmp dword ptr[dwLoginBackCanvasFixRetn]
	}
}

int nHeightOfsettedLoginViewRecFix = 0; int nWidthOfsettedLoginViewRecFix = 0;
int nTopOfsettedLoginViewRecFix = 0; int nLeftOfsettedLoginViewRecFix = 0;

__declspec(naked) void ccLoginViewRecFix() {
	__asm {
		push	nHeightOfsettedLoginViewRecFix
		push	nWidthOfsettedLoginViewRecFix
		push	nTopOfsettedLoginViewRecFix
		push	nLeftOfsettedLoginViewRecFix
		jmp dword ptr[dwLoginViewRecFixRetn]
	}
}

int yOffsetOfLoginBackBtnFix = 0; int xOffsetOfLoginBackBtnFix = 0;

__declspec(naked) void ccLoginBackBtnFix() {	//un used
	__asm {
		mov    esi, yOffsetOfLoginBackBtnFix
		push	esi
		mov    edi, xOffsetOfLoginBackBtnFix
		push	edi
		push   0x3e8
		jmp dword ptr[dwLoginBackBtnFixRetn]
	}
}

int a1x = 0; int a2x = 0; int a2y = 0; int a3 = 0; int a1y = 0;

__declspec(naked) void ccLoginDescriptorFix() {
	__asm {
		and edx, 0x3f
		add    edx, 0x21
		add    edx, a2y
		cmp     ecx, edi
		setl   bl
		mov     ecx, esi
		mov    DWORD PTR[esi + 0x4], 0xaf7084
		mov    DWORD PTR[esi + 0x8], 0xaf7080
		neg     ebx
		sbb     ebx, ebx
		and ebx, a3	//and ebx, 0x64
		add     ebx, eax
		push    ebx
		push    edx
		xor eax, eax
		add eax, a1x //a1x
		push    eax
		push    edx
		push    eax
		push    edx
		mov    eax, a2x	//mov    eax, 0xffffff6b
		push    eax
		push	edi
		jmp dword ptr[dwLoginDescriptorFixRetn]
	}
}

int MoreGainMsgsOffset = 6;

__declspec(naked) void ccMoreGainMsgs() {
	__asm {
		mov    eax, DWORD PTR[edi + 0x10]
		cmp    eax, MoreGainMsgsOffset
		jmp dword ptr[dwMoreGainMsgsRetn]
	}
}

int MoreGainMsgsFadeOffset = 0;

__declspec(naked) void ccMoreGainMsgsFade() {
	__asm {
		add eax, MoreGainMsgsFadeOffset
		push 3
		jmp dword ptr[dwMoreGainMsgsFadeRetn]
	}
}

int MoreGainMsgsFade1Offset = 0;

__declspec(naked) void ccMoreGainMsgsFade1() {
	__asm {
		push MoreGainMsgsFade1Offset
		jmp dword ptr[dwMoreGainMsgsFade1Retn]
	}
}

int yOffsetOfMuruengraidPlayer = 50; int xOffsetOfMuruengraidPlayer = 169;

__declspec(naked) void ccMuruengraidPlayer() {
	__asm {
		push yOffsetOfMuruengraidPlayer
		push xOffsetOfMuruengraidPlayer
		push ecx
		jmp dword ptr[dwMuruengraidPlayerRetn]
	}
}

int yOffsetOfMuruengraidClock = 26; int xOffsetOfMuruengraidClock = 400;

__declspec(naked) void ccMuruengraidClock() {
	__asm {
		push yOffsetOfMuruengraidClock
		push xOffsetOfMuruengraidClock
		push ecx
		jmp dword ptr[dwMuruengraidClockRetn]
	}
}

int yOffsetOfMuruengraidMonster = 50; int xOffsetOfMuruengraidMonster = 631;

__declspec(naked) void ccMuruengraidMonster() {
	__asm {
		push yOffsetOfMuruengraidMonster
		push xOffsetOfMuruengraidMonster
		push ecx
		jmp dword ptr[dwMuruengraidMonsterRetn]
	}
}

int yOffsetOfMuruengraidMonster1 = 32; int xOffsetOfMuruengraidMonster1 = 317;

__declspec(naked) void ccMuruengraidMonster1() {
	__asm {
		push yOffsetOfMuruengraidMonster1
		push xOffsetOfMuruengraidMonster1
		jmp dword ptr[dwMuruengraidMonster1Retn]
	}
}

int yOffsetOfMuruengraidMonster2 = 32; int xOffsetOfMuruengraidMonster2 = 482;

__declspec(naked) void ccMuruengraidMonster2() {
	__asm {
		push yOffsetOfMuruengraidMonster2
		push xOffsetOfMuruengraidMonster2
		jmp dword ptr[dwMuruengraidMonster2Retn]
	}
}

int yOffsetOfMuruengraidEngBar = 86; int xOffsetOfMuruengraidEngBar = 17;

__declspec(naked) void ccMuruengraidEngBar() {
	__asm {
		push yOffsetOfMuruengraidEngBar
		push xOffsetOfMuruengraidEngBar
		push ecx
		jmp dword ptr[dwMuruengraidEngBarRetn]
	}
}

int yOffsetOfMuruengraidEngBar1 = 130; int xOffsetOfMuruengraidEngBar1 = 20;

__declspec(naked) void ccMuruengraidEngBar1() {
	__asm {
		push yOffsetOfMuruengraidEngBar1
		push xOffsetOfMuruengraidEngBar1
		push ecx
		jmp dword ptr[dwMuruengraidEngBar1Retn]
	}
}

int yOffsetOfMuruengraidEngBar2 = 80; int xOffsetOfMuruengraidEngBar2 = 9;

__declspec(naked) void ccMuruengraidEngBar2() {
	__asm {
		push yOffsetOfMuruengraidEngBar2
		push xOffsetOfMuruengraidEngBar2
		push ecx
		jmp dword ptr[dwMuruengraidEngBar2Retn]
	}
}

int yOffsetOfMuruengraidClearRoundUI = 260; int xOffsetOfMuruengraidClearRoundUI = 400;

__declspec(naked) void ccMuruengraidClearRoundUI() {
	__asm {
		mov ecx, esi
		push yOffsetOfMuruengraidClearRoundUI
		push xOffsetOfMuruengraidClearRoundUI
		jmp dword ptr[dwMuruengraidClearRoundUIRetn]
	}
}

int yOffsetOfMuruengraidTimerCanvas = 28; int xOffsetOfMuruengraidTimerCanvas = 112;

__declspec(naked) void ccMuruengraidTimerCanvas() {
	__asm {
		push yOffsetOfMuruengraidTimerCanvas
		movsd
		push xOffsetOfMuruengraidTimerCanvas
		jmp dword ptr[dwMuruengraidTimerCanvasRetn]
	}
}

int yOffsetOfMuruengraidTimerMinutes = 0; int xOffsetOfMuruengraidTimerMinutes = 0;

__declspec(naked) void ccMuruengraidTimerMinutes() {
	__asm {
		mov    DWORD PTR[esi + 0x848], edi
		push eax
		push yOffsetOfMuruengraidTimerMinutes
		push xOffsetOfMuruengraidTimerMinutes
		jmp dword ptr[dwMuruengraidTimerMinutesRetn]
	}
}

int yOffsetOfMuruengraidTimerSeconds = 0; int xOffsetOfMuruengraidTimerSeconds = 68;

__declspec(naked) void ccMuruengraidTimerSeconds() {
	__asm {
		mov ecx, esi
		push edx
		push yOffsetOfMuruengraidTimerSeconds
		push xOffsetOfMuruengraidTimerSeconds
		jmp dword ptr[dwMuruengraidTimerSecondsRetn]
	}
}

int yOffsetOfMuruengraidTimerBar = 16; int xOffsetOfMuruengraidTimerBar = 345;

__declspec(naked) void ccMuruengraidTimerBar() {
	__asm {
		push yOffsetOfMuruengraidTimerBar
		push xOffsetOfMuruengraidTimerBar
		jmp dword ptr[dwMuruengraidTimerBarRetn]
	}
}

int xOffsetOfMuruengraidMonster1_2 = 318;

__declspec(naked) void ccMuruengraidMonster1_2() {
	__asm {
		mov    edx, xOffsetOfMuruengraidMonster1_2
		jmp dword ptr[dwMuruengraidMonster1_2Retn]
	}
}

__declspec(naked) void ccStatsSubMov() {
	__asm {
		mov     ecx, esi
		add   DWORD PTR[ebp + 0x8], 84 //260-176
		push   DWORD PTR[ebp + 0x8]
		jmp dword ptr[dwStatsSubMovRetn]
	}
}
//beginning of packed client run caves
__declspec(naked) void ccCLoginSendCheckPasswordPacket() {
	__asm {
		jmp dword ptr[dwCLoginSendCheckPasswordPacketRetn]
	}
}

__declspec(naked) void cc0x00A63FF3() {
	__asm {
		push   ebp
		mov    ebp, esp
		push   0xffffffff
		push   0xb412e0
		push   0xa62478
		mov    eax, fs:0x0
		push   eax
		mov    DWORD PTR fs : 0x0, esp
		sub    esp, 0x58
		push   ebx
		push   esi
		push   edi
		mov    DWORD PTR[ebp - 0x18], esp
		call   DWORD PTR ds : 0xaf0074
		xor edx, edx
		jmp dword ptr[dw0x00A63FF3Retn]
	}
}
//end of packed client run caves

//char CUIStatusBarChatLogAddBypassLimit = 0xFF;
__declspec(naked) void ccCUIStatusBarChatLogAddBypass() {
	__asm {
		cmp    DWORD PTR[eax - 0x4], 0xFF
		jbe CUIStatusBarChatLogAddBypass_Dest
		jmp dword ptr[dwCUIStatusBarChatLogAddBypassRetn]
		CUIStatusBarChatLogAddBypass_Dest:
		jmp dword ptr[dwCUIStatusBarChatLogAddBypass2Retn]
	}
}

const char myWzFile[] = "TamingMob";
const char* ptrmyWzFile = myWzFile;

#pragma endregion

#pragma region AdventureMS Gameplay Changes

#pragma region Custom Skill Work

// 커스텀 스킬 주소
int doActiveJmpBack = 0x0096793B; // 점프 복귀 주소
int aBuff = 0x00969284; // 버프 스킬용
int aMelee = 0x009690AE; // 근접 스킬용 (LTRB 처리)
int aMagic = 0x0096928B; // 마법 스킬용
int aShoot = 0x009690E9; // 원거리 스킬용
int aPrepare = 0x00969229; // 준비 스킬용 (미테스트)
int aTeleport = 0x00969146; // 텔레포트용 (미테스트)
int aFlying = 0x009683ED; // 비행 스킬용 (미테스트)
int aRecovery = 0x00969217; // 회복 스킬용 (미테스트)
int aBoundJump = 0x0096897A; // 바운드 점프용 (미테스트)
int aSummon = 0x009689DF; // 소환 스킬용 (미테스트)
int aCombatStep = 0x00969026; // 전투 스텝용 (미테스트)

// 커스텀 스킬 목록
void _declspec(naked)doActiveSkills() 
{
	_asm 
	{
		// 하단에 커스텀 스킬 추가
		// mov eax, 스킬 코드
		// cmp esi, eax
		// je[스킬 타입]
		
		mov eax, 1101008
		cmp esi, eax
		je[melee]

		mov eax, 4201006
		cmp esi, eax
		je[melee]

		mov eax, 1201008
		cmp esi, eax
		je[melee]

		mov eax, 1301008
		cmp esi, eax
		je[melee]

		mov eax, 5001008
		cmp esi, eax
		je[melee]

		mov eax, 5101008
		cmp esi, eax
		je[melee]

		mov eax, 4101006
		cmp esi, eax
		je[shoot]

		mov eax, 1301009
		cmp esi, eax
		je[buff]

		mov eax, 1201009
		cmp esi, eax
		je[buff]

		mov eax, 3101006
		cmp esi, eax
		je[buff]

		mov eax, 3201006
		cmp esi, eax
		je[buff]

		mov eax, 5101009
		cmp esi, eax
		je[buff]

		mov eax, 5201009
		cmp esi, eax
		je[buff]

		// Jumps back to Original Location
		mov eax, 2301005
		jmp[doActiveJmpBack]

		// Jump Locations / Assignment
		buff : jmp[aBuff]
		melee : jmp[aMelee]
		magic : jmp[aMagic]
		shoot : jmp[aShoot]
		prep : jmp[aPrepare]
		tele : jmp[aTeleport]
		fly : jmp[aFlying]
		recover : jmp[aRecovery]
		jump : jmp[aBoundJump]
		summon : jmp[aSummon]
		combat : jmp[aCombatStep]
	}
}

// 커스텀 스킬 ID 확인
// 여기에 커스텀 스킬 ID 추가
// 배열에 있는 스킬 ID와 일치하면 true 반환
bool isSkillIDMatched(int nSkillID)
{
	const int skillIDs[] = 
	{
		1101008, 1201008, 1301008, 1301009, 1201009, 3201006, 3101006, 4101006, 4201006, 5101009, 5201009, 5001008, 5101008
	};

	return std::find(std::begin(skillIDs), std::end(skillIDs), nSkillID) != std::end(skillIDs);
}

// 스킬 ID 매칭 및 CodeCave 실행
// 스킬 ID가 매칭되면 CodeCave 실행
// 매칭 안 되면 원래 코드로 복원
auto pDoActiveSkill = (int(__thiscall*)(int, int, int, int))0x00966F7A;
int(__fastcall CUserLocal__DoActiveSkill_t)(int _This, void* edx, int nSkillID, unsigned int nScanCode, int pnConsumeCheck)
{
	if (isSkillIDMatched(nSkillID))
	{
		Memory::CodeCave((void*)doActiveSkills, 0x0096792A, 0);
	}
	else
	{
		Memory::WriteByte(0x0096792A, 0x0F);
		Memory::WriteByte(0x0096792A + 1, 0x8F);
		Memory::WriteByte(0x0096792A + 2, 0x71);
		Memory::WriteByte(0x0096792A + 3, 0x09);
		Memory::WriteByte(0x0096792A + 4, 0x00);
	}
	return pDoActiveSkill(_This, nSkillID, nScanCode, pnConsumeCheck);
}

#pragma endregion

// 기존 스킬 수정
#pragma region Existing Skill Modifications

// LTRB Handling for Non-bullet Skills
typedef int(__cdecl* is_attack_area_set_by_data_t)(int nSkillID);
is_attack_area_set_by_data_t _is_attack_area_set_by_data;
int __cdecl is_attack_area_set_by_data(int nSkillID) // 비투사체 스킬의 LTRB(공격 범위) 처리
{
	// 비투사체 스킬 목록
	if (nSkillID == 4001002 || nSkillID == 3201003 || nSkillID == 4101006 || nSkillID == 3201005)
	{
		return 1;
	}
	return _is_attack_area_set_by_data(nSkillID);
}

// LTRB Handling for Custom Shoot Skills
typedef int(__cdecl* is_rect_attack_shoot_skill_t)(int nSkillID);
is_rect_attack_shoot_skill_t _is_rect_attack_shoot_skill;
int __cdecl is_rect_attack_shoot_skill(int nSkillID) // 커스텀 원거리 스킬의 LTRB 처리
{
	// List Skills Here for LTRB-Shoot Modification
	if (nSkillID == 4101006 || nSkillID == 3201005)
	{
		return 1;
	}
	return _is_rect_attack_shoot_skill(nSkillID);
}

// 투사체 스킬의 공격 범위를 LTRB로 처리
DWORD dwFireArrow = 0x00955DA8;
DWORD dwFireArrowRet = 0x00955DAD;
DWORD dwFireSucc = 0x00956372;
__declspec(naked) void FireArrow() {
	__asm {

		cmp eax, 2101005 // Poison Breath
		je success
		cmp eax, 2301005 // Holy Ray
		je success
		cmp eax, 4101006 // Shuriken MAX
		je success
		cmp eax, 3201005 // Iron Arrow
		je success
		cmp eax, 0x0021E3CB
		jmp dwFireArrowRet
		success :
		jmp dwFireSucc
	}
}

// LTRB Handling for Bullet Skills
DWORD dwFireBulletAdd = 0x00956445;
DWORD dwFireBulletSucc = 0x0095645B;
DWORD dwFireBulletRet = 0x0095644E;
__declspec(naked) void FireArrowBullet() {
	__asm {
		cmp dword ptr[ebp - 0x14], 2101005 // Poison Breath
		je success
		cmp dword ptr[ebp - 0x14], 2301005 // Holy Ray
		je success
		cmp dword ptr[ebp - 0x14], 4101006 // Shuriken MAX
		je success
		cmp dword ptr[ebp - 0x14], 3201005 // Iron Arrow
		je success
		jmp dwFireBulletRet
		success :
		jmp dwFireBulletSucc
	}
}

// 리코일 샷 쿨타임 감소
DWORD dwRecoilShot = 0x00953646;
DWORD dwRecoilShotRet = 0x0095364D;
DWORD dwRecoilShotSucc = 0x00953669;
__declspec(naked) void RecoilShotLowerCD() {
	__asm {
		cmp eax, 5201006
		je success
		jmp dwRecoilShotRet
		success :
		mov eax, 100 // 원래 쿨타임: 2000ms
			jmp dwRecoilShotSucc
	}
}


// 빅뱅 차징 제거
DWORD dwBB = 0x00967ECC;
DWORD dwBBReturn = 0x00967ED2;
DWORD dwBBS = 0x0096928B;
__declspec(naked) void BigBang() { // 비숍용 빅뱅 차징 제거
	__asm
	{
		je BBS
		jmp dword ptr[dwBBReturn]
		BBS:
		jmp dword ptr[dwBBS]
	}
}


// Remove Charge: Big Bang IL
DWORD dwBB2 = 0x00967DEE;
DWORD dwBB2Return = 0x00967DF4;
DWORD dwBBS2 = 0x0096928B;
__declspec(naked) void BigBang2() { // 아크메이지(썬콜)용 빅뱅 차징 제거
	__asm
	{
		je BBS2
		jmp dword ptr[dwBB2Return]
		BBS2:
		jmp dword ptr[dwBBS2]
	}
}

// Remove Charge: Big Bang FP
DWORD dwBB3 = 0x00967D10;
DWORD dwBB3Return = 0x00967D16;
DWORD dwBBS3 = 0x0096928B;
__declspec(naked) void BigBang3() { // 아크메이지(불독)용 빅뱅 차징 제거
	__asm
	{
		je BBS3
		jmp dword ptr[dwBB3Return]
		BBS3:
		jmp dword ptr[dwBBS3]
	}
}

// 피어싱 애로우 차징 제거
DWORD dwPA = 0x00968048;
DWORD dwPAReturn = 0x00967B8B;
DWORD dwPAS = 0x009690E9;
__declspec(naked) void PA() {
	__asm {
		je PAS
		jmp dword ptr[dwPAReturn]
		PAS:
		jmp dword ptr[dwPAS]
	}
}

// 콕스크류 블로우 차징 제거
DWORD dwCB = 0x00968278;
DWORD dwCBReturn = 0x00967B8B;
DWORD dwCBS = 0x009690AE;
__declspec(naked) void CorkscrewBlow() {
	__asm {
		je CBS
		jmp dword ptr[dwCBReturn]
		CBS:
		jmp dword ptr[dwCBS]
	}
}

#pragma endregion

#pragma endregion

				// 편의 기능
#pragma region AdventureMS QoL

// 충전식 화살(화살을 표창 / 총알처럼 충전 가능하게 변경)

  // 지정된 주소에 CodeCave 생성
  // 화살 타입 ID 비교 추가:
void CreateCodecaveAt(DWORD patchAddress, DWORD jmpto, DWORD jmpback) {
	BYTE* cave = (BYTE*)VirtualAlloc(nullptr, 64, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!cave) return;

	BYTE* p = cave;

	// cmp eax, 206 = 화살 타입 1
	*p++ = 0x3D; *p++ = 0xCE; *p++ = 0x00; *p++ = 0x00; *p++ = 0x00; // cmp eax, 206
	*p++ = 0x0F; *p++ = 0x84;                                        // je
	DWORD reljmp = jmpto - ((DWORD)p + 4);
	memcpy(p, &reljmp, 4); p += 4;

	// cmp eax, 207 = 화살 타입 2
	*p++ = 0x3D; *p++ = 0xCF; *p++ = 0x00; *p++ = 0x00; *p++ = 0x00;
	*p++ = 0x0F; *p++ = 0x84;
	reljmp = jmpto - ((DWORD)p + 4);
	memcpy(p, &reljmp, 4); p += 4;

	// cmp eax, 233 = 화살 타입 3
	*p++ = 0x3D; *p++ = 0xE9; *p++ = 0x00; *p++ = 0x00; *p++ = 0x00;
	*p++ = 0x0F; *p++ = 0x84;
	reljmp = jmpto - ((DWORD)p + 4);
	memcpy(p, &reljmp, 4); p += 4;

	// jmp back
	*p++ = 0xE9;
	reljmp = jmpback - ((DWORD)p + 4);
	memcpy(p, &reljmp, 4);

	// Final jump from original address to this codecave
	DWORD relToCave = (DWORD)cave - patchAddress - 5;
	BYTE patch[5] = { 0xE9 };
	memcpy(patch + 1, &relToCave, 4);
	WriteProcessMemory(GetCurrentProcess(), (void*)patchAddress, patch, 5, nullptr);
}

DWORD GetNearJumpTarget(DWORD instrAddr) {
	BYTE* bytes = reinterpret_cast<BYTE*>(instrAddr);
	if ((bytes[0] == 0x0F && (bytes[1] == 0x84 || bytes[1] == 0x85))) { // JE or JNZ
		int32_t relOffset = *reinterpret_cast<int32_t*>(bytes + 2);
		return instrAddr + 6 + relOffset;
	}
	return 0;
}

BYTE ReadByteAt(DWORD address) {
	return *(BYTE*)address;
}

void replace() { // 모든 총알/표창 코드에 화살 추가
	const char* searchAoB = "3D CF 00 00 00";
	unsigned int address;
	int skipCount = 0;
	std::vector<DWORD> foundAddresses;


	while ((address = Memory::FindAoB(searchAoB, 0x00400000, 0x007AAAAA, skipCount)) != 0) {
		foundAddresses.push_back((DWORD)address);
		skipCount++;
	}

	for (DWORD addr : foundAddresses) {
		DWORD jmpto = addr + 7 + ReadByteAt(addr + 6);
		DWORD returnAfter = addr + 7;
		CreateCodecaveAt(addr, jmpto, returnAfter);
	}
}

DWORD Bypass3 = 0x00755A24; // 충전 비용 표시
DWORD Bypass2 = 0x00755B4E; // 충전 버튼 항상 표시
DWORD Bypass4 = 0x00756C8B; // 충전 가능 여부 체크 스킵

void RechargeArrows() {
	replace(); //replaces all code instances of bullets or throwing star statements to include arrrows
	Memory::PatchNop(Bypass2, 6); //allows us to draw the button for recharge no matter what on bullets/arrows/throwingstars 
	Memory::PatchNop(Bypass3, 6); // Draws the recharge costs
	Memory::PatchNop(Bypass4, 6); // skips something in the recharge function the checks again if they are full or not
}

// 제작 시스템 카테고리 수정
DWORD use = 0x0082313A;
DWORD setup = 0x00823136;
DWORD etc = 0x0082312F;
DWORD cash = 0x0082314D;

__declspec(naked) void other()
{
	__asm
	{
		push 5467
		jmp cash
	}
}

__declspec(naked) void makerStuff()
{
	__asm
	{
		sub eax, 200
		je used

		sub    eax, ecx
		je setupd

		sub eax, ecx
		je etcd

		jmp other

		used : jmp  use
		setupd : jmp setup
		etcd : jmp etc
	}
}

// 자동 배분 버튼 비활성화
DWORD autoAssign_ret = 0x008C5926;
__declspec(naked) void autoAssignRemove()
{
	_asm {
		lea ecx, [eax + 4]
		mov eax, [ecx]
		push 0
		jmp autoAssign_ret
	}
}

// 월드맵 중앙 정렬
int wordMapX, wordMapY;
DWORD wordMapUIccRtn = 0x009EB5A1;
__declspec(naked) void wordMapUIcc()
{
	__asm {
		push 20Ch
		push 29Ah
		push wordMapY
		push wordMapX
		jmp wordMapUIccRtn
	}
}

#pragma endregion

#pragma region AdventureMS Fixes

// 마우스 휠 스크롤 수정
DWORD fixMouseWheelAddr = 0x009E8090;
DWORD fixMouseWheelRetJmpAddr = 0x009E809F;
DWORD SetCursorVectorPos = 0x0059A0CB;
__declspec(naked) void fixMouseWheelHook() {
	__asm {
		// is mouse wheel
		cmp eax, 522
		je[halo3]
		mov eax, dword ptr[edi]
		shr eax, 0x10
		push eax
		movzx eax, word ptr[edi]
		push eax
		call SetCursorVectorPos
		halo3 :
		jmp[fixMouseWheelRetJmpAddr]
	}
}

// 채팅창 텍스트 정렬
DWORD chat_Y_offset_retn = 0x008DD6BE;
DWORD call_func_chat_cave = 0x00403382;
__declspec(naked) void chat_Y_offset_cave()
{
	_asm {
		call call_func_chat_cave
		push eax
		mov eax, [ebp - 28h]
		dec eax
		mov[ebp - 28h], eax
		pop eax
		jmp chat_Y_offset_retn
	}
}

// 몬스터북 텍스트 정렬
DWORD pos1Rtn = 0x0086437D;
DWORD pos2Rtn = 0x0086449A;
DWORD pos3Rtn = 0x008645B9;
__declspec(naked) void pos1() // Y=146 위치
{
	__asm {
		push 146
		sub eax, edi
		push eax
		jmp pos1Rtn
	}
}
__declspec(naked) void pos2() // Y=180 위치
{
	__asm {
		push 180
		sub eax, edi
		push eax
		jmp pos2Rtn
	}
}
__declspec(naked) void pos3() // Y=200 위치
{
	__asm {
		push 200
		sub eax, edi
		push eax
		jmp pos3Rtn
	}
}

#pragma endregion

#pragma region AdventureMS Unused

/*
// 얼굴 ID 제한 확장 (고버전 얼굴 사용)
DWORD dwFace = 0x005C94F3;
DWORD dwFaceRet = 0x005C9505;
DWORD dwFaceAddy = 0x005C95BF;
DWORD dwAddyIDK = 0x005C958D;
Cave(FaceCave) {
	cmp eax, 0x02
		je addyFace
		cmp eax, 0x05
		je addyFace
		cmp eax, 0x07
		je addyFace
		cmp eax, 0x08
		jle addyIDK
		jmp dword ptr[dwFaceRet]

		addyFace:
	jmp dword ptr[dwFaceAddy]

		addyIDK :
		jmp dword ptr[dwAddyIDK]
}
_EndCave
//Increase face limit (npc)
DWORD dwFaceNPC = 0x009ACA99;
DWORD dwFaceNPCret = 0x009ACAA4;
DWORD dwFaceAddy2 = 0x009ACAA0;
Cave(FaceNPC) {
	idiv ecx
		cmp eax, 0x02
		je addyFace2
		cmp eax, 0x05
		je addyFace2
		cmp eax, 0x07
		je addyFace2
		cmp eax, 0x08
		je addyFace2
		jmp dword ptr[dwFaceNPCret]

		addyFace2:
	jmp dword ptr[dwFaceAddy2]
}
_EndCave
*/

/*
// 어깨 장비 슬롯 추가
int equipGridMapX[] = { 5, 38, 71, 104, 137 };
int equipGridMapY[] = { 35, 68, 100, 132, 166 , 197 , 233 };
int equipGridMap[7][5] = { {0, 1, 0, 53, 54}, {49, 2, 0, 15, 16}, {0, 0, 3, 4, 51}, {9, 5, 17, 11, 10}, {8, 6, 50, 12, 13}, {0, 0, 7, 0, 0}, {18, 19, 20, 0, 0} };
int getEquipGridPosition(int cursorX, int cursorY)
{
	const int padding = 0;
	for (int y = 0; y < 7; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			int topLeftX = equipGridMapX[x];
			int topLeftY = equipGridMapY[y];
			int bottomRightX = topLeftX + 32 + (padding * 2);
			int bottomRightY = topLeftY + 32 + (padding * 2);
			if (cursorX >= topLeftX && cursorX < bottomRightX && cursorY >=
				topLeftY && cursorY < bottomRightY)
			{
				return equipGridMap[y][x];
			}
		}
	}

	return 0;
}

std::pair<int, int> getPixelPositionFromNumber(int number)
{
	for (int y = 0; y < 7; ++y)
	{
		for (int x = 0; x < 5; ++x)
		{
			if (equipGridMap[y][x] == number)
			{
				// Scrape our array for the raw base coord data
				int pixelX = equipGridMapX[x];
				int pixelY = equipGridMapY[y] + 32;
				return { pixelX, pixelY };
			}
		}
	}

	return { -1, -1 }; // Invalid req, dump it off-screen
}

int getPixelPositionYFromNumber(int number) { return getPixelPositionFromNumber(number).second; }
int getPixelPositionXFromNumber(int number) { return getPixelPositionFromNumber(number).first; }

DWORD putEquipPosition_Hook_Ret = 0x007FEF58;
_declspec(naked) void putEquipPosition_Hook()
{
	_asm {
		mov eax, [ebp + 0x8];
		push eax;
		call getPixelPositionYFromNumber;
		mov edi, eax;
		pop eax;
		mov eax, [ebp + 0x8];
		push edi;
		push eax;
		call getPixelPositionXFromNumber;
		mov edi, eax;
		pop eax;
		push edi;
		jmp[putEquipPosition_Hook_Ret];
	}
}

End Trinket Changes(Shoulder)

*/

/*
// S공격속도 제한을 0으로 설정 (작동 안함)
DWORD attSpdCap = 0x00765086;
__declspec(naked) void setAttSpdCap()
{
	__asm {
		add[ebp + 0x08], edx
		xor eax, eax // set to 0
		cmp[ebp + 0x08], eax
		jmp attSpdCap
	}
}

// 고버전 헤어/얼굴 지원
DWORD faceRtn = 0x005C95BF;
DWORD hairRtn = 0x005C958D;
DWORD faceHairCaveRtn = 0x005C9505;
DWORD faceHairCave2Rtn = 0x009ACAA0;
DWORD faceHairCave2Jmp = 0x009ACAA4;
DWORD faceHairCave3Rtn = 0x009ACAAC;
__declspec(naked) void faceHairCave()
{
	__asm {
		cmp eax, 2
		jz label_face
		cmp eax, 3
		jz label_hair
		cmp eax, 4
		jz label_hair
		cmp eax, 5
		jz label_face
		cmp eax, 6
		jz label_hair

		jmp faceHairCaveRtn

		label_face :
		jmp faceRtn

			label_hair :
		jmp hairRtn
	}
}
__declspec(naked) void faceHairCave2()
{
	__asm {
		cmp eax, 2
		jz label
		cmp eax, 5
		jz label
		jmp faceHairCave2Jmp

		label :
		jmp faceHairCave2Rtn
	}
}
__declspec(naked) void faceHairCave3()
{
	__asm {
		cmp eax, 3
		jz label
		cmp eax, 4
		jz label
		cmp eax, 6

		label:
		setnz cl
			jmp faceHairCave3Rtn
	}

// Hair / Face Uncap
DWORD dwHairFaceUncap1 = 0x005C94F3;
DWORD dwHairFaceUncap2 = 0x009ACA9B;
DWORD dwHairFaceUncapCapRetn = 0x005C9505;
DWORD dwHairFaceUncapFaceRetn = 0x005C95BF;
DWORD dwHairFaceUncapHairRetn = 0x005C958D;
DWORD dwHairFaceUncapRetn = 0x009ACAAD;
__declspec(naked) void HairFaceIdUncap1()
{
	__asm {
		cmp eax, 0x2
		je FACE_RET
		cmp eax, 0x5
		je FACE_RET
		cmp eax, 0x3
		je HAIR_RET
		cmp eax, 0x4
		je HAIR_RET
		cmp eax, 0x6
		je HAIR_RET
		jmp CAP_RET
		FACE_RET :
		jmp dword ptr[dwHairFaceUncapFaceRetn]
			HAIR_RET :
			jmp dword ptr[dwHairFaceUncapHairRetn]
			CAP_RET :
			jmp dword ptr[dwHairFaceUncapCapRetn]
	}
}

__declspec(naked) void HairFaceIdUncap2()
{
	__asm {
		cmp eax, 0x2
		je FACE_RET
		cmp eax, 0x5
		je FACE_RET
		cmp eax, 0x3
		je HAIR_RET
		cmp eax, 0x4
		je HAIR_RET
		cmp eax, 0x6
		je HAIR_RET
		jmp SKIN_RET
		FACE_RET :
		mov eax, 0x0
			mov ecx, 0x0
			jmp JMP_RET
			HAIR_RET :
		mov eax, 0x1
			mov ecx, 0x1
			jmp JMP_RET
			SKIN_RET :
		mov eax, 0x2
			mov ecx, 0x2
			jmp JMP_RET
			JMP_RET :
		jmp dword ptr[dwHairFaceUncapRetn]
	}
}

// 캐시 이펙트 ID 범위 확장 (501xxxx까지)
DWORD dwCashEffExpansion1 = 0x0093C144;
DWORD dwCashEffExpansion1CheckRtm = 0x0093C156;
DWORD dwCashEffExpansion1Rtm = 0x0093C163;
__declspec(naked) void cash_effect_expansion_1()
{
	__asm {
		mov eax, [ebp + 8] // item ID
		cdq
		mov ecx, 0x2710 // 10000
		idiv ecx
		cmp eax, 0x1F5 // 501
		je enable_loop

		mov eax, [ebp + 8] // item ID
		cdq
		mov ecx, 0x3E8 // 1000
		idiv ecx
		jmp dword ptr[dwCashEffExpansion1CheckRtm] // back to check for 4290xxx

		enable_loop:
		jmp dword ptr[dwCashEffExpansion1Rtm] // set ani type to 32
	}
}

DWORD dwCashEffExpansion2 = 0x0093C67A;
DWORD dwCashEffExpansion2CheckRtm = 0x0093C690;
DWORD dwCashEffExpansion2Rtm = 0x0093C69A;
__declspec(naked) void cash_effect_expansion_2()
{
	__asm {
		mov eax, [ebp + 8] // item ID
		cdq
		mov ecx, 0x2710 // 10000
		idiv ecx
		cmp eax, 0x1F5 // 501
		je enable_loop

		mov eax, [ebp + 8] // item ID
		cdq
		mov ecx, 0x3E8 // 1000
		idiv ecx
		jmp dword ptr[dwCashEffExpansion2CheckRtm] // back to check for 4290xxx

		enable_loop:
		jmp dword ptr[dwCashEffExpansion2Rtm] // set ani type to 32
	}
}

*/

#pragma endregion