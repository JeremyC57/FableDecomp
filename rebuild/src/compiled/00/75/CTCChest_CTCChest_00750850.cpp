// CTCChest::~CTCChest() -- chest component dtor: kill a live opening particle, then the member dtors in reverse order (the particle pointer, the opener helper) and the implicit CTCContainerRewardHero / CTCContainer dtors (the container item vector of (count, def name) pairs) before ~CTCBase
// Leaf: ??1CTCChest@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00750850, 121 bytes, tail jump to the base destructor.
// Identity: the first store is vtable 0x0126957C, RTTI `.?AVCTCChest@@` (rtti_bases: CTCChest : CTCContainerRewardHero :
//   CTCContainer : CTCBase); its slot 0 is the scalar deleting dtor 0x00750980, the only caller (call at 0x00750983). Ego_r.pdb
//   has `??1CTCChest@@UAE@XZ` (and no destructor symbol for CTCContainer / CTCContainerRewardHero: their dtors are implicit,
//   hence no base vtable stores). The catalog label `CModeOutOfActionBase::~CModeOutOfActionBase` is wrong.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (three times), CThing::Kill(bool) 0x004C9B80,
//   ~CBaseIntelligentPointer 0x00A01C10, ~CCharString 0x0099EAE0 (one per 8-byte pair), free 0x00BFEA14 (the vector block),
//   CTCBase::~CTCBase 0x00686830 (tail jump).
// Layout: Ego_r.pdb with the retail 12-byte vector: CTCContainer::ContainerItems +0x0C, CTCContainerRewardHero::Enabled +0x18,
//   CreateObjectsThatTimeOut +0x19, CTCChest::ChestOpener +0x1C (COpenCloseHelper, 0x14), ChestOpen +0x30, POpenParticle +0x31
//   (packed CIntelligentPointer). No engine header.
// Shape: user-written dtor whose body is IS_THING_ALIVE + Kill(true); the bases' implicit dtors are inlined (no vtable stores).
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

class CCharString { public:
    void* PStringData;                                      // +0x00
    ~CCharString();                                         // 0x0099EAE0
};
class CTCContainer : public CTCBase { public:
    std::vector<std::pair<long, CCharString> >  ContainerItems;             // +0x0C
};
class CTCContainerRewardHero : public CTCContainer { public:
    bool                                        Enabled;                    // +0x18
    bool                                        CreateObjectsThatTimeOut;   // +0x19
};
class COpenCloseHelper { public:                            // 0x14 bytes
    bool  Opened;                                           // +0x00
    bool  Closed;                                           // +0x01
    bool  Opening;                                          // +0x02
    bool  Closing;                                          // +0x03
    float OpenCloseTransitionAmount;                        // +0x04
    float LastOpenCloseTransitionAmount;                    // +0x08
    float TotalTimeToOpen;                                  // +0x0C
    float TotalTimeToClose;                                 // +0x10
};
class CTCChest : public CTCContainerRewardHero { public:   // vtable 0x0126957C
    COpenCloseHelper                            ChestOpener;                // +0x1C
    bool                                        ChestOpen;                  // +0x30
    CIntelligentPointer<CThing>                 POpenParticle;              // +0x31
    virtual ~CTCChest();
};

CTCChest::~CTCChest()
{
    if (IS_THING_ALIVE(POpenParticle))
        POpenParticle->Kill(true);
}