// CTCTurncoat::~CTCTurncoat() -- turncoat spell component dtor: kill a live turncoated effect, then the member dtors in reverse order (the stage-effect pointer vector, the originator and effect thing pointers, the turncoat def pointer) and ~CTCBase
// Leaf: ??1CTCTurncoat@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C7960, 149 bytes, tail jump to the base destructor.
// Identity: the first store is vtable 0x0126E6F4, RTTI `.?AVCTCTurncoat@@` (rtti_bases: CTCTurncoat : CTCBase); its slot 0
//   is the scalar deleting dtor 0x007C8100. Ego_r.pdb has `??1CTCTurncoat@@UAE@XZ`; the PDB member offsets (PDef +0x14,
//   TurncoatedEffect +0x18, TurncoatOriginator +0x24, TurncoatStageEffects +0x3C) are exactly the ones used here.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (three times), CThing::Kill(bool) 0x004C9B80, free 0x00BFEA14 (the
//   vector block), ~CBaseIntelligentPointer 0x00A01C10 (twice), CTCBase::~CTCBase 0x00686830 (tail jump); virtual calls:
//   each vector element's slot-0 destructor (flag 0: STLport _Destroy), the def's slot 1 on its last release.
// Layout: Ego_r.pdb CTCTurncoat (0x50, retail 0x4C with the 12-byte STLport vector). engine/CTCTurncoat.h has the same
//   offsets (PDef +0x14, TurncoatedEffect +0x18, TurncoatOriginator +0x24, TurncoatStageEffects +0x3C, sizeof 0x4C) but
//   flattens the vptr and the member objects into opaque bytes, so it cannot carry a destructor chain: local model with
//   the header's offsets (NOTES.md "Header debt").
// Shape: user-written dtor whose body is IS_THING_ALIVE + Kill(true) (as ~CTCForcePushed 0x00798620).
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

class CTurncoatDef;
class CTCTurncoat : public CTCBase { public:               // vtable 0x0126E6F4
    long                                        WillTurncoatTimer;                          // +0x0C
    float                                       CurrentTurncoatedness;                      // +0x10
    CDefPointer<const CTurncoatDef>             PDef;                                       // +0x14
    CIntelligentPointer<CThing>                 TurncoatedEffect;                           // +0x18
    long                                        TurncoatTimeLast;                           // +0x20
    CIntelligentPointer<CThing>                 TurncoatOriginator;                         // +0x24
    bool                                        TurncoatOverride;                           // +0x2C
    long                                        TimeDelayBeforeResistanceRecoveryFrames;    // +0x30
    long                                        CurrentRecoveryTimer;                       // +0x34
    float                                       ResistanceRecoveryPerFrame;                 // +0x38
    std::vector<CIntelligentPointer<CThing> >   TurncoatStageEffects;                       // +0x3C
    float                                       CreatureTurncoatResistance;                 // +0x48
    virtual ~CTCTurncoat();
};

CTCTurncoat::~CTCTurncoat()
{
    if (IS_THING_ALIVE(TurncoatedEffect))
        TurncoatedEffect->Kill(true);
}