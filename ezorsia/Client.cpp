#include "stdafx.h"
#include "AddyLocations.h"
#include "codecaves.h"

// Variable Declarations
int Client::m_nGameHeight = 768;
int Client::m_nGameWidth = 1366;
int Client::MsgAmount = 10;
std::string Client::ServerIP_Address = "xxx.xxx.xx.xx";


#pragma region Ezorsia Prepackaged

void Client::UpdateGameStartup()
{

	Memory::CodeCave(cc0x00A63FF3, dw0x00A63FF3, dw0x00A63FF3Nops); //fix start @0x00A63FF3, may be unnecessary, but dump of vanilla client showed broken code here
	Memory::CodeCave(ccCLoginSendCheckPasswordPacket, dwCLoginSendCheckPasswordPacket, CLoginSendCheckPasswordPacketNops); //CLogin::SendCheckPasswordPacket: At the start of the sequence of pushes that contains 0C9h, place a long jmp to further down in the method to the SystemInfo basic block.Do auth patches for encoding the correct strings(user / pw)
	Memory::WriteByte(0x005F6B87 + 2, 0x08); //CLogin::SendCheckPasswordPacket/sub_5F6952		end 005F6C6F //??
	Memory::WriteByte(0x005F6BA0 + 2, 0xA0); //CLogin::SendCheckPasswordPacket //?? //not sure what these edits specifically do, but hendi's client has them in place for getting rid of checks at this part
	unsigned char CA_005F6BA4[] = { 0xFF, 0x75, 0x0C, 0x90, 0x90, 0x90, 0x90 }; //CLogin::SendCheckPasswordPacket //??
	Memory::WriteByteArray(0x005F6BA4, CA_005F6BA4, sizeof(CA_005F6BA4)); //CLogin::SendCheckPasswordPacket //??
	Memory::WriteByte(0x00496633 + 1, 0x47); //kill CSecurityClient::OnPacket//ty chronicle for idea for this
	Memory::FillBytes(0x009F1C04, 0x90, 5);//run from packed client //WinMain: nop ShowStartUpWndModal	//get rid of pop-up at start of game
	Memory::WriteByte(0x009F242F, 0xEB); //run from packed client //WinMain: jz->jmp for ShowADBalloon code (pretty much at the end of method, above push with small number)
	Memory::WriteByte(0x009F6EDC, 0xEB); //kill if statement in broken //void __thiscall CWvsApp::CreateMainWindow(CWvsApp *this)
	Memory::WriteByte(0x009F74EA + 3, resmanLoadAMNT); //replace their wz load list size with ours //void __thiscall CWvsApp::InitializeResMan(CWvsApp *this)
	Memory::CodeCave(LoadUItwice, dwLoadUItwice, dwLoadUItwiceNOPs);//working after a check after CWvsApp::InitializeInput
	Memory::FillBytes(0x00C08459, 0x20, 0x00C0846E - 0x00C08459);//remove elevation requests
	Memory::WriteByte(0x00C08459, 0x22);//remove elevation requests	//thanks stelmo for showing me how to do this
	Memory::WriteString(0x00C08459 + 1, "asInvoker");//remove elevation requests	//not working from dll
	Memory::WriteByte(0x00C08463, 0x22);//remove elevation requests	//thanks stelmo for showing me how to do this
	Memory::WriteByte(0x0049C2CD + 1, 0x01);//remove elevation requests	//still not working unfortunately	//still keeping this to checks for admin privilege
	Memory::WriteByte(0x0049CFE8 + 1, 0x01);//likely requires affecting WINAPI CreateProcess, which requires a launcher		//because a packed client cannot be directly edited for these offsets
	Memory::WriteByte(0x0049D398 + 1, 0x01);//remove elevation requests	//still not working unfortunately
	Memory::WriteByte(0x0040013E, 0x2F);  //4g edit, not sure if it still works after execution

}

void Client::UpdateResolution()
{
	nStatusBarY = Client::m_nGameHeight - 578;

	Memory::CodeCave(AdjustStatusBar, dwStatusBarVPos, 5);
	Memory::CodeCave(AdjustStatusBarBG, dwStatusBarBackgroundVPos, 5);
	Memory::CodeCave(AdjustStatusBarInput, dwStatusBarInputVPos, 9);

	Memory::WriteInt(dwApplicationHeight + 1, m_nGameHeight);//push 600
	Memory::WriteInt(dwApplicationWidth + 1, m_nGameWidth);	//push 800 ; CWvsApp::InitializeGr2D
	Memory::WriteInt(dwCursorVectorVPos + 2, (int)floor(-m_nGameHeight / 2));//push -300				!!moves all interactable UI elements!!
	Memory::WriteInt(dwCursorVectorHPos + 2, (int)floor(-m_nGameWidth / 2));	//push -400 ; CInputSystem::SetCursorVectorPos				!!moves all interactable UI elements!!
	Memory::WriteInt(dwUpdateMouseLimitVPos + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(dwUpdateMouseLimitHPos + 1, m_nGameWidth);	//mov ecx,800 ; CInputSystem::UpdateMouse
	Memory::WriteInt(dwCursorPosLimitVPos + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(dwCursorPosLimitHPos + 1, m_nGameWidth);	//mov eax,800 ; CInputSystem::SetCursorPos
	Memory::WriteInt(dwViewPortHeight + 3, m_nGameHeight);//lea eax,[esi+eax-600]
	Memory::WriteInt(dwViewPortWidth + 3, m_nGameWidth);	//lea eax,[ecx+eax-800]

	Memory::WriteInt(dwToolTipLimitVPos + 1, m_nGameHeight - 1); //mov eax,599 ; CUIToolTip::MakeLayer
	Memory::WriteInt(dwToolTipLimitHPos + 1, m_nGameWidth - 1); //mov eax,799 ; CUIToolTip::MakeLayer

	Memory::WriteInt(dwTempStatToolTipDraw + 3, -m_nGameWidth + 6); //lea eax,[eax+ecx-797] ; CTemporaryStatView::ShowToolTip
	Memory::WriteInt(dwTempStatToolTipFind + 3, -m_nGameWidth + 6); //lea eax,[eax+ecx-797] ; CTemporaryStatView::FindIcon
	Memory::WriteInt(dwTempStatIconVPos + 2, (m_nGameHeight / 2) - 23);	//sub ebx,277 ; Skill icon buff y-pos
	Memory::WriteInt(dwTempStatIconHpos + 3, (m_nGameWidth / 2) - 3);	//lea eax,[eax+esi+397] ; Skill icon buff x-pos
	Memory::WriteInt(dwTempStatCoolTimeVPos + 2, (m_nGameHeight / 2) - 23);	//sub ebx,277 ; Skill icon cooltime y-pos
	Memory::WriteInt(dwTempStatCoolTimeHPos + 3, (m_nGameWidth / 2) - 3);	//lea eax,[eax+esi+397] ; Skill icon cooltime x-pos

	Memory::WriteInt(dwQuickSlotInitVPos + 1, m_nGameHeight + 1);//add eax,533
	Memory::WriteInt(dwQuickSlotInitHPos + 1, 798); //push 647 //hd800
	Memory::WriteInt(dwQuickSlotVPos + 2, m_nGameHeight + 1);//add esi,533
	Memory::WriteInt(dwQuickSlotHPos + 1, 798); //push 647 //hd800
	Memory::WriteInt(dwQuickSlotCWndVPos + 2, (600 - m_nGameHeight) / 2 - 427); //lea edi,[eax-427]
	Memory::WriteInt(dwQuickSlotCWndHPos + 2, -798); //lea ebx,[eax-647]

	Memory::WriteInt(dwByteAvatarMegaHPos + 1, m_nGameWidth); //push 800 ; CAvatarMegaphone::ByeAvatarMegaphone ; IWzVector2D::RelMove
	Memory::WriteInt(dwAvatarMegaWidth + 1, m_nGameWidth); //push 800 ; CAvatarMegaphone ; CreateWnd

	Memory::WriteInt(0x0043717B + 1, m_nGameHeight);//mov edi,600
	Memory::WriteInt(0x00437181 + 1, m_nGameWidth);	//mov esi,800 ; CreateWnd
	Memory::WriteInt(0x0053808B + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x00538091 + 1, m_nGameWidth);	//push 800 ; RelMove?
	Memory::WriteInt(0x004CC160 + 3, m_nGameWidth);	//mov [ebp-16],800 ; CreateWnd
	Memory::WriteInt(0x004CC2C5 + 2, m_nGameHeight);//cmp ecx,600
	Memory::WriteInt(0x004CC2B0 + 1, m_nGameWidth);	//mov eax,800 ; CreateWnd
	Memory::WriteInt(0x004D59B2 + 1, m_nGameHeight);//mov eax,800
	Memory::WriteInt(0x004D599D + 1, m_nGameWidth);	//mov eax,800 ; CreateWnd
	Memory::WriteInt(0x0085F36C + 2, m_nGameWidth);	//cmp edx,800
	Memory::WriteInt(0x0085F374 + 1, m_nGameWidth - 80);	//mov ecx,720 ; CreateDlg
	Memory::WriteInt(0x008EBC58 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x008EBC3C + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x009966B5 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x009966CA + 2, m_nGameWidth);	//cmp edi,800
	Memory::WriteInt(0x009966D2 + 1, m_nGameWidth - 100);	//mov edx,700 ; CreateDlg
	Memory::WriteInt(0x009A3E7F + 1, m_nGameHeight);//mov edx,600
	Memory::WriteInt(0x009A3E72 + 1, m_nGameWidth);	//mov edx,800 ; CreateDlg
	Memory::WriteInt(0x0045B898 + 1, m_nGameWidth - 225);	//push 575 ##ED  //smega x axis fade
	Memory::WriteInt(0x005386F0 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x005386F5 + 1, m_nGameWidth);	//push 800 ; CField::DrawFearEffect
	Memory::WriteInt(0x0055B808 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0055B80D + 1, m_nGameWidth);	//mov edi,800
	Memory::WriteInt(0x0055B884 + 1, m_nGameWidth);	//push 600 ; RelMove?
	Memory::WriteInt(0x007E15BE + 1, m_nGameWidth);	//push 800 ; CreateWnd
	Memory::WriteInt(0x007E16B9 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x007E16BE + 1, m_nGameWidth);	//push 800 ; CWnd::GetCanvas //!!length of server message at top
	Memory::WriteInt(0x008AA266 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x008AA26B + 1, m_nGameWidth);	//push 800 ; CreateWnd
	Memory::WriteInt(0x009F6E99 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x009F6EA0 + 1, m_nGameWidth);	//push 800 ; StringPool#1162 (MapleStoryClass)

	Memory::WriteInt(0x007CF48F + 1, m_nGameHeight);//mov eax,600 ; 
	Memory::WriteInt(0x007CF49D + 1, m_nGameWidth);	//mov eax,800 ; IWzVector2D::RelMove
	Memory::WriteInt(0x008A12F4 + 1, m_nGameHeight);//mov eax,600 ; 
	Memory::WriteInt(0x008A1302 + 1, m_nGameWidth);	//mov eax,800 ; IWzVector2D::RelMove
	Memory::WriteInt(0x007F257E + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x007F258F + 1, m_nGameWidth);	//push 800 ; CWnd::CreateWnd
	Memory::WriteInt(0x0046B85C + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x0046B86A + 1, m_nGameWidth);	//mov eax,800 ; IWzVector2D::RelMove
	Memory::WriteInt(0x009994D8 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x009994ED + 2, m_nGameWidth);	//cmp edi,800
	Memory::WriteInt(0x009994F5 + 1, m_nGameWidth - 100);	//mov edx,700 ; CreateDlg

	Memory::WriteInt(0x0062FC4A + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0062FC4F + 1, m_nGameWidth);	//push 800 ; IWzGr2DLayer::Getcanvas
	Memory::WriteInt(0x0062FE63 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0062FE68 + 1, m_nGameWidth);	//push 800 ; IWzGr2DLayer::Getcanvas
	Memory::WriteInt(0x0062F9C6 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0062F9CB + 1, m_nGameWidth);	//push 800; (UI/Logo/Wizet)
	Memory::WriteInt(0x0062F104 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x0062F109 + 1, m_nGameWidth);	//mov eax,800 ; Rectangle
	Memory::WriteInt(0x006406D5 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x006406C3 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x0064050A + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x006404F8 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x00640618 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x00640690 + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x0064061D + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x0064064B + 1, m_nGameHeight);//mov ecx,600
	Memory::WriteInt(0x00640606 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x0064067E + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x00640639 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x0064043E + 1, (int)floor(m_nGameWidth / 2));	//mov edi,400
	Memory::WriteInt(0x00640443 + 1, (int)floor(m_nGameHeight / 2));	//mov esi,300
	Memory::WriteInt(0x00640626 + 1, (int)floor(m_nGameWidth / 2));	//add eax,400 ; bunch of modulus stuff

	Memory::WriteInt(0x00641038 + 2, m_nGameHeight);//??possibly related to player display
	Memory::WriteInt(0x0064103F + 2, m_nGameWidth);//??possibly related to player display
	Memory::WriteInt(0x00641048 + 1, (int)floor(-m_nGameHeight / 2));	//mov esi,-300
	Memory::WriteInt(0x00641050 + 1, (int)floor(-m_nGameWidth / 2));		//mov esi,-400 ;
	Memory::WriteInt(0x00641A19 + 3, m_nGameHeight);//mov [ebp+28],600
	Memory::WriteInt(0x00641A12 + 3, m_nGameWidth);	//mov [ebp+32],800 ; idk
	Memory::WriteInt(0x00641B38 + 3, m_nGameHeight);//mov [ebp-32],600
	Memory::WriteInt(0x00641B2E + 3, m_nGameWidth);	//mov [ebp-36],800 ; CAnimationDisplayer::SetCenterOrigin

	Memory::WriteInt(0x006CD842 + 1, (int)floor(m_nGameWidth / 2));	//push 400 ; RelMove?

	Memory::WriteInt(0x0059A0A2 + 6, (int)floor(m_nGameHeight / 2));	//mov [ebx+2364],300
	Memory::WriteInt(0x0059A09C + 2, (int)floor(m_nGameWidth / 2));	//mov [esi],400	; CInputSystem::LoadCursorState
	Memory::WriteInt(0x0080546C + 1, m_nGameHeight);//mov edi,600
	Memory::WriteInt(0x00805459 + 1, m_nGameWidth);	//mov edx,800 ; CUIEventAlarm::CreateEventAlarm
	Memory::WriteInt(0x008CFD4B + 1, m_nGameHeight - 22);	//push 578
	Memory::WriteInt(0x008CFD50 + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x0053836D + 1, (int)floor(-m_nGameHeight / 2));//push -300
	Memory::WriteInt(0x00538373 + 1, (int)floor(-m_nGameWidth / 2));	//push -400	; RelMove?
	Memory::WriteInt(0x0055BB2F + 1, (int)floor(-m_nGameHeight / 2));//push -300
	Memory::WriteInt(0x0055BB35 + 1, (int)floor(-m_nGameWidth / 2));	//push -400 ; RelMove?

	Memory::WriteInt(0x005A8B46 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005A8B56 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005A9B42 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005A9B52 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005AADAA + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005AADBA + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005ABC65 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005ABC75 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005ACB29 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005ACB39 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005C187E + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005C188E + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005C2D62 + 1, m_nGameHeight);//mov eax,600
	Memory::WriteInt(0x005C2D72 + 1, m_nGameWidth);	//mov eax,800 ; RelMove?
	Memory::WriteInt(0x005E3FA0 + 1, m_nGameHeight);//push 600

	Memory::WriteInt(0x005F64DE + 1, (int)floor(-m_nGameHeight / 2));	//push -300 ;
	Memory::WriteInt(0x005F6627 + 1, (int)floor(-m_nGameHeight / 2));	//push -300 ;

	Memory::WriteInt(0x0060411C + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0060F79B + 1, (m_nGameHeight / 2) - 201);//??possibly related to login utildlg
	Memory::WriteInt(0x0060F7A4 + 1, (m_nGameHeight / 2) - 181);//??possibly related to login utildlg
	Memory::WriteInt(0x0060F7AC + 1, (m_nGameWidth / 2) - 201);//??possibly related to login utildlg
	Memory::WriteInt(0x0060F7B5 + 1, (m_nGameWidth / 2) - 181);//??possibly related to login utildlg
	Memory::WriteInt(0x00613093 + 1, (m_nGameHeight / 2) - 200);//??likely related to login pop-up msg
	Memory::WriteInt(0x00613099 + 1, (m_nGameWidth / 2) - 45);//??likely related to login pop-up msg
	Memory::WriteInt(0x0061DAFF + 1, (m_nGameHeight / 2) - 150);//??likely related to login pop-up msg
	Memory::WriteInt(0x0061DB08 + 1, (m_nGameHeight / 2) - 130);//??likely related to login pop-up msg
	Memory::WriteInt(0x0061DB10 + 1, (m_nGameWidth / 2) - 201);//??likely related to login pop-up msg
	Memory::WriteInt(0x0061DB19 + 1, (m_nGameWidth / 2) - 181);//??likely related to login pop-up msg

	Memory::WriteInt(0x004372B1 + 1, (int)floor(-m_nGameHeight / 2));//push -300
	Memory::WriteInt(0x004372B6 + 1, (int)floor(-m_nGameWidth / 2));	//push -400 ; RelMove?
	Memory::WriteInt(0x006CE3AB + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x007E1CB7 + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x008D82F5 + 1, m_nGameHeight - 22);	//push 578
	Memory::WriteInt(0x008D82FA + 1, m_nGameWidth);	//push 800 ; CreateWnd?
	Memory::WriteInt(0x00935870 + 1, (int)floor(m_nGameHeight / 2));	//push 300
	Memory::WriteInt(0x0093586B + 1, m_nGameWidth);	// push 800 ; RelMove? (Skills)
	Memory::WriteInt(0x009DFD5C + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x009DFED2 + 1, m_nGameHeight);//mov ecx,600	; IWzVector2D::RelMove
	Memory::WriteInt(0x009F6ADD + 1, (int)floor(m_nGameHeight / 2)); //push 300 ; MapleStoryClass
	Memory::WriteInt(0x006D50D8 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0074BAA9 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0074B951 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0074B4A2 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0074B3B7 + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x006421B3 + 1, m_nGameHeight);//push 600 ; CSoundMan::PlayBGM

	Memory::WriteInt(0x0059EB49 + 1, m_nGameHeight);//push 600 ; CSoundMan::PlayBGM
	Memory::WriteInt(0x008D247B + 1, m_nGameHeight - 33);	//push 567 ; IWzVector2D::RelMove
	Memory::WriteInt(0x008DEB93 + 1, m_nGameHeight - 20);	//push 580
	Memory::WriteInt(0x008DEE2F + 1, m_nGameHeight - 20);	//push 580
	Memory::WriteInt(0x008D2765 + 1, m_nGameHeight - 19);	//push 581
	Memory::WriteInt(0x008D29B4 + 1, m_nGameHeight - 19);	//push 581
	Memory::WriteInt(0x008D8BFE + 1, m_nGameHeight - 19);	//push 581
	Memory::WriteInt(0x008D937E + 1, m_nGameHeight - 19);	//push 581 //008D9373  move mana bar outline? //ty rynyan
	Memory::WriteInt(0x008D9AC9 + 1, m_nGameHeight - 19);	//push  
	Memory::WriteInt(0x008D1D50 + 1, m_nGameHeight - 22);	//push 578
	Memory::WriteInt(0x008D1D55 + 1, m_nGameWidth);	//push 800
	Memory::WriteInt(0x008D1FF4 + 1, m_nGameHeight - 22);	//push 578
	Memory::WriteInt(0x008D1FF9 + 1, m_nGameWidth);	//push 800 ; CUIStatusBar
	Memory::WriteInt(0x0062F5DF + 1, m_nGameHeight);//push 600
	Memory::WriteInt(0x0062F5E4 + 1, m_nGameWidth);	//push 800 ; (UI/Logo/NXXXon)
	Memory::WriteInt(0x004EDB89 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x004EDB78 + 1, m_nGameHeight);//mov ecx,600 ; CreateWnd
	Memory::WriteInt(0x004EDAD8 + 1, m_nGameWidth);	//mov ecx,800
	Memory::WriteInt(0x009F7079, m_nGameHeight);	// dd 600
	Memory::WriteInt(0x009F707E, m_nGameWidth);	// dd 800
	Memory::WriteInt(0x00BE2738, (int)floor(m_nGameWidth / 2));	// dd 400
	Memory::WriteInt(0x00BE2DF4, (int)floor(m_nGameHeight / 2));	// dd 300
	Memory::WriteInt(0x00BE2DF0, (int)floor(m_nGameWidth / 2));	// dd 400
	Memory::WriteInt(0x00640656 + 2, (int)floor(-m_nGameWidth / 2));		//add edi,-400 ;

	Memory::WriteInt(0x006CE4C6 + 1, (int)floor(-m_nGameWidth / 2));		//push -400 ;
	Memory::WriteInt(0x009E2E85 + 1, (int)floor(-m_nGameHeight / 2));	//push -300		overall screen visible UI scaling
	Memory::WriteInt(0x009E2E8B + 1, (int)floor(-m_nGameWidth / 2));		//push -400 ;	overall screen visible UI scaling

	Memory::WriteInt(0x0093519A + 1, (int)floor(-m_nGameHeight / 2));	//push -300 ;
	Memory::WriteInt(0x00954433 + 1, (int)floor(-m_nGameHeight / 2));	//push -300 ;
	Memory::WriteInt(0x00981555 + 1, (int)floor(-m_nGameHeight / 2));	//push -300 ;
	Memory::WriteInt(0x00981F7A + 2, (int)floor(-m_nGameHeight / 2));	//push -300 ;
	Memory::WriteInt(0x00A448B0 + 2, (int)floor(-m_nGameHeight / 2));	//push -300 ; CWvsPhysicalSpace2D::Load]

	Memory::WriteInt(0x0066BACE + 2, (int)floor(-m_nGameWidth / 2));		//and ecx,-400
	Memory::WriteInt(0x009B76BD + 3, (int)floor(-m_nGameHeight / 2));	//push -300
	Memory::WriteInt(0x009B76CB + 3, (int)floor(m_nGameHeight / 2));		//push 300

	Memory::WriteInt(0x009F7078 + 1, m_nGameHeight);//??related to application dimensions	//(ragezone release merge)//thanks mr mr of ragezone for these addresses
	Memory::WriteInt(0x009F707D + 1, m_nGameWidth);//??related to application dimensions

	Memory::WriteInt(0x0058C8A6 + 1, m_nGameWidth);//??

	Memory::WriteInt(0x004EDABF + 1, m_nGameHeight);//??

	Memory::WriteInt(0x00991854 + 1, m_nGameHeight);//??unknown cwnd function
	Memory::WriteInt(0x0099185F + 1, (m_nGameWidth / 2) - 134);//??unknown cwnd function
	Memory::WriteInt(0x00991867 + 1, (m_nGameWidth / 2) - 133);//??unknown cwnd function
	Memory::WriteInt(0x00992BA7 + 1, (int)floor(m_nGameWidth / 2));//??unknown cwnd function, possibly related to cutildlg
	Memory::WriteInt(0x00992BAC + 1, (int)floor(m_nGameHeight / 2));//??unknown cwnd function, possibly related to cutildlg

	Memory::WriteInt(0x007E1E07 + 2, m_nGameWidth);//??related to displaying server message at top of screen
	Memory::WriteInt(0x007E19CA + 2, m_nGameWidth);//??related to displaying server message at top of screen

	Memory::WriteInt(0x005362B2 + 1, (m_nGameWidth / 2) - 129);//??related to boss bar
	Memory::WriteInt(0x005364AA + 2, (m_nGameWidth / 2) - 128);//??related to boss bar

	Memory::WriteInt(0x00592A08 + 1, (m_nGameWidth / 2) - 125);//??likely related to mouse pos

	Memory::WriteInt(0x00621226 + 1, (m_nGameWidth / 2) - 216);//??possibly related to logo
	Memory::WriteByte(0x0062121E + 1, 0x01);//??possibly related to logo

	Memory::WriteInt(0x008C069F + 1, (m_nGameHeight / 2) - 14);//??related to status bar
	Memory::WriteInt(0x008C06A4 + 1, (m_nGameWidth / 2) - 158);//???related to status bar

	Memory::WriteInt(0x00A24D0B + 1, (m_nGameWidth / 2) - 129);//??

	Memory::WriteInt(0x00BE273C, 128);//??
	Memory::WriteByte(0x00A5FC2B, 0x05);//??
	Memory::WriteByte(0x0089B636 + 2, 0x01); //related to exp gain/item pick up msg, seems to affect msg height ! originally 1U but changed because unsigned int crashes it after char select
	Memory::WriteByte(0x00592A06 + 1, 0x01);//???likely related to mouse pos

	Memory::WriteInt(0x00744EB4 + 1, m_nGameWidth);//??related to in-game taking screenshot functionality
	Memory::WriteInt(0x00744EB9 + 1, m_nGameHeight);//??related to in-game taking screenshot functionality
	Memory::WriteInt(0x00744E2A + 1, 3 * m_nGameWidth * m_nGameHeight);//??related to in-game taking screenshot functionality
	Memory::WriteInt(0x00744E43 + 1, m_nGameWidth * m_nGameHeight);//??related to in-game taking screenshot functionality
	Memory::WriteInt(0x00744DA6 + 1, 4 * m_nGameWidth * m_nGameHeight);//??related to in-game taking screenshot functionality

	Memory::WriteInt(0x00897BB4 + 1, (m_nGameWidth / 2) - 143);//??related to exp gain/item pick up msg

	int msgAmntOffset, msgAmnt; msgAmnt = MsgAmount; msgAmntOffset = msgAmnt * 14;
	Memory::WriteInt(0x0089B639 + 1, m_nGameHeight - 6 - msgAmntOffset);//inventory/exp gain y axis //####hd100 //90
	Memory::WriteInt(0x0089B6F7 + 1, m_nGameWidth - 405);//inventory/exp gain x axis //310 //####hd415 //405

	Memory::WriteInt(0x0089AF33 + 1, 400);//length of pick up and exp gain message canvas //found with help from Davi
	Memory::WriteInt(0x0089B2C6 + 1, 400);//address to move the message in the canvas adjusted above to the center of the new canvas  //thanks chris

	Memory::WriteInt(0x0089AEE2 + 3, msgAmnt);//moregainmsgs part 1
	MoreGainMsgsOffset = msgAmnt;	//param for ccmoregainmssgs
	Memory::CodeCave(ccMoreGainMsgs, dwMoreGainMsgs, MoreGainMsgsNOPs); //moregainmsgs part 2
	MoreGainMsgsFadeOffset = 15000;	//param for ccmoregainmssgsFade
	Memory::CodeCave(ccMoreGainMsgsFade, dwMoreGainMsgsFade, MoreGainMsgsFadeNOPs); //moregainmsgsFade
	MoreGainMsgsFade1Offset = 255 * 4 / 3;	//param for ccmoregainmssgsFade
	Memory::CodeCave(ccMoreGainMsgsFade1, dwMoreGainMsgsFade1, MoreGainMsgsFade1NOPs); //moregainmsgsFade1

	Memory::WriteInt(0x0045B337 + 1, m_nGameWidth);//related to smega display  //likely screen area where pop up starts for smega
	Memory::WriteInt(0x0045B417 + 1, m_nGameWidth - 225);//smega with avatar x axis for duration on screen

	Memory::WriteInt(0x007C2531 + 1, m_nGameHeight - 80);//??

	Memory::WriteInt(0x0089B796 + 1, m_nGameHeight - 18);//???related to exp gain/item pick up msg //??!!found in another diff also !!!!!!!!!!!!

	Memory::WriteInt(0x00849E39 + 1, m_nGameHeight - 177); //system menu pop up
	//00849E3F related to previous^
	Memory::WriteInt(0x0084A5B7 + 1, m_nGameHeight - 281); //shortcuts pop up	//0x84A5BD -  System Options "X" Position. if needed
	//0084A5BD related to previous^

	Memory::WriteInt(0x00522C73 + 1, m_nGameHeight - 92);// ??various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00522E65 + 1, m_nGameHeight - 92); // ??various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052307E + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00523359 + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00523595 + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up //quest complete y axis
	Memory::WriteInt(0x0052378B + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x0052397D + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00523BB1 + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00523DA5 + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00523FA3 + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x005243DB + 1, m_nGameHeight - 92);// various requests like party, guild, friend, family, invites that pop up
	Memory::WriteInt(0x00523154 + 1, m_nGameHeight - 102);//?? various requests like party, guild, friend, family, invites that pop up
	//0052315C part of the previous^

	int reqPopOffset = 41;
	Memory::WriteInt(0x0052418C + 1, m_nGameHeight - 102);//party quest available pop-up y axis		my first address find own my own
	Memory::WriteInt(0x00523092 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up	//trade		 //thank you Rain for the width addresses
	Memory::WriteInt(0x0052336D + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up //Party Invite
	Memory::WriteInt(0x00522E79 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up //friend request
	Memory::WriteInt(0x00522C87 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up	// Guild Invite
	Memory::WriteInt(0x0052379F + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up//??
	Memory::WriteInt(0x00523991 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up/??
	Memory::WriteInt(0x00523BC5 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up/??
	Memory::WriteInt(0x00523DC5 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up// ??
	Memory::WriteInt(0x00523FB7 + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up// ??
	Memory::WriteInt(0x005243EF + 1, 464 - reqPopOffset);//various requests like party, guild, friend, family, invites that pop up//??

	Memory::WriteByte(0x00533B03, 0xb8);	//boss bar extend to window
	Memory::WriteInt(0x00533B03 + 1, m_nGameWidth - 15);	//boss bar	extend to window
	Memory::WriteByte(0x00534370, 0xb9);	//boss bar	extend to window
	Memory::WriteInt(0x00534370 + 1, m_nGameWidth - 22);	//boss bar	extend to window

	myHeight = (Client::m_nGameHeight - 600) / 2;//cash shop fix for frame area	//recalc offsets
	myWidth = (Client::m_nGameWidth - 800) / 2;//cash shop fix for frame area		//recalc offsets
	nHeightOfsetted1 = 316; nWidthOfsetted1 = 256; nTopOfsetted1 = 0 + myHeight; nLeftOfsetted1 = 0 + myWidth; //parameters for fix1
	Memory::CodeCave(CashShopFix1, dwCashFix1, dwCashFix1NOPs);
	nHeightOfsetted2 = 104; nWidthOfsetted2 = 256; nTopOfsetted2 = 318 + myHeight; nLeftOfsetted2 = -1 + myWidth; //parameters for fix2
	Memory::CodeCave(CashShopFix2, dwCashFix2, dwCashFix2NOPs);
	nHeightOfsetted3 = 163; nWidthOfsetted3 = 246; nTopOfsetted3 = 426 + myHeight; nLeftOfsetted3 = 0 + myWidth; //parameters for fix3
	Memory::CodeCave(CashShopFix3, dwCashFix3, dwCashFix3NOPs);
	nHeightOfsetted4 = 78; nWidthOfsetted4 = 508; nTopOfsetted4 = 17 + myHeight; nLeftOfsetted4 = 272 + myWidth; //parameters for fix4
	Memory::CodeCave(CashShopFix4, dwCashFix4, dwCashFix4NOPs);
	nHeightOfsetted5 = 430; nWidthOfsetted5 = 412; nTopOfsetted5 = 95 + myHeight; nLeftOfsetted5 = 275 + myWidth; //parameters for fix5
	Memory::CodeCave(CashShopFix5, dwCashFix5, dwCashFix5NOPs);	//main part of shop, item listings	//thanks angel for stuff that helped
	nHeightOfsetted6 = 358; nWidthOfsetted6 = 90; nTopOfsetted6 = 157 + myHeight; //parameters for fix6
	Memory::CodeCave(CashShopFix6, dwCashFix6, dwCashFix6NOPs);//code cave 6 //best sellers
	Memory::WriteInt(0x004694BA + 1, myWidth + 690);//nleft, actual drawn part	//best sellers
	nHeightOfsetted7 = 56; nWidthOfsetted7 = 545; nTopOfsetted7 = 530 + myHeight; nLeftOfsetted7 = 254 + myWidth; //parameters for fix7
	Memory::CodeCave(CashShopFix7, dwCashFix7, dwCashFix7NOPs);
	nHeightOfsetted8 = 22; nWidthOfsetted8 = 89; nTopOfsetted8 = 97 + myHeight; nLeftOfsetted8 = 690 + myWidth; //parameters for fix8
	Memory::CodeCave(CashShopFix8, dwCashFix8, dwCashFix8NOPs);
	Memory::CodeCave(CashShopFixOnOff, dwCashFixOnOff, dwCashFixOnOffNOPs);	//fix for preview On/Off button not being accurate on entering cash shop //thanks windyboy

	nHeightOfsettedPrev = 165 + myHeight; nWidthOfsettedPrev = 212 + myWidth; nTopOfsettedPrev = 40 + myHeight; nLeftOfsettedPrev = 24 + myWidth; //parameters for fix cash preview
	Memory::CodeCave(CashShopFixPrev, dwCashFixPrev, dwCashFixPrevNOPs); //cash shop preview fix

	Memory::WriteInt(0x00641F61 + 1, (int)floor(m_nGameWidth / 2));	//mov ebc,400 ;  VRleft		//camera movement
	Memory::WriteInt(0x00641FC8 + 1, (int)floor(m_nGameHeight / 2));	//add eax,300  ; VRTop //camera movement //not working for most maps
	Memory::WriteInt(0x0064208F + 1, (int)floor(m_nGameHeight / 2));	//add eax,300  ; VRbottom //camera movement //not working for most maps

	myAlwaysViewRestoreFixOffset = myHeight; //parameters for fix view restore all maps number ?????working????!!!
	Memory::CodeCave(AlwaysViewRestoreFix, dwAlwaysViewRestoreFix, dwAlwaysViewRestoreFixNOPs);	//fix view restora on all maps, currently does nothing; i likely looked in the wrong area

	if (MainMain::CustomLoginFrame) {
		Memory::WriteInt(0x005F481E + 1, (int)floor(-m_nGameHeight / 2));//push -300				!!game login frame!! turn this on if you edit UI.wz and use a frame that matches your res
		Memory::WriteInt(0x005F4824 + 1, (int)floor(-m_nGameWidth / 2));	//push -400 ; RelMove?				!!game login frame!! turn this on if you edit UI.wz and use a frame that matches your res
	}

	if (MainMain::bigLoginFrame) {
		Memory::WriteInt(0x005F464D + 1, m_nGameWidth - 165);	//mov eax,800 ; RelMove?	//game version number for login frames that hug the side of the screen //you will still need to offset ntop, and that may require a code cave if your height resolution is too big
	}
	else {
		nTopOfsettedVerFix = 10 + myHeight; nLeftOfsettedVerFix = 645 + myWidth; //parameters for fix version number
		Memory::CodeCave(VersionNumberFix, dwVersionNumberFix, dwVersionNumberFixNOPs);	//game version number fix //use this if you use no frame or default client frame
	}

	if (!MainMain::bigLoginFrame) {
		nHeightOfsettedLoginBackCanvasFix = 352 + myHeight; nWidthOfsettedLoginBackCanvasFix = 125 + myWidth;//para for world select buttonsViewRec
		nTopOfsettedLoginBackCanvasFix = 125 + myHeight; nLeftOfsettedLoginBackCanvasFix = 0 + myWidth;
		Memory::CodeCave(ccLoginBackCanvasFix, dwLoginBackCanvasFix, LoginBackCanvasFixNOPs);	//world select buttons fix		//thank you teto for pointing out my error in finding the constructor

		nHeightOfsettedLoginViewRecFix = 167 + myHeight; nWidthOfsettedLoginViewRecFix = 540 + myWidth;//para for ViewRec fix
		nTopOfsettedLoginViewRecFix = 51 + myHeight; nLeftOfsettedLoginViewRecFix = 136 + myWidth;
		Memory::CodeCave(ccLoginViewRecFix, dwLoginViewRecFix, LoginViewRecFixNOPs);	//world ViewRec fix	

		a1x = 0 + myWidth; a2x = -149 + myWidth; a2y = 0 + myHeight; a3 = 25; a1y = -250; //a4 = 0;	//LoginDescriptor params
		Memory::WriteInt(0x0060D849 + 1, 300 + a1y); //speed 1	//temporary fix by increasing the speed of display until i get good enough at procedural programming 
		Memory::CodeCave(ccLoginDescriptorFix, dwLoginDescriptorFix, LoginDescriptorFixNOPs);	//world LoginDescriptor fix	
	}

	int customEngY = -62, customEngX = -22, dojangYoffset = 0;	//myHeight //-55-35 (myHeight*250/100)	-(myWidth*53/100) 140 -130
	yOffsetOfMuruengraidPlayer = 50 + dojangYoffset; xOffsetOfMuruengraidPlayer = 169 + myWidth; //params
	Memory::CodeCave(ccMuruengraidPlayer, dwMuruengraidPlayer, MuruengraidPlayerNOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidClock = 26 + dojangYoffset; xOffsetOfMuruengraidClock = 400 + myWidth; //params
	Memory::CodeCave(ccMuruengraidClock, dwMuruengraidClock, MuruengraidClockNOPs);	//muruengraid scaling
	yOffsetOfMuruengraidMonster = 50 + dojangYoffset; xOffsetOfMuruengraidMonster = 631 + myWidth; //params
	Memory::CodeCave(ccMuruengraidMonster, dwMuruengraidMonster, MuruengraidMonsterNOPs);	//muruengraid scaling
	yOffsetOfMuruengraidMonster1 = 32 + dojangYoffset; xOffsetOfMuruengraidMonster1 = 317 + myWidth; //params
	Memory::CodeCave(ccMuruengraidMonster1, dwMuruengraidMonster1, MuruengraidMonster1NOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidMonster2 = 32 + dojangYoffset; xOffsetOfMuruengraidMonster2 = 482 + myWidth; //params
	Memory::CodeCave(ccMuruengraidMonster2, dwMuruengraidMonster2, MuruengraidMonster2NOPs);	//muruengraid scaling
	yOffsetOfMuruengraidEngBar = 86 + dojangYoffset + customEngY; xOffsetOfMuruengraidEngBar = 17 + myWidth + customEngX; //params
	Memory::CodeCave(ccMuruengraidEngBar, dwMuruengraidEngBar, MuruengraidEngBarNOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidEngBar1 = 130 + dojangYoffset + customEngY; xOffsetOfMuruengraidEngBar1 = 20 + myWidth + customEngX; //params
	Memory::CodeCave(ccMuruengraidEngBar1, dwMuruengraidEngBar1, MuruengraidEngBar1NOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidEngBar2 = 80 + dojangYoffset + customEngY; xOffsetOfMuruengraidEngBar2 = 9 + myWidth + customEngX; //params
	Memory::CodeCave(ccMuruengraidEngBar2, dwMuruengraidEngBar2, MuruengraidEngBar2NOPs);	//muruengraid scaling	
	yOffsetOfMuruengraidClearRoundUI = 260 + myHeight; xOffsetOfMuruengraidClearRoundUI = 400 + myWidth; //params
	Memory::CodeCave(ccMuruengraidClearRoundUI, dwMuruengraidClearRoundUI, MuruengraidClearRoundUINOPs);	//muruengraid scaling
	yOffsetOfMuruengraidTimerBar = 16 + dojangYoffset; xOffsetOfMuruengraidTimerBar = 345 + myWidth; //params
	Memory::CodeCave(ccMuruengraidTimerBar, dwMuruengraidTimerBar, MuruengraidTimerBarNOPs);	//muruengraid scaling
	xOffsetOfMuruengraidMonster1_2 = 318 + myWidth; //params	//finally fixed this bugger
	Memory::CodeCave(ccMuruengraidMonster1_2, dwMuruengraidMonster1_2, MuruengraidMonster1_2NOPs);	//muruengraid scaling
}

void Client::UpdateLogin()
{
	// Login Screen
	Memory::CodeCave(PositionLoginDlg, dwLoginCreateDlg, 14);
	Memory::CodeCave(PositionLoginUsername, dwLoginUsername, 11);
	Memory::CodeCave(PositionLoginPassword, dwLoginPassword, 8);
	Memory::WriteInt(dwLoginInputBackgroundColor + 3, 0xFFF8FAFF); // ARGB value
	Memory::WriteByte(dwLoginInputFontColor + 3, 1); // bool: true=black, false=white
	Memory::WriteInt(dwLoginLoginBtn + 1, -127); // x-pos
	Memory::WriteInt(dwLoginFindPasswordBtn + 1, -127); // x-pos
	Memory::WriteInt(dwLoginQuitBtn + 1, -127); // x-pos
	Memory::WriteInt(dwLoginFindIDBtn + 1, -127); // x-pos
	Memory::WriteByte(dwLoginFindIDBtn + 1, -127); // x-pos
	Memory::WriteByte(dwLoginWebHomeBtn + 1, -127); // x-pos
	Memory::WriteByte(dwLoginWebRegisterBtn + 1, -127); // x-pos
}

#pragma endregion

void Client::AdventureMS()
{

#pragma region AdventureMS Gameplay Changes

#pragma region Custom Skill Work

	// 커스텀 액티브 스킬 활성화(새로운 스킬 추가 가능하게 함)
	Memory::SetHook(true, reinterpret_cast<void**>(&pDoActiveSkill), CUserLocal__DoActiveSkill_t);

	// LTRB(Left-Top-Right-Bottom) 처리(스킬 공격 범위를 데이터 기반으로 처리)
	_is_attack_area_set_by_data = reinterpret_cast<is_attack_area_set_by_data_t>(0x007666CB);
	Memory::SetHook(TRUE, reinterpret_cast<PVOID*>(&_is_attack_area_set_by_data), is_attack_area_set_by_data);

	// LTRB(Left-Top-Right-Bottom) 처리(원거리 스킬의 공격 범위 커스텀)
	_is_rect_attack_shoot_skill = reinterpret_cast<is_rect_attack_shoot_skill_t>(0x00766722);
	Memory::SetHook(TRUE, reinterpret_cast<PVOID*>(&_is_rect_attack_shoot_skill), is_rect_attack_shoot_skill);

#pragma endregion
				// 기존 스킬 변경
#pragma region Existing Skill Changes

	// 아란 콤보 유지시간 5초(5000)로 설정
	Memory::WriteInt(0x0094BDC8 + 1, 0x13B8);
	Memory::WriteInt(0x00960708 + 1, 0x13B8);
	Memory::WriteInt(0x0096095B + 1, 0x13B8);

	// 아란 버프 수정 - 물방/마방
	Memory::WriteByte(0x007669D6 + 2, 0x09);
	
	// 파워가드 변경(반사 스킬 수정)
	Memory::PatchNop(0x00958803, 2);
	Memory::PatchNop(0x00958805, 3);
	Memory::PatchNop(0x00958816, 11); // 보스 반감 제거
	Memory::PatchNop(0x009593AE, 3); // 회복 이펙트 제거

	// 즉시 파이널어택
	Memory::WriteByte(0x0095795E, 0x83);
	Memory::WriteByte(0x0095795E + 1, 0xC0);
	Memory::WriteByte(0x0095795E + 2, 0x00);

	// 모든 스킬 넉백 거리 700으로 변경(기본 300에서 700으로 증가)
	Memory::WriteByte(0x0066BA5E + 1, 0xB4);
	Memory::WriteByte(0x0066Ba5E + 2, 0x02);

	// 돌진 쿨타임 감소 100ms
	// Memory::WriteByte(0x00952EF9 + 2, 0x64);
	// Memory::WriteByte(0x00952EF9 + 3, 0x00);

	// 공중에서 돌진 사용
	// Memory::PatchNop(0x00950B58, 6);
	// Memory::PatchNop(0x00950B63, 6);
	// Memory::PatchNop(0x00950B6E, 6);
	
	// 스턴 마스터리 크리티컬 펀치(스턴 관련 스킬 수정)
	// Memory::PatchNop(0x0078E1DC, 2);

	// 어썰터 타겟 불필요
	// Memory::WriteByte(0x00951347 + 1, 0x1C);

	// TryShootAttack CodeCaving for Ranged Skills to be LTRB Handled
	// Memory::SetHook(true, reinterpret_cast<void**>(&ltrbshoothook), ltrb);
	// Memory::CodeCave(LtRb_Eval, 0x00953E2C, 6); // CodeCave to move skills into tryShootAttack

	// 파이어 애로우 / 엔젤레이 / 홀리 애로우 다중 타격
	Memory::CodeCave(FireArrow, dwFireArrow, 5);
	Memory::CodeCave(FireArrowBullet, dwFireBulletAdd, 5);
	Memory::FillBytes(0x0095644A, 0x90, 2);

	// 배틀쉽 탑승 애니메이션 속도 조절
	Memory::WriteValue(0x00936B2D + 6, 28);

	// 다크사이트 중 물약 사용 허용
	Memory::FillBytes(0x0094F6AB, 0x90, 6);

	// 공중에서 부메랑 스텝
	Memory::WriteValue(0x00950B4D + 2, 0x00950C53 - (0x00950B4D + 6));

	// 리코일 샷 쿨타임 감소
	Memory::CodeCave(RecoilShotLowerCD, dwRecoilShot, 5);
	Memory::FillBytes(0x0095364B, 0x90, 2);

	// 공중 텔레포트
	Memory::PatchNop(0x00957C2D, 6);

	// 빅뱅 차징 제거
	Memory::CodeCave(BigBang, dwBB, 5);
	Memory::CodeCave(BigBang2, dwBB2, 5);//I/L
	Memory::CodeCave(BigBang3, dwBB3, 5);//F/P

	// 피어싱 애로우 차징 제거
	Memory::CodeCave(PA, dwPA, 5);

	// 콕스크류 블로우 차징 제거
	Memory::CodeCave(CorkscrewBlow, dwCB, 5);

	// 이동 중 공격 가능
	// Memory::WriteByte(0x0095F97A, 0xEB);
	// Memory::WriteByte(0x0095F97A + 1, 0x59);
	// Memory::WriteByte(0x009CBFB0, 0xEB);
	// Memory::PatchNop(0x0094C3BB, 6);

#pragma endregion
				// 제작(메이커) 시스템 변경
#pragma region Maker Changes
 
	Memory::WriteByte(0x005CA1B8 + 2, 0x32); // 50 이상 스탯 아이템 보라색 표시
	Memory::WriteByte(0x005CA5FA + 2, 0x64); // 주문서 작 아이템 노란색 (+100 스탯)

	// Update Maker Categories - Gwen
	// Memory::CodeCave((void*)makerStuff, 0x008230FA, 0);
	// Memory::CodeCave((void*)Replace, 0x005C9F7A, 0);

	// 드롭다운 카테고리 그리기 생략(불필요한 메뉴 숨김)
	Memory::PatchNop(0x0082354F, 64); // 몬스터 기타 > 크리스탈
	// Memory::PatchNop(0x0082350F, 64); // Disassemble Equipment
	// Memory::PatchNop(0x00823818, 21); // ???

	// 모든 장비에 모든 촉진제 허용
	// Memory::WriteByte(0x0082660B, 0xEB);
	// Memory::WriteByte(0x0082660B + 1, 0x4D);

	// 메소 표시 위치 변경
	Memory::WriteByte(0x008251D3 + 1, 0xE1);

	// Expand the Maker dropdown menu to 20 - Pepa
	// Memory::WriteByte(0x008223BD + 6, 0x14);

	// Draw Functions - Pepa
	// Memory::WriteByte(0x008258DC + 2, 0x06); // Handle up to 6 Gem Slots for disabled icons
	// Memory::WriteByte(0x008258E8 + 2, 0xE); // Change Starting Draw Gem Slot Location
	// Memory::PatchNop(0x00824D1C, 31); // Skip Drawing Catalyst

	// SetItemSlotRect - Pepa
	// Memory::WriteByte(0x00822A1C + 1, 0xE); // Change Starting Set Gem Location
	// Memory::PatchNop(0x00822A67, 27); // Skip setting RecipeSlot
	// Memory::PatchNop(0x00822A82, 29); // Skip setting CatalystSlot
	// Memory::PatchNop(0x00822A54, 19); // Skip setting CatalystDropSlot
	// Memory::PatchNop(0x00823B10, 5); // Skip setting Catalyst
	// Memory::PatchNop(0x00823B17, 5); // Skip setting Recipe

#pragma endregion

	// 충전식 화살(화살을 충전 아이템으로 변경)
	RechargeArrows();

	// 자동배분 버튼 비활성화
	Memory::CodeCave(autoAssignRemove, 0x008C5920, 5);
	
	// 한손 무기 데미지 증가(한손 도끼 / 둔기 배율 3.2 → 3.6)
	unsigned char damageLocation3_6[] = { 0x68, 0xE8, 0xAF, 0x00 };
	Memory::WriteByteArray(0x0078F6B0 + 2, damageLocation3_6, 4); //CalcDamage
	Memory::WriteByteArray(0x008C2DFD + 2, damageLocation3_6, 4); //Character sheet

	// 양손 무기 데미지 증가(양손 도끼 / 둔기 배율 3.4 → 4.4)
	unsigned char damageLocation4_4[] = { 0x48, 0xE8, 0xAF, 0x00 };
	Memory::WriteByteArray(0x008C2C56 + 2, damageLocation4_4, 4); //Character sheet
	Memory::WriteByteArray(0x0078F24A + 2, damageLocation4_4, 4); //CalcDamage

	// 플레이어 스탯 제한 해제
	Memory::WriteByteArray(0x00AFE8A0, Uncap_Array, sizeof(Uncap_Array));
	Memory::WriteValue(0x008C3304 + 1, 2147483647);
	Memory::WriteByteArray(0x00780620 + 1, Uncap_Stat_Arr_1, sizeof(Uncap_Stat_Arr_1));
	Memory::WriteByteArray(0x0077E055 + 1, Uncap_Stat_Arr_1, sizeof(Uncap_Stat_Arr_1));
	Memory::WriteByteArray(0x0077E12F + 1, Uncap_Stat_Arr_1, sizeof(Uncap_Stat_Arr_1));
	Memory::WriteByteArray(0x0077E215 + 1, Uncap_Stat_Arr_1, sizeof(Uncap_Stat_Arr_1));
	Memory::WriteByteArray(0x0078FF5F + 1, Uncap_Stat_Arr_1, sizeof(Uncap_Stat_Arr_1));
	Memory::WriteByteArray(0x0079166C + 1, Uncap_Stat_Arr_1, sizeof(Uncap_Stat_Arr_1));
	Memory::WriteByteArray(0x00791CD5 + 1, Uncap_Stat_Arr_1, sizeof(Uncap_Stat_Arr_1));
	Memory::WriteByteArray(0x007806D0 + 1, Uncap_Stat_Arr_1, sizeof(Uncap_Stat_Arr_1)); // Accuracy
	Memory::WriteByteArray(0x00780702 + 1, Uncap_Stat_Arr_1, sizeof(Uncap_Stat_Arr_1)); // Avoidability

	// 몬스터 스탯 제한 해제
	Memory::WriteValue(0x0067DD1D + 1, 999999);
	Memory::WriteValue(0x00793499 + 1, 999999);
	Memory::WriteValue(0x00793107 + 1, 999999);
	Memory::WriteValue(0x007926DD + 1, 999999);
	Memory::WriteValue(0x0077E215 + 1, 999999);
	Memory::WriteValue(0x00780620 + 1, 999999);
	// Memory::WriteByte(0x007930C5, 0xEB); // Check for Physical DMG Miss (these essentially force the player to ALWAYS be hit)
	// Memory::WriteByte(0x00793484, 0xEB); // Check for Magic DMG Miss

	// 이동속도 제한 해제 - 최대 400
	Memory::WriteValue(0x00780746, 400);
	Memory::WriteValue(0x008c4287, 400);
	Memory::WriteValue(0x0094D91F, 400);

	// Mount Speed Cap Removal - Teto (untested)
	// Memory::WriteValue(0x0094D7EE + 1, 600); // CUserLocal::SetShoeAttr
	// Memory::WriteValue(0x008C41E1 + 1, 600); // CUIStatDetail::Draw
	// Memory::WriteValue(0x007AFDE7 + 1, 600); // CTamingMobTemplate::RegisterTamingMob

	// 전직 간 SP 체크 제거(SP 남아도 전직 가능)
	Memory::WriteByte(0x008AD01A, 0xE9);
	Memory::WriteValue(0x008AD01A + 1, 0x008AD227 - (0x008AD01A + 5));

	// 사다리/로프 등반 속도
	Memory::WriteValue(0x009CC6F9 + 2, 0x00C1CF80); // Switch Address
	Memory::WriteDouble(0x00C1CF80, 5.0); // Speed

	// 원거리 무기 근접 공격 제거
	Memory::WriteByte(0x009516C2, 0xE9);
	Memory::WriteByte(0x009516C2 + 1, 0xc8);
	Memory::WriteByte(0x009516C2 + 2, 0xfc);
	Memory::WriteByte(0x009516C2 + 3, 0xff);
	Memory::WriteByte(0x009516C2 + 4, 0xff);

	// 투비(스킬 딜레이 핵 (무적 관련)) ??
	Memory::FillBytes(0x00485C01, 0x90, 2);
	Memory::FillBytes(0x00485C21, 0x90, 2);
	Memory::FillBytes(0x00485C32, 0x90, 2);

#pragma endregion
				// 편의 기능
#pragma region AdventureMS Quality of Life

	// 비밀번호 길이 12자 이상 허용 (최대 20자)
	Memory::WriteByte(0x00620F43 + 2, 20); // make the packet sendable
	Memory::WriteInt(0x00620A21 + 3, 20); // input limit

	// 캐시템 창고에 넣기 가능
	Memory::PatchNop(0x007C6EF7, 6);
	Memory::PatchNop(0x007C6F03, 6);

	// 캐시템 바닥에 버리기 가능
	Memory::PatchNop(0x004F350C, 6);
	Memory::PatchNop(0x004F351E, 6);
	
	// 캐시템 교환 가능
	Memory::PatchNop(0x004F3FB8, 6);
	Memory::PatchNop(0x004F3FC4, 6);

	// 캐시템 NPC에게 판매 가능
	Memory::WriteByte(0x004F427A, 0xEB);
	Memory::WriteByte(0x004F427A + 1, 0x17);
	Memory::WriteByte(0x006FDA67, 0xEB);
	Memory::WriteByte(0x006FDA67 + 1, 0x12);
	Memory::WriteByte(0x006FDA72, 0xEB);

	// Remove call to check if there are cash items in the list, which prompts the birthday check (Put Cash Items in Shop) - Pepa
	Memory::WriteByte(0x006FCC33, 0xB8);
	Memory::WriteByte(0x006FCC33 + 1, 0x01);
	Memory::WriteByte(0x006FCC33 + 2, 0x00);
	Memory::WriteByte(0x006FCC33 + 3, 0x00);
	Memory::WriteByte(0x006FCC33 + 4, 0x00);

	// Remove Parens Formatting from Time Remaining Window (Put Cash Items in Shop) - Pepa
	Memory::WriteByte(0x00AF2450 + 2, 0x20);
	Memory::WriteByte(0x00AF2450 + 5, 0x00);

	// Remove Date / Time Check for Cash Item Sales - Pepa
	Memory::WriteByte(0x006FD63D, 0xEB);

	// Skip Sending Messages for Cash Items, Pass a Normal Sale Experience - Pepa
	Memory::WriteByte(0x006FD31F, 0x90);
	Memory::WriteByte(0x006FD31F + 1, 0xE9);
	
	// 2차 비밀번호 키보드 입력
	Memory::FillBytes(0x0076F7DE, 0x90, 6);
	
	// 항상 창모드 + 인트로 생략
	unsigned char forced_window[] = { 0xb8, 0x00, 0x00, 0x00, 0x00 };
	Memory::WriteByteArray(0x009F7A9B, forced_window, sizeof(forced_window));
	Memory::FillBytes(0x0062EE54, 0x90, 21);

	// 세계지도 화면 중앙에 표시
	wordMapX = (Client::m_nGameWidth - 666) / 2;
	wordMapY = (Client::m_nGameHeight - 524) / 2;
	Memory::CodeCave(wordMapUIcc, 0x009EB594, 13);

	// "아직 이 스킬을 사용할 수 없습니다" 제거(스킬 쿨타임 메시지 제거)
	Memory::PatchNop(0x00967707, 12);

	// "이 카드는 이미 가득 찼습니다" 제거(몬스터북 메시지 제거)
	Memory::PatchNop(0x00A08283, 18);

	// "숨을 돌려야 합니다" 제거(피로도 메시지 제거)
	Memory::WriteByte(0x00452316, 0x7C);

	// 모든 장비 성별 무관 착용
	Memory::PatchNop(0x00460AED, 2);

	// "너무 많이 채팅합니다" 제거(채팅 제한 메시지 제거)
	Memory::WriteByte(0x00490651, 0xEB);
	Memory::WriteByte(0x00490652, 0x1D);

	// "같은 말 반복" 제거(도배 방지 메시지 제거)
	Memory::WriteByte(0x00490607, 0xEB);
	Memory::WriteByte(0x00490609, 0x27);

	// 같은 말 3번 차단 제거(도배 차단 해제)
	Memory::WriteByte(0x004905EB, 0xEB);
	Memory::WriteByte(0x004CAA09, 0xEB);
	Memory::WriteByte(0x004CAA84, 0xEB);

	// 패밀리 시스템 제거
	unsigned char skip_family_draw[] = { 0xE9, 0x32, 0x01, 0x00, 0x00 };
	Memory::WriteByteArray(0x00A061A3, skip_family_draw, 5);
	Memory::WriteByte(0x008FFF70, 0xEB); // remove blue family button
	Memory::WriteByte(0x009970ED, 0xEB); // remove blue 'add junior' panel on right click
	Memory::WriteByte(0x00996BE5 + 3, 0x0D); // smaller bar
	Memory::WriteByte(0x00996D12 + 2, 0x08); // smaller bar floor

	// 제작(메이커) 대기시간 없앰
	Memory::WriteByte(0x826F92 + 2, 0x08);
	Memory::WriteByte(0x826F92 + 3, 0x01);
	Memory::WriteByte(0x826F92 + 4, 0x00);
	Memory::WriteByte(0x826F92 + 5, 0x00);

	// 펫 위치를 캐릭터 뒤로
	Memory::WriteByte(0x0070451B + 2, 1);

				// UI 확장
/*#pragma region Expanded UIs

	// Expanded Storage - Goose
	Memory::WriteByte(0x007C7C27 + 3, 0xC9); // Draw Icons in Trunk Inv
	Memory::WriteByte(0x007C82C3 + 3, 0xBF); // Draw Tooltips in Trunk Inv
	Memory::WriteByte(0x007C8035 + 3, 0xC9); // Draw Icons in Player inv
	Memory::WriteByte(0x007C8385 + 3, 0xBF); // Draw Tooltips in Player Inv
	Memory::WriteByte(0x007C65B6 + 1, 0xC6); // Storage Meso Button Y
	Memory::WriteByte(0x007C6631 + 1, 0xC6); // Player Meso Button 
	Memory::WriteByte(0x007C8197 + 1, 0xC8); // Player and Merchant Mesos Y offset

	// Merchant Scrollbar Length - Goose
	Memory::WriteByte(0x007C69DC + 1, 0x64);
	Memory::WriteByte(0x007C69DC + 2, 0x01);
	Memory::WriteByte(0x007C70B3 + 2, 0xF9); // Scrollbar Fix, ty Angel

	// Player Scrollbar Length - Goose
	Memory::WriteByte(0x007C6A3A + 1, 0x40);
	Memory::WriteByte(0x007C6A3A + 2, 0x01);
	Memory::WriteByte(0x007C7081 + 2, 0xF8); // Scrollbar Fix, ty Angel

	// Expanded Vendor - Goose
	Memory::WriteByte(0x00753DB8 + 1, 0x64);
	Memory::WriteByte(0x00753DB8 + 2, 0x01);
	Memory::WriteByte(0x00754719 + 2, 0xF8);
	Memory::WriteByte(0x00753E19 + 1, 0x64);
	Memory::WriteByte(0x00753E19 + 2, 0x01);
	Memory::WriteByte(0x0075474B + 2, 0xF8);
	Memory::WriteByte(0x00755748 + 3, 0xDE);
	Memory::WriteByte(0x00755E44 + 2, 0x09);
	Memory::WriteByte(0x007560D5 + 3, 0xDE);
	Memory::WriteByte(0x00756197 + 3, 0xDE);
	Memory::WriteByte(0x007540A3 + 1, 0xD8);
	Memory::WriteByte(0x007557D6, 0xB8);
	Memory::WriteInt(0x007557D6 + 1, (DWORD)&shop_btn_Array);
	Memory::WriteByte(0x007557D6 + 5, 0x90);
	Memory::WriteByte(0x00755A2A, 0xB8);
	Memory::WriteInt(0x00755A2A + 1, (DWORD)&shop_btn_Array);
	Memory::WriteByte(0x00755A2A + 5, 0x90);
	Memory::WriteByte(0x00755ACC, 0xB8);
	Memory::WriteInt(0x00755ACC + 1, (DWORD)&shop_btn_Array);
	Memory::WriteByte(0x00755ACC + 5, 0x90);
	Memory::WriteByte(0x00755B0B, 0xB8);
	Memory::WriteInt(0x00755B0B + 1, (DWORD)&shop_btn_Array);
	Memory::WriteByte(0x00755B0B + 5, 0x90);
	Memory::WriteByte(0x00755AB9, 0xB9);
	Memory::WriteInt(0x00755AB9 + 1, (DWORD)&shop_btn_Array);
	Memory::WriteByte(0x00755AB9 + 5, 0x90);

	// Expanded Skills - Goose
	Memory::WriteByte(0x008AA86F + 1, 0x73); // Extend rendering area to fit modified wz
    Memory::WriteByte(0x008AACD5 + 1, 0xF0); // Extends scrollbar
    Memory::WriteByte(0x008AAE23 + 1, 0x59); // Moves Macro button down
    
    Memory::WriteByte(0x008AD9F2 + 2, 0x4F); // Draw extra tooltips
    Memory::WriteByte(0x008ACE76 + 3, 0x66); // Draw Extra icons
    Memory::WriteByte(0x008AD7B4 + 2, 0xFB); // Scrollbar Fix
    Memory::WriteByte(0x008AC4DF + 1, 0x5B); // Skillpoints Y offset
    Memory::WriteByte(0x008AADAC + 3, 0x67); // Level-up buttons expanded
    Memory::WriteByte(0x008AB929 + 2, 0xE0); // Makes new buttons clickable
    
    Memory::WriteByte(0x008AD903 + 2, 0x06); // Increases buttons to be read in SetButton
    Memory::WriteByte(0x008AD7F8 + 2, 0x06); // Increases buttons to be read in SetButton

    //CUISkill::OnCreate - Goose
    Memory::WriteByte(0x008AAD3C + 1, 5); //lea eax
    Memory::WriteInt(0x008AAD3C + 2, (DWORD)&skillup_btn_Array + 12); 

    //CUISKill::SetButton - Goose
    Memory::WriteByte(0x008AD920 + 1, 0x34); // lea ecx, [eax*8..
    Memory::WriteByte(0x008AD920 + 2, 0xC5); // lea ecx, [eax*8..
    Memory::WriteInt(0x008AD920 + 3, (DWORD)&skillup_btn_Array + 12);

#pragma endregion

#pragma endregion
*/
				// 버그 수정
#pragma region AdventureMS Fixes

	// 파티 버튼이 "차단"으로 바뀌는 버그 수정(파티 UI 버그 수정)
	Memory::WriteByte(0x0091DCEC + 2, 0x18);
	Memory::WriteByte(0x0091DDE8 + 2, 0x18);

	// 아란 콤보/스킬 위치 수정(HD 해상도 대응 위치 조정)
	Memory::WriteInt(0x00960581 + 3, m_nGameWidth - 42);
	Memory::WriteInt(0x00960839 + 1, m_nGameWidth - 42);
	Memory::WriteInt(0x00960C67 + 1, m_nGameWidth - 10);
	Memory::WriteInt(0x00960DED + 1, m_nGameWidth - 10);

	// 파티퀘스트 클리어 메시지 위치 수정
	Memory::WriteInt(0x0053500F + 1, m_nGameWidth / 2); //CField::ShowScreenEffect

	// 캐릭터 선택 스탯 텍스트 이동(캐릭터 선택 화면 UI 정렬)
	Memory::WriteByte(0x00602D80 + 1, 0x38); // Job
	Memory::WriteByte(0x00602E25 + 1, 0x38); // LV.
	Memory::WriteByte(0x00602F7F + 1, 0x38); // STR
	Memory::WriteByte(0x006030D4 + 1, 0x38); // DEX
	Memory::WriteByte(0x00602ED0 + 1, 0x92); // FAME, INT, LUK
	Memory::WriteByte(0x00603437 + 1, 0x9B); // World Ranking Change
	Memory::WriteByte(0x006038B6 + 1, 0x9B); // Job Ranking Change
	// Memory::WriteByte(0x00603786 + 1, 0x69); // Job Ranking "Ranked at X"

	// 캐릭터 선택 서버명 제거(서버 이름 표시 제거)
	Memory::WriteByte(0x0060DB9B, 0x90);
	Memory::WriteByte(0x0060DB9B + 1, 0xE9);

	// 몬스터 마그넷 고정 적에게 적용(움직이지 않는 몬스터도 적용)
	Memory::FillBytes(0x0096C554, 0x90, 4);

	// 채팅창 텍스트 정렬(채팅 UI 정렬)
	// Memory::WriteByte(0x008DD05A + 2, 0x4);
	// Memory::WriteByte(0x008DD067 + 2, 0x2);
	Memory::CodeCave(chat_Y_offset_cave, 0x008DD6B9, 5);

	// 마우스 휠 버그 수정
	Memory::CodeCave(fixMouseWheelHook, fixMouseWheelAddr, 15);

	// 몬스터북 텍스트 정렬
	Memory::WriteByte(0x0086425B + 1, 82); // 5
	Memory::CodeCave(pos1, 0x00864378, 5); // 56
	Memory::CodeCave(pos2, 0x00864495, 5); // 89
	Memory::CodeCave(pos3, 0x008645B4, 5); // 108
	Memory::WriteInt(0x008646B8 + 1, 221); // 146

	// 길드 엠블럼 하단 라인 누락 수정(길드 마크 표시 버그 수정)
	Memory::WriteByte(0x005F12EF + 2, 0x05);

#pragma endregion

				// 미사용 코드
#pragma region AdventureMS Unused
	
	// AP 배분 버튼 전체 제거
	// Memory::PatchNop(0x008C4CB6, 5);
	
	// 장신구 슬롯 추가 (어깨) (어깨 장비 슬롯 추가)
	// Memory::WriteByte(0x005C973A + 2, 0x73);
	// Memory::WriteByte(0x004F1CA9 + 2, 0x34);
	// Memory::WriteByte(0x007FEFBC + 2, 0x33);
	// Memory::WriteByte(0x007FEE93 + 3, 0x34);
	// Memory::WriteByte(0x0077EDEF + 2, 0x34);
	// Memory::WriteByte(0x0077E48D + 3, 0x34);
	// Memory::WriteByte(0x0077EE07 + 2, 0x34);
	// Memory::PatchNop(0x007FEF39, 0x007FEF58 - 0x007FEF39);
	// Memory::CodeCave(putEquipPosition_Hook, 0x007FEF39, 5);
	
	// 데미지 / 공격속도 제한(데미지 상한 설정)
	// Memory::WriteDouble(0x00AF8298, 5); // Double for DMG Calculations
	// Memory::CodeCave(setAttSpdCap, 0x00765080, 1); // AttackSpeed0Cap // Needs Work

	// 퀵슬롯 버튼 제거 = 단축키 버튼 제거
	// Memory::PatchNop(0x008D3598, 3);
	// Memory::WriteInt(0x008D3274, 0x288);
	// Memory::WriteInt(0x008D32FB, 0x2A6);
	// Memory::WriteInt(0x008D3382, 0x2C4);
	// Memory::WriteInt(0x008D3409, 0x2E2);
	// Memory::WriteInt(0x008D3490, 0x300);

	// 월드맵 제한 증가(월드맵 확장)
	// Memory::WriteByte(0x009EA030, 0x81);
	// Memory::WriteByte(0x009EA031, 0xFE);
	// Memory::WriteByte(0x009EA032, 0xB4);

	// 헤어 ID 수정(헤어스타일 ID 버그 수정) << ???
	// Memory::WriteByte(0x005C94FC + 2, 7);
	// Memory::WriteByte(0x005C94FF + 1, 0x8E);

	// 헤어/얼굴 제한 해제
	// Memory::CodeCave(faceHairCave, 0x005C94F3, 18); // High Version Hair & Face 1
	// Memory::CodeCave(faceHairCave2, 0x009ACA9B, 5); // High Version Hair & Face 2
	// Memory::CodeCave(faceHairCave3, 0x009ACAA6, 6); // High Version Hair & Face 3
	// Memory::CodeCave(HairFaceIdUncap1, dwHairFaceUncap1, 18);
	// Memory::CodeCave(HairFaceIdUncap2, dwHairFaceUncap2, 18);

	// 망토 (캐시) 이펙트 ID 확장(망토 효과 ID 범위 확장)
	// Memory::WriteValue(0x0093C144  + 1, 0x2710); // mov ecx, 10000
	// Memory::WriteValue(0x0093C14F  + 1, 0x1F5); // cmp eax, 501
	// Memory::WriteValue(0x0093C67E  + 1, 0x2710); // mov ecx, 10000
	// Memory::WriteValue(0x0093C689  + 1, 0x1F5); // cmp eax, 501
	// Memory::WriteValue(0x0095B112 + 1, 0x2710); // mov ecx, 10000
	// Memory::WriteValue(0x0095B11F + 1, 0x1F5); // cmp eax, 501
	// Memory::CodeCave(cash_effect_expansion_1, dwCashEffExpansion1, dwCashEffExpansion1NOPs);
	// Memory::CodeCave(cash_effect_expansion_2, dwCashEffExpansion2, dwCashEffExpansion2NOPs);

#pragma endregion

}
