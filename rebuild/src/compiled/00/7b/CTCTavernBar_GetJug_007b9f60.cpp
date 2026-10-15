// CTCTavernBar::GetJug(unsigned long index) -- returns (by value) an intelligent pointer to the index-th jug, or a null pointer past the end
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B9F60, length 94 bytes (ret 8: hidden result + index).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem(CBaseObject*) @ 0x00A01B90,
// GetPItem() const @ 0x00A01B50. Vtable 0x0125A858 = CIntelligentPointer<CThingObject> (stored after the base constructor).
// Ego_r.pdb CTCTavernBar (0x24): Jugs (vector<CIntelligentPointer<CThingObject>>) +0x0c, Barman +0x1c; the retail vector is
// 12 bytes (first +0x0c, last +0x10, end +0x14) with 8-byte elements (`sar eax,3`).
// Shape: the result is built in the caller's slot through the pointer's inline converting constructor (null path, the
// fall-through after the unsigned `index >= size()` test) or its inline copy constructor (`Jugs[index]`, first loaded
// into edi before the base constructor call). The element is reached through the retail vector's iterator OBJECT
// (`*(begin() + n)`, the landed CRetailVector of 0x004AF3C0): a raw `first[index]` forms the element address before the
// constructor and computes size() first-load-first (94v94); the iterator keeps index/first in ebx/edi and re-derives
// `lea ecx,[edi+ebx*8]` after the base constructor.
struct CBaseObject;
struct CThingObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                    // +0x04
    CBaseIntelligentPointer();               // 0x00A01B10
    CBaseObject* GetPItem() const;           // 0x00A01B50
    void SetPItem(CBaseObject* item);        // 0x00A01B90
};

template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0125A858 for T = CThingObject
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};

template<class T> struct CRetailVector {     // retail std::vector: first, last, end (landed 0x004AF3C0 model)
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) {}
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CTCTavernBar {
    void*         __vftable;                 // +0x00
    unsigned char _pad_0x04[8];              // CTCBase
    CRetailVector<CIntelligentPointer<CThingObject> > Jugs;   // +0x0c
    CIntelligentPointer<CThingObject> GetJug(unsigned long index);
};

CIntelligentPointer<CThingObject> CTCTavernBar::GetJug(unsigned long index)
{
    if (index >= Jugs.size())
        return CIntelligentPointer<CThingObject>(0);
    return Jugs[index];
}