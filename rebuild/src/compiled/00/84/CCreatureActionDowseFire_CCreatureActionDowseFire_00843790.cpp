// CCreatureAction_DowseFire::CCreatureAction_DowseFire(CThingAICreature*, CThingBuilding*, unsigned long) -- base CCreatureActionBase(owner, -1.0, 1.0, the owner's "STANDARD_SCARED" animation, zeroed sound, 0, 0), then PAICreature = owner, PBuilding = the building, FireNumber
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00843790, length 169 bytes (+ int3 padding), `ret 0Ch`. Vtable 0x01274D0C.
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("STANDARD_SCARED" @ 0x01273564), the owner's animation lookup 0x006924B0
// (`__fastcall`), CCreatureActionBase::CCreatureActionBase 0x00693B30, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90.
// Shape: the literal's implicit CCharString temporary is built before the other base arguments (as the landed 0x008424F0 family);
// the two intelligent pointers are init-list members constructed from raw pointers (inline converting ctors: base ctor, derived
// vtable -- CIntelligentPointer<CThingAICreature> 0x0123E7E0, CIntelligentPointer<CThingBuilding> 0x01244344 -- SetPItem).
// Layout: PAICreature / PBuilding / FireNumber at retail +0xa8 / +0xb0 / +0xb8 (PDB +0xb4 / +0xbc / +0xc4).
struct CThingAICreature;
struct CThingBuilding;
struct CThingCreatureBase;
struct CAnimationEntry;
struct CBaseObject;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CSoundPair {                             // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCreatureActionBase {                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};
CAnimationEntry* __fastcall GetAnimationEntry(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    virtual void v00();
    CBaseObject* PItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointerAICreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingAICreature>, vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointerAICreature(CThingAICreature* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
struct CIntelligentPointerBuilding : CBaseIntelligentPointer {     // CIntelligentPointer<CThingBuilding>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerBuilding(CThingBuilding* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};

struct CCreatureAction_DowseFire : CCreatureActionBase {   // vtable 0x01274D0C
    CIntelligentPointerAICreature PAICreature;  // +0xa8 (PDB +0xb4)
    CIntelligentPointerBuilding   PBuilding;    // +0xb0 (PDB +0xbc)
    unsigned long                 FireNumber;   // +0xb8 (PDB +0xc4)
    virtual void v00();
    CCreatureAction_DowseFire(CThingAICreature* creature, CThingBuilding* building, unsigned long fireNumber);
};

CCreatureAction_DowseFire::CCreatureAction_DowseFire(CThingAICreature* creature, CThingBuilding* building, unsigned long fireNumber)
    : CCreatureActionBase((CThingCreatureBase*)creature, -1.0f, 1.0f, GetAnimationEntry((CThingCreatureBase*)creature, "STANDARD_SCARED"), CSoundPair(), 0, 0)
    , PAICreature(creature)
    , PBuilding(building)
    , FireNumber(fireNumber)
{
}