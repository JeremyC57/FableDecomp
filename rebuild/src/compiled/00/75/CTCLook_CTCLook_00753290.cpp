// CTCLook::~CTCLook() -- creature look (head/eye turning) component dtor: unless the parent thing is dead, unregister this appearance component from the parent's graphic appearance TC; then the member dtors in reverse order (the two head-turn anim counted pointers, the turning-anim vector, the appearance and look def pointers), the CAGraphicAppearanceComponent base and ~CTCBase
// Leaf: ??1CTCLook@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00753290, 228 bytes, tail jump to the base destructor.
// Identity: the vtable stores 0x01269744 / 0x012697B8 both carry RTTI `.?AVCTCLook@@` (rtti_bases: CTCLook : CTCBase (+0),
//   NGraphicAppearance::CAGraphicAppearanceComponent (+0xC)); slot 0 of 0x01269744 is the scalar deleting dtor 0x00755460,
//   the only caller (call at 0x00755463). Ego_r.pdb has `??1CTCLook@@UAE@XZ`; the catalog label is right.
// Callees: NGraphicAppearance::CTCGraphicAppearance::UnregisterComponent(CAGraphicAppearanceComponent&) 0x004C0000 (FableWin.pdb
//   reference spelling, as in chunk 563's 0x00727B40), operator delete 0x00BFE9BC (the counted pointers' info blocks), free
//   0x00BFEA14 (the TurningAnims block), CTCBase::~CTCBase 0x00686830 (tail jump); virtual calls: each count block's
//   DeleteFunc, each def's slot 1 on its last release. The CAGraphicAppearanceComponent dtor is inline: only its vptr store
//   0x0125A170 remains.
// Layout: Ego_r.pdb CTCLook (0xD4) with the retail 12-byte vector: the component base +0x0C, PParentCreature +0x10, PDef +0x14,
//   PAppearanceDef +0x18, TurningAnims +0x8C, CachedHeadPos +0xA1 (packed C3DVector), PHeadTurnAnimH +0xBC, PHeadTurnAnimV
//   +0xC4, FirstTimeThrough +0xCC. CThing: PTCGraphicAppearance +0x64, Dead bit 0 of +0x91. No engine header.
// Shape: user body `if (!ParentThing->IsDead()) ParentThing->PTCGraphicAppearance->UnregisterComponent(*this);`;
//   CCountedPointer's inline release (`ReleaseData(); Data = 0; Info = 0;`) re-reads Info after each step.
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
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
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
namespace NGraphicAppearance {
class CTCGraphicAppearance;
class CGraphicAppearanceDrawInfo;
class CMeshInfo;
class CAGraphicAppearanceComponent { public:                // vtable 0x0125A170
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info) = 0;   // slot 0
    virtual void OnAppearanceClear() = 0;                   // slot 1
    virtual void GetMeshesUsed(std::vector<CMeshInfo>& meshes) const;   // slot 2
    ~CAGraphicAppearanceComponent() { }
};
class CTCGraphicAppearance { public:
    void UnregisterComponent(CAGraphicAppearanceComponent& component);   // 0x004C0000
};
}
class CThing { public:                                      // retail offsets (Ego_r minus 0x10 past Components)
    unsigned char                               _pad_0x00[0x64];
    NGraphicAppearance::CTCGraphicAppearance*   PTCGraphicAppearance;   // +0x64
    unsigned char                               _pad_0x68[0x29];
    bool                                        Dead : 1;               // +0x91 bit 0
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

class CThingCreatureBase;
class CLookDef;
class CAppearanceDef;
class CPrimitive;
class CAnimationEntry;
namespace NAnimationEntryBodge { class CAnimEntryLite; }

class CTCLook : public CTCBase, public NGraphicAppearance::CAGraphicAppearanceComponent { public:   // vtables 0x01269744 / 0x012697B8
    CThingCreatureBase*                                     PParentCreature;                        // +0x10
    CDefPointer<const CLookDef>                             PDef;                                   // +0x14
    CDefPointer<const CAppearanceDef>                       PAppearanceDef;                         // +0x18
    float                                                   RecoveryAngleToLookAtXY;                // +0x1C
    float                                                   RecoveryAngleToLookAtYZ;                // +0x20
    float                                                   BodyAngleXY;                            // +0x24
    float                                                   BodyAngleYZ;                            // +0x28
    float                                                   HeadAngleRelToBodyXY;                   // +0x2C
    float                                                   HeadAngleRelToBodyYZ;                   // +0x30
    float                                                   EyeAnglesRelToHeadXY;                   // +0x34
    float                                                   EyeAnglesRelToHeadYZ;                   // +0x38
    long                                                    LookTimer;                              // +0x3C
    bool                                                    UsingOldStyleTurning;                   // +0x40
    bool                                                    TurningViaAnimationDisable;             // +0x41
    float                                                   FractionOfTurningToAcceptEachFrame;     // +0x44
    long                                                    HeadTurnState;                          // +0x48
    long                                                    BodyTurnState;                          // +0x4C
    bool                                                    CanTurnBody;                            // +0x50
    float                                                   BodyVelocityXY;                         // +0x54
    float                                                   BodyVelocityYZ;                         // +0x58
    float                                                   HeadVelocityXY;                         // +0x5C
    float                                                   HeadVelocityYZ;                         // +0x60
    long                                                    LookFlags;                              // +0x64
    C3DVector                                               PosToLookAt;                            // +0x68
    C3DVector                                               DirectionToLookIn;                      // +0x74
    bool                                                    EyeAnglesChanged;                       // +0x80
    CPrimitive*                                             LeftEyePrimitive;                       // +0x84
    CPrimitive*                                             RightEyePrimitive;                      // +0x88
    std::vector<std::pair<float, const CAnimationEntry*> >  TurningAnims;                           // +0x8C
    float                                                   TurningViaAnimMinAngle;                 // +0x98
    CAnimationEntry*                                        PCurrentTurningAnim;                    // +0x9C
    bool                                                    TargetAcquired;                         // +0xA0
    C3DVector                                               CachedHeadPos;                          // +0xA1
    long                                                    HeadCacheTimer;                         // +0xB0
    long                                                    TimeToStopLooking;                      // +0xB4
    bool                                                    OneFrameDelay;                          // +0xB8
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>   PHeadTurnAnimH;                         // +0xBC
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>   PHeadTurnAnimV;                         // +0xC4
    bool                                                    FirstTimeThrough;                       // +0xCC
    virtual ~CTCLook();
    virtual bool OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance& appearance, const NGraphicAppearance::CGraphicAppearanceDrawInfo& info);
    virtual void OnAppearanceClear();
};

CTCLook::~CTCLook()
{
    if (!ParentThing->IsDead())
        ParentThing->PTCGraphicAppearance->UnregisterComponent(*this);
}