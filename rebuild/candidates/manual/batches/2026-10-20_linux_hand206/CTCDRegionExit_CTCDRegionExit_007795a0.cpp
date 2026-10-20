// CTCDRegionExit::~CTCDRegionExit() -- a region exit dying while it holds the screen in letterbox mode (and the world still shows it) ends the letterbox and tells the living hero's region display to finish showing the region name; then the entrance pointer and the CTCBase go
// Leaf: ??1CTCDRegionExit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007795A0, 167 bytes (plain `ret`). Vtable 0x0126AB54.
// Callees: CTCBase::GetWorld() const @ 0x00686A80, CDisplayEngine::EndLetterBox @ 0x004349D0 on the main game component's +0x28 (the landed
// body 0x004349D0 clears its +0x7c/+0x7d flags; Ego_r.pdb puts PDisplayEngine at +0x28), CTCBase::GetPlayerManager() @ 0x00686D40,
// CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroCreature @ 0x00487DC0, CVectorMap<ETCInterfaceType, CTCBase*>::
// LowerBound @ 0x0040F020, CTCRegionDisplay::FinishShowingRegionName @ 0x00639930, ~CBaseIntelligentPointer @ 0x00A01C10,
// ~CTCBase @ 0x00686830. Global: g_pMainGameComponent @ 0x013B86A0.
// Layout: Ego_r.pdb CTCDRegionExit: InLetterBoxMode +0x0d, PEntranceConnectedTo +0x2a (unaligned: the pointer classes are
// `pack(1)`); the world's letterbox-shown flag at +0xdf (named descriptively); the hero's region display = interface 0x8e
// (bit 14 of the +0x30 word, `test ch,40h`).
// Shape: a user destructor (own vtable store first); the hero is alive-tested as a raw pointer; the region display is the
// unchecked out-parameter GetInterface (the absent arm reads the key's frame word); the member and base dtors follow.
struct CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                              // +0x91 bit 0 Dead
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
inline bool IsThingAlive(const CThing* thing) { return thing && !(thing->Flags91 & 1); }
struct CTCRegionDisplay {
    void FinishShowingRegionName();                     // 0x00639930
};
struct CPlayer {
    CThing* GetHeroCreature();                          // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};
struct CWorld {
    unsigned char _pad00[0xdf];
    bool LetterBoxShown;                                // +0xdf
};
struct CDisplayEngine {
    void EndLetterBox();                                // 0x004349D0
};
struct CMainGameComponent {
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                     // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    ~CBaseIntelligentPointer();                         // 0x00A01C10
};
struct CIntelligentPointer : CBaseIntelligentPointer { };   // CIntelligentPointer<CThing>
#pragma pack(pop)
struct CTCBase {
    virtual ~CTCBase();                                 // 0x00686830
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    const CWorld* GetWorld() const;                     // 0x00686A80
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
};
struct CTCDRegionExit : CTCBase {                       // vtable 0x0126AB54
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
    CIntelligentPointer PEntranceConnectedTo;           // +0x2a
    virtual ~CTCDRegionExit();
};

CTCDRegionExit::~CTCDRegionExit()
{
    if (InLetterBoxMode && GetWorld()->LetterBoxShown) {
        g_pMainGameComponent->PDisplayEngine->EndLetterBox();
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
        if (IsThingAlive(hero)) {
            CTCRegionDisplay* display;
            hero->GetInterface(&display, 0x8e);
            display->FinishShowingRegionName();
        }
    }
}
