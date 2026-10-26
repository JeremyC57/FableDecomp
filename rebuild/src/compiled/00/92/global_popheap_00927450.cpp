// _global::__pop_heap(CIntelligentPointer<const CThing>* first, CIntelligentPointer<const CThing>* last, CIntelligentPointer<const CThing>* result, CIntelligentPointer<const CThing> value, bool (__fastcall* comp)(CIntelligentPointer<const CThing>, CIntelligentPointer<const CThing>), int*) -- SGI pop_heap core: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00927450, length 92 bytes (+4 int3 padding). The catalog label
// "CTCInventoryBase::GetItemDescription" is propagated and wrong: the SGI __pop_heap of 0x008081B0 instantiated for
// CIntelligentPointer<const CThing> (vtable 0x01237F1C) with a `__fastcall` function-pointer comparator taking both pointers by value
// (manifest name of the callee 0x00926850: "_Adjust_heap<CIntelligentPointer<CThing const>*,int,...,bool (__fastcall*)(...)>").
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90, ::CBaseIntelligentPointer() @ 0x00A01B10,
// ::~CBaseIntelligentPointer @ 0x00A01C10, __adjust_heap @ 0x00926850. Callers 0x00927577 / 0x00927727.
// Shape: identical to 0x008081B0 (speed TU, /Gr __fastcall, __forceinline copy constructor and assignment).
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                        // +4
    CBaseIntelligentPointer();                   // 0x00A01B10
    ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CBaseObject* item);            // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(const CIntelligentPointerConstThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other) { SetPItem(other.GetPItem()); return *this; }
};
typedef bool (__fastcall* TThingCompare)(CIntelligentPointerConstThing, CIntelligentPointerConstThing);
void __fastcall __adjust_heap(CIntelligentPointerConstThing* first, long holeIndex, long len,
                              CIntelligentPointerConstThing value, TThingCompare comp);   // 0x00926850
void __fastcall __pop_heap(CIntelligentPointerConstThing* first, CIntelligentPointerConstThing* last, CIntelligentPointerConstThing* result,
                           CIntelligentPointerConstThing value, TThingCompare comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}