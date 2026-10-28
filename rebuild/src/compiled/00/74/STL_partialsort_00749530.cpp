// std::__partial_sort(CThing** first, CThing** middle, CThing** last, CThing**, CCompare_FurthestFromInitPos2D comp) -- heap-select the thing pointers the predicate orders first into [first, middle) and sort them
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) @ 0x00749530, 104 bytes, `ret 0Ch` (link-time __fastcall: first in ecx, middle in edx,
//   then last, the value-type tag pointer and the comparator on the stack).
// Callees: __make_heap @ 0x00749050 (first, middle, comp, 0, 0), CCompare_FurthestFromInitPos2D::operator() @ 0x007483E0
//   (landed; its body is repeated below as the same-TU definition, with CThing::GetPos @ 0x004C73D0),
//   __adjust_heap @ 0x00748C90 (the inlined __pop_heap), sort_heap @ 0x00749390 (out of line, `ret 4`; the manifest's
//   `_Sort_heap<long*>` is a propagation).
// Label: the manifest's "IntrosortPhase_FinalHeapSort" is descriptive; the body is STLport `__partial_sort` over the
//   CThing* elements of the CCompare_FurthestFromInitPos2D sort family (sibling of the 0x2C-entry 0x00759420).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, T(*i), comp, DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` -- the comparator
//   dword is loaded once into ebp for every by-value pass, the predicate runs on its argument slot; inside the inlined
//   __pop_heap the value is read before `*i = *first` and the length is (middle - first) >> 2. The comparator dword
//   only stays in ebp (with `last` re-read from its slot) when the comparator's operator() body is compiled EARLIER in
//   the TU (retail 0x007483E0 is part of this sort TU; its landed body is reproduced below under `auto_inline(off)`);
//   with an external declaration VC7.1 keeps `last` in ebp and re-reads the comparator (108v104).
class C3DVector { public:
    float X, Y, Z;
};

class C2DVector { public:
    float X, Y, Z;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLengthSquared() const { float r = Y * Y; r += X * X; return r; }
};

class CThing { public:
    unsigned char _pad00[0x6c];
    unsigned char m_flags6c;                    // +0x6c bit 3: creature
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
};

class CThingCreatureBase : public CThing { public:
    unsigned char _pad92[0xe8 - 0x92];
    float InitialPosX;                          // +0xe8
    float InitialPosY;                          // +0xec
    C2DVector GetOffsetFromInitialPos2D() const
    {
        const C3DVector& position = GetPos();
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
void __fastcall __make_heap(CThing** first, CThing** last, CCompare_FurthestFromInitPos2D comp, CThing**, int*);          // 0x00749050
void __fastcall __adjust_heap(CThing** first, int holeIndex, int len, CThing* val, CCompare_FurthestFromInitPos2D comp);  // 0x00748C90
void __fastcall sort_heap(CThing** first, CThing** last, CCompare_FurthestFromInitPos2D comp);                            // 0x00749390

inline void make_heap(CThing** first, CThing** last, CCompare_FurthestFromInitPos2D comp)
{
    __make_heap(first, last, comp, (CThing**)0, (int*)0);
}

template <class RandomAccessIter, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}

void __fastcall __partial_sort(CThing** first, CThing** middle, CThing** last, CThing**, CCompare_FurthestFromInitPos2D comp)
{
    make_heap(first, middle, comp);
    for (CThing** i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, *i, comp, (int*)0);
    sort_heap(first, middle, comp);
}
}