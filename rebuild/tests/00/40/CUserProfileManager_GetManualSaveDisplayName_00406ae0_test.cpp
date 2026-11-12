// Behaviour test for CUserProfileManager::GetManualSaveDisplayName (0x00406AE0): bank text + L" " + slot number from the main game
// component's bank, else the front end's, else the slot's file name.
#include <cstdio>
#include <string>
#include <cstring>
typedef std::basic_string<wchar_t> WStr;
struct CDataBank { WStr Text; int Lookups; WStr GetTextBySymbol(const std::string& symbol) { ++Lookups; return symbol == "TEXT_GUI_SAVE_MANUAL_SAVE_NAME" ? Text : WStr(L"?"); } };
class CMainGameComponent { public: CDataBank* TextBank; };
class CNewFrontendGameComponent { public: CDataBank* TextBank; };
static CMainGameComponent* PMainGameComponent = 0;
static CNewFrontendGameComponent* m_pNewFrontendGameComponent = 0;
static WStr GFIntToWideString(long value)
{
    wchar_t buffer[32]; std::swprintf(buffer, L"%d", (int)value);
    return WStr(buffer);
}
class CUserProfileManager { public:
    WStr GetManualSaveFileName(long slot) const { return WStr(L"ManualSave") + GFIntToWideString(slot); }
    WStr GetManualSaveDisplayName(long slot) const
    {
        if (const CMainGameComponent* game = PMainGameComponent)
            return game->TextBank->GetTextBySymbol("TEXT_GUI_SAVE_MANUAL_SAVE_NAME") + L" " + GFIntToWideString(slot);
        if (const CNewFrontendGameComponent* frontEnd = m_pNewFrontendGameComponent)
            return frontEnd->TextBank->GetTextBySymbol("TEXT_GUI_SAVE_MANUAL_SAVE_NAME") + L" " + GFIntToWideString(slot);
        return GetManualSaveFileName(slot);
    }
};
int main()
{
    CUserProfileManager manager;
    CDataBank gameBank; gameBank.Text = L"Save"; gameBank.Lookups = 0;
    CDataBank frontBank; frontBank.Text = L"Sauvegarde"; frontBank.Lookups = 0;
    CMainGameComponent game = { &gameBank };
    CNewFrontendGameComponent frontEnd = { &frontBank };
    bool ok = true;
    ok = ok && manager.GetManualSaveDisplayName(3) == L"ManualSave3";          // neither component: the file name
    m_pNewFrontendGameComponent = &frontEnd;
    ok = ok && manager.GetManualSaveDisplayName(12) == L"Sauvegarde 12";        // front end only
    ok = ok && frontBank.Lookups == 1 && gameBank.Lookups == 0;
    PMainGameComponent = &game;
    ok = ok && manager.GetManualSaveDisplayName(-7) == L"Save -7";              // the game's bank wins
    ok = ok && frontBank.Lookups == 1 && gameBank.Lookups == 1;
    m_pNewFrontendGameComponent = 0;
    ok = ok && manager.GetManualSaveDisplayName(0) == L"Save 0";
    if (!ok) { std::printf("00406ae0_TEST FAIL\n"); return 1; }
    std::printf("00406ae0_TEST PASS\n");
    return 0;
}