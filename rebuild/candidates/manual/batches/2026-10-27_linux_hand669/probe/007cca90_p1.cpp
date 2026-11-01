// CTCMultiStrike::~CTCMultiStrike() -- multi-strike component dtor: the members in reverse order (the particle-effect pointer vector, the target and weapon pointers, the def pointer) and ~CTCBase
// Leaf: ??1CTCMultiStrike
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CBaseClassNonCopyable.h"

class CThing;
class CHitLocation;
class CSpecialAbilitiesMultiStrikeDef;
class CBaseObjectPointer;
class C3DVector { public: float X, Y, Z; };
#pragma pack(push, 1)
class CBaseIntelligentPointer {                                          // vtable 0x0129C95C, 8 bytes
public:
    CBaseIntelligentPointer();                                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                                  // 0x00A01C10
private:
    CBaseObjectPointer* PData;                                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
#pragma pack(pop)

class CTCBase : public CBaseClassNonCopyable {              // retail 0x0C bytes
public:
    CTCBase(CThing& parent);                                // 0x00686800
    virtual ~CTCBase();                                     // 0x00686830
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};

class CTCMultiStrike : public CTCBase {                     // vtable 0x0126EE0C
public:
    virtual ~CTCMultiStrike();
    CDefPointer<const CSpecialAbilitiesMultiStrikeDef>  PDef;               // +0x0C
    long                                        SpellLevel;                 // +0x10
    bool                                        StrikeStarted;              // +0x14
    long                                        DelayTimer;                 // +0x18
    long                                        NumHits;                    // +0x1C
    CHitLocation*                               PHitLocation;               // +0x20
    CIntelligentPointer<CThing>                 PWeapon;                    // +0x24
    C3DVector                                   PointOfHit;                 // +0x2C
    C3DVector                                   HitDirection;               // +0x38
    CIntelligentPointer<CThing>                 PTarget;                    // +0x44
    std::vector<CIntelligentPointer<CThing> >   ParticleEffects;            // +0x4C
};

CTCMultiStrike::~CTCMultiStrike()
{
}
