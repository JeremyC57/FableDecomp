// CCreatureAction_DowseFire::CCreatureAction_DowseFire(const CCreatureAction_DowseFire&) -- copy constructor: base copy, the AI-creature and building pointers re-pointed at the source's items, the fire number copied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084C540, length 117 bytes, `ret 4`. Vtable 0x01274D0C.
// Callees: CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) @ 0x0062EBC0,
//          CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (vtables CIntelligentPointer<CThingAICreature> 0x0123E7E0,
//          CIntelligentPointer<CThingBuilding> 0x01244344).
// Shape: the landed BanditKingThrowWeapons copy shape: each pointer member is addressed once in ebx, runs the base
//   constructor, stores its vtable and sets the item read from the source's member (`lea ecx,[edi+N]`); the
//   FireNumber dword is copied last.
// Layout: Ego_r.pdb puts PAICreature/PBuilding/FireNumber at +0xb4/+0xbc/+0xc4 (PDB CCreatureActionBase 0xb4 bytes,
//   retail 0xa8): +0xa8/+0xb0/+0xb8 here; engine/CCreatureAction_DowseFire.h is the PDB build and is not used.
class CBaseObject;
class CThingAICreature;
class CThingBuilding;

struct CBaseIntelligentPointer {            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                   // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer { public:   // one vtable per instantiation
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(const CCreatureActionBase& other);     // 0x0062EBC0
};

class CCreatureAction_DowseFire : public CCreatureActionBase { public:   // vtable 0x01274D0C
    CIntelligentPointer<CThingAICreature> PAICreature;   // +0xa8 (PDB +0xb4)
    CIntelligentPointer<CThingBuilding> PBuilding;       // +0xb0 (PDB +0xbc)
    unsigned long FireNumber;                            // +0xb8 (PDB +0xc4)
    virtual void v00();
    CCreatureAction_DowseFire(const CCreatureAction_DowseFire& other);
};

CCreatureAction_DowseFire::CCreatureAction_DowseFire(const CCreatureAction_DowseFire& other)
    : CCreatureActionBase(other)
    , PAICreature(other.PAICreature)
    , PBuilding(other.PBuilding)
    , FireNumber(other.FireNumber)
{
}