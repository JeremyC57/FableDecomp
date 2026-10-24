// std::sort_heap(SThingPointerPair* first, SThingPointerPair* last, CKeyPairCompareLess comp) -- SGI sort_heap over a (long, CIntelligentPointer<CThing>) vector map: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A38C0, length 140 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,C3DHelperPoint>*,...>` is propagated over the
// byte-identical skeleton. The callee 0x006A34A0 is the SGI `__pop_heap` of a 12-byte pair (`ret 18h`: result,
// the 12-byte value, the comparator, the Distance*); it forwards to the manifest's
// `_Adjust_heap<pair<long,CIntelligentPointer<CThing>>*,...>` @ 0x006A3300 and destroys its value through
// ~CBaseIntelligentPointer, so the element is pair<long, CIntelligentPointer<CThing>> (stride 12).
// Callees: __pop_heap @ 0x006A34A0; CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem
// @ 0x00A01B50, ::SetPItem @ 0x00A01B90. Vtable: CIntelligentPointer 0x01238C6C (rtti-vtables.tsv).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline; the value copy is built in the argument area by the pair's implicit copy constructor:
// the key dword, then CIntelligentPointer's user copy constructor (base ctor, derived vtable store,
// `SetPItem(other.GetPItem())`). Pointer model as in the landed CThingSoundEmitter sources.
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                       // +0x04
    CBaseIntelligentPointer();                           // 0x00A01B10
    ~CBaseIntelligentPointer();                          // 0x00A01C10
    CThing* GetPItem() const;                            // 0x00A01B50
    void SetPItem(CThing* item);                         // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct SThingPointerPair {                  // std::pair<long, CIntelligentPointer<CThing>> (12 bytes)
    long                     first;         // +0x00
    CIntelligentPointerThing second;        // +0x04
};
struct CKeyPairCompareLess {                // empty, passed by value
    bool operator()(const SThingPointerPair& a, const SThingPointerPair& b) const { return a.first < b.first; }
};

void __fastcall __pop_heap(SThingPointerPair* first, SThingPointerPair* last, SThingPointerPair* result,
                           SThingPointerPair value, CKeyPairCompareLess comp, int*);   // 0x006A34A0

inline void __pop_heap_aux(SThingPointerPair* first, SThingPointerPair* last, SThingPointerPair*,
                           CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SThingPointerPair(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(SThingPointerPair* first, SThingPointerPair* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (SThingPointerPair*)0, comp);
}

void __fastcall sort_heap(SThingPointerPair* first, SThingPointerPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}