// std::__unguarded_insertion_sort_aux(CThing** first, CThing** last, CThing**, CCompare_FurthestFromInitPos2D comp) -- insertion-sort pass with no lower-bound test over thing pointers ordered by the "furthest from initial position (2D)" predicate
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) @ 0x00748C10, 96 bytes, `ret 8` (link-time __fastcall: first in ecx, last in edx, the
//   value-type tag pointer and the comparator on the stack).
// Callees: CCompare_FurthestFromInitPos2D::operator()(CThing*, CThing*) const @ 0x007483E0 (landed, __fastcall `ret 8`,
//   never reads `this`; its landed body is repeated below as the same-TU definition), CThing::GetPosition @ 0x004C73D0
//   (inside that body).
// Callers: the thing-pointer sort family 0x007485xx-0x007497xx (the landed sort 0x00749740 calls this address as its
//   unguarded insertion pass).
// Label: the manifest's "Insertion_MoveElement" is descriptive; the body is STLport `__unguarded_insertion_sort_aux`.
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp)` with the linear insert
//   inlined: the value lives in ebx, `next` in esi and `last` in edi; `last` is spilled to the `push ecx` word and
//   re-read at the loop test, and the predicate's `this` (the inlined by-value copy of the EMPTY comparator) shares that
//   word (`lea ecx,[esp+18h]`). That only happens when the comparator's operator() body is compiled EARLIER in the TU
//   (retail 0x007483E0 is part of this sort TU): its landed body is reproduced below, ahead of this function, under
//   `auto_inline(off)`. With an external declaration VC7.1 byte-copies the empty functor into a separate slot (100v96),
//   and a by-reference comparator calls through the argument slot (96v96).
struct C3DVector {
    float X, Y, Z;
};

struct C2DVector {
    float X, Y, Z;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLengthSquared() const { float r = Y * Y; r += X * X; return r; }
};

struct CThing {
    unsigned char _pad00[0x6c];
    unsigned char m_flags6c;                    // +0x6c bit 3: creature
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

struct CThingCreatureBase : CThing {
    unsigned char _pad92[0xe8 - 0x92];
    float InitialPosX;                          // +0xe8
    float InitialPosY;                          // +0xec
    C2DVector GetOffsetFromInitialPos2D() const
    {
        const C3DVector& position = GetPosition();
        return C2DVector(InitialPosX - position.X, InitialPosY - position.Y);
    }
};

inline CThingCreatureBase* AsCreature(CThing* thing)
{
    CThingCreatureBase* creature = 0;
    if (thing && (thing->m_flags6c & 8))
        creature = (CThingCreatureBase*)thing;
    return creature;
}

inline bool IsAlive(const CThing* thing)
{
    return thing && !(thing->m_flags91 & 1);
}

struct CCompare_FurthestFromInitPos2D {
    bool operator()(CThing* a, CThing* b) const;
};

#pragma auto_inline(off)
bool CCompare_FurthestFromInitPos2D::operator()(CThing* a, CThing* b) const
{
    if (a == b)
        return false;
    CThingCreatureBase* creatureA = AsCreature(a);
    CThingCreatureBase* creatureB = AsCreature(b);
    if (IsAlive(creatureA) || IsAlive(creatureB)) {
        if (IsAlive(creatureA)) {
            if (!IsAlive(creatureB))
                return true;
            C2DVector offsetA = creatureA->GetOffsetFromInitialPos2D();
            C2DVector offsetB = creatureB->GetOffsetFromInitialPos2D();
            if (offsetA.GetLengthSquared() > offsetB.GetLengthSquared())
                return true;
        }
    }
    return false;
}
#pragma auto_inline(on)

namespace std {
template <class RandomAccessIter, class T, class Compare>
inline void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

void __fastcall __unguarded_insertion_sort_aux(CThing** first, CThing** last, CThing**, CCompare_FurthestFromInitPos2D comp)
{
    for (CThing** i = first; i != last; ++i)
        __unguarded_linear_insert(i, *i, comp);
}
}