#pragma once
class Client
{
public:
#pragma region Ezorsia Prepackaged

	static void UpdateGameStartup();
	static void UpdateResolution();
	static void UpdateLogin();
	static int m_nGameHeight;
	static int m_nGameWidth;
	static int MsgAmount;
	static std::string dFont;
	static std::string ServerIP_Address;

#pragma endregion

#pragma region AdventureMS Custom

	static void AdventureMS();

#pragma endregion
};