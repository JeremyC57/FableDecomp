// _STL::swap(CIntelligentPointer<CThing>& a, CIntelligentPointer<CThing>& b) -- STLport swap of two intelligent thing pointers through a temporary copy
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00807370, 90 bytes (+ int3 padding), /Gr `__fastcall` (a in ecx, b in
//   edx), plain `ret`.
// Name: STLport `swap<CIntelligentPointer<CThing> >` (the iter_swap of the thing-pointer introsort under
//   CDoNotUseBinaryPredicate); the catalog's "_Swap_adl<CIntelligentPointer<CThing> >" is the Dinkumware spelling.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90,
//   ~CBaseIntelligentPointer 0x00A01C10; CIntelligentPointer<CThing> vtable 0x01238C6C (stored after the base ctor).
// Shape: STLport `_Tp __tmp = __a; __a = __b; __b = __tmp;` -- the copy constructor is the base constructor, the vtable
//   store and SetPItem(a.GetPItem()); each assignment is SetPItem(other.GetPItem()); the temporary is destroyed by a
//   direct call to the base destructor.
class CThing;
class CBaseObject;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                 // 0x00A01B10
    virtual ~CBaseIntelligentPointer();        // 0x00A01C10
    CBaseObject* GetPItem() const;             // 0x00A01B50
    void SetPItem(CBaseObject* item);          // 0x00A01B90
    void* PData;                               // +0x4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};

namespace _STL {
template <class _Tp> void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap(CIntelligentPointer<CThing>&, CIntelligentPointer<CThing>&);
}