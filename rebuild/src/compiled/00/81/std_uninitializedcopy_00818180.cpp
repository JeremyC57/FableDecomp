// std::__uninitialized_copy(SParticleMorphRecord* first, SParticleMorphRecord* last, SParticleMorphRecord* result, const __false_type&) -- STLport placement-copy of a range of 28-byte particle-morph records holding two ref-counted definition pointers
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe @ 0x00818180, 96 bytes, `ret 8`: __fastcall (the algorithm TUs are /Gr), first in ecx, last in edx;
// the destination and the const __false_type& tag on the stack; returns the destination end in eax.
// Callers: 0x00818D07 and 0x00818D80, the vector insert/grow 0x00818CA0 reached from push_back 0x00818DC0, whose only
// caller is CParticleMorphs_PopulateFromDefinitions (0x00816B73): the pushed record is { long id, def pointer,
// def pointer, two zeroed dwords, bool = 1, a dword read from def +0x70 }, each def pointer add-ref'd through +4.
// Label: catalogued `_Move<CTVertexLandscapeBackground*,CTVertexLandscapeBackground*>`, a propagated name (GOTCHA 1097:
// `ret 8` + placement null test = __uninitialized_copy). The record's real (nested, PDB-stripped) type is not
// recoverable; it is modelled from the bytes.
// Shape: STLport `__uninitialized_copy(first, last, result, __false_type)` -- `for (; first != last; ++first, ++cur)
// _Construct(&*cur, *first); return cur;` -- throw() placement new (null test), the record's implicit memberwise copy
// constructor (the two counted pointers add-ref with a null test: `inc dword ptr [esi+4]`).
inline void* operator new(unsigned int, void* where) throw() { return where; }
class CRefCountedDef {                            // ref-counted definition: vptr, RefCount at +4
public:
    virtual void v00();
    long RefCount;                                // +0x4
};
class CRefDefPointer {                            // 4-byte intrusive pointer, add-refs on copy
public:
    CRefCountedDef* PDef;
    CRefDefPointer(const CRefDefPointer& o) : PDef(o.PDef) { if (PDef) ++PDef->RefCount; }
};
struct SParticleMorphRecord {                     // 0x1C bytes (implicit memberwise copy constructor)
    long           m_id;                          // +0x00
    CRefDefPointer m_def0;                        // +0x04
    CRefDefPointer m_def1;                        // +0x08
    long           m_0c;                          // +0x0C
    long           m_10;                          // +0x10
    bool           m_14;                          // +0x14
    long           m_18;                          // +0x18
};
namespace std {
struct __false_type { __false_type() { } };
template <class T1, class T2>
inline void _Construct(T1* p, const T2& val) { new (p) T1(val); }

template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template SParticleMorphRecord* __fastcall __uninitialized_copy(SParticleMorphRecord*, SParticleMorphRecord*,
                                                               SParticleMorphRecord*, const __false_type&);
}