// _global::Construct_PairIntelligentPointerThingDangerStatus_00748a00(std::pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>*, const std::pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus>&) -- STLport _Construct: placement copy-construct one map node value
// Leaf: Construct_PairIntelligentPointerThingDangerStatus_00748a00
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00748A00, length 51 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CBaseIntelligentPointer::SetPItem @ 0x00A01B90; CIntelligentPointer<const CThing> vtable 0x01237F1C.
// Shape: /Gr TU (__fastcall: destination in ecx, source in edx); `new (p) T(v)` null-tests the placement result, then the
// implicit pair copy constructor inlines the CIntelligentPointer copy constructor and the two-word CDangerStatus.
// The manifest label `_Cons_val<allocator<...>>` is propagated; the body is STLport _Construct. CDangerStatus is modelled
// by its size only (two words copied memberwise at +0x08/+0x0C).
inline void* operator new(unsigned int, void* p) { return p; }

class CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {         // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
namespace NStealth {
class CDangerStatus { public:
    long m_level;                           // +0x00
    long m_time;                            // +0x04
};
}
struct CThingDangerPair {                   // std::pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>
    CIntelligentPointer first;              // +0x00
    NStealth::CDangerStatus second;         // +0x08
};

void __fastcall Construct_PairIntelligentPointerThingDangerStatus_00748a00(CThingDangerPair* p, const CThingDangerPair& value)
{
    new (p) CThingDangerPair(value);
}