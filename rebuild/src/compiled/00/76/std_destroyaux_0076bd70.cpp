// std::__destroy_aux(SParticleVector* first, SParticleVector* last, const __false_type&) -- destroy a range of vector<ParticleAndEndPoints> (two strings per entry, then the block)
// Leaf: ?__destroy_aux@std@@YIXPAU?$CRetailVector@UParticleAndEndPoints@FableDestroy_0076bd70@@@FableDestroy_0076bd70@@0ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076BD70, length 83 bytes, `ret 4`.
// Element: vector<ParticleAndEndPoints> (12 bytes), the element of CTCObjectAugmentations::ParticleEffects
// (Ego_r.pdb: vector<vector<ParticleAndEndPoints> >); the landed CTCObjectAugmentations::ClearAllParticleEffects
// 0x007674A0 calls this body for that member's clear(). ParticleAndEndPoints is 20 bytes (PeriodicFrames +0x10);
// its two CCharStrings at +0 and +4 are destroyed in reverse order (+4 first), the other words are plain.
// Callees: CCharString::~CCharString @ 0x0099EAE0, free @ 0x00BFEA14.
// Shape: STLport 4.x `__destroy_aux(first, last, const __false_type&)` from a /Gr TU (`__fastcall`: first in ecx, last
// in edx, the empty tag reference on the stack, `ret 4`): `for (; first != last; ++first) _Destroy(&*first);` with the
// element's implicit destructor inlined.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 4` (no destination), the element stride and the inline destructor calls name this body.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct __false_type { __false_type() { } };
namespace FableDestroy_0076bd70 {
struct CCharString {                                    // 4 bytes: the shared string-data pointer
    void* m_pData;
    ~CCharString();                                     // 0x0099EAE0 (out of line)
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
struct ParticleAndEndPoints {                           // 20 bytes (Ego_r.pdb size; field names as the landed 0x007674A0 model)
    CCharString StartDummy;                             // +0x00
    CCharString EndDummy;                               // +0x04
    long        ParticleType;                           // +0x08
    long        Unknown0c;                              // +0x0C
    long        PeriodicFrames;                         // +0x10
};
typedef CRetailVector<ParticleAndEndPoints> SParticleVector;
}
namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }
void __fastcall __destroy_aux(FableDestroy_0076bd70::SParticleVector* first, FableDestroy_0076bd70::SParticleVector* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}