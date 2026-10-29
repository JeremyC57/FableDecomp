// CTCHealLife::~CTCHealLife() -- heal-life spell component dtor (empty user body): the member dtors in reverse order (the healing-target pointer, the health-accumulator float vector, the healing-effect pointer vector, the things-to-heal list, the removed-item pointer, the heal-life def pointer) and ~CTCBase
// Leaf: ??1CTCHealLife@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C3F10, 146 bytes, tail jump to the base destructor.
// Identity: the first store is vtable 0x0126E36C, RTTI `.?AVCTCHealLife@@` (rtti_bases: CTCHealLife : CTCBase); its slot 0 is
//   the scalar deleting dtor 0x007C53D0. Ego_r.pdb has `??1CTCHealLife@@UAE@XZ`. The catalog label
//   `CTCActionUseBuyHouse::~CTCActionUseBuyHouse` is wrong. The PDB member offsets (PDef +0x0C, PItemRemoved +0x1C,
//   ThingsToHeal +0x24, HealingEffects +0x30 less 8 for the 4-byte STLport list = +0x28, HealthIncreaseAccum +0x34,
//   PHealingTarget +0x40) are the ones used here.
// Callees: ~CBaseIntelligentPointer 0x00A01C10 (twice), free 0x00BFEA14 (two vector blocks and the list sentinel),
//   std::_List_base<CIntelligentPointer<CThingCreatureBase> >::clear 0x007C4E80 (out of line: walks the nodes, destroys each
//   element through its slot-0 destructor and frees the node), CTCBase::~CTCBase 0x00686830 (tail jump); virtual calls:
//   each vector element's slot-0 destructor (flag 0: STLport _Destroy), the def's slot 1 on its last release.
// Layout: Ego_r.pdb CTCHealLife (0x58, retail 0x48); no engine header.
// Shape: user-declared virtual dtor with an empty body; STLport ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }.
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
    void clear();                                           // 0x007C4E80 (this instantiation)
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { public:
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

class CThingCreatureBase;
class CSpecialAbilitiesHealLifeDef;
class CTCHealLife : public CTCBase { public:               // vtable 0x0126E36C
    CDefPointer<const CSpecialAbilitiesHealLifeDef>         PDef;                   // +0x0C
    float                                                   TotalHealthAccum;       // +0x10
    unsigned long                                           LoopingSound;           // +0x14
    long                                                    SpellLevel;             // +0x18
    CIntelligentPointer<CThing>                             PItemRemoved;           // +0x1C
    std::list<CIntelligentPointer<CThingCreatureBase> >     ThingsToHeal;           // +0x24
    std::vector<CIntelligentPointer<CThing> >               HealingEffects;         // +0x28
    std::vector<float>                                      HealthIncreaseAccum;    // +0x34
    CIntelligentPointer<CThing>                             PHealingTarget;         // +0x40
    virtual ~CTCHealLife();
};

CTCHealLife::~CTCHealLife()
{
}