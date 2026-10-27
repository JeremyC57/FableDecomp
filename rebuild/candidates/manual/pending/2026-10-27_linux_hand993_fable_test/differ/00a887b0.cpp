// CSBCCorridor::CreateLineList(std::vector<C3DVector>& lines) -- append both end points of every corridor wall (z = 0) to a debug line list
// Grade: DIFFER(217v211) -- retail hoists the two z = 0 stores above the loop and forwards z = 0 from the shared zero register
// Leaf: CreateLineList
// (ebx, also the placement-new null test) into both fast-path copies; ours stores z inside the loop, re-reads it in the
// copy, keeps 0x0c in ebp and opens a 0x1c frame. Named, const and member-assigned locals all gave the same body.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A887B0, length 211 bytes (virtual, `ret 4`).
// PDB: ?CreateLineList@CSBCCorridor@@UAEXAAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@Z.
// Callees: vector<C3DVector>::_M_insert_overflow(pos, x, __true_type, 1, true) @ 0x0066E370 (manifest "Vector_InsertRange").
// Layout: as the landed CSBCCorridor::Reset 0x00A88080 -- Walls is the STLport list<CWall> sentinel pointer at +0x18
// (engine/CSBCCorridor.h agrees on the offset); a CWall is two 2D end points.
// Shape: per wall two C3DVector locals are built from the end points (the loop-invariant z = 0 stores are hoisted
// above the loop, the zero register doubles as the placement-new null test), then two inlined STLport push_backs:
// a memberwise placement copy at the finish (z forwarded from the zero register) or the overflow path with the
// empty __true_type tag homed in the dead `lines` argument slot. The 16-aligned loop head is a speed TU.
#include "engine/CSBCCorridor.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct __true_type { __true_type() { } };       // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
template<class T> struct allocator { };
template<class T, class A = allocator<T> > struct vector {      // STLport: start, finish, end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);
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
}
// 0x0066E370
template<> void std::vector<C3DVector>::_M_insert_overflow(C3DVector* position, const C3DVector& x, const __true_type&,
                                                           unsigned long fill_len, bool atend);

struct CWall {
    float m_start[2];
    float m_end[2];
};

struct CRetailWallList {                       // STLport list<CWall>: the sentinel node pointer
    struct node {
        node* next;                            // +0x00
        node* prev;                            // +0x04
        CWall value;                           // +0x08
    };
    node* m_pNode;
};

struct CSBCCorridorRetail {                    // retail model of CSBCCorridor
    virtual void CreateLineList(std::vector<C3DVector>& lines);
    unsigned char   _base[0x14];               // +0x04 steering-behaviour base members
    CRetailWallList Walls;                     // +0x18
};
FABLE_STATIC_ASSERT(offsetof(CSBCCorridor, Walls) == 0x18);

void CSBCCorridorRetail::CreateLineList(std::vector<C3DVector>& lines)
{
    for (CRetailWallList::node* n = Walls.m_pNode->next; n != Walls.m_pNode; n = n->next) {
        C3DVector start(n->value.m_start[0], n->value.m_start[1], 0.0f);
        C3DVector end(n->value.m_end[0], n->value.m_end[1], 0.0f);
        lines.push_back(start);
        lines.push_back(end);
    }
}
