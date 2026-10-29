// CTCFireheartMinigame::FrameUpdate() -- while the minigame session runs, keep the "stop" prompt on the subtitle button 2 (unless text or a cutscene is showing) and end a move once the owner's scripted-control action list is empty
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E76B0, length 167 bytes (+ int3 padding). __fastcall, `ret`.
//         CTCFireheartMinigame vtable slot 9 (the catalog's "vfunc_9"); Ego_r.pdb `?FrameUpdate@CTCFireheartMinigame@@UAEXXZ`.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("TEXT_GUI_FISHING_STOP" @ 0x0126D788),
//          CSubtitleRenderer::SetButton2OverrideText(const CCharString&) @ 0x0048F881, CVectorMap<ETCInterfaceType,
//          CTCBase*>::LowerBound @ 0x0040F020 (interface 0x1F CTCScriptedControl, word 0 bit 31: `test ecx,ecx ; jns`),
//          CTCScriptedControl::IsActionListEmpty() const @ 0x007122C0. Global g_pPlayerGui @ 0x013B8790.
// Shape: the GUI pointer is read for the two flag tests and re-read after the string ctor; the prompt string is a named
//        local in its own block; the scripted control comes from an inline accessor that leaves its result unset when
//        the interface is absent (the miss path reads the shared frame word), merged into eax for the call.
// Layout: Ego_r.pdb CTCFireheartMinigame matches retail here (InSession +0x4C, Moving +0x70); CPlayerGui::SubtitleRenderer
//         +0x18 (PDB); retail CSubtitleRenderer flags sit 8 below the PDB: InCutscene +0xDE, DisplayingText +0xE5.
// Layout (supersedes any local-model note above): engine/CSubtitleRenderer.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CSubtitleRenderer CSubtitleRendererDonor
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer
class CThing;
class CTCBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class CSubtitleRenderer : public CSubtitleRendererDonor {
public:
    void SetButton2OverrideText(const CCharString& text);   // 0x0048F881
};

class CPlayerGui {
public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;             // +0x18
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);           // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
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