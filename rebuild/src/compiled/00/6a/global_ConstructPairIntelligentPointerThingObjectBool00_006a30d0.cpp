// _global::Construct_PairIntelligentPointerThingObjectBool_006a30d0(std::pair<CIntelligentPointer<CThingObject>,bool>*, const std::pair<CIntelligentPointer<CThingObject>,bool>&) -- STLport _Construct: placement copy-construct one pair<CIntelligentPointer<CThingObject>, bool>
// Leaf: Construct_PairIntelligentPointerThingObjectBool_006a30d0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A30D0, length 45 bytes.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90; CIntelligentPointer vtable 0x01238C6C (identical-COMDAT folded).
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
struct CIntelligentPointer : CBaseIntelligentPointer {         // CIntelligentPointer<CThingObject>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CThingObjectBoolPair {               // std::pair<CIntelligentPointer<CThingObject>, bool>
    CIntelligentPointer first;              // +0x00
    bool second;                            // +0x08
};

void __fastcall Construct_PairIntelligentPointerThingObjectBool_006a30d0(CThingObjectBoolPair* p, const CThingObjectBoolPair& value)
{
    new (p) CThingObjectBoolPair(value);
}