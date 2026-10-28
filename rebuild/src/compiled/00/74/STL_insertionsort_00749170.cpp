// std::__insertion_sort(CThing** first, CThing** last, CCompare_FurthestFromInitPos2D comp) -- insertion sort of thing pointers: a value ordered before the first shifts the run up with memmove, anything else goes through the unguarded insert
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) @ 0x00749170, 113 bytes, `ret 4` (link-time __fastcall: first in ecx, last in
//   edx, the comparator on the stack).
// Callees: CCompare_FurthestFromInitPos2D::operator()(CThing*, CThing*) const @ 0x007483E0 (landed; its body is repeated
//   below as the same-TU definition, with CThing::GetPos @ 0x004C73D0), memmove @ 0x00BFEAE6
//   (MSVCR71 import thunk), __unguarded_linear_insert(last, val, comp) @ 0x00748810 (__fastcall, out of line, `ret 4`).
// Callers: the landed sort 0x00749740 (its inlined __final_insertion_sort calls this address).
// Label: the manifest's "Vector_InsertSorted" is descriptive; the body is SGI/STLport `__insertion_sort` (3-argument form).
// Shape: `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, VALUE_TYPE, comp)`
//   with __linear_insert inline: `T val = *i; if (comp(val, *first)) { copy_backward(first, i, i + 1); *first = val; }
//   else __unguarded_linear_insert(i, val, comp);` -- copy_backward of pointers is STLport's __copy_trivial_backward
//   (`num = (char*)last - (char*)first; num > 0 ? memmove(result - num, first, num) : result`), the byte count
//   strength-reduced into edi; the predicate runs on the comparator argument slot itself (`lea ecx,[esp+20h]`) and
//   the out-of-line insert gets the argument dword pushed again. STLport's by-value comparator of the inlined
//   __linear_insert only folds onto the argument slot when the comparator's operator() body is compiled EARLIER in the
//   TU (retail 0x007483E0 is part of this sort TU; its landed body is reproduced below under `auto_inline(off)`); with
//   an external declaration VC7.1 copies the empty functor into a second frame slot (125v113).
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

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
inline void* __copy_trivial_backward(const void* first, const void* last, void* result)
{
    const int num = (const char*)last - (const char*)first;
    return (num > 0) ? memmove((char*)result - num, first, num) : result;
}

inline CThing** copy_backward(CThing** first, CThing** last, CThing** result)
{
    return (CThing**)__copy_trivial_backward(first, last, result);
}

void __fastcall __unguarded_linear_insert(CThing** last, CThing* val, CCompare_FurthestFromInitPos2D comp);   // 0x00748810

template <class RandomAccessIter, class T, class Compare>
inline void __linear_insert(RandomAccessIter first, RandomAccessIter last, T*, Compare comp)
{
    T val = *last;
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

void __fastcall __insertion_sort(CThing** first, CThing** last, CCompare_FurthestFromInitPos2D comp)
{
    if (first == last)
        return;
    for (CThing** i = first + 1; i != last; ++i)
        __linear_insert(first, i, (CThing**)0, comp);
}
}