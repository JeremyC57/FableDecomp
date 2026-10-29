// CMiniMapDisplay::UpdateMarkersInternal() -- when a region is loaded (+0xb8), add the hero marker: the live hero thing (game -> player manager (+0x1c) -> GetPlayer @ 0x00449970 -> GetPControlledCreature @ 0x00487DC0, dead bit +0x91 clear) with the first graphic index of the player GUI def's marker list (+0x958) through AddMarker (0x0082A500)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A960, length 79 bytes.
// Callees: CPlayerManager::GetPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0 (both labels propagated), PeekGuiDef @ 0x00438570 (`__fastcall`, no arguments), CMiniMapDisplay::AddMarker @ 0x0082A500 ((thing, graphic index), `ret 8`). The game object is the global @ 0x013B86A0. The retail region word is at +0xb8 (engine/CMiniMapDisplay.h puts PRegion at +0xd4), so the layout is local. Shape: `this` in edi, the hero in esi, the def's list reached through an inline front() whose `this` (def + 0x958) retail leaves in eax, `ret`.
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                        // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetPlayer();                          // 0x00449970
};
class CGame { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* m_pPlayerManager;              // +0x1c
};
extern CGame* g_pGame;                             // 0x013B86A0

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
class CMiniMapDisplay { public:
    virtual void v00();
    unsigned char _pad04[0xb4];                    // +0x04 .. +0xb8
    CRegion*      m_pRegion;                       // +0xb8 (retail)
    void AddMarker(const CThing* thing, long graphicIndex);   // 0x0082A500
    void UpdateMarkersInternal();
};

void CMiniMapDisplay::UpdateMarkersInternal()
{
    if (m_pRegion) {
        CThing* hero = g_pGame->m_pPlayerManager->GetPlayer()->GetPControlledCreature();
        if (hero && !hero->m_dead)
            AddMarker(hero, CPlayerGui::PeekGuiDef().m_markerGraphics.front());
    }
}