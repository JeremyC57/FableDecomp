// _global::_Adjust_heap(std::pair<long,CIntelligentPointer<CThing> >*, long, long, std::pair<long,CIntelligentPointer<CThing> >, CKeyPairCompareLess) -- Dinkumware heap sift-down of (long key, intelligent thing pointer) pairs by signed key, then _Push_heap @ 0x006A2E20; destroys the by-value pair
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006a3300, 218 bytes.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90,
// ~CBaseIntelligentPointer 0x00A01C10, _Push_heap 0x006A2E20 (same instantiation).
// first in ecx, hole in edx (link-time __fastcall); bottom, the 12-byte pair by value and the empty
// comparator follow (`ret 14h`); `top` lives in the `push ecx` slot.
// Element: pair<long, CIntelligentPointer<CThing> > = key +0, pointer (vptr +4, item +8), as the
// label says.  Shape: the element assignment is the implicit pair operator= = key dword move +
// CIntelligentPointer::operator= (GetPItem of the source, SetPItem on the destination); the argument
// copy is the implicit pair copy constructor = key, then base ctor + vtable store 0x01238C6C +
// Get/SetPItem; the generated destructor of the pointer stores no vtable and just calls the base
// destructor.  The key test is a SIGNED `jge`.  Model as the landed CTCBoss::SetHelperCreature
// 0x007AF310 (CBaseIntelligentPointer / CIntelligentPointer<T>).
namespace FableHeap_006a3300 {

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

template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};

template<class K, class V> struct pair {
    K first;
    V second;
};

typedef pair<long, CIntelligentPointer<CThing> > SKeyedThing;

struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const SKeyedThing& a, const SKeyedThing& b) const { return a.first < b.first; }
};

void __fastcall _Push_heap(SKeyedThing* first, long hole, long top, SKeyedThing value, CKeyPairCompareLess pred);   // 0x006A2E20

void __fastcall _Adjust_heap(SKeyedThing* first, long hole, long bottom, SKeyedThing value, CKeyPairCompareLess pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}

}