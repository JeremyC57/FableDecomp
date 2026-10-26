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
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {         // CIntelligentPointer<CThingObject>, vtable 0x01238C6C
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CThingObjectBoolPair {               // std::pair<CIntelligentPointer<CThingObject>, bool>
    CIntelligentPointer<CThing> first;              // +0x00
    bool second;                            // +0x08
};

void __fastcall Construct_PairIntelligentPointerThingObjectBool_006a30d0(CThingObjectBoolPair* p, const CThingObjectBoolPair& value)
{
    new (p) CThingObjectBoolPair(value);
}