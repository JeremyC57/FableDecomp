// NCreatureMode::CModeBriarRoseConcentrate::~CModeBriarRoseConcentrate() -- Briar Rose concentrate creature-mode dtor (compiler-generated): the member dtors in reverse order (the sword and body effect pointers, the Briar Rose def pointer, the imposter pointer vector, the target pointer, the group anim filter) and ~CModeStandBase
// Leaf: ??1CModeBriarRoseConcentrate@NCreatureMode@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743120, 147 bytes, tail jump to the base destructor.
// Identity: the only call is from 0x00743100, slot 0 (the scalar deleting destructor) of vtable 0x01268548, RTTI
//   `.?AVCModeBriarRoseConcentrate@NCreatureMode@@` (rtti_bases: CModeBriarRoseConcentrate : CModeStandBase :
//   CTCCreatureModeManager::CModeBase). Ego_r.pdb has `??1CModeBriarRoseConcentrate@NCreatureMode@@UAE@XZ`. No vtable store of
//   its own: the destructor is implicit. The only vtable store is CAnimFilterBase's 0x0122FFCC at +0x68 (the Filter member's
//   inline base dtor; CAnimFilterOfGroup's own dtor is implicit too).
// Callees: ~CBaseIntelligentPointer 0x00A01C10 (three times), free 0x00BFEA14 (the vector block), ~CCharString 0x0099EAE0
//   (Filter.Group), CModeStandBase::~CModeStandBase 0x0073D860 (tail jump, bound `??1CModeStandBase@@UAE@XZ`); virtual calls:
//   the def's slot 1 on its last release, each vector element's slot-0 destructor (flag 0: STLport _Destroy).
// Layout: Ego_r.pdb CModeBriarRoseConcentrate (0xA8) with the retail 12-byte vector: Filter +0x68 (CAnimFilterOfGroup: vptr,
//   Group +0x6C, GroupCRC +0x70), Timer +0x74, PTarget +0x78, Imposters +0x80, Imposter +0x8C, PDef +0x90, PBodyEffect +0x94,
//   PSwordEffect +0x9C. CModeStandBase is 0x68 bytes (no containers). No engine header.
// Shape: the implicit destructor, emitted out of line through a qualified explicit destructor call in a one-line helper
//   (EmitImplicitDestructor, not a retail function).
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

class CCharString { public:
    void* PStringData;                                      // +0x00
    ~CCharString();                                         // 0x0099EAE0
};
class CAnimationEntry;
class CAnimFilterBase { public:                             // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
    virtual bool operator()(const CAnimationEntry& entry) const = 0;
    virtual CAnimFilterBase* Clone() const = 0;
};
class CAnimFilterOfGroup : public CAnimFilterBase { public:   // 0xC bytes
    CCharString     Group;                                  // +0x04
    unsigned long   GroupCRC;                               // +0x08
    virtual bool operator()(const CAnimationEntry& entry) const;
    virtual CAnimFilterBase* Clone() const;
};

class CTCCreatureModeManager { public:
    class CModeBase { public:                               // 0x1C bytes
        CThing*             ParentThing;                    // +0x04
        CAnimFilterOfGroup  WeaponBasedFilter;              // +0x08
        long                FramesBeforeRemovingSelf;       // +0x14
        bool                Active;                         // +0x18
        virtual ~CModeBase();
    };
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
};
namespace NAnimationEntryBodge { class CAnimEntryLite; }
class CModeStandBase : public CTCCreatureModeManager::CModeBase { public:   // 0x68 bytes
    long                                                    IdleCounter;                    // +0x1C
    long                                                    FramesToKeepUpdatingMovement;   // +0x20
    bool                                                    ForceAddNewAnims;               // +0x24
    float                                                   MaxRunSpeed;                    // +0x28
    float                                                   MaxJogSpeed;                    // +0x2C
    float                                                   MaxWalkSpeed;                   // +0x30
    CCharString                                             PoseAnimName;                   // +0x34
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>   PStandAnimation;                // +0x38
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>   PSlowWalkAnimation;             // +0x40
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>   PWalkAnimation;                 // +0x48
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>   PJogAnimation;                  // +0x50
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>   PRunAnimation;                  // +0x58
    CCountedPointer<NAnimationEntryBodge::CAnimEntryLite>   PSprintAnimation;               // +0x60
    virtual ~CModeStandBase();                              // 0x0073D860
};

class CThingCreatureBase;
class CBriarRoseDef;
namespace NCreatureMode {
class CModeBriarRoseConcentrate : public CModeStandBase { public:   // vtable 0x01268548
    CAnimFilterOfGroup                          Filter;             // +0x68
    long                                        Timer;              // +0x74
    CIntelligentPointer<CThingCreatureBase>     PTarget;            // +0x78
    std::vector<CIntelligentPointer<CThing> >   Imposters;          // +0x80
    bool                                        Imposter;           // +0x8C
    CDefPointer<const CBriarRoseDef>            PDef;               // +0x90
    CIntelligentPointer<CThing>                 PBodyEffect;        // +0x94
    CIntelligentPointer<CThing>                 PSwordEffect;       // +0x9C
};
}

void EmitImplicitDestructor(NCreatureMode::CModeBriarRoseConcentrate* p)   // emission device only (not a retail function)
{
    p->NCreatureMode::CModeBriarRoseConcentrate::~CModeBriarRoseConcentrate();
}