// _global::_Cons_val(CIntelligentPointer<const CThing>*, const CIntelligentPointer<const CThing>&) -- STL construct helper: placement-copy-construct an intelligent thing pointer into raw vector storage (null-checked placement new)
// Flags: /Gr
// Leaf: _Cons_val
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00511f50, 39 bytes.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90.
// Shape: link-time __fastcall (/Gr STL TU): destination in ecx (esi), source in edx (edi).  The
// placement `operator new` returns its argument, so VC tests it (`test esi,esi ; je`) before the
// inlined copy constructor: base ctor, the derived vtable store 0x0123E7E0, then
// SetPItem(source.GetPItem()).  Twin of 0x004AA030 (another CIntelligentPointer<T> vtable).  Model as the landed CTCBoss::SetHelperCreature 0x007AF310.
inline void* operator new(unsigned int, void* where) { return where; }

namespace FableCons_00511f50 {

class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
};

typedef CIntelligentPointer<const CThing> CThingPointer;

void __fastcall _Cons_val(CThingPointer* ptr, const CThingPointer& value)
{
    new ((void*)ptr) CThingPointer(value);
}

}