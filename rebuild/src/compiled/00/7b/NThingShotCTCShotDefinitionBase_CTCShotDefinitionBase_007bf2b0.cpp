// NThingShot::CTCShotDefinitionBase::~CTCShotDefinitionBase() -- shot-definition component base dtor (empty user body): the member dtors in reverse order (best-collision thing pointer, ability and shot def pointers, weapon / shooter / target thing pointers, the hit-things pointer vector, the effect pointer) and ~CTCBase
// Leaf: ??1CTCShotDefinitionBase@NThingShot@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BF2B0, 153 bytes, tail jump to the base destructor.
// Identity: the first store is vtable 0x0126E1A4, RTTI `.?AVCTCShotDefinitionBase@NThingShot@@` (rtti_bases:
//   NThingShot::CTCShotDefinitionBase : CTCBase; base of the CTCShotDefinitionProjectile / MultiArrow / Throw ... family);
//   its slot 0 is the scalar deleting dtor 0x007C2CF0. Ego_r.pdb has `??1CTCShotDefinitionBase@NThingShot@@UAE@XZ`; the
//   catalog label lacks the namespace. The PDB member offsets (PEffect +0x0C, HitThingsList +0x14, PTargetThing +0x24 less 4
//   for the 12-byte STLport vector = +0x20, PShooter +0x28, PWeapon +0x30, PShotDef +0x38, PShotAbilityDef +0x3C,
//   PBestCollisionThing +0x88) are the ones used here.
// Callees: ~CBaseIntelligentPointer 0x00A01C10 (five times), free 0x00BFEA14 (the vector block), CTCBase::~CTCBase 0x00686830
//   (tail jump); virtual calls: each vector element's slot-0 destructor (flag 0: STLport _Destroy), each def's slot 1 on its
//   last release.
// Layout: Ego_r.pdb CTCShotDefinitionBase (0xA0, retail 0x9C); C3DVector is 1-aligned (TargetPos +0x51); no engine header.
// Shape: user-declared virtual dtor with an empty body (the vptr store marks a user-written dtor).
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
    ~CDefPointer() { Release(); }
};

class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
};

#pragma pack(push, 1)
class C3DVector { public:                                  // retail C3DVector is 1-aligned
    float X;
    float Y;
    float Z;
};
#pragma pack(pop)

class CThingShotDef;
class CMeleeCombatAbilityDef;
class CHitLocation;
enum ECollideType { };                                      // values not needed here
namespace NThingShot {
class CTCShotDefinitionBase : public CTCBase { public:     // vtable 0x0126E1A4
    CIntelligentPointer<CThing>                     PEffect;                    // +0x0C
    std::vector<CIntelligentPointer<const CThing> > HitThingsList;              // +0x14
    CIntelligentPointer<const CThing>               PTargetThing;               // +0x20
    CIntelligentPointer<CThing>                     PShooter;                   // +0x28
    CIntelligentPointer<CThing>                     PWeapon;                    // +0x30
    CDefPointer<const CThingShotDef>                PShotDef;                   // +0x38
    CDefPointer<const CMeleeCombatAbilityDef>       PShotAbilityDef;            // +0x3C
    float                                           AccuracyPercentage;         // +0x40
    float                                           DamageMultiplier;           // +0x44
    float                                           ShotBuildupTime;            // +0x48
    long                                            StartFrame;                 // +0x4C
    bool                                            SubShotsTriggered;          // +0x50
    C3DVector                                       TargetPos;                  // +0x51
    long                                            DieOnFrame;                 // +0x60
    bool                                            PassedTarget;               // +0x64
    bool                                            MadeSound;                  // +0x65
    bool                                            KnockDown;                  // +0x66
    bool                                            Decapitate;                 // +0x67
    bool                                            CanBeDeflected;             // +0x68
    float                                           InitialVelocity;            // +0x6C
    C3DVector                                       BestCollisionPos;           // +0x70
    C3DVector                                       BestCollisionNormal;        // +0x7C
    CIntelligentPointer<CThing>                     PBestCollisionThing;        // +0x88
    ECollideType                                    BestCollisionType;          // +0x90
    CHitLocation*                                   PBestCollisionHitLocation;  // +0x94
    bool                                            MultiArrowShotType;         // +0x98
    virtual ~CTCShotDefinitionBase();
};
} // namespace NThingShot

NThingShot::CTCShotDefinitionBase::~CTCShotDefinitionBase()
{
}