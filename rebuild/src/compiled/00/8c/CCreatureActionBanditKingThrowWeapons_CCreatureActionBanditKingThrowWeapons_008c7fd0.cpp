// CCreatureAction_BanditKingThrowWeapons::CCreatureAction_BanditKingThrowWeapons(CThingCreatureBase*, CThingCreatureBase*) -- base CCreatureActionBase(owner, -1.0, 0.75, the owner's "BK_THROW_WEAPONS" animation, zeroed sound, 0, 0), then Target = the second argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C7FD0, length 175 bytes (+ int3 padding). Vtable 0x0128193C (rtti-vtables.tsv);
// Target is a CIntelligentPointer<CThingCreatureBase> (vtable 0x0123E7D8) at retail +0xa8 (PDB +0xb4: the PDB base is 0xc larger).
// Callees: CCharString::CCharString 0x0099EBF0 / ::~CCharString 0x0099EAE0, the owner's animation lookup by name 0x006924B0
// (`__fastcall`), CCreatureActionBase::CCreatureActionBase 0x00693B30, CBaseIntelligentPointer ctor 0x00A01B10, SetPItem 0x00A01B90,
// GetPItem 0x00A01B50, ~CBaseIntelligentPointer 0x00A01C10.
// Shape: the literal's CCharString temporary is built before every other base argument (GOTCHAS); the member is default-constructed
// in the init list and assigned in the body from a converted temporary (`Target = CIntelligentPointer(target)`: temporary ctor,
// SetPItem, GetPItem, the member's SetPItem, temporary dtor) -- the temporary reuses the dead string slot.
class CCharString { public:                        // engine string (4 bytes: data pointer)
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CThingCreatureBase;
class CAnimationEntry;
class CBaseObject;

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};
CAnimationEntry* __fastcall GetAnimationEntry(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CBaseIntelligentPointer {            // 8 bytes: vptr + item
    virtual void v00();
    CBaseObject* PItem;                     // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature() : CBaseIntelligentPointer() { }
    CIntelligentPointerCreature(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
};

class CCreatureAction_BanditKingThrowWeapons : public CCreatureActionBase { public:   // vtable 0x0128193C
    CIntelligentPointerCreature Target;     // +0xa8 (PDB +0xb4)
    virtual void v00();
    CCreatureAction_BanditKingThrowWeapons(CThingCreatureBase* owner, CThingCreatureBase* target);
};

CCreatureAction_BanditKingThrowWeapons::CCreatureAction_BanditKingThrowWeapons(CThingCreatureBase* owner, CThingCreatureBase* target)
    : CCreatureActionBase(owner, -1.0f, 0.75f, GetAnimationEntry(owner, "BK_THROW_WEAPONS"), CSoundPair(), 0, 0)
{
    Target = CIntelligentPointerCreature(target);
}