#ifndef FABLE_ENGINE_NUISYSTEM_CVIEWPORT_H
#define FABLE_ENGINE_NUISYSTEM_CVIEWPORT_H
// NUISystem::CViewport (new, chunk 726). Members and virtuals from Ego_r.pdb / FableWin.pdb (struct_layouts_egor.tsv
// CViewport 424 bytes: Handle +396, FakeChildren +404, GraphicIndex +420; name sets ??0CViewport@NUISystem@@QAE@J@Z,
// ??1CViewport@NUISystem@@UAE@XZ, AddFakeChild, ClearHandle, Draw, FakeChildDying, GetDataFromDef, GetDynamicSize,
// GetSize, Initialise). Retail offsets are the PDB's minus 0x30 (the retail CChangingStateComponent is 0x15C):
// the constructor 0x0054EA00 builds Handle at +0x15C (CBaseClass ctor on `lea edi,[esi+15Ch]`, vtable 0x0122F598),
// the 12-byte STLport vector at +0x164 and clears +0x170; CScrollingViewport::GetScrollingSpeed 0x0053FC00 reads its
// first own member at +0x174, so sizeof is 0x174. Vtables 0x01249A5C (primary) / 0x01249A38 (+4) / 0x01249A30 (+0x18).
// The one virtual CViewport adds is AddFakeChild (slot 142, 0x0053FCD0 in CScrollingViewport's table).
#include "engine/NUISystem_CChangingStateComponent.h"
// CEnginePrimitiveHandle's inline destructor needs the retail pointee model and the polymorphic CIVCountedPointeeBase (as engine/NUISystem_CUIDef.h does for C2DVector).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#ifndef FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#endif
#include "engine/CEnginePrimitiveHandle.h"
class CIEngine;
namespace NUISystem {
class CViewport : public CChangingStateComponent {
public:
    CViewport(long);
    CViewport(const CViewport&);
    virtual ~CViewport();
    void GetDataFromDef();
    virtual void Initialise();
    virtual void Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*);
    virtual C2DVector GetSize() const;
    virtual void FakeChildDying(CComponent*);
    virtual void ClearHandle();
    virtual C2DVector GetDynamicSize() const;
    virtual void AddFakeChild(CComponent*);                 // slot 142
protected:
    CEnginePrimitiveHandle Handle;                          // +0x15c
    std::vector<CComponent*,std::allocator<CComponent*> > FakeChildren;   // +0x164
    unsigned long GraphicIndex;                             // +0x170
};
FABLE_STATIC_ASSERT(sizeof(CViewport)==0x174);
}
#endif
