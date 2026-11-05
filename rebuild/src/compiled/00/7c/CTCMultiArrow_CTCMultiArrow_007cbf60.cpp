// 007cbf60 CTCMultiArrow::~CTCMultiArrow
// Flags:
// Leaf: ??1CTCMultiArrow@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp> inline void __fastcall _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    _Tp* begin() {return this->_M_start;}
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

#include "engine/CBaseClassNonCopyable.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define CThing CThingFields
#define CTCGraphicAppearance CTCGraphicAppearanceFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#undef CTCGraphicAppearance
#define CClass CClassFields
#include "engine/CClass.h"
#undef CClass
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#pragma pack(push,1)
class CClass:public CBaseObject{unsigned char unreadClass[sizeof(CClassFields)-sizeof(CBaseObject)];};
// Layout: native CThing Dead bit at91; true object/class prefix retained.
class CThing:public CClass{
 unsigned char unreadThing[0x91-sizeof(CClass)];bool Dead:1;unsigned char unreadFlags:7;
 unsigned char unreadTail[sizeof(CThingFields)-0x92];
public:bool IsDead()const{return Dead;}void Kill(bool);
};
#pragma pack(pop)
#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& p)
{
    if (p.Get() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}

class CTCBase : public CBaseClassNonCopyable { protected:
    CThing& ParentThing;
private:                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
public:
    virtual ~CTCBase();                                     // 0x00686830
};

class CSpecialAbilitiesMultiArrowDef;
class CTCMultiArrow : public CTCBase { private:             // vtable 0x0126ED7C
    CDefPointer<const CSpecialAbilitiesMultiArrowDef>   PDef;                       // +0x0C
    long                                                NumberOfShots;              // +0x10
    long                                                NumberInVolley;             // +0x14
    long                                                NumOfVolleys;               // +0x18
    long                                                SpellLevel;                 // +0x1C
    bool                                                RemovedEffect;              // +0x20
    long                                                CreateAmmoEffectCount;      // +0x24
    long                                                CreateAmmoEffectDelay;      // +0x28
    long                                                CreateAmmoEffectTimer;      // +0x2C
    std::vector<CIntelligentPointer<CThing> >           AmmoEffects;                // +0x30
    CIntelligentPointer<CThing>                         BowEffect;                  // +0x3C
    bool                                                CutSceneCancelled;          // +0x44
public:
    virtual ~CTCMultiArrow();
};

CTCMultiArrow::~CTCMultiArrow()
{
    if (IS_THING_ALIVE(BowEffect))
        BowEffect->Kill(true);
    for (long i = 0; i < (long)AmmoEffects.size(); ++i) {
        if (IS_THING_ALIVE(AmmoEffects[i]))
            (*(AmmoEffects.begin()+i))->Kill(true);
    }
}