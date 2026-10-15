// _global::__pop_heap(CIntelligentPointer<CThing>* first, CIntelligentPointer<CThing>* last, CIntelligentPointer<CThing>* result, CIntelligentPointer<CThing> value, CDoNotUseBinaryPredicate comp, int*) -- SGI pop_heap core: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008081B0, length 92 bytes. The catalog label "CTCInventoryBase::GetItemDescription"
// is propagated over a byte-identical skeleton and wrong: this is the SGI
//   __pop_heap(first, last, result, T value, Compare comp, Distance*) { *result = *first;
//                                                                      __adjust_heap(first, Distance(0), Distance(last - first), value, comp); }
// instantiated for CIntelligentPointer<CThing> (8 bytes: vtable 0x01238C6C + item) with the anonymous-namespace
// CDoNotUseBinaryPredicate (callers 0x00808437 / 0x00808CB7 are the matching pop_heap/sort_heap front ends).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90, ::CBaseIntelligentPointer() @ 0x00A01B10,
// ::~CBaseIntelligentPointer @ 0x00A01C10, __adjust_heap<CIntelligentPointer<CThing>*, int, ..., CDoNotUseBinaryPredicate> @ 0x00807520.
// Shape: the SGI algorithm TUs are /Gr (free templates are __fastcall: first in ecx, the second argument in edx, `ret 14h` for
// result + the 8-byte value + comp + Distance*); this TU is speed-optimised (`mov eax,[esp+1Ch] ; push eax` for comp, no
// "s" pass-through push). The element copy for __adjust_heap is built in the argument area by the inlined user copy
// constructor (base constructor, vtable store, SetPItem(other.GetPItem())); the assignment and the copy constructor must be
// __forceinline. The by-value `value` parameter is destroyed by this callee at the end (`lea ecx,[esp+14h] ; call ~`).
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                        // +4
    CBaseIntelligentPointer();                   // 0x00A01B10
    ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    __forceinline CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CDoNotUseBinaryPredicate { };
void __fastcall __adjust_heap(CIntelligentPointerThing* first, long holeIndex, long len,
                              CIntelligentPointerThing value, CDoNotUseBinaryPredicate comp);   // 0x00807520
void __fastcall __pop_heap(CIntelligentPointerThing* first, CIntelligentPointerThing* last, CIntelligentPointerThing* result,
                           CIntelligentPointerThing value, CDoNotUseBinaryPredicate comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}