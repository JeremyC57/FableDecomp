// CTCBerserk::~CTCBerserk() -- berserk ability component dtor: kill the live start-up and body effects, then the member dtors in reverse order (the berserk def pointer, the two effect pointers, the dependent-things pointer vector) and ~CTCBase
// Leaf: ??1CTCBerserk@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0078C080, 202 bytes, tail jump to the base destructor.
// Identity: the first store is vtable 0x0126BB94, RTTI `.?AVCTCBerserk@@` (rtti_bases: CTCBerserk : CTCBase); its slot 0 is
//   the scalar deleting dtor 0x0078DB00, the only caller (call at 0x0078DB03). Ego_r.pdb has `??1CTCBerserk@@UAE@XZ`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (six times), CThing::Kill(bool) 0x004C9B80 (twice),
//   ~CBaseIntelligentPointer 0x00A01C10 (twice), free 0x00BFEA14 (the vector block), CTCBase::~CTCBase 0x00686830 (tail jump);
//   virtual calls: the def's slot 1 on its last release, each vector element's slot-0 destructor (flag 0: STLport _Destroy).
// Layout: Ego_r.pdb CTCBerserk with the retail 12-byte vector (DependentThings +0x4C, StartUpEffect +0x58, BerserkBodyEffect
//   +0x60, PDef +0x68), the offsets of engine/CTCBerserk.h. Header debt: that header flattens the vptr and CTCBase into data
//   members (so no virtual destructor can be declared on it); a local model with the same offsets is used instead.
// Shape: user-written dtor whose body is two inline IS_THING_ALIVE + Kill(true) blocks (VC7.1 keeps the shared 1 in ebx).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();                       // 0x0099A300
};
class CBaseObjectPointer;
class CBaseObject;
class CThing { public:                                      // retail flag byte +0x91 (Dead bit 0)
    unsigned char _pad_0x00[0x91];
    bool          Dead : 1;                                 // +0x91 bit 0
    bool IsDead() const { return Dead; }
    void Kill(bool immediately);                            // 0x004C9B80
};

#pragma pack(push, 1)
class CBaseIntelligentPointer { public:                     // vtable 0x0129C95C, 8 bytes
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* Get() const { return GetPointer(); }
    T* operator->() const { return GetPointer(); }
private:
    T* GetPointer() const { return (T*)GetPItem(); }
};
#pragma pack(pop)

template <class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p)
{
    if (p.Get() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}

class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                       // +0x04
};
class CResource : public CIVCountedPointeeBase { public:
};
class CDefPointeeBase : public CResource { public:
};
template <class T> class CDefPointer { public:
    CDefPointeeBase* Object;                                // +0x00
    void Release()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    ~CDefPointer() { Release(); }
};

class C3DVector { public:
    float X;
    float Y;
    float Z;
};

class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
};

class CSpecialAbilitiesBerserkDef;
class CTCBerserk : public CTCBase { public:                // vtable 0x0126BB94
    long                                            Timer;                      // +0x0C
    long                                            DarkenEnvTimer;             // +0x10
    float                                           StrengthPercentage;         // +0x14
    float                                           MaxIncreaseInStrength;      // +0x18
    float                                           ReleasedAnimationTimer;     // +0x1C
    long                                            AnimTimer;                  // +0x20
    float                                           BerserkDamageToMe;          // +0x24
    float                                           BerserkDamageToEnemy;       // +0x28
    float                                           BerserkScale;               // +0x2C
    float                                           RumbleLevel;                // +0x30
    float                                           RumbleSmoothness;           // +0x34
    float                                           RumbleAttack;               // +0x38
    float                                           RumbleDecay;                // +0x3C
    unsigned long                                   BerserkingLoopingSound;     // +0x40
    long                                            SpellLevel;                 // +0x44
    bool                                            JustReleased;               // +0x48
    bool                                            AbilityInitialised;         // +0x49
    std::vector<CIntelligentPointer<CThing> >       DependentThings;            // +0x4C
    CIntelligentPointer<CThing>                     StartUpEffect;              // +0x58
    CIntelligentPointer<CThing>                     BerserkBodyEffect;          // +0x60
    CDefPointer<const CSpecialAbilitiesBerserkDef>  PDef;                       // +0x68
    void*                                           RadialBlurFadeKey;          // +0x6C
    bool                                            Paused;                     // +0x70
    bool                                            Pausable;                   // +0x71
    bool                                            IsInCutscene;               // +0x72
    virtual ~CTCBerserk();
};

CTCBerserk::~CTCBerserk()
{
    if (IS_THING_ALIVE(StartUpEffect))
        StartUpEffect->Kill(true);
    if (IS_THING_ALIVE(BerserkBodyEffect))
        BerserkBodyEffect->Kill(true);
}