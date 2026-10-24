// _global::Construct_PairIntelligentPointerThingTCBase_004cc3c0(std::pair<CIntelligentPointer<CThing>,CTCBase*>*, const std::pair<CIntelligentPointer<CThing>,CTCBase*>&) -- STLport _Construct: placement copy-construct one map/vector element
// Leaf: Construct_PairIntelligentPointerThingTCBase_004cc3c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CC3C0, length 45 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Shape: /Gr TU, so the free template is __fastcall (destination in ecx, source in edx). `new (p) T(v)` makes VC test the
// placement result for null (`test esi,esi ; je`), then the implicit pair copy constructor runs inline: the
// CIntelligentPointer copy constructor (base default ctor out of line, own vtable store, SetPItem(other.GetPItem()),
// the same inline shape as the landed 0x008C7D80 model) and the CTCBase* word.
// The manifest label `_Cons_val<allocator<pair<CIntelligentPointer<CThing>,CTCBase*>>>` is propagated; the body is STLport
// _Construct (no allocator `this`).
inline void* operator new(unsigned int, void* p) { return p; }

struct CThing;
struct CTCBase;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {         // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CThingTCPair {                       // std::pair<CIntelligentPointer<CThing>, CTCBase*>
    CIntelligentPointer first;              // +0x00
    CTCBase* second;                        // +0x08
};

void __fastcall Construct_PairIntelligentPointerThingTCBase_004cc3c0(CThingTCPair* p, const CThingTCPair& value)
{
    new (p) CThingTCPair(value);
}