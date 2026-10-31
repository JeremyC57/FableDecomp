// CTCInventoryMap::TeleportHeroToBoastingArea() const -- while the hero creature is alive and an AI creature with property 0x72 (the boasting-area marker) exists, show the mini map on the region load and queue a move-hero-to-region game event (0x2C) carrying that creature's position and facing angle, flags all false, from the main player
// Grade: DIFFER(379v379) -- filter temporary and marker swap frame slots (retail filter +0x10, marker +0xC shared with the AddFloat temporaries; ours filter +0xC, marker +0x10) and so the esi load of the marker reads +0x14 for +0x10; everything else identical
// Leaf: ?TeleportHeroToBoastingArea@CTCInventoryMap@@QBEXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FB4B0, 379 bytes, `ret`, esp frame (speed TU), /GS (cookie 0x0139C8A8,
//   __security_check_cookie @ 0x00BFE9F9).
// Identity: Ego_r.pdb ?TeleportHeroToBoastingArea@CTCInventoryMap@@QBEXXZ (const; the earlier attempt was non-const).
// Callees (PDB spellings, reused from the landed sibling TeleportHeroToGuildSealRecallLocation 0x005FAD40):
//   CTCBase::PeekPlayerManager() const 0x00686D50, CPlayerManager::GetMainPlayer() const 0x00449960 (engine/CPlayerManager.h),
//   CPlayer::PeekPControlledCreature() const 0x00487DD0, CTCBase::PeekWorld() const 0x00686A80 (inside the inline
//   CTCBase::PeekThingSearchTools -> CWorld::PeekThingSearchTools, both Ego_r names), CTCBase::GetWorld() const 0x00686A70,
//   CThingSearchTools::GetPFirstLeafThing<CThingFilter_AICreatureHasProperty, CThingAICreature> 0x006156E0 (Ego_r),
//   CThing::GetPos() const 0x004C73D0, CMemoryBuffer::Copy 0x009F1760, CPlayerManager::GetMainPlayerNumber 0x00449700 via
//   g_pMainGameComponent 0x013B86A0 (GOTCHAS chunk 200) ->PPlayerManager +0x1C, CWorld::HandleMoveHeroToRegionGameEvent
//   (const CGameEvent&) 0x0049EAF0 (Ego_r spelling), physics slot 63 GetAngleXY (+0xFC).
// Levers this wave (one per grade): PDB accessor chain PeekThingSearchTools (same residue); marker in an inner block with an
//   early return (angle float moves to the marker slot, filter still below); named filter at if-scope / in the block (filter
//   now above the marker, but the floats take a fresh slot: frame +4); bool result local (same as block form).
//   Also (probe 7): an inline pointer-returning GetPFirstLeafThing<F,T>(filter) helper puts filter above marker as retail, but
//   adds a slot (frame 0x3C) and a null test (387v379). Next idea: a helper shape that both nests the marker and lets the
//   AddFloat temporaries reuse its slot without a fresh one.
//   Earlier: named filter local first; GetWorld before boaster copy; marker at function scope; forceinline helper.
class C3DVector { public: float X, Y, Z; };
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddFloat(float value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetAngleXY() const;                 // slot 63 (+0xFC)
};
class CThing { public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PPhysics;                         // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char IsDead : 1;                         // +0x91 bit 0
    const C3DVector& GetPos() const;                  // 0x004C73D0
};
class CThingAICreature : public CThing { };
class CThingPlayerCreature : public CThing { };
class CThingFilter_AICreatureHasProperty { public:
    long Property;
    CThingFilter_AICreatureHasProperty(long property) : Property(property) { }
};
class CThingSearchTools { public:
    template <class F, class T> bool GetPFirstLeafThing(const F& filter, T** out) const;   // 0x006156E0
};
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;            // 0x00487DD0
};
#include "engine/CPlayerManager.h"
class CMainGameComponent { public:
    unsigned char _pad00[0x1c];
    CPlayerManager* PPlayerManager;                   // +0x1C scoped_ptr<CPlayerManager>
};
extern CMainGameComponent* g_pMainGameComponent;      // 0x013B86A0
class CWorld { public:
    unsigned char _pad000[0x20];
    CThingSearchTools* PThingSearchTools;             // +0x20 scoped_ptr<CThingSearchTools>
    unsigned char _pad024[0x12f - 0x24];
    bool DisplayMiniMapOnRegionLoad;                  // +0x12F
    const CThingSearchTools& PeekThingSearchTools() const { return *PThingSearchTools; }
    void HandleMoveHeroToRegionGameEvent(const CGameEvent& event);   // 0x0049EAF0
};
class CTCBase { protected:
    CWorld& GetWorld() const;                         // 0x00686A70
    const CWorld& PeekWorld() const;                  // 0x00686A80
    const CPlayerManager& PeekPlayerManager() const;  // 0x00686D50
    const CThingSearchTools& PeekThingSearchTools() const { return PeekWorld().PeekThingSearchTools(); }
};
class CTCInventoryMap : public CTCBase { public:
    void TeleportHeroToBoastingArea() const;
};

void CTCInventoryMap::TeleportHeroToBoastingArea() const
{
    const CThing* hero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    if (hero && !hero->IsDead) {
        CThingAICreature* marker;
        if (PeekThingSearchTools().GetPFirstLeafThing(CThingFilter_AICreatureHasProperty(0x72), &marker)) {
            CThingAICreature* boaster = marker;
            GetWorld().DisplayMiniMapOnRegionLoad = true;
            CGameEvent event(0x2c);
            event.AddFloat(boaster->GetPos().X);
            event.AddFloat(boaster->GetPos().Y);
            event.AddFloat(boaster->GetPos().Z);
            event.AddFloat(boaster->PPhysics->GetAngleXY());
            event.AddBool(false);
            event.AddBool(false);
            event.AddBool(false);
            event.Player = (signed char)g_pMainGameComponent->PPlayerManager->GetMainPlayerNumber();
            GetWorld().HandleMoveHeroToRegionGameEvent(event);
        }
    }
}
