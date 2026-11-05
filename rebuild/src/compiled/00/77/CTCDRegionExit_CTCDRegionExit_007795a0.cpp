// CTCDRegionExit::~CTCDRegionExit() -- a region exit dying while it holds the screen in letterbox mode (and the world still shows it) ends the letterbox and tells the living hero's region display to finish showing the region name; then the entrance pointer and the CTCBase go
// Leaf: ??1CTCDRegionExit
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007795A0, 167 bytes (plain `ret`). Vtable 0x0126AB54.
// Callees: CTCBase::PeekWorld() const @ 0x00686A80, CDisplayEngine::EndLetterBox @ 0x004349D0 on the main game component's +0x28 (the landed
// body 0x004349D0 clears its +0x7c/+0x7d flags; Ego_r.pdb puts PDisplayEngine at +0x28), CTCBase::GetPlayerManager() @ 0x00686D40,
// CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*>::
// LowerBound @ 0x0040F020, CTCRegionDisplay::FinishShowingRegionName @ 0x00639930, ~CBaseIntelligentPointer @ 0x00A01C10,
// ~CTCBase @ 0x00686830. Global: CMainGameComponent::Get() @ 0x013B86A0.
// Layout: Ego_r.pdb CTCDRegionExit: InLetterBoxMode +0x0d, PEntranceConnectedTo +0x2a (unaligned: the pointer classes are
// `pack(1)`); the world's letterbox-shown flag at +0xdf (named descriptively); the hero's region display = interface 0x8e
// (bit 14 of the +0x30 word, `test ch,40h`).
// Shape: a user destructor (own vtable store first); the hero is alive-tested as a raw pointer; the region display is the
// unchecked out-parameter GetInterface (the absent arm reads the key's frame word); the member and base dtors follow.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
inline bool IsThingAlive(const CThing* thing) { return thing && !thing->IsDead(); }
class CTCRegionDisplay { public:
    void FinishShowingRegionName();                     // 0x00639930
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                          // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CWorld { public:
    unsigned char _pad00[0xdf];
    bool LetterBoxShown;                                // +0xdf
};
class CDisplayEngine { public:
    void EndLetterBox();                                // 0x004349D0
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                     // +0x28
};
#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)
class CTCBase { public:
    virtual ~CTCBase();                                 // 0x00686830
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
    const CWorld& PeekWorld() const;   // 0x00686A80
};
class CTCDRegionExit : public CTCBase { public:                       // vtable 0x0126AB54
    bool Active;                                        // +0x0c
    bool InLetterBoxMode;                               // +0x0d
    bool WaitingForConfirmation;                        // +0x0e
    bool WaitingForActivationOrPlayerToLeave;           // +0x0f
    bool WaitingForPlayerToLeaveMessageArea;            // +0x10
    float Radius;                                       // +0x14
    float MessageRadius;                                // +0x18
    unsigned __int64 EntranceConnectedToUID;            // +0x20
    bool ReversedOnMiniMap;                             // +0x28
    bool HiddenOnMiniMap;                               // +0x29
    CIntelligentPointer<CThing> PEntranceConnectedTo;           // +0x2a
    virtual ~CTCDRegionExit();
};

CTCDRegionExit::~CTCDRegionExit()
{
    if (InLetterBoxMode && PeekWorld().LetterBoxShown) {
        CMainGameComponent::Get()->PDisplayEngine->EndLetterBox();
        CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (IsThingAlive(hero)) {
            CTCRegionDisplay* display;
            hero->GetTC(TCI_REGION_DISPLAY, &display);
            display->FinishShowingRegionName();
        }
    }
}