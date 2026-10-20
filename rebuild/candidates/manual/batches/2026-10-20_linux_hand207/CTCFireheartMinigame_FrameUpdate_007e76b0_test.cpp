#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
class CThing;
class CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSubtitleRenderer {
public:
    unsigned char _pad00[0xde];
    bool InCutscene;                                // +0xDE
    unsigned char _padDF[0xe5 - 0xdf];
    bool DisplayingText;                            // +0xE5
    void SetButton2OverrideText(const CCharString& text);   // 0x0048F881
};

class CPlayerGui {
public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;             // +0x18
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

class CTCScriptedControl {                          // interface 0x1F
public:
    bool IsActionListEmpty() const;                 // 0x007122C0
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline CTCScriptedControl* GetScriptedControl()
    {
        CTCScriptedControl* control;
        if (HasTC(0x1f))
            control = (CTCScriptedControl*)GetTC(0x1f);
        return control;
    }
};

class CTCFireheartMinigame {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x4c - 0x08];
    bool InSession;                                 // +0x4C
    bool ForceQuit;                                 // +0x4D
    unsigned char _pad4e[0x70 - 0x4e];
    bool Moving;                                    // +0x70
    void FrameUpdate();
};

void CTCFireheartMinigame::FrameUpdate()
{
    if (InSession) {
        if (!g_pPlayerGui->SubtitleRenderer.DisplayingText && !g_pPlayerGui->SubtitleRenderer.InCutscene) {
            CCharString text("TEXT_GUI_FISHING_STOP", -1);
            g_pPlayerGui->SubtitleRenderer.SetButton2OverrideText(text);
        }
        if (Moving) {
            if (ParentThing->GetScriptedControl()->IsActionListEmpty())
                Moving = false;
        }
    }
}

// ---- test stubs for the retail callees ----
static const char* g_text; static int g_sets; static bool g_empty;
CCharString::CCharString(const char* text, long) : PStringData((void*)text) {}
CCharString::~CCharString() {}
void CSubtitleRenderer::SetButton2OverrideText(const CCharString& t) { g_text = (const char*)t.PStringData; ++g_sets; }
static CPlayerGui g_gui;
CPlayerGui* g_pPlayerGui = &g_gui;
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
bool CTCScriptedControl::IsActionListEmpty() const { return g_empty; }
void CTCFireheartMinigame::v00() {}

int main()
{
    int ok = 1;
    std::memset(&g_gui, 0, sizeof(g_gui));
    CTCScriptedControl control;
    CInterfaceMap::SEntry e; e.type = 0x1f; e.component = (CTCBase*)&control;
    CThing owner; std::memset(&owner, 0, sizeof(owner));
    owner.m_interfaces.first = &e; owner.m_interfaces.last = &e + 1; owner.m_presentBits[0] = 1u << 31;
    CTCFireheartMinigame game; game.ParentThing = &owner; game.InSession = false; game.Moving = true;
    game.FrameUpdate();                 // not in session: nothing
    if (g_sets != 0 || !game.Moving) ok = 0;
    game.InSession = true; g_empty = false;
    game.FrameUpdate();                 // prompt set, still moving
    if (g_sets != 1 || std::strcmp(g_text, "TEXT_GUI_FISHING_STOP") != 0 || !game.Moving) ok = 0;
    g_empty = true; g_gui.SubtitleRenderer.InCutscene = true;
    game.FrameUpdate();                 // cutscene: no prompt; action list empty -> move over
    if (g_sets != 1 || game.Moving) ok = 0;
    g_gui.SubtitleRenderer.InCutscene = false; g_gui.SubtitleRenderer.DisplayingText = true;
    game.FrameUpdate();
    if (g_sets != 1) ok = 0;
    if (!ok) { std::printf("007e76b0_TEST FAIL\n"); return 1; }
    std::printf("007e76b0_TEST PASS\n");
    return 0;
}
