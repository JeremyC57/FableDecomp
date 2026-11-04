// CSBCCorridor::CreateLineList(std::vector<C3DVector>& lines) -- append both end points of every corridor wall (z = 0) to a debug line list
// Leaf: ?CreateLineList@CSBCCorridor@@UAEXAAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A887B0, length 211 bytes (virtual, `ret 4`).
// Callees: vector<C3DVector>::_M_insert_overflow(pos, x, __true_type, 1, true) @ 0x0066E370.
// Layout: as the landed CSBCCorridor::Reset 0x00A88080 -- Walls is the STLport list<CWall> sentinel pointer at +0x18;
//   CWall (Ego_r.pdb) is Point1, Point2, ToInternal (C2DVector each).
// Shape: per wall two C3DVector locals of the end points with z = 0, then two inlined STLport push_backs (placement copy at
//   the finish, or the overflow path). The read-only same-TU _M_insert_overflow body (GOTCHAS 00a26d40 rule: the locals'
//   addresses do not escape) lets VC7.1 hoist the z = 0 stores above the loop and forward z from the zero register.
class C2DVector {
public:
    float X, Y;
};
#include "engine/C3DVector.h"
class __true_type { public: __true_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {      // STLport: start, finish, end of storage
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x0066E370
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
template <class T, class A> T* GrowVectorStorage_0066e370(vector<T, A>* v, unsigned int n);
#pragma auto_inline(off)
template <class T, class A> __declspec(noinline) void vector<T, A>::_M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend)
{
    T* p = GrowVectorStorage_0066e370(this, fill_len);
    new (p) T(x);
    if (atend)
        _M_finish = p + fill_len;
}
#pragma auto_inline(on)
}
class CWall {                                  // 24 bytes (Ego_r.pdb)
public:
    C2DVector Point1;                          // +0x00
    C2DVector Point2;                          // +0x08
    C2DVector ToInternal;                      // +0x10
};
struct CRetailWallList {                       // STLport list<CWall>: the sentinel node pointer
    struct node {
        node* next;                            // +0x00
        node* prev;                            // +0x04
        CWall value;                           // +0x08
    };
    node* m_pNode;
};
class CSBCCorridor {                           // retail layout (STLport list)
public:
    virtual void CreateLineList(std::vector<C3DVector>& lines);
    unsigned char   _base[0x14];               // +0x04 steering-behaviour base members
    CRetailWallList Walls;                     // +0x18
};

void CSBCCorridor::CreateLineList(std::vector<C3DVector>& lines)
{
    for (CRetailWallList::node* n = Walls.m_pNode->next; n != Walls.m_pNode; n = n->next) {
        C3DVector start(n->value.Point1.X, n->value.Point1.Y, 0.0f);
        C3DVector end(n->value.Point2.X, n->value.Point2.Y, 0.0f);
        lines.push_back(start);
        lines.push_back(end);
    }
}