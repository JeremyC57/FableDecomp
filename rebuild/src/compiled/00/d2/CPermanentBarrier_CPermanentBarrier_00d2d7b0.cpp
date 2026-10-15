// CPermanentBarrier::CPermanentBarrier(CQ_EndGameFocalSitesScript*, CQ_SunnyvaleMasterData*, const CScriptThing&) -- construct the permanent-barrier entity script: the parent's quest interface, its thing, parent, master data and two empty thing vectors
// Leaf: ??0CPermanentBarrier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D2D7B0, length 76 bytes.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90. Vtables 0x012C3224 (the entity-script
// base, as in the landed Create_CVtable_* factories) and 0x012CC48C (this class, no RTTI).
// Identity: the manifest label CProjectileWeapon::CProjectileWeapon is propagated. The factory 0x00D2D730
// allocates 0x34 bytes; of the Ego_r.pdb entity scripts whose retail size is 0x34 only CPermanentBarrier
// (ParentClass CQ_EndGameFocalSitesScript*, PMasterData, two vector<CScriptThing>) has six zero-initialised
// words after PMasterData; its Main uses the "HK_MAGIC_BARRIER" string at 0x012CC4A8.
// Shape: the root's interface pointer before the base vtable (dead root vtable), the thing copy after it,
// ParentClass in the init list (before the derived vtable store), the two vectors' inline constructors,
// PMasterData a body assignment (stored last).
struct CScriptThing_Calls {
    unsigned char _body[12];
    CScriptThing_Calls(const CScriptThing_Calls& other);                // 0x004ABE90
};

struct CGameScriptInterface_Calls;

struct CQuestScriptBase_Retail {                            // the parent quest script: interface pointer at +0x40
    unsigned char         _pad[0x40];
    CGameScriptInterface_Calls* GSI;                              // +0x40
};

struct CQ_EndGameFocalSitesScript : CQuestScriptBase_Retail { };
struct CQ_SunnyvaleMasterData;

struct CEntityScriptRoot {                                  // root: its vtable store is dead
    virtual void v00();
    CGameScriptInterface_Calls* PQuest;                           // +0x04
    CEntityScriptRoot(CGameScriptInterface_Calls* quest) : PQuest(quest) { }
};

struct CEntityScriptBase : CEntityScriptRoot {              // vtable 0x012C3224
    virtual void v00();
    CScriptThing_Calls Thing;                                     // +0x08
    CEntityScriptBase(CQuestScriptBase_Retail* parent, const CScriptThing_Calls& thing)
        : CEntityScriptRoot(parent->GSI), Thing(thing) { }
};

struct CScriptThingVector {                                 // retail std::vector<CScriptThing_Calls> (12 bytes)
    CScriptThing_Calls* m_pFirst;
    CScriptThing_Calls* m_pLast;
    CScriptThing_Calls* m_pEnd;
    CScriptThingVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CPermanentBarrier : CEntityScriptBase {              // vtable 0x012CC48C, 0x34 bytes
    virtual void v00();
    CQ_EndGameFocalSitesScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;                // +0x18
    CScriptThingVector          particle_effectVector;      // +0x1c
    CScriptThingVector          MyMarkerVector;             // +0x28
    CPermanentBarrier(CQ_EndGameFocalSitesScript* parent, CQ_SunnyvaleMasterData* masterData, const CScriptThing_Calls& thing);
};

CPermanentBarrier::CPermanentBarrier(CQ_EndGameFocalSitesScript* parent, CQ_SunnyvaleMasterData* masterData, const CScriptThing_Calls& thing)
    : CEntityScriptBase(parent, thing), ParentClass(parent)
{
    PMasterData = masterData;
}