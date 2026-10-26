// std::iter_swap(SThingEntry* a, SThingEntry* b) -- STLport iter_swap of two (long key, CIntelligentPointer<CThing>) pairs through a by-value temporary
// Leaf: ?iter_swap@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A2D40, length 114 bytes (+ int3 padding), `ret`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem
//   @ 0x00A01B90, ::~CBaseIntelligentPointer @ 0x00A01C10; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Identity: called by the vector-map sort's __unguarded_partition 0x006A3440 with ecx = first, edx = last (two
//   register arguments, no stack argument: `ret`). The catalog's `_Swap_adl<CIntelligentPointer<CThing>>` is a
//   Dinkumware spelling; retail's STL is STLport 4.x, whose `iter_swap(a, b)` forwards to the inline
//   `__iter_swap(a, b, _Tp*)`: `_Tp tmp = *a; *a = *b; *b = tmp;` -- written here as iter_swap's own body (left as
//   a separate inline template, VC7.1 keeps __iter_swap out of line and iter_swap becomes a 3-argument call).
// Element: pair<long, CIntelligentPointer<CThing> > (12 bytes; the landed _Adjust_heap 0x006A3300 / __push_heap
//   0x006A2E20 / sort_heap 0x006A38C0 model). The pair's implicit copy = key then CIntelligentPointer's user copy
//   constructor (base ctor, vtable store, SetPItem(other.GetPItem())); the implicit operator= = key move then
//   `SetPItem(other.GetPItem())`; the temporary's generated destructor calls only the base destructor.
// Shape: /Gr STL algorithm TU (__fastcall); the temporary lives in the frame (`sub esp,0Ch`).
class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {                        // 8 bytes: vptr, item block
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};

template <class K, class V> struct pair {
    K first;
    V second;
};

typedef pair<long, CIntelligentPointer<CThing> > SThingEntry;   // 12 bytes: key +0, pointer +4 (vptr) / +8 (item)

namespace std {

void __fastcall iter_swap(SThingEntry* __a, SThingEntry* __b)
{
    SThingEntry __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

}