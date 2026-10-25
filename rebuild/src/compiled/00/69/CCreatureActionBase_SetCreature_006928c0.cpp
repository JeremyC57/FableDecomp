// CCreatureActionBase::SetCreature(CThingCreatureBase& creature) -- point the action's PCreature at the creature
// Leaf: ?SetCreature@CCreatureActionBase@@IAEXAAVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006928C0, 71 bytes, thiscall `ret 4`: a stack CIntelligentPointer<CThingCreatureBase>
//   (CBaseIntelligentPointer() 0x00A01B10, then its vtable 0x0123E7D8, SetPItem 0x00A01B90 with the creature), assigned
//   into PCreature (+0x08) through GetPItem 0x00A01B50 / SetPItem, then ~CBaseIntelligentPointer 0x00A01C10.
// Identity (FINDINGS chunk 384; was the address-named CClass_006928c0::AssignTarget_006928c0): Ego_r.pdb
//   `?SetCreature@CCreatureActionBase@@IAEXAAVCThingCreatureBase@@@Z` (name set; protected) and struct_layouts_egor
//   CCreatureActionBase +0x08 PCreature is CIntelligentPointer<CThingCreatureBase>; the callers are the landed
//   CCreatureActionBase::GetPActionFromEnum 0x00692E50 (which already declares 0x006928C0 as SetCreature(CThingCreatureBase&),
//   called on the freshly cloned action) and CCombatActionBase::GetPCombatActionFromEnum 0x00857430 (same clone-then-set).
// Shape: `PCreature = &creature;` -- the converting constructor builds the temporary (inline default base ctor + vtable
//   store + SetPItem), the inline assignment copies GetPItem into SetPItem, the temporary is destroyed.
// Layout: engine/CCreatureActionBase.h keeps PCreature as opaque bytes, so the class is modelled locally with the member
//   at the same offset (+0x08, size 0xB4).
class CBaseObject;
class CThingCreatureBase;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();               // 0x00A01C10
    CBaseObject* PItem;                               // +0x04
    CBaseIntelligentPointer();                        // 0x00A01B10
    CBaseObject* GetPItem() const;                    // 0x00A01B50
    void SetPItem(CBaseObject* item);                 // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0123E7D8 for CThingCreatureBase
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CCreatureActionBase {
public:
    virtual void v00();
    unsigned char                           _pad04[0x04];
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char                           _pad10[0xb4 - 0x10];
protected:
    void SetCreature(CThingCreatureBase& creature);
};

void CCreatureActionBase::SetCreature(CThingCreatureBase& creature)
{
    PCreature = &creature;
}
