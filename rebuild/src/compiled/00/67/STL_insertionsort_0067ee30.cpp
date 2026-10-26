// std::__insertion_sort(CDegradableInfo* first, CDegradableInfo* last, CDegradableDef::CIsHealth1GTHealth2 comp) -- STLport insertion sort of CDegradableDef's degradation stages by descending HealthPercentage, __linear_insert and __copy_backward inlined: a value above the first element shifts the run up one slot and goes first, anything else goes through the out-of-line __unguarded_linear_insert
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067EE30, 204 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// comparator slot on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callee: 0x0067E550 __unguarded_linear_insert(last, value, comp) (landed as global_unguardedlinearinsert_0067e550, which
// names this body as its caller at 0x0067EEDE).
// Layout (Ego_r.pdb CDegradableInfo, 28 bytes): HealthPercentage +0, EngineGraphic (CEngineGraphic) +4,
// SmashParticleEmitter +0x14, BlocksNavigation +0x18 -- the landed callee's model; CIsHealth1GTHealth2 is empty.
// Label: catalogued as `_Insertion_sort1<CDegradableInfo*, CDegradableDef::CIsHealth1GTHealth2, CDegradableInfo>`
// (Dinkumware name); the code is STLport's `__insertion_sort` with `__linear_insert(first, i, *i, comp)` inlined.
// Shape: the by-value `*i` is a 7-dword `rep movsd` into the frame; `fld ; fcomp [first] ; test ah,41h ; jne` is
// `value > *first`; here (unlike the 12-byte siblings) copy_backward's counted loop is inlined -- the count
// `(i - first) / 28` by the 0x92492493 reciprocal, one `rep movsd` per element moved down (VC7.1 leaves plain
// `inline` copy_backward / __copy_backward / __linear_insert out of line here, so all three are __forceinline); the else arm block-copies `*i`
// again into the argument area for the unguarded insert.
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

class CEngineGraphic { public:                           // 16 bytes (PDB: BankIndex, AnimStep, RenderSizeX, AdditiveAlpha, Type)
    long m_words[4];
};

class CDegradableInfo { public:                          // 28 bytes, POD
    float          HealthPercentage;              // +0x00
    CEngineGraphic EngineGraphic;                 // +0x04
    long           SmashParticleEmitter;          // +0x14
    bool           BlocksNavigation;              // +0x18
};

class CDegradableDef { public:
    class CIsHealth1GTHealth2 { public:                  // empty functor (PDB size 1), passed in a 4-byte slot
        bool operator()(const CDegradableInfo& a, const CDegradableInfo& b) const { return a.HealthPercentage > b.HealthPercentage; }
    };
};
typedef CDegradableDef::CIsHealth1GTHealth2 CIsHealth1GTHealth2;

void __fastcall __unguarded_linear_insert(CDegradableInfo* last, CDegradableInfo value, CIsHealth1GTHealth2 comp);   // 0x0067E550

__forceinline CDegradableInfo* __copy_backward(CDegradableInfo* first, CDegradableInfo* last, CDegradableInfo* result,
                                        const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

__forceinline CDegradableInfo* copy_backward(CDegradableInfo* first, CDegradableInfo* last, CDegradableInfo* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

__forceinline void __linear_insert(CDegradableInfo* first, CDegradableInfo* last, CDegradableInfo value, CIsHealth1GTHealth2 comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}

void __fastcall __insertion_sort(CDegradableInfo* first, CDegradableInfo* last, CIsHealth1GTHealth2 comp)
{
    if (first == last)
        return;
    for (CDegradableInfo* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}