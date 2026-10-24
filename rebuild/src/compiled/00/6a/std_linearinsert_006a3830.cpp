// std::__linear_insert(SThingEntry* first, SThingEntry* last, SThingEntry val, CKeyPairCompareLess comp) -- STLport insertion of val at last: a new minimum shifts the whole prefix up one slot, otherwise the unguarded insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3830, length 136 bytes (+ int3 padding), `ret 10h`.
// Callees: std::__copy_backward @ 0x006A2EB0 (landed as CopyBackward_PairLongThing), std::__unguarded_linear_insert
//   @ 0x006A2DC0 (this chunk), CBaseIntelligentPointer ctor @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem
//   @ 0x00A01B90, ::~CBaseIntelligentPointer @ 0x00A01C10; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Identity: called by __insertion_sort 0x006A3B90 with the element copied into the argument area. The catalog's
//   "Heap_InsertAndAdjust" is a descriptive auto label; the body is STLport's `__linear_insert` with the value
//   supplied by the caller (`if (__comp(__val, *__first)) { copy_backward(__first, __last, __last + 1);
//   *__first = __val; } else __unguarded_linear_insert(__last, __val, __comp);`).
// Element: pair<long, CIntelligentPointer<CThing> > (landed _Adjust_heap model).
// Shape: /Gr TU (__fastcall: first ecx, last edx). copy_backward is inline down to the random-access
//   `__copy_backward(first, last, result, random_access_iterator_tag(), (int*)0)`; the empty tag temporary is
//   homed in the dead comparator slot (`lea eax,[esp+1Ch]`). The else arm copies val into the argument area.
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

template <class K, class V> struct pair {
    K first;
    V second;
};

typedef pair<long, CIntelligentPointer<CThing> > SThingEntry;   // 12 bytes: key +0, pointer +4 (vptr) / +8 (item)

struct CKeyPairCompareLess {                            // empty comparator, passed by value
    bool operator()(const SThingEntry& a, const SThingEntry& b) const { return a.first < b.first; }
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

namespace std {

SThingEntry* __fastcall __copy_backward(SThingEntry* __first, SThingEntry* __last, SThingEntry* __result,
                                        const random_access_iterator_tag&, int*);   // 0x006A2EB0
void __fastcall __unguarded_linear_insert(SThingEntry* __last, SThingEntry __val, CKeyPairCompareLess __comp);   // 0x006A2DC0

inline SThingEntry* copy_backward(SThingEntry* __first, SThingEntry* __last, SThingEntry* __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __linear_insert(SThingEntry* __first, SThingEntry* __last, SThingEntry __val, CKeyPairCompareLess __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

}