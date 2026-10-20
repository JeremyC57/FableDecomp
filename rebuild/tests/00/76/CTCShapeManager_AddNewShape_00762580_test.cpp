// Behaviour test for 00762580: src/00762580.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CTCShapeManager::AddNewShape(CCountedPointer<NShape::CShape> shape)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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

// ---- stubs and driver ----
#include <cstdlib>
using namespace NShape;
static int g_added = 0, g_destroy = 0, g_grow = 0, g_open = 0; static long g_type = 0; static long g_openIndex = -1;
void CShape::v00() {} void CShape::v02() {} void CShape::v03() {} void CShape::v04() {}
long CShape::GetShapeType() const { return g_type; }
void CShape::OnAddedToManager() { ++g_added; }
void CShape::DeleteData() { ++g_destroy; }
void CTCShapeManager::v00() {}
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{ ++g_grow; CShapeVector* v = (CShapeVector*)storage; long n = v->_M_finish - v->_M_start;
  CShapePointer* s = (CShapePointer*)malloc((n + count) * sizeof(CShapePointer)); if (n) memcpy(s, v->_M_start, n * sizeof(CShapePointer));
  v->_M_start = s; v->_M_finish = s + n + count; v->_M_end_of_storage = v->_M_finish; return s + n; }
void* OpenVectorSlot_s7(void* storage, long index)
{ ++g_open; g_openIndex = index; CShapeVector* v = (CShapeVector*)storage; long n = v->_M_finish - v->_M_start;
  CShapePointer* s = (CShapePointer*)malloc((n + 1) * sizeof(CShapePointer)); memcpy(s, v->_M_start, index * sizeof(CShapePointer)); memcpy(s + index + 1, v->_M_start + index, (n - index) * sizeof(CShapePointer));
  v->_M_start = s; v->_M_finish = s + n + 1; v->_M_end_of_storage = v->_M_finish; return s + index; }
static CShapePointer& make(CShape* shape, SCountBlock<CShape>* block)
{ block->m_references = 1; block->m_pfnDestroy = &CShape::DeleteData; block->m_pItem = shape; CShapePointer* p = (CShapePointer*)malloc(sizeof(CShapePointer)); p->Data = shape; p->Info = block; return *p; }   // a never-destroyed holder: one reference
int main()
{
    static CShape a, b, c; static SCountBlock<CShape> ba, bb, bc;
    static unsigned char raw[sizeof(CTCShapeManager)]; CTCShapeManager& m = *(CTCShapeManager*)raw;
    m.PersistantShapes._M_start = m.PersistantShapes._M_finish = m.PersistantShapes._M_end_of_storage = 0;
    g_type = 0; m.AddNewShape(make(&a, &ba));
    CHECK(g_added == 1 && g_grow == 1 && m.PersistantShapes._M_finish - m.PersistantShapes._M_start == 1 && m.SelectedShape == m.PersistantShapes._M_start);
    CHECK(m.PersistantShapes._M_start[0].Data == &a && ba.m_references == 2);   // the holder and the stored copy; the by-value argument was released
    g_type = 1; m.AddNewShape(make(&b, &bb));
    CHECK(g_open == 1 && g_openIndex == 0 && m.PersistantShapes._M_start[0].Data == &b && m.PersistantShapes._M_start[1].Data == &a && m.SelectedShape == m.PersistantShapes._M_start);
    CHECK(bb.m_references == 2);
    g_type = 2; m.AddNewShape(make(&c, &bc));
    CHECK(m.PersistantShapes._M_finish - m.PersistantShapes._M_start == 3 && m.SelectedShape == m.PersistantShapes._M_start + 2 && m.PersistantShapes._M_start[2].Data == &c);
    CHECK(g_added == 3 && g_destroy == 0);
    if (fails == 0) printf("00762580_TEST PASS\n"); else printf("00762580_TEST FAIL\n");
    return fails;
}