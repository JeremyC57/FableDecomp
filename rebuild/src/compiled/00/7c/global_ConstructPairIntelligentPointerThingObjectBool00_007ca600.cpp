// _global::Construct_PairIntelligentPointerThingObjectBool_007ca600(std::pair<CIntelligentPointer<CThingObject>,bool>*, const std::pair<CIntelligentPointer<CThingObject>,bool>&) -- STLport _Construct: placement copy-construct one pair<CIntelligentPointer<CThingObject>, bool>
// Leaf: Construct_PairIntelligentPointerThingObjectBool_007ca600
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CA600, length 45 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90; CIntelligentPointer vtable 0x0125A858.
// Shape: /Gr TU (__fastcall: destination in ecx, source in edx); `new (p) T(v)` null-tests the placement result, then the
// implicit pair copy constructor inlines the CIntelligentPointer copy constructor (base ctor, vtable store,
// SetPItem(other.GetPItem())) and the bool byte (`mov al ; mov [esi+8],al`).
// The manifest label `_Cons_val<allocator<...>>` is propagated; the body is STLport _Construct.
inline void* operator new(unsigned int, void* p) { return p; }

class CThingObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThingObject* m_pItem;                  // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CThingObject* GetPItem() const;         // 0x00A01B50
    void SetPItem(CThingObject* item);      // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {         // CIntelligentPointer<CThingObject>, vtable 0x0125A858
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CThingObjectBoolPair {               // std::pair<CIntelligentPointer<CThingObject>, bool>
    CIntelligentPointer first;              // +0x00
    bool second;                            // +0x08
};

void __fastcall Construct_PairIntelligentPointerThingObjectBool_007ca600(CThingObjectBoolPair* p, const CThingObjectBoolPair& value)
{
    new (p) CThingObjectBoolPair(value);
}