// CTCInGameMenu::GlobalUpdate(bool) -- per-frame menu work: queues a pending load-game event, counts down a deferred save, and drives the "video options changed" dialog's 15-second revert timeout
// Grade: DIFFER(860v860) -- whole body matches except the SetPosition push ecx/fsubp scheduling swap shared with the advisor rows 0047e651/0047f30f/00576c4f/00576ed1 (retail pushes the float arg slot after the X fsubp); 4 grades
// Flags: /GS
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                   // 0x0099EAE0
};
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CWideString(const CWideString& other);            // 0x0099B720
    ~CWideString();                                   // 0x0099B510
    CWideString& operator+=(const unsigned short* text);        // 0x0099B940
    const CWideString& operator+=(const CWideString& other);    // 0x0099B8D0
};
CWideString __fastcall operator+(const unsigned short* left, const CWideString& right);   // 0x0099BFF0
CWideString __fastcall IntToWideString(long value);   // 0x00405F00
struct CDataBank {
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
float GetScreenWidth();                               // 0x0041CC14
float GetScreenHeight();                              // 0x0041CC2B
double __fastcall GFGetTime();                        // 0x009E1BC0

struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                              // +0x00
    signed char   Player;                            // +0x04
    unsigned char Data[32];                          // +0x05
    unsigned char EndPos;                            // +0x25
    bool          Valid;                             // +0x26
    bool          Replacement;                       // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { Copy(sizeof(value), &value); }
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                          // +0x14
    void AddGameEvent(const CGameEvent& event);      // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;     // 0x013B86A0
inline CDataBank* GetTextBank() { return g_pMainGameComponent->TextBank; }

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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBD0 (CThing*)
struct CThingCreatureBase : CThing {
    unsigned char _pad_0x4c[0x90 - 0x4C];
    signed char   PlayerNumber;                      // +0x90
};
struct CPlayer {
    CThingCreatureBase* GetCharacterThing();         // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                        // 0x00449970
};
struct CTCInventory {
    void RevertVideoChanges();                              // 0x00582754
};
struct CUserProfileManager {
    unsigned char _pad00[0x34];
    bool          DisplayDialogOnOptionsChanged;            // +0x34
    void Apply();                                           // 0x0040BC80
    void Save();                                            // 0x0040A740
    static CUserProfileManager& __fastcall Get();           // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
};

namespace NUISystem {
#define V10(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3(); virtual void n##4(); \
    virtual void n##5(); virtual void n##6(); virtual void n##7(); virtual void n##8(); virtual void n##9();
struct CComponentBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual struct CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    V10(v1) V10(v2)
    virtual void v30();
    virtual void SetPosition(const C2DVector& pos, float time, long mode);   // slot 31 (+0x7C)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39();
    V10(v4) V10(v5) V10(v6) V10(v7)
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84(); virtual void v85(); virtual void v86();
    virtual long GetState();                          // slot 87 (+0x15C)
    virtual void v88(); virtual void v89();
    V10(v9) V10(w0) V10(w1) V10(w2) V10(w3)
    virtual void w40(); virtual void w41(); virtual void w42(); virtual void w43();
    virtual void SetWideText(const CWideString& text, bool a, bool b);   // slot 144 (+0x240)
    virtual void SetText(const CCharString& text, bool a, bool b);   // slot 145 (+0x244)
};
struct CComponent : CComponentBase {
};
struct CYesNo : CComponent {
};
struct CManager {
    C2DVector GetUIScale();                           // 0x0041CF47
    static CManager* __fastcall GetInstance();        // 0x0041E5F2
};
}

struct CTCInGameMenu {
    void*                  __vftable;                 // +0x00
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
    CPlayerManager* GetPlayerManager();               // 0x00686D40 (CTCBase)
    void SaveGame(long index);                        // 0x0047E196
    void GlobalUpdate(bool);
};

enum { GAME_EVENT_LOAD_GAME = 0x3C };

void CTCInGameMenu::GlobalUpdate(bool)
{
    if (!InGameMenuOpen)
        return;
    if (LoadGameIndex != -1) {
        CThingCreatureBase* const& hero = GetPlayerManager()->GetMainPlayer()->GetCharacterThing();
        if (IS_THING_ALIVE<CThing*>(hero)) {
            CGameEvent event(GAME_EVENT_LOAD_GAME);
            event.AddLong(LoadGameIndex);
            event.Player = hero->PlayerNumber;
            g_pMainGameComponent->AddGameEvent(event);
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
    if (PVideoOptionsChangedDialog && PVideoOptionsChangedDialog->GetState() == 1 && VideoChangesTimer != -1.0) {
        if (GFGetTime() - VideoChangesTimer > 15.0) {
            CThing* parent = ParentThing;
            CTCInventory* inventory;
            if (parent->GetInterface(&inventory, 0x11) && inventory)
                inventory->RevertVideoChanges();
            CUserProfileManager::Get().DisplayDialogOnOptionsChanged = false;
            CUserProfileManager::Get().Apply();
            CUserProfileManager::Get().Save();
            CUserProfileManager::Get().DisplayDialogOnOptionsChanged = true;
            NUISystem::CComponent* text = PVideoOptionsChangedDialog->FindChild(CCharString("PC_GAME_OPTIONS_CHANGED_TEXT", -1));
            if (text)
                text->SetText("TEXT_GUI_OPTIONS_RESET_TIMEOUT", true, true);
            C2DVector pos(GetScreenWidth() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().X * 204.0f,
                          GetScreenHeight() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().Y * 52.0f);
            PVideoOptionsChangedDialog->SetPosition(pos, 0.0f, 0);
            VideoChangesTimer = -1.0;
        } else {
            NUISystem::CComponent* text = PVideoOptionsChangedDialog->FindChild(CCharString("PC_GAME_OPTIONS_CHANGED_TEXT", -1));
            if (text) {
                CWideString message = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_OPTIONS_CHANGED_SUCCESSFULLY");
                message += L"\n";
                message += GetTextBank()->GetTextBySymbol("TEXT_GUI_OPTIONS_REVERTING_IN");
                message += L" " + IntToWideString((long)(15.0 - (GFGetTime() - VideoChangesTimer)));
                text->SetWideText(message, true, true);
            }
        }
    }
}
