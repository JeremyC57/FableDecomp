// CTCForcePushed::~CTCForcePushed() -- force-pushed thing component dtor: kill a live push particle, then the member dtors in reverse order (the emitter vector block, the force-push def pointer, the two thing pointers) and ~CTCBase
// Leaf: ??1CTCForcePushed@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00798620, 120 bytes, tail jump to the base destructor.
// Identity: the first store is vtable 0x0126C09C, RTTI `.?AVCTCForcePushed@@` (rtti_bases: CTCForcePushed : CTCBase); its
//   slot 0 is the scalar deleting dtor 0x00798CC0, the only caller (call at 0x00798CC3). Ego_r.pdb has
//   `??1CTCForcePushed@@UAE@XZ`; the PDB member offsets (PParticle +0x14, PDef +0x40, Emitters_created +0x44) are exactly the
//   ones used here. The catalog label `CTCTurncoat::~CTCTurncoat` is wrong (CTCTurncoat has PDef at +0x14).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (three times), CThing::Kill(bool) 0x004C9B80, free 0x00BFEA14 (the
//   vector block), ~CBaseIntelligentPointer 0x00A01C10 (twice), CTCBase::~CTCBase 0x00686830 (tail jump); the def's slot 1
//   (CIVCountedPointeeBase::Delete) on its last release.
// Layout: Ego_r.pdb CTCForcePushed (0x50); no engine header. CIntelligentPointer is packed (pack(1), as the PDB offsets show).
// Shape: user-written dtor whose body is the kill of the particle; CDefPointer's inline Release reads the pointer once;
//   STLport ~vector of a pointer type only frees the block.
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

class CSpecialAbilitiesForcePushDef;
class CTCForcePushed : public CTCBase { public:            // vtable 0x0126C09C
    CIntelligentPointer<CThing>                     PThingCausingForcePush;     // +0x0C
    CIntelligentPointer<CThing>                     PParticle;                  // +0x14
    C3DVector                                       PositionForcePushStarted;   // +0x1C
    float                                           DistanceBeingPushed;        // +0x28
    C3DVector                                       DirectionBeingPushed;       // +0x2C
    float                                           DistanceMovingPerFrame;     // +0x38
    bool                                            Initialised;                // +0x3C
    bool                                            StartedMoving;              // +0x3D
    CDefPointer<const CSpecialAbilitiesForcePushDef> PDef;                      // +0x40
    std::vector<CThing*>                            Emitters_created;           // +0x44
    virtual ~CTCForcePushed();
};

CTCForcePushed::~CTCForcePushed()
{
    if (IS_THING_ALIVE(PParticle))
        PParticle->Kill(true);
}