// CTCTavernGame::ClearBox() -- release every box primitive handle (telling the primitive when the last handle goes), then empty the box-handle vector
// Leaf: ClearBox
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EC5C0, length 136 bytes (padded row 144).
// Callees: the primitive's virtual slot 3 (+0xC, passed the handle, when this is the last handle) and slot 1 (destroy, at refcount 0); STLport
// __copy<CEnginePrimitiveHandle*>(first, last, result, const random_access_iterator_tag&, int*) @ 0x00440260 (__fastcall, `ret 0Ch`;
// manifest "_Move<CEnginePrimitiveHandle*,CEnginePrimitiveHandle*>"); CEnginePrimitiveHandle virtual destructor (slot 0, explicit call with
// flags 0) per erased element.
// Layout (retail; PDB 16-byte vectors are 12 here): BoxHandles (vector<CEnginePrimitiveHandle>) +0x50/+0x54/+0x58 (PDB +0x5C).
// CEnginePrimitiveHandle (8 bytes): vptr + the counted engine primitive (count at +4).
// Shape: the handle reset is an inline member (`if (GetRefCount() == 1) P->slot3(this); if (p = P) { release; P = 0; } P = 0;` with the
// null-safe GetRefCount -- `P && P->count == 1` hoists the 1 into ebp -- and the release through a pointer local); the clear is
// the inline STLport `erase(begin(), end())`: `__copy(last, finish, first, tag, (int*)0)`, the range destroy, then `finish = i`.
struct CEnginePrimitiveHandle;
struct CEnginePrimitive {
    virtual void v00();
    virtual void Destroy();                                     // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0xc)
    long RefCount;                                              // +0x04
};
struct CEnginePrimitiveHandle {
    virtual ~CEnginePrimitiveHandle();
    CEnginePrimitive* P;                                        // +0x04
    long GetRefCount() const { return P ? P->RefCount : 0; }
    void Reset()
    {
        if (GetRefCount() == 1)
            P->OnLastHandleReleased(this);
        if (CEnginePrimitive* p = P) {
            if (--p->RefCount == 0)
                p->Destroy();
            P = 0;
        }
        P = 0;
    }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
CEnginePrimitiveHandle* __fastcall __copy(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last, CEnginePrimitiveHandle* result,
                                          const random_access_iterator_tag& tag, int* distance);   // 0x00440260
struct CHandleVector {                          // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    CEnginePrimitiveHandle* begin() { return _M_start; }
    CEnginePrimitiveHandle* end() { return _M_finish; }
    static void _Destroy(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last)
    {
        for (; first != last; ++first)
            first->~CEnginePrimitiveHandle();
    }
    CEnginePrimitiveHandle* erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last)
    {
        CEnginePrimitiveHandle* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
struct CTCTavernGame {
    void* __vfptr;                              // +0x00 (CTCBase)
    void* ParentThing;                          // +0x04
    unsigned char _pad08[0x44 - 0x08];
    CHandleVector Handles;                      // +0x44
    CHandleVector BoxHandles;                   // +0x50
    void ClearBox();
};

void CTCTavernGame::ClearBox()
{
    for (CEnginePrimitiveHandle* h = BoxHandles.begin(); h != BoxHandles.end(); ++h)
        h->Reset();
    BoxHandles.clear();
}