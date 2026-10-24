// std::__destroy_aux(SAttackParticleVector* first, SAttackParticleVector* last, const __false_type&) -- destroy a range of vector<AttackParticleAndEndPoints> (pointer, two strings per entry, then the block)
// Leaf: ?__destroy_aux@std@@YIXPAU?$CRetailVector@UAttackParticleAndEndPoints@FableDestroy_0076bdd0@@@FableDestroy_0076bdd0@@0ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076BDD0, length 91 bytes, `ret 4`.
// Element: vector<AttackParticleAndEndPoints> (12 bytes), the element of CTCObjectAugmentations::AttackParticleEffects
// (Ego_r.pdb: vector<vector<AttackParticleAndEndPoints> >); the landed ClearAllParticleEffects 0x007674A0 calls this
// body for that member's clear(). AttackParticleAndEndPoints is 24 bytes: ParticleEffect (CIntelligentPointer<CThing>)
// +0x10 is destroyed first through the out-of-line base destructor (no vtable store: VC7.1 drops it in the implicit
// destructor), then the CCharStrings at +4 and +0.
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10, CCharString::~CCharString @ 0x0099EAE0,
// free @ 0x00BFEA14.
// Shape: STLport 4.x `__destroy_aux(first, last, const __false_type&)` from a /Gr TU (`__fastcall`: first in ecx, last
// in edx, the empty tag reference on the stack, `ret 4`): `for (; first != last; ++first) _Destroy(&*first);` with the
// element's implicit destructor inlined.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 4` (no destination), the element stride and the inline destructor calls name this body.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct __false_type { __false_type() { } };
namespace FableDestroy_0076bdd0 {
struct CCharString {                                    // 4 bytes: the shared string-data pointer
    void* m_pData;
    ~CCharString();                                     // 0x0099EAE0 (out of line)
};
class CThing;
struct CBaseIntelligentPointer {                        // 8 bytes: vtable + item
    virtual void v00();
    CThing* PItem;                                      // +0x04
    ~CBaseIntelligentPointer();                         // 0x00A01C10 (out of line)
};
template <class T>
inline void DestroyRange(T* first, T* last)             // STLport _Destroy(first, last) -> __destroy_aux, inlined
{
    for (; first != last; ++first)
        first->~T();
}
template <class T>
struct CRetailVector {                                  // STLport vector<T>: 12 bytes (start, finish, end of storage)
    T* _M_start;                                        // +0
    T* _M_finish;                                       // +4
    T* _M_end_of_storage;                               // +8
    ~CRetailVector()
    {
        DestroyRange(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);                             // _Vector_base: deallocate through the malloc allocator
    }
};
struct AttackParticleAndEndPoints {                     // 24 bytes (Ego_r.pdb size; first four field names as ParticleAndEndPoints)
    CCharString             StartDummy;                 // +0x00
    CCharString             EndDummy;                   // +0x04
    long                    ParticleType;               // +0x08
    long                    Unknown0c;                  // +0x0C
    CBaseIntelligentPointer ParticleEffect;             // +0x10 CIntelligentPointer<CThing>
};
typedef CRetailVector<AttackParticleAndEndPoints> SAttackParticleVector;
}
namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }
void __fastcall __destroy_aux(FableDestroy_0076bdd0::SAttackParticleVector* first, FableDestroy_0076bdd0::SAttackParticleVector* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}