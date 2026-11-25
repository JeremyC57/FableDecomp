// CMiniMapDisplay::UpdateMarkersInternal() -- when a region is loaded (+0xb8), add the hero marker: the live hero thing (game -> player manager (+0x1c) -> GetPlayer @ 0x00449970 -> GetPControlledCreature @ 0x00487DC0, dead bit +0x91 clear) with the first graphic index of the player GUI def's marker list (+0x958) through AddMarker (0x0082A500)
// Leaf: ?UpdateMarkersInternal@CMiniMapDisplay@@IAEXXZ
// Naming wave 18: the global stand-in g_pGame (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A960, length 79 bytes.
// Callees: CPlayerManager::GetPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0 (both labels propagated), PeekGuiDef @ 0x00438570 (`__fastcall`, no arguments), CMiniMapDisplay::AddMarker @ 0x0082A500 ((thing, graphic index), `ret 8`). The game object is the global @ 0x013B86A0. The retail region word is at +0xb8 (engine/CMiniMapDisplay.h puts PRegion at +0xd4), so the layout is local. Shape: `this` in edi, the hero in esi, the def's list reached through an inline front() whose `this` (def + 0x958) retail leaves in eax, `ret`.
#include "engine/CThing.h"
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                        // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                   // 0x00449970 ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ (Ego_r / FableWin; NF153: was the stand-in GetPlayer)
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* m_pPlayerManager;              // +0x1c
};

struct CArray_long {                               // CArray<long>: begin, end, ...
    long* m_pBegin;
    long* m_pEnd;
    long& front() { return *m_pBegin; }
    const long& front() const { return *m_pBegin; }
};
class CPlayerGuiDef { public:
    unsigned char _pad00[0x958];
    CArray_long   m_markerGraphics;                // +0x958
};
class CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef(); };            // 0x00438570

class CRegion;
// Naming (NF442 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x0082A500 ?AddAutoMarker@CMiniMapDisplay@@IAEXABVCThing@@J@Z (was ?AddMarker@CMiniMapDisplay@@QAEXPBVCThing@@J@Z)
class CMiniMapDisplay { public:
    virtual void v00();
    unsigned char _pad04[0xb4];                    // +0x04 .. +0xb8
    CRegion*      m_pRegion;                       // +0xb8 (retail)
    protected:
    void UpdateMarkersInternal();
    public:
 protected: void AddAutoMarker(CThing const&, long); };

void CMiniMapDisplay::UpdateMarkersInternal()
{
    if (m_pRegion) {
        CThing* hero = CMainGameComponent::Get()->m_pPlayerManager->GetMainPlayer().GetPControlledCreature();
        if (hero && !hero->IsDead())
            reinterpret_cast<CMiniMapDisplay*>(this)->AddAutoMarker(*(hero), CPlayerGui::PeekGuiDef().m_markerGraphics.front());
    }
}