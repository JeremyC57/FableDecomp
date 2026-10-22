// Grade: DIFFER(679v664) -- first-pass model: frame 0x20 v 0xc, levels kept in ebx (retail ebp), and the inline STLport push_back/_M_insert_overflow and the grid's cell resize schedule differently.
// CEngineScene::ResizeGrid(long levels) -- grow or shrink the scene's level grids: append new CEngineSceneGrid levels (2^level cells per side, cell table sized side*side) linked to the previous level, or detach and drop levels from the top
// Leaf: ResizeGrid
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B94540, length 672 bytes, `ret 4`.
// Callees: operator new 0x00BFEA1A, CBase::CBase 0x0099A2F0, memmove 0x00BFEAE6, vector<CSceneCell*>::_M_fill_insert 0x00B97250,
//   count-block destroy fn 0x00B94F90, malloc 0x00BFEA0E / free 0x00BFEA14, _Destroy range 0x00B96920, CEngineSceneGrid::LinkToParent
//   0x00B93860, CEngineSceneGrid::Clear 0x00B91F00, operator delete 0x00BFE9BC.
// Layout: local model; the grid vector is a 12-byte STLport vector of 8-byte counted pointers at +0x18.
#include <string.h>
#include <stdlib.h>
#include <new>

struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
void __fastcall DestroySceneGrid(void* grid);      // 0x00B94F90

struct CBase {
    virtual void VSlot0();
    CBase();                                       // 0x0099A2F0
};
struct CSceneCell;
typedef CSceneCell* PCell;
static __forceinline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
struct CCellVector {
    PCell* _M_start;
    PCell* _M_finish;
    PCell* _M_end_of_storage;
    CCellVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    unsigned long size() const { return _M_finish - _M_start; }
    void _M_fill_insert(PCell* position, unsigned long count, const PCell& value);   // 0x00B97250
    __forceinline void resize(unsigned long newSize, const PCell& value)
    {
        if (newSize < size()) {
            _M_finish = (PCell*)__copy_trivial(_M_finish, _M_finish, _M_start + newSize);
        } else
            _M_fill_insert(_M_finish, newSize - size(), value);
    }
};
struct CEngineSceneGrid : CBase {                  // vtable 0x012A3E58
    long              Level;                       // +0x04
    CEngineSceneGrid* Parent;                      // +0x08
    CEngineSceneGrid* Child;                       // +0x0c
    long              Side;                        // +0x10
    CCellVector       Cells;                       // +0x14
    __forceinline CEngineSceneGrid(long level) : Parent(0), Child(0)
    {
        Level = level;
        Side = 1 << level;
        PCell none = 0;
        Cells.resize(Side * Side, none);
    }
    virtual void VSlot1();
    void LinkToParent();                           // 0x00B93860
    void Clear();                                  // 0x00B91F00
};
struct CGridPointer {
    CEngineSceneGrid* m_pObject;
    SCountBlock*      m_pCount;
    __forceinline CGridPointer(CEngineSceneGrid* object) : m_pObject(object), m_pCount(0)
    {
        if (object) {
            SCountBlock* block = new SCountBlock;
            if (block) {
                block->m_references = 1;
                block->m_pfnDestroy = DestroySceneGrid;
                block->m_pObject = object;
            }
            m_pCount = block;
        }
    }
    __forceinline CGridPointer(const CGridPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    __forceinline ~CGridPointer()
    {
        if (m_pCount) {
            if (--m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
    }
    __forceinline void Release()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
void __fastcall DestroyGridRange(CGridPointer* first, CGridPointer* last, const void* tag);   // 0x00B96920
struct CGridVector {
    CGridPointer* _M_start;
    CGridPointer* _M_finish;
    CGridPointer* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    __forceinline void _M_insert_overflow(CGridPointer* position, const CGridPointer& x, unsigned long fillLen)
    {
        unsigned long oldSize = size();
        unsigned long len = oldSize + (fillLen > oldSize ? fillLen : oldSize);
        CGridPointer* newStart = len ? (CGridPointer*)malloc(len * sizeof(CGridPointer)) : 0;
        CGridPointer* newFinish = newStart;
        for (CGridPointer* p = _M_start; p != position; ++p, ++newFinish)
            if (newFinish)
                new (newFinish) CGridPointer(*p);
        if (newFinish)
            new (newFinish) CGridPointer(x);
        ++newFinish;
        DestroyGridRange(_M_start, _M_finish, &fillLen);
        if (_M_start)
            free(_M_start);
        _M_start = newStart;
        _M_finish = newFinish;
        _M_end_of_storage = newStart + len;
    }
    __forceinline void push_back(const CGridPointer& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) CGridPointer(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, 1);
    }
    __forceinline void pop_back()
    {
        --_M_finish;
        _M_finish->Release();
    }
};

struct CEngineScene {
    unsigned char _pad00[0x18];
    CGridVector   Grids;                           // +0x18
    void ResizeGrid(long levels);
};

void CEngineScene::ResizeGrid(long levels)
{
    if (levels < 1)
        levels = 1;
    while (Grids.size() < (unsigned long)levels) {
        long level = Grids.size();
        CGridPointer grid(new CEngineSceneGrid(level));
        Grids.push_back(grid);
        CEngineSceneGrid* parent = Grids._M_start[level - 1].m_pObject;
        grid.m_pObject->Parent = parent;
        parent->Child = grid.m_pObject;
        parent->LinkToParent();
    }
    while (Grids.size() > (unsigned long)levels) {
        unsigned long top = Grids.size() - 1;
        CEngineSceneGrid* below = Grids._M_start[top - 1].m_pObject;
        Grids._M_start[top].m_pObject->Clear();
        below->Child = 0;
        Grids.pop_back();
    }
}
