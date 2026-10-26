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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {         // CIntelligentPointer<CThingObject>, vtable 0x0125A858
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CThingObjectBoolPair {               // std::pair<CIntelligentPointer<CThingObject>, bool>
    CIntelligentPointer<CThingObject> first;              // +0x00
    bool second;                            // +0x08
};

void __fastcall Construct_PairIntelligentPointerThingObjectBool_007ca600(CThingObjectBoolPair* p, const CThingObjectBoolPair& value)
{
    new (p) CThingObjectBoolPair(value);
}