// CTCBattleCharge::~CTCBattleCharge() -- battle-charge ability component dtor: fade or kill the shield and both feet effects, then the member dtors in reverse order (the people-hit pointer vector, five thing pointers, the battle-charge def pointer) and ~CTCBase
// Leaf: ??1CTCBattleCharge@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A73F0, 161 bytes, tail jump to the base destructor.
// Identity: the first store is vtable 0x0126CBFC, RTTI `.?AVCTCBattleCharge@@` (rtti_bases: CTCBattleCharge : CTCBase); its
//   slot 0 is the scalar deleting dtor 0x007A9800, the only caller (call at 0x007A9803). Ego_r.pdb has
//   `??1CTCBattleCharge@@UAE@XZ`; the PDB member offsets (PTarget +0x0C, PDef +0x14, CollidedThing +0x24, PItemRemoved +0x44,
//   ShieldEffect +0x50, LeftFeetEffect +0x58, RightFeetEffect +0x60, PeopleHit +0x70) are exactly the ones used here. The
//   catalog label `CTCActionUseBed::~CTCActionUseBed` is wrong.
// Callees: CTCSpecialAbilities::FadeOrKillEffect(CIntelligentPointer<CThing>&) 0x0072ABD0 (static __fastcall; Ego_r
//   `?FadeOrKillEffect@CTCSpecialAbilities@@SIXAAV?$CIntelligentPointer@VCThing@@@@@Z`, the reference overload: ecx is the
//   pointer member's address and the body calls GetPItem on it; the CThing* overload is 0x0072AC40), free 0x00BFEA14 (the
//   vector block), ~CBaseIntelligentPointer 0x00A01C10 (six times), CTCBase::~CTCBase 0x00686830 (tail jump); virtual calls:
//   each vector element's slot-0 destructor (flag 0: STLport _Destroy), the def's slot 1 on its last release.
// Layout: Ego_r.pdb CTCBattleCharge (0x98) with the retail 12-byte vector; no engine header. CIntelligentPointer and C3DVector
//   are packed (DirectionOverrideVector at +0x2F, as in the PDB).
// Shape: user-written dtor whose body is the three FadeOrKillEffect calls; the members die in reverse declaration order.
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

class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
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

#pragma pack(push, 1)
class C3DVector { public:                                  // retail C3DVector is 1-aligned
    float X;
    float Y;
    float Z;
};
#pragma pack(pop)

class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
};

class CSpecialAbilitiesBattleChargeDef;
class CTCSpecialAbilities { public:
    static void __fastcall FadeOrKillEffect(CIntelligentPointer<CThing>& effect);   // 0x0072ABD0
};

class CTCBattleCharge : public CTCBase { public:           // vtable 0x0126CBFC
    CIntelligentPointer<CThing>                         PTarget;                    // +0x0C
    CDefPointer<const CSpecialAbilitiesBattleChargeDef> PDef;                       // +0x14
    C3DVector                                           PositionStartedFrom;        // +0x18
    CIntelligentPointer<CThing>                         CollidedThing;              // +0x24
    bool                                                StartedMoving;              // +0x2C
    bool                                                JustReleased;               // +0x2D
    bool                                                DirectionOverride;          // +0x2E
    C3DVector                                           DirectionOverrideVector;    // +0x2F
    float                                               DashDistance;               // +0x3C
    long                                                SpellLevel;                 // +0x40
    CIntelligentPointer<CThing>                         PItemRemoved;               // +0x44
    float                                               RadiusOfEffect;             // +0x4C
    CIntelligentPointer<CThing>                         ShieldEffect;               // +0x50
    CIntelligentPointer<CThing>                         LeftFeetEffect;             // +0x58
    CIntelligentPointer<CThing>                         RightFeetEffect;            // +0x60
    long                                                PoseDelay;                  // +0x68
    bool                                                StartPoseDelay;             // +0x6C
    bool                                                HitSomething;               // +0x6D
    std::vector<CIntelligentPointer<CThing> >           PeopleHit;                  // +0x70
    C3DVector                                           DisplacementVector;         // +0x7C
    bool                                                IsZTargetting;              // +0x88
    bool                                                FakeCollideWithSomething;   // +0x89
    long                                                ChargeDelayFrameCount;      // +0x8C
    bool                                                CompletedExitModes;         // +0x90
    virtual ~CTCBattleCharge();
};

CTCBattleCharge::~CTCBattleCharge()
{
    CTCSpecialAbilities::FadeOrKillEffect(ShieldEffect);
    CTCSpecialAbilities::FadeOrKillEffect(LeftFeetEffect);
    CTCSpecialAbilities::FadeOrKillEffect(RightFeetEffect);
}