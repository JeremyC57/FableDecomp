// CTCAssassinRush::~CTCAssassinRush() -- assassin-rush ability component dtor: fade out a still-running radial blur, then the member dtors in reverse order (the final-action counted pointer, the item-alpha pointer vector, the collided-thing and both trail pointers, the def pointer, the target pointer) and ~CTCBase
// Leaf: ??1CTCAssassinRush@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A9D60, 221 bytes, tail jump to the base destructor.
// Identity: the first store is vtable 0x0126CF2C, RTTI `.?AVCTCAssassinRush@@` (rtti_bases: CTCAssassinRush : CTCBase); its
//   slot 0 is the scalar deleting dtor 0x007AB5E0. Ego_r.pdb has `??1CTCAssassinRush@@UAE@XZ`. The PDB member offsets (PTarget
//   +0x0C, PDef +0x14, AssassinTrail +0x18, AssassinDisplacementTrail +0x20, CollidedThing +0x39, ItemAlphaList +0x64, then
//   less 4 for the 12-byte STLport vector: FinalAction +0x90, RadialBlurFadeKey +0x98) are the ones used here, as in the
//   landed CTCAssassinRush::FrameUpdate 0x007AB190.
// Callees: CTCBase::GetWorld 0x00686A70 (`?GetWorld@CTCBase@@IBEAAVCWorld@@XZ`), CEnvironmentSpellEffectInterpolator::
//   RadialBlurFadeOut(float, void*) 0x006B82E0 (Ego_r `?RadialBlurFadeOut@CEnvironmentSpellEffectInterpolator@@QAEXMPAX@Z`;
//   reached through the inline CWorld::GetEnvironment() (scoped_ptr +0x18) and CEnvironment::GetSpellEffectInterpolator()
//   (scoped_ptr, retail +0x1964)), operator delete 0x00BFE9BC (the count block), free 0x00BFEA14 (the vector block),
//   ~CBaseIntelligentPointer 0x00A01C10 (four times), CTCBase::~CTCBase 0x00686830 (tail jump); virtual / indirect calls:
//   each vector element's slot-0 destructor (flag 0), the count block's DeleteFunc, the def's slot 1 on its last release.
// Layout: Ego_r.pdb CTCAssassinRush (0xAC, retail 0xA8); CIntelligentPointer and C3DVector 1-aligned; no engine header.
//   CSpecialAbilitiesAssassinRushDef: Ego_r offsets less 4 (CParentDefClassBase holds a CVectorMap, 0x14 in Ego_r, 0x10 in
//   retail): AssassinEndSeconds +0x4C (the landed FrameUpdate reads DistFromFloor +0x44 for the blur height).
// Shape: user-written dtor `if (RadialBlurFadeKey) { ...RadialBlurFadeOut(PDef->AssassinEndSeconds, RadialBlurFadeKey);
//   RadialBlurFadeKey = 0; }`; CCountedPointer's member dtor (`ReleaseData(); Data = 0; Info = 0;`).
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
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
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
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
    const T* operator->() const { return static_cast<const T*>(Object); }
    ~CDefPointer() { Release(); }
};

class CWorld;
class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
protected:
    CWorld& GetWorld() const;                               // 0x00686A70
};

#pragma pack(push, 1)
class C3DVector { public:                                  // retail C3DVector is 1-aligned
    float X;
    float Y;
    float Z;
};
#pragma pack(pop)

class CCPPointerInfo { public:                              // CCountedPointer count block
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void*         Data;                                     // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    __forceinline void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
};

template <class T> class scoped_ptr { public:
    T* ptr;
    T& operator*() const { return *ptr; }
};
class CEnvironmentSpellEffectInterpolator { public:
    void RadialBlurFadeOut(float time, void* key);          // 0x006B82E0
};
class CEnvironment { public:
    unsigned char                                   _pad_0x00[0x1964];
    scoped_ptr<CEnvironmentSpellEffectInterpolator> SpellEffectInterpolator;    // +0x1964
    CEnvironmentSpellEffectInterpolator& GetSpellEffectInterpolator() { return *SpellEffectInterpolator; }
};
class CWorld { public:
    unsigned char            _pad_0x00[0x18];
    scoped_ptr<CEnvironment> PEnvironment;                  // +0x18
    CEnvironment& GetEnvironment() { return *PEnvironment; }
};

class CDefClassBase : public CDefPointeeBase { public:
};
class CParentDefClassBase : public CDefClassBase { public:
    unsigned char _pad_0x08[0x3C - 0x08];                   // CResource / CDefClassBase / CParentDefClassBase members
};
class CSpecialAbilitiesAssassinRushDef : public CParentDefClassBase { public:
    float DistanceAwayFromtargetToAssassinRushTo;           // +0x3C
    float SafeCollideRadiusPercentage;                      // +0x40
    float DistFromFloor;                                    // +0x44
    float AssassinStartSeconds;                             // +0x48
    float AssassinEndSeconds;                               // +0x4C
};

class CCreatureActionBase;
class CTCAssassinRush : public CTCBase { public:           // vtable 0x0126CF2C
    CIntelligentPointer<CThing>                         PTarget;                        // +0x0C
    CDefPointer<const CSpecialAbilitiesAssassinRushDef> PDef;                           // +0x14
    CIntelligentPointer<CThing>                         AssassinTrail;                  // +0x18
    CIntelligentPointer<CThing>                         AssassinDisplacementTrail;      // +0x20
    long                                                TrailDelay;                     // +0x28
    bool                                                StartTrailDelay;                // +0x2C
    C3DVector                                           PositionStartedFrom;            // +0x2D
    CIntelligentPointer<CThing>                         CollidedThing;                  // +0x39
    bool                                                StartedMoving;                  // +0x41
    bool                                                JustReleased;                   // +0x42
    long                                                SlowTimeLength;                 // +0x44
    bool                                                DirectionOverride;              // +0x48
    C3DVector                                           DirectionOverrideVector;        // +0x49
    C3DVector                                           BehindTargetPos;                // +0x55
    std::vector<CIntelligentPointer<CThing> >           ItemAlphaList;                  // +0x64
    float                                               DashDistance;                   // +0x70
    C3DVector                                           LocalRelativeVectorForCamera;   // +0x74
    C3DVector                                           RelativeVectorForCamera;        // +0x80
    long                                                SpellLevel;                     // +0x8C
    CCountedPointer<CCreatureActionBase>                FinalAction;                    // +0x90
    void*                                               RadialBlurFadeKey;              // +0x98
    bool                                                UseCollision;                   // +0x9C
    bool                                                IsZTargetting;                  // +0x9D
    bool                                                ScheduleForCancel;              // +0x9E
    bool                                                FakeCollideWithSomething;       // +0x9F
    long                                                RushDelayFrameCount;            // +0xA0
    bool                                                CompletedExitModes;             // +0xA4
    virtual ~CTCAssassinRush();
};

CTCAssassinRush::~CTCAssassinRush()
{
    if (RadialBlurFadeKey) {
        GetWorld().GetEnvironment().GetSpellEffectInterpolator().RadialBlurFadeOut(PDef->AssassinEndSeconds, RadialBlurFadeKey);
        RadialBlurFadeKey = 0;
    }
}