// CTCKraken::CTCKraken(CThing* parent) -- Kraken boss component: CTCBase(parent), a null def pointer, five null intelligent pointers, empty explosion and tentacle lists, cleared strike state, phase and mode 0
// Leaf: ??0CTCKraken
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DEFA7, length 144 bytes (frame-pointer, size-optimised module).
// Label correction: the catalog's "CTCDPhysicalObstruction::CTCDPhysicalObstruction" is propagated and wrong: the vtable stored,
// 0x01241B94, is CTCKraken's (rtti-vtables.tsv), and the stores fit Ego_r.pdb CTCKraken with retail 4-byte lists.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 (x5, then the
// CIntelligentPointer<CThing> vtable 0x01238C6C held in edi), list<CIntelligentPointer<CThing> >::list(const allocator&) @ 0x004B8920
// (x2, out of line).
// Layout (retail): PKrakenDef +0xC, PChamber +0x10, PCentreMarker +0x18, PTarget +0x20, PHeadEffect +0x28, ExplosionList +0x30,
// PBeamWeaponEffect +0x34, DelayBeforeBeam +0x3C, PTCBoss +0x40, Tentacles +0x44, AttackingTentacleIterator +0x48,
// TentacleStrikeCounter +0x4C, TentacleReadyToStrike +0x50, HeadTrackTarget +0x51, HeadHasBeenHit +0x52, LastStrikeTick +0x54,
// KillBeamCounter +0x58, ReturnHeadToWater +0x5C, Phase +0x60, Mode +0x64.
// Shape: `optimize("s")` with frame pointers (`optimize("y", off)`): the lists' default `allocator()` argument is an empty temporary
// homed in the top byte of the dead parent-argument slot (`lea eax,[ebp+0Bh]`); one ebx zero register.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CThing;
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCBase(CThing* parent);                    // 0x00686800
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {     // vtable 0x01238C6C
    virtual void v01();
    CIntelligentPointerThing() {}
};
struct CThingPointerAllocator {                 // std::allocator<CIntelligentPointer<CThing> > (empty)
    CThingPointerAllocator() {}
};
struct CThingPointerList {                      // list<CIntelligentPointer<CThing> > (retail: one sentinel pointer)
    void* _M_node;
    CThingPointerList(const CThingPointerAllocator& allocator = CThingPointerAllocator());   // 0x004B8920
};
struct CDefPointer_CKrakenDef {
    const void* m_pDef;
    CDefPointer_CKrakenDef() : m_pDef(0) {}
};
struct CTCBoss;
struct CTCKraken : CTCBase {
    CDefPointer_CKrakenDef PKrakenDef;          // +0x0c
    CIntelligentPointerThing PChamber;          // +0x10
    CIntelligentPointerThing PCentreMarker;     // +0x18
    CIntelligentPointerThing PTarget;           // +0x20
    CIntelligentPointerThing PHeadEffect;       // +0x28
    CThingPointerList ExplosionList;            // +0x30
    CIntelligentPointerThing PBeamWeaponEffect; // +0x34
    long DelayBeforeBeam;                       // +0x3c
    CTCBoss* PTCBoss;                           // +0x40
    CThingPointerList Tentacles;                // +0x44
    void* AttackingTentacleIterator;            // +0x48
    long TentacleStrikeCounter;                 // +0x4c
    bool TentacleReadyToStrike;                 // +0x50
    bool HeadTrackTarget;                       // +0x51
    bool HeadHasBeenHit;                        // +0x52
    long LastStrikeTick;                        // +0x54
    long KillBeamCounter;                       // +0x58
    bool ReturnHeadToWater;                     // +0x5c
    long Phase;                                 // +0x60 EKrakenBossPhase
    long Mode;                                  // +0x64 EKrakenMode
    virtual void v01();
    CTCKraken(CThing* parent);
};

CTCKraken::CTCKraken(CThing* parent)
    : CTCBase(parent)
    , TentacleStrikeCounter(0)
    , TentacleReadyToStrike(false)
    , HeadTrackTarget(false)
    , HeadHasBeenHit(false)
    , LastStrikeTick(0)
    , KillBeamCounter(0)
    , Phase(0)
    , Mode(0)
{
}