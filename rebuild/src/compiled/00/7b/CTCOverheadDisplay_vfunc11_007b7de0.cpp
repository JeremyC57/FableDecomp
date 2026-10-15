// CTCOverheadDisplay::vfunc_11(long) -- clears PrimitiveHandles (vector<CEnginePrimitiveHandle> at +0x1c: erase of the whole [first, last) range); the argument is unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B7DE0, length 19 bytes.
// Callees: vector<T>::erase(first, last) @ 0x00440D70 (Vector_EraseRange, a __fastcall taking two element pointers).
// Shape: the inlined clear() pushes m_pLast then m_pFirst (right to left) and forms `this` of the vector with
// `add ecx,1Ch`; the erase result is dropped and the `ret 4` of the unused argument blocks a tail jump.
struct CEnginePrimitiveHandle;

struct CRetailHandleVector {                         // VC6 Dinkumware vector: three words
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    CEnginePrimitiveHandle* erase(CEnginePrimitiveHandle* first, CEnginePrimitiveHandle* last);   // 0x00440D70
    void clear() { erase(m_pFirst, m_pLast); }
};

struct CTCBase_007b7de0 {
    virtual void v00();
    unsigned char _pad04[8];                        // +0x04 ParentThing, +0x08 flags
};

struct CTCOverheadDisplay : CTCBase_007b7de0 {
    unsigned char       _pad0c[0x1c - 0x0c];       // DamageValues/RepairValues/RenownValues (4-byte retail lists), PDef
    CRetailHandleVector PrimitiveHandles;                   // +0x1c
    virtual void vfunc_11(long unused);
};

void CTCOverheadDisplay::vfunc_11(long)
{
    PrimitiveHandles.clear();
}