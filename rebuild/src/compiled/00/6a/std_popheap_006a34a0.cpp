// std::__pop_heap(SThingEntry* first, SThingEntry* last, SThingEntry* result, SThingEntry val, CKeyPairCompareLess comp, int*) -- STLport pop step: move the heap top to result, then re-heap [first, last) around val
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A34A0, length 126 bytes (+ int3 padding), `ret 18h`.
// Callees: std::__adjust_heap @ 0x006A3300 (landed as `_Adjust_heap`), CBaseIntelligentPointer ctor @ 0x00A01B10,
//   ::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90, ::~CBaseIntelligentPointer @ 0x00A01C10; vtable 0x01238C6C.
// Identity: called by the landed sort_heap 0x006A38C0 (whose header names this address `__pop_heap`, `ret 18h`)
//   and by __partial_sort 0x006A3BF0. The catalog's "Heap_PopAndSiftDown" is a descriptive auto label; the body is
//   STLport's `*__result = *__first; __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);`.
// Element: pair<long, CIntelligentPointer<CThing> > (landed _Adjust_heap model): operator= = key move +
//   SetPItem(other.GetPItem()); the by-value copy of val is built in the argument area by the pair's implicit
//   copy constructor (key, then CIntelligentPointer's user copy ctor: base ctor, vtable store, Get/SetPItem).
// Shape: /Gr TU (__fastcall: first ecx, last edx; result, the 12-byte val, the empty comparator and the unused
//   Distance* on the stack); arguments are built right to left (comp, val, then the /12 length, hole 0 in edx).
class CThing;
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)other.GetPItem()); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

template <class K, class V> struct pair {
    K first;
    V second;
};

typedef pair<long, CIntelligentPointer<CThing> > SThingEntry;   // 12 bytes: key +0, pointer +4 (vptr) / +8 (item)

struct CKeyPairCompareLess {                            // empty comparator, passed by value
    bool operator()(const SThingEntry& a, const SThingEntry& b) const { return a.first < b.first; }
};

namespace std {

void __fastcall __adjust_heap(SThingEntry* __first, int __holeIndex, int __len, SThingEntry __val, CKeyPairCompareLess __comp);   // 0x006A3300

void __fastcall __pop_heap(SThingEntry* __first, SThingEntry* __last, SThingEntry* __result, SThingEntry __val, CKeyPairCompareLess __comp, int*)
{
    *__result = *__first;
    __adjust_heap(__first, int(0), int(__last - __first), __val, __comp);
}

}