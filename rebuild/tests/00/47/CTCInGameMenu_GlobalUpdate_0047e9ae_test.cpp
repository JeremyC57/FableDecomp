// TEST: CTCInGameMenu::GlobalUpdate(bool) -- per-frame menu work: queues a pending load-game event, counts down a deferred save, and drives the "video options changed" dialog's 15-second revert timeout
// Behaviour test for 0x0047E9AE: the row source below (pragmas dropped, unnamed filler virtual slots given empty bodies),
// then recording stubs for every callee and a main that drives each path. C++03; builds with VC7.1 /Od and g++.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef _MSC_VER
#define __fastcall
#endif
#define FABLE_FORCEINLINE inline
#ifdef _MSC_VER
#define TEST_WSTR_NL L"\n"
#define TEST_WSTR_SP L" "
#else
static const unsigned short TEST_WSTR_NL[] = { 10, 0 };
static const unsigned short TEST_WSTR_SP[] = { 32, 0 };
#endif
// CTCInGameMenu::GlobalUpdate(bool) -- per-frame menu work: queues a pending load-game event, counts down a deferred save, and drives the "video options changed" dialog's 15-second revert timeout
// Leaf: ?GlobalUpdate@CTCInGameMenu@@UAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047E9AE, 860 bytes; frame-pointer size TU (`optimize("y", off)` + `("s", on)`), /GS cookie.
// Shape: the X centre term is written `(GetBackBufferWidth() * 0.5f) - scale.X * K`; the redundant parentheses add a no-code PAREN
//   tuple that moves the float-argument slot `push ecx` below the X `fsubp`, as retail (the unparenthesised form pushes it first).
// Flags: /GS

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                   // 0x0099EAE0
};
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString(const CWideString& other);            // 0x0099B720
    ~CWideString();                                   // 0x0099B510
    CWideString& operator+=(const unsigned short* text);        // 0x0099B940
    const CWideString& operator+=(const CWideString& other);    // 0x0099B8D0
};
CWideString __fastcall operator+(const unsigned short* left, const CWideString& right);   // 0x0099BFF0
CWideString __fastcall GFIntToWideString(long value);   // 0x00405F00
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
namespace NUISystem {
class CUIScreenUnitConverter { public:                // Ego_r static __fastcall getters
    static float __fastcall GetBackBufferWidth();     // 0x0041CC14
    static float __fastcall GetBackBufferHeight();    // 0x0041CC2B
};
}
double __fastcall GFGetTime();                        // 0x009E1BC0

class CGameEvent { public:
    long          Type;                              // +0x00
    signed char   Player;                            // +0x04
    unsigned char Data[32];                          // +0x05
    unsigned char EndPos;                            // +0x25
    bool          Valid;                             // +0x26
    bool          Replacement;                       // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddData(unsigned long length, const void* data);
    void AddSLONG(long value) { AddData(sizeof(value), &value); }
    void SetPlayer(long player) { Player = (signed char)player; }
};
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                          // +0x14
    void AddGameEvent(const CGameEvent& event);      // 0x0041649C
    const CDataBank& GetTextBank() const { return *TextBank; }
    static CMainGameComponent* __fastcall Get();
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0
inline CMainGameComponent* __fastcall CMainGameComponent::Get() { return g_pMainGameComponent; }
enum { TCI_INVENTORY = 17 };

class CTCBase;
struct CPresenceBits { bool test(long kind); };             // bitset<>::test 0x00410DE0
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CPresenceBits present;                                  // +0x20 std::bitset of interface types
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                             // +0x44
    bool IsDead() const;                                    // CThing::IsDead (called only by the out-of-line IS_THING_ALIVE body)
    template<class T> FABLE_FORCEINLINE bool GetTC(long interfaceType, T** ppOut)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
// IS_THING_ALIVE<CThingPlayerCreature*> 0x0043DBD0 (Ego_r ??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z): the
//   shared engine/ThingAlive.h body, visible but out of line (FABLE_RETAIL_IS_THING_ALIVE_NOINLINE) as in the retail TU, so VC7.1
//   keeps the hero in edi across the call (0x0047E9E6).
template<class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // test: defined with the stubs
class CThingCreatureBase : public CThing { public:
    unsigned char _pad_0x4c[0x90 - 0x4C];
    signed char   PlayerNumber;                      // +0x90
    unsigned long GetPlayerNumber() const { return PlayerNumber; }
};
class CThingPlayerCreature : public CThingCreatureBase { public: };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0 (Ego_r ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ)
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                        // 0x00449970 (Ego_r ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ)
};
class CTCInventory { public:
    void RevertVideoChanges();                              // 0x00582754
};
class CUserProfileManager { public:
    unsigned char _pad00[0x34];
    bool          DisplayDialogOnOptionsChanged;            // +0x34
    void SetDisplayDialogOnOptionsChanged(bool display) { DisplayDialogOnOptionsChanged = display; }
    void Apply();                                           // 0x0040BC80
    void Save();                                            // 0x0040A740
    static CUserProfileManager& __fastcall Get();           // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
};

namespace NUISystem {
#define V10(n) virtual void n##0() {} virtual void n##1() {} virtual void n##2() {} virtual void n##3() {} virtual void n##4() {} \
    virtual void n##5() {} virtual void n##6() {} virtual void n##7() {} virtual void n##8() {} virtual void n##9() {}
struct CUIInterface {
    virtual void v00() {} virtual void v01() {} virtual void v02() {}
    virtual class CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {}
    V10(v1) V10(v2)
    virtual void v30() {}
    virtual void ChangePosition(const C2DVector& pos, float time, long mode);   // slot 31 (+0x7C)
    virtual void v32() {} virtual void v33() {} virtual void v34() {} virtual void v35() {} virtual void v36() {} virtual void v37() {}
    virtual void v38() {} virtual void v39() {}
    V10(v4) V10(v5) V10(v6) V10(v7)
    virtual void v80() {} virtual void v81() {} virtual void v82() {} virtual void v83() {} virtual void v84() {} virtual void v85() {} virtual void v86() {}
    virtual long GetTargetStateIndex();                          // slot 87 (+0x15C)
    virtual void v88() {} virtual void v89() {}
    V10(v9) V10(w0) V10(w1) V10(w2) V10(w3)
    virtual void w40() {} virtual void w41() {} virtual void w42() {} virtual void w43() {}
    virtual void SetValue(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
    virtual void SetValue(const CCharString& text, bool a, bool b);   // slot 145 (+0x244)
};
class CComponent : public CUIInterface { public:
};
class CYesNo : public CComponent { public:
};
class CText : public CComponent { public:
};
class CManager { public:
    C2DVector GetUIScale() const;                           // 0x0041CF47
    static CManager* __fastcall GetInstance();        // 0x0041E5F2
};
}

class CTCBase { public:                             // the thing-component base: vptr +0x00 (4-byte aligned base sub-object)
protected:
    CPlayerManager& GetPlayerManager();               // 0x00686D40 (Ego_r ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ)
public:
    virtual void GlobalUpdate(bool);                  // Ego_r ?GlobalUpdate@CTCInGameMenu@@UAEX_N@Z overrides it
};
class CTCInGameMenu : public CTCBase { public:
    CThing*                ParentThing;               // +0x04 (CTCBase)
    unsigned char          _pad_0x08[0x04];
    bool                   InGameMenuOpen;            // +0x0C
    unsigned char          _pad_0x0d[0x57];
    long                   LoadGameIndex;             // +0x64 (PDB +0x78)
    unsigned char          _pad_0x68[0x48];
    NUISystem::CYesNo*     PVideoOptionsNotAcceptedDialog;   // +0xB0
    NUISystem::CYesNo*     PVideoOptionsChangedDialog;       // +0xB4
    unsigned char          _pad_0xb8[0x08];
    double                 VideoChangesTimer;         // +0xC0
    long                   SaveGameIndex;             // +0xC8 (PDB +0xE8)
    long                   FramesToWaitToSave;        // +0xCC
    void SaveGame(long index);                        // 0x0047E196
    virtual void GlobalUpdate(bool);
};




CWideString __fastcall GFIntToWideString(long value);                // 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z

using namespace NUISystem;

void CTCInGameMenu::GlobalUpdate(bool)
{
    if (!InGameMenuOpen)
        return;
    if (LoadGameIndex != -1) {
        CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (IS_THING_ALIVE(hero)) {
            CGameEvent event(60);                                    // the load-game event (EGameEventType 60; no enumerator dump)
            event.AddSLONG(LoadGameIndex);
            event.SetPlayer(hero->GetPlayerNumber());
            CMainGameComponent::Get()->AddGameEvent(event);
        }
        LoadGameIndex = -1;
    }
    if (SaveGameIndex != -1) {
        if (FramesToWaitToSave > 0) {
            --FramesToWaitToSave;
        } else {
            SaveGame(SaveGameIndex);
            SaveGameIndex = -1;
            FramesToWaitToSave = -1;
        }
    }
    if (PVideoOptionsChangedDialog && PVideoOptionsChangedDialog->GetTargetStateIndex() == 1 && VideoChangesTimer != -1.0) {
        if (GFGetTime() - VideoChangesTimer > 15.0) {
            CThing* parent = ParentThing;
            CTCInventory* inventory;
            if (parent->GetTC(TCI_INVENTORY, &inventory) && inventory)
                inventory->RevertVideoChanges();
            CUserProfileManager::Get().SetDisplayDialogOnOptionsChanged(false);
            CUserProfileManager::Get().Apply();
            CUserProfileManager::Get().Save();
            CUserProfileManager::Get().SetDisplayDialogOnOptionsChanged(true);
            CText* text = static_cast<CText*>(PVideoOptionsChangedDialog->FindChild("PC_GAME_OPTIONS_CHANGED_TEXT"));
            if (text)
                text->SetValue("TEXT_GUI_OPTIONS_RESET_TIMEOUT", true, true);
            C2DVector pos((CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f) - CManager::GetInstance()->GetUIScale().X * 204.0f,
                          CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - CManager::GetInstance()->GetUIScale().Y * 52.0f);
            PVideoOptionsChangedDialog->ChangePosition(pos, 0.0f, false);
            VideoChangesTimer = -1.0;
        } else {
            CText* text = static_cast<CText*>(PVideoOptionsChangedDialog->FindChild("PC_GAME_OPTIONS_CHANGED_TEXT"));
            if (text) {
                CWideString message = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_OPTIONS_CHANGED_SUCCESSFULLY");
                message += TEST_WSTR_NL;
                message += CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_OPTIONS_REVERTING_IN");
                message += TEST_WSTR_SP + GFIntToWideString((long)(15.0 - (GFGetTime() - VideoChangesTimer)));
                text->SetValue(message, true, true);
            }
        }
    }
}

// ---------------- test stubs ----------------
static char g_log[4096];
static int g_bad, g_live;
static char g_buf[512];
static void Log(const char* s) { if (strlen(g_log) + strlen(s) < sizeof(g_log) - 1) strcat(g_log, s); else g_bad = 1; }
class CCharStringData { public: char Text[80]; };
CCharString::CCharString(const char* text, long length) { if (length != -1) g_bad = 1; PStringData = new CCharStringData; strncpy(PStringData->Text, text, 79); PStringData->Text[79] = 0; ++g_live; }
CCharString::CCharString(const CCharString& other) { PStringData = new CCharStringData(*other.PStringData); ++g_live; }
CCharString::~CCharString() { delete PStringData; --g_live; }
class CWideStringData { public: char Text[256]; };       // the test keeps the wide text as narrow characters
static CWideString MakeWide(const char* text)
{
    CWideString* raw = (CWideString*)malloc(sizeof(CWideString));
    raw->PStringData = new CWideStringData; strcpy(raw->PStringData->Text, text); ++g_live;
    CWideString copy(*raw);
    raw->~CWideString(); free(raw);
    return copy;
}
static void AppendNarrow(char* to, const unsigned short* text) { size_t n = strlen(to); while (*text && n < 250) to[n++] = (char)*text++; to[n] = 0; }
CWideString::CWideString(const CWideString& other) { PStringData = new CWideStringData(*other.PStringData); ++g_live; }
CWideString::~CWideString() { delete PStringData; --g_live; }
CWideString& CWideString::operator+=(const unsigned short* text) { AppendNarrow(PStringData->Text, text); return *this; }
const CWideString& CWideString::operator+=(const CWideString& other) { strcat(PStringData->Text, other.PStringData->Text); return *this; }
CWideString __fastcall operator+(const unsigned short* left, const CWideString& right)
{
    char text[256] = ""; AppendNarrow(text, left); strcat(text, right.PStringData->Text); return MakeWide(text);
}
CWideString __fastcall GFIntToWideString(long value) { char text[32]; sprintf(text, "%ld", value); return MakeWide(text); }
CWideString CDataBank::GetTextBySymbol(const CCharString& symbol) const { char text[128]; sprintf(text, "<%s>", symbol.PStringData->Text); return MakeWide(text); }
static float g_screenW = 800.0f, g_screenH = 600.0f;
float __fastcall NUISystem::CUIScreenUnitConverter::GetBackBufferWidth() { return g_screenW; }
float __fastcall NUISystem::CUIScreenUnitConverter::GetBackBufferHeight() { return g_screenH; }
static double g_time = 100.0;
double __fastcall GFGetTime() { return g_time; }
void CGameEvent::AddData(unsigned long length, const void* data) { long v; memcpy(&v, data, sizeof(long)); sprintf(g_buf, "Copy(%lu,%ld);", length, v); Log(g_buf); }
void CMainGameComponent::AddGameEvent(const CGameEvent& event) { sprintf(g_buf, "Event(%ld,%d);", event.Type, (int)event.Player); Log(g_buf); }
static CDataBank g_bank;
static CMainGameComponent g_mainGame;
CMainGameComponent* g_pMainGameComponent = &g_mainGame;
static bool g_hasInventory = true;
bool CPresenceBits::test(long kind) { return kind == 0x11 && g_hasInventory; }
static CTCInventory g_inventory;
static CInterfaceMap::SEntry g_entries[2] = { { 0x11, (CTCBase*)(void*)&g_inventory }, { 0x7f, 0 } };
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return *type <= 0x11 ? first : first + 1; }
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing) { if (thing != 0) { if (!thing->IsDead()) return true; } return false; }
static bool g_heroDead = false;
bool CThing::IsDead() const { return g_heroDead; }
static CThingPlayerCreature g_hero;
static CThingPlayerCreature* g_pHero = &g_hero;
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { return g_pHero; }
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
static CPlayerManager g_players;
CPlayerManager& CTCBase::GetPlayerManager() { return g_players; }
void CTCInGameMenu::SaveGame(long index) { sprintf(g_buf, "Save(%ld);", index); Log(g_buf); }
void CTCInventory::RevertVideoChanges() { Log("Revert;"); }
static CUserProfileManager g_profile;
CUserProfileManager& __fastcall CUserProfileManager::Get() { return g_profile; }
void CUserProfileManager::Apply() { sprintf(g_buf, "Apply(%d);", (int)DisplayDialogOnOptionsChanged); Log(g_buf); }
void CUserProfileManager::Save() { sprintf(g_buf, "ProfileSave(%d);", (int)DisplayDialogOnOptionsChanged); Log(g_buf); }
static NUISystem::CYesNo g_dialog, g_text;
static NUISystem::CComponent* g_child = &g_text;
static long g_state = 1;
namespace NUISystem {
CComponent* CUIInterface::FindChild(CCharString name) { sprintf(g_buf, "Find(%s);", name.PStringData->Text); Log(g_buf); return g_child; }
void CUIInterface::ChangePosition(const C2DVector& pos, float time, long mode) { sprintf(g_buf, "Pos(%g,%g,%g,%ld);", pos.X, pos.Y, time, mode); Log(g_buf); }
long CUIInterface::GetTargetStateIndex() { return g_state; }
void CUIInterface::SetValue(const CWideString& text, bool a, bool b) { sprintf(g_buf, "Wide(%s,%d,%d);", text.PStringData->Text, (int)a, (int)b); Log(g_buf); }
void CUIInterface::SetValue(const CCharString& text, bool a, bool b) { sprintf(g_buf, "Text(%s,%d,%d);", text.PStringData->Text, (int)a, (int)b); Log(g_buf); }
static CManager g_manager;
CManager* __fastcall CManager::GetInstance() { return &g_manager; }
C2DVector CManager::GetUIScale() const { return C2DVector(2.0f, 0.5f); }
}
void CTCBase::GlobalUpdate(bool) { }
static bool Check(const char* what, const char* expected)
{
    bool ok = strcmp(g_log, expected) == 0 && !g_bad && g_live == 0;
    if (!ok) printf("%s\n  got      %s\n  expected %s\n", what, g_log, expected);
    g_log[0] = 0;
    return ok;
}
int main()
{
    bool ok = true;
    g_mainGame.TextBank = &g_bank;
    g_hero.PlayerNumber = 2;
    static CThingCreatureBase parent;
    parent.m_interfaces.first = g_entries; parent.m_interfaces.last = g_entries + 2;
    CTCInGameMenu menu;
    menu.ParentThing = &parent;
    menu.InGameMenuOpen = false;
    menu.LoadGameIndex = 3; menu.SaveGameIndex = 1; menu.FramesToWaitToSave = 0;
    menu.PVideoOptionsNotAcceptedDialog = 0; menu.PVideoOptionsChangedDialog = &g_dialog; menu.VideoChangesTimer = 90.0;
    menu.GlobalUpdate(false);                           // menu closed: nothing happens
    ok = Check("closed", "") && ok;
    menu.InGameMenuOpen = true; g_time = 100.0;         // pending load + save; 10 s into the revert countdown
    menu.GlobalUpdate(false);
    char expected[256];                                 // AddSLONG copies sizeof(long) bytes (4 on the retail target)
    sprintf(expected, "Copy(%u,3);Event(60,2);Save(1);Find(PC_GAME_OPTIONS_CHANGED_TEXT);"
        "Wide(<TEXT_GUI_OPTIONS_CHANGED_SUCCESSFULLY>\n<TEXT_GUI_OPTIONS_REVERTING_IN> 5,1,1);", (unsigned)sizeof(long));
    ok = Check("countdown", expected) && ok;
    ok = ok && menu.LoadGameIndex == -1 && menu.SaveGameIndex == -1 && menu.FramesToWaitToSave == -1 && menu.VideoChangesTimer == 90.0;
    menu.SaveGameIndex = 4; menu.FramesToWaitToSave = 2; g_time = 106.0;   // waiting to save; past 15 s: revert
    menu.GlobalUpdate(false);
    ok = Check("revert", "Revert;Apply(0);ProfileSave(0);Find(PC_GAME_OPTIONS_CHANGED_TEXT);Text(TEXT_GUI_OPTIONS_RESET_TIMEOUT,1,1);"
        "Pos(-8,274,0,0);") && ok;
    ok = ok && menu.FramesToWaitToSave == 1 && menu.SaveGameIndex == 4 && menu.VideoChangesTimer == -1.0 && g_profile.DisplayDialogOnOptionsChanged;
    menu.GlobalUpdate(false);                           // timer stopped: only the save countdown
    ok = Check("idle", "") && ok;
    ok = ok && menu.FramesToWaitToSave == 0;
    g_pHero = 0; menu.LoadGameIndex = 5; menu.VideoChangesTimer = 200.0; g_time = 300.0; g_hasInventory = false; g_child = 0;
    menu.GlobalUpdate(false);                           // no hero: no load event; revert with no inventory and no text child
    ok = Check("no hero", "Save(4);Apply(0);ProfileSave(0);Find(PC_GAME_OPTIONS_CHANGED_TEXT);Pos(-8,274,0,0);") && ok;
    ok = ok && menu.LoadGameIndex == -1;
    g_pHero = &g_hero; g_heroDead = true; menu.LoadGameIndex = 6;
    menu.GlobalUpdate(false);                           // dead hero: the pending load is dropped without an event
    ok = Check("dead hero", "") && ok;
    ok = ok && menu.LoadGameIndex == -1;
    puts(ok ? "0047e9ae_TEST PASS" : "0047e9ae_TEST FAIL");
    return ok ? 0 : 1;
}