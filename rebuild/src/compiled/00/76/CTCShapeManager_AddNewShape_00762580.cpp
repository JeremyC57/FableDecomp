// CTCShapeManager::AddNewShape(CCountedPointer<NShape::CShape> shape) -- notify the shape (slot 5), then insert a type-1 shape at the front of PersistantShapes (selecting the first) or append any other (selecting the last); the by-value pointer is released on exit
// Leaf: AddNewShape
// Retail: Fable.exe 0x00762580, 148 bytes (`ret 8`).
// Callees: NShape::CShape virtual slot 5 (+0x14, no arguments) and slot 1 (+0x04, shape type as a long),
// vector<CCountedPointer<NShape::CShape>>::insert(iterator, const T&) @ 0x007658B0 (manifest
// "Vector_Insert_CCountedPointer_MoveAndRealloc"), _M_insert_overflow(pos, x, __false_type, 1, true) @ 0x007656D0
// (manifest "Vector_Reserve_AndCopy"), the count block's destroy member pointer, operator delete @ 0x00BFE9BC.
// Layout: retail CTCShapeManager: PersistantShapes (STLport vector, 8-byte CCountedPointer elements) +0x0C,
// SelectedShape +0x18. Count block {references +0, destroy +4, item +8}.
// Shape: STLport push_back inline (placement copy with its null test, `add [ecx+4],8`); the overflow's
// __false_type() temporary lands in the shape argument's own slot.
struct __false_type { __false_type() {} };
inline void* operator new(unsigned int, void* where) { return where; }
void __cdecl operator delete(void* block);          // 0x00BFE9BC

namespace NShape {
struct CShape {
    virtual void v00();
    virtual long GetShapeType() const;              // slot 1 (+0x04)
    virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnAddedToManager();                // slot 5 (+0x14)
    void DeleteData();
};
}
template <class T> struct SCountBlock {
    long m_references;                              // +0x00
    void (T::*m_pfnDestroy)();                      // +0x04
    T*   m_pItem;                                   // +0x08
};
template <class T> struct CCountedPointer {
    T* Data;                                        // +0x00
    SCountBlock<T>* Info;                           // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info) ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->m_references == 0) {
            (Info->m_pItem->*Info->m_pfnDestroy)();
            operator delete(Info);
        }
    }
    T* operator->() const { return Data; }
};
typedef CCountedPointer<NShape::CShape> CShapePointer;
struct CShapeVector {                               // STLport vector<CCountedPointer<NShape::CShape>>
    CShapePointer* _M_start;
    CShapePointer* _M_finish;
    CShapePointer* _M_end_of_storage;
    CShapePointer* begin() const { return _M_start; }
    CShapePointer* end() const { return _M_finish; }
    CShapePointer* insert(CShapePointer* pos, const CShapePointer& x);   // 0x007658B0
    void _M_insert_overflow(CShapePointer* pos, const CShapePointer& x, const __false_type&,
                            unsigned long fill_len, bool atend);         // 0x007656D0
    void push_back(const CShapePointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CShapePointer(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);   // stand-in: the reallocation (returns the new end slot)
#pragma auto_inline(off)
// Same-TU body of the vector growth instantiation 0x007656D0: placement copy of the value read through its const reference
// into the new storage; the empty tag is never touched.
__declspec(noinline) void CShapeVector::_M_insert_overflow(CShapePointer* pos, const CShapePointer& x, const __false_type& p2, unsigned long fill_len, bool atend)
{
    CShapePointer* p = (CShapePointer*)GrowVectorStorage_s7(this, fill_len);
    if (p)
        new (p) CShapePointer(x);
}
#pragma auto_inline(on)
void* OpenVectorSlot_s7(void* vector, long index);   // stand-in: makes room at the index (grows, moves the tail up)
#pragma auto_inline(off)
// Same-TU body of vector::insert 0x007658B0: open a slot at the position, copy-construct the value read through its const
// reference into it, return the slot.
__declspec(noinline) CShapePointer* CShapeVector::insert(CShapePointer* pos, const CShapePointer& x)
{
    long index = pos - _M_start;
    CShapePointer* slot = (CShapePointer*)OpenVectorSlot_s7(this, index);
    if (slot)
        new (slot) CShapePointer(x);
    return _M_start + index;
}
#pragma auto_inline(on)
struct CTCShapeManager {
    virtual void v00();
    void*          ParentThing;                     // +0x04 (CTCBase)
    bool           ToBeRemoved;                     // +0x08
    bool           IsInGlobalUpdateTCs;             // +0x09
    CShapeVector   PersistantShapes;                // +0x0c
    CShapePointer* SelectedShape;                   // +0x18
    void AddNewShape(CShapePointer shape);
};

void CTCShapeManager::AddNewShape(CShapePointer shape)
{
    shape->OnAddedToManager();
    if (shape->GetShapeType() == 1) {
        PersistantShapes.insert(PersistantShapes.begin(), shape);
        SelectedShape = PersistantShapes.begin();
    } else {
        PersistantShapes.push_back(shape);
        SelectedShape = PersistantShapes.end() - 1;
    }
}