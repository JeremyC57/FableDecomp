// CTCWillLightning::CLightningBeam::~CLightningBeam() -- lightning-spell beam dtor: kill a live beam particle, then the member dtors in reverse order (the lightning def pointer, the child-beam list, the target, home and particle thing pointers)
// Leaf: ??1CLightningBeam@CTCWillLightning@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A0770, 126 bytes, tail jump to the last member destructor.
// Identity: called by 0x007A40B0 (the `??_G` helper) and 0x007A4120, the delete function of CCountedPointer<CLightningBeam>
//   (`delete (CLightningBeam*)data`; it is stored as data at 0x007A18FC / 0x007A4236). Ego_r.pdb has the non-virtual
//   `??1CLightningBeam@CTCWillLightning@@QAE@XZ` and `??_GCLightningBeam@CTCWillLightning@@QAEPAXI@Z` (no vtable: nothing
//   is stored at +0). The Ego_r CLightningBeam layout (m_tpParticleHandle +0x04, m_tcpHome +0x0C, m_tcpTarget +0x14,
//   m_lChildren +0x4C, PDef +0x7C) less 8 for the STLport list after +0x4C gives every offset used here (PDef +0x74).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (three times), CThing::Kill(bool) 0x004C9B80,
//   _List_base<CCountedPointer<CTCWillLightning::CLightningBeam> >::clear 0x007A3910, free 0x00BFEA14 (the list sentinel),
//   ~CBaseIntelligentPointer 0x00A01C10 (three times; the last as the tail jump); the def's slot 1 on its last release.
// Shape: user-written non-virtual dtor whose body is IS_THING_ALIVE + Kill(true); STLport ~_List_base() { clear();
//   _M_node.deallocate(_M_node._M_data, 1); } with clear out of line.
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
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base { public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;   // +0x0
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear();                                           // 0x007A3910
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
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

#pragma pack(push, 1)
class C3DVector { public:                                  // retail C3DVector is 1-aligned
    float X;
    float Y;
    float Z;
};
#pragma pack(pop)


class CSpecialAbilitiesLightningSpellDef;
class CTCWillLightning { public:
    enum ELightningState { };                               // values not needed here
    class CLightningBeam;
};
template <class T> class CCountedPointer { public:
    T*    Data;                                             // +0x00
    void* Info;                                             // +0x04 (CCPPointerInfo*)
};
class CTCWillLightning::CLightningBeam { public:          // no vtable
    ELightningState                                         m_lsState;              // +0x00
    CIntelligentPointer<CThing>                             m_tpParticleHandle;     // +0x04
    CIntelligentPointer<CThing>                             m_tcpHome;              // +0x0C
    CIntelligentPointer<CThing>                             m_tcpTarget;            // +0x14
    C3DVector                                               m_vStartPos;            // +0x1C
    C3DVector                                               m_vEndPos;              // +0x28
    C3DVector                                               m_vDestination;         // +0x34
    float                                                   m_fDamagePerHit;        // +0x40
    float                                                   m_fTime;                // +0x44
    float                                                   m_fTimeBase;            // +0x48
    std::list<CCountedPointer<CLightningBeam> >             m_lChildren;            // +0x4C
    bool                                                    m_bTemporary;           // +0x50
    bool                                                    m_bMainBeam;            // +0x51
    bool                                                    m_bRightHand;           // +0x52
    float                                                   m_fActivationTime;      // +0x54
    float                                                   m_fDeActivationTime;    // +0x58
    float                                                   m_fLife;                // +0x5C
    float                                                   m_fDecapitationTime;    // +0x60
    float                                                   m_fDecapitationClock;   // +0x64
    float                                                   m_fDelay;               // +0x68
    long                                                    m_OnhitEveryFrame;      // +0x6C
    long                                                    m_nFrameCount;          // +0x70
    CDefPointer<const CSpecialAbilitiesLightningSpellDef>   PDef;                   // +0x74
    bool                                                    SuccessfulHit;          // +0x78
    ~CLightningBeam();
};

CTCWillLightning::CLightningBeam::~CLightningBeam()
{
    if (IS_THING_ALIVE(m_tpParticleHandle))
        m_tpParticleHandle->Kill(true);
}