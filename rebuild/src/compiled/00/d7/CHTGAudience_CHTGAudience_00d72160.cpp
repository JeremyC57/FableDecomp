// CHTG_Audience::CHTG_Audience(CQ_HangingTreeGoodScript*, CQ_SunnyvaleMasterData*, const CScriptThing&) -- construct the hanging-tree audience entity script: quest interface, thing, parent, master data and three empty script things
// Leaf: ??0CHTG_Audience
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D72160, length 126 bytes (+ int3 padding).
// Identity: the catalog label "CAIStateGroup_JackOfBladesBase::CAIStateGroup_JackOfBladesBase" is propagated
// and wrong (that constructor is the landed 0x00902FE8). The only caller is the factory at 0x00D720E0, which
// allocates 0x40 bytes; the only Ego_r.pdb entity script of 0x40 bytes with three CScriptThing members at
// +0x1c/+0x28/+0x34 is CHTG_Audience (ParentClass CQ_HangingTreeGoodScript*, PMasterData, ChopBlockMarker,
// ExecutionerGuy, PrisonerGuy). The derived vtable 0x012D292C has no RTTI.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) @ 0x004ABE90, CBase::CBase @ 0x0099A2D0 (x3).
// Vtables: 0x012C3224 (the entity-script base, as in the landed CPermanentBarrier 0x00D2D7B0), 0x012D292C (this
// class), 0x01238C8C (CScriptThing, rtti-vtables.tsv).
//
// Shape: the landed CPermanentBarrier shape -- the root's quest-interface pointer (parent->+0x40) stored
// before the base vtable (the root's own vtable store is dead), the thing copied after it, ParentClass in the
// init list (before the derived vtable store), then the three CScriptThing members' inline default
// constructors (CBase() out of line, the CScriptThing vtable, the PImp counted pointer zeroed from one ebx
// zero register; CBase must own the vptr, else the members shift by 4), PMasterData a body assignment (stored last).
namespace N_0099a2d0 { struct CBase {                                  // polymorphic root (owns the vptr)
    virtual void v00();
    CBase();                                    // 0x0099A2D0
}; } using N_0099a2d0::CBase;

struct SCountBlock;

struct CCountedPointer {
    void* m_pObject;                            // +0
    SCountBlock* m_pCount;                      // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

class CScriptThing : public CBase { public:                   // vtable 0x01238C8C, 12 bytes
    virtual void v00();
    CCountedPointer PImp;                       // +0x04
    CScriptThing() : CBase() { }
    CScriptThing(const CScriptThing& other);    // 0x004ABE90
};

class CGameScriptInterface;

struct CQuestScriptBase_Retail {                // the parent quest script: interface pointer at +0x40
    unsigned char _pad[0x40];
    CGameScriptInterface* GSI;                  // +0x40
};

struct CQ_HangingTreeGoodScript : CQuestScriptBase_Retail { };
struct CQ_SunnyvaleMasterData;

struct CEntityScriptRoot {                      // root: its vtable store is dead
    virtual void v00();
    CGameScriptInterface* PQuest;               // +0x04
    CEntityScriptRoot(CGameScriptInterface* quest) : PQuest(quest) { }
};

class CEntityScriptBase : public CEntityScriptRoot { public:  // vtable 0x012C3224
    virtual void v00();
    CScriptThing Thing;                         // +0x08
    CEntityScriptBase(CQuestScriptBase_Retail* parent, const CScriptThing& thing)
        : CEntityScriptRoot(parent->GSI), Thing(thing) { }
};

struct CHTG_Audience : CEntityScriptBase {      // vtable 0x012D292C, 0x40 bytes
    virtual void v00();
    CQ_HangingTreeGoodScript* ParentClass;      // +0x14
    CQ_SunnyvaleMasterData* PMasterData;        // +0x18
    CScriptThing ChopBlockMarker;               // +0x1c
    CScriptThing ExecutionerGuy;                // +0x28
    CScriptThing PrisonerGuy;                   // +0x34
    CHTG_Audience(CQ_HangingTreeGoodScript* parent, CQ_SunnyvaleMasterData* masterData, const CScriptThing& thing);
};

CHTG_Audience::CHTG_Audience(CQ_HangingTreeGoodScript* parent, CQ_SunnyvaleMasterData* masterData, const CScriptThing& thing)
    : CEntityScriptBase(parent, thing), ParentClass(parent)
{
    PMasterData = masterData;
}