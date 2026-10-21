// NUISystem::CComponent::Draw(CIEngine&, CEnginePrimitiveHandle*, long, long*, CComponent*) -- work out this component's render layer (its own Layer, plus the caller's layer unless it is independent or layer-independent) and draw every child it owns (or that is a bastard child) and that is not drawn from a viewport, first from Children and then from ChildrenToDelete
// Retail: Fable.exe (TLC, VC7.1) @ 0x00530260, 387 bytes (400 with padding). Vtable 0x0124608C slot 2 (also
// inherited by NUISystem::CYesNo 0x0122F884).
// Callees: virtual slots 2 (+0x08, Draw on each child), 52 (+0xd0, GetParent), 100 (+0x190, Flags bit 7),
//          101 (+0x194, Flags bit 6), 104 (+0x1a0, Flags bit 5) and 105 (+0x1a4, FlagsSetTwo bit 0). The slot
//          names are guesses from the Ego_r PDB's CComponent member list (the bodies are one-bit getters).
//
// Shape: both flag getters are called before the layer test (two bool locals, no short-circuit); the layer
// expression is an if/else, so `movsx` of the signed-char Layer appears on both arms. The flag getter of
// slot 105 is called twice per child (as retail). In the ChildrenToDelete loop the bastard-child test still
// indexes Children (+0xb0) -- the retail body does, so the source does too. `this == GetParent()` keeps the
// retail `cmp esi,eax` operand order.
// Layout: local model -- NUISystem::CComponent has no engine header; the PDB layout (fable_types.h) shifted
// to retail as in the landed 0x005339B0 / 0x0052F3E0 rows: Children +0xb0, ChildrenToDelete +0xbc, Layer +0x12f.
namespace NUISystem {

class CComponent;
struct CIEngine;
struct CEnginePrimitiveHandle;
struct CCountedComponentPointer {           // CCountedPointer<NUISystem::CComponent>
    CComponent* Object;
    void*       Info;
    CComponent* operator->() const { return Object; }
};
struct CComponentPointerVector {            // STLport vector<CCountedPointer<CComponent>> (12 bytes)
    CCountedComponentPointer* _M_start;
    CCountedComponentPointer* _M_finish;
    CCountedComponentPointer* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    CCountedComponentPointer& operator[](unsigned int i) { return _M_start[i]; }
};
class CComponent {
public:
    virtual void VSlot0();
    virtual void VSlot1();
    virtual void Draw(CIEngine& engine, CEnginePrimitiveHandle* primitive, long layer, long* count, CComponent* drawer);   // slot 2 (+0x08)
    virtual void VSlot3();
    virtual void VSlot4();
    virtual void VSlot5();
    virtual void VSlot6();
    virtual void VSlot7();
    virtual void VSlot8();
    virtual void VSlot9();
    virtual void VSlot10();
    virtual void VSlot11();
    virtual void VSlot12();
    virtual void VSlot13();
    virtual void VSlot14();
    virtual void VSlot15();
    virtual void VSlot16();
    virtual void VSlot17();
    virtual void VSlot18();
    virtual void VSlot19();
    virtual void VSlot20();
    virtual void VSlot21();
    virtual void VSlot22();
    virtual void VSlot23();
    virtual void VSlot24();
    virtual void VSlot25();
    virtual void VSlot26();
    virtual void VSlot27();
    virtual void VSlot28();
    virtual void VSlot29();
    virtual void VSlot30();
    virtual void VSlot31();
    virtual void VSlot32();
    virtual void VSlot33();
    virtual void VSlot34();
    virtual void VSlot35();
    virtual void VSlot36();
    virtual void VSlot37();
    virtual void VSlot38();
    virtual void VSlot39();
    virtual void VSlot40();
    virtual void VSlot41();
    virtual void VSlot42();
    virtual void VSlot43();
    virtual void VSlot44();
    virtual void VSlot45();
    virtual void VSlot46();
    virtual void VSlot47();
    virtual void VSlot48();
    virtual void VSlot49();
    virtual void VSlot50();
    virtual void VSlot51();
    virtual CComponent* GetParent();                                // slot 52 (+0xd0)
    virtual void VSlot53();
    virtual void VSlot54();
    virtual void VSlot55();
    virtual void VSlot56();
    virtual void VSlot57();
    virtual void VSlot58();
    virtual void VSlot59();
    virtual void VSlot60();
    virtual void VSlot61();
    virtual void VSlot62();
    virtual void VSlot63();
    virtual void VSlot64();
    virtual void VSlot65();
    virtual void VSlot66();
    virtual void VSlot67();
    virtual void VSlot68();
    virtual void VSlot69();
    virtual void VSlot70();
    virtual void VSlot71();
    virtual void VSlot72();
    virtual void VSlot73();
    virtual void VSlot74();
    virtual void VSlot75();
    virtual void VSlot76();
    virtual void VSlot77();
    virtual void VSlot78();
    virtual void VSlot79();
    virtual void VSlot80();
    virtual void VSlot81();
    virtual void VSlot82();
    virtual void VSlot83();
    virtual void VSlot84();
    virtual void VSlot85();
    virtual void VSlot86();
    virtual void VSlot87();
    virtual void VSlot88();
    virtual void VSlot89();
    virtual void VSlot90();
    virtual void VSlot91();
    virtual void VSlot92();
    virtual void VSlot93();
    virtual void VSlot94();
    virtual void VSlot95();
    virtual void VSlot96();
    virtual void VSlot97();
    virtual void VSlot98();
    virtual void VSlot99();
    virtual bool IsBastardChild();                                  // slot 100 (+0x190), Flags bit 7
    virtual bool IsIndependent();                                   // slot 101 (+0x194), Flags bit 6
    virtual void VSlot102();
    virtual void VSlot103();
    virtual bool IsLayerIndependent();                              // slot 104 (+0x1a0), Flags bit 5
    virtual bool IsDrawFromViewport();                              // slot 105 (+0x1a4), FlagsSetTwo bit 0
    unsigned char _pad04[0xac];             // +0x04
    CComponentPointerVector Children;       // +0xb0
    CComponentPointerVector ChildrenToDelete;   // +0xbc
    unsigned char _padc8[0x64];             // +0xc8
    unsigned char Flags;                    // +0x12c
    unsigned char Type;                     // +0x12d
    unsigned char FlagsSetTwo;              // +0x12e
    char          Layer;                    // +0x12f
    long          DefIndex;                 // +0x130
};

void CComponent::Draw(CIEngine& engine, CEnginePrimitiveHandle* primitive, long layer, long* count, CComponent* drawer)
{
    bool independent = IsIndependent();
    bool layerIndependent = IsLayerIndependent();
    long renderLayer;
    if (!independent && !layerIndependent)
        renderLayer = Layer + layer;
    else
        renderLayer = Layer;
    unsigned int i;
    for (i = 0; i < Children.size(); ++i) {
        if (this == Children[i]->GetParent() || Children[i]->IsBastardChild()) {
            if (!Children[i]->IsDrawFromViewport() && !Children[i]->IsDrawFromViewport())
                Children[i]->Draw(engine, primitive, renderLayer, count, this);
        }
    }
    for (i = 0; i < ChildrenToDelete.size(); ++i) {
        if (this == ChildrenToDelete[i]->GetParent() || Children[i]->IsBastardChild()) {
            if (!ChildrenToDelete[i]->IsDrawFromViewport() && !ChildrenToDelete[i]->IsDrawFromViewport())
                ChildrenToDelete[i]->Draw(engine, primitive, renderLayer, count, this);
        }
    }
}

}