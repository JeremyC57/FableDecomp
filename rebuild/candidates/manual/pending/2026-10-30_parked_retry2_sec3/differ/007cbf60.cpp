// CTCMultiArrow::~CTCMultiArrow() -- multi-arrow spell component dtor: kill a live bow effect and every live ammo effect, then the member dtors in reverse order (the bow effect pointer, the ammo-effect pointer vector, the multi-arrow def pointer) and ~CTCBase
// Grade: DIFFER(241v241) -- third element address (retail mov eax ; push 1 ; lea ecx,[eax+ebp]); tried (this sweep): flat operator-> (no GetPointer level), .Get()->Kill
// Leaf: ??1CTCMultiArrow@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CBF60, 241 bytes, tail jump to the base destructor.
// Identity: the first store is vtable 0x0126ED7C, RTTI `.?AVCTCMultiArrow@@` (rtti_bases: CTCMultiArrow : CTCBase); its slot 0
//   is the scalar deleting dtor 0x007CC990. Ego_r.pdb has `??1CTCMultiArrow@@UAE@XZ`; the PDB member offsets (PDef +0x0C,
//   AmmoEffects +0x30, BowEffect +0x40 less 4 for the 12-byte STLport vector = +0x3C) are the ones used here.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::Kill(bool) 0x004C9B80, ~CBaseIntelligentPointer 0x00A01C10,
//   free 0x00BFEA14 (the vector block), CTCBase::~CTCBase 0x00686830 (tail jump); virtual calls: each vector element's
//   slot-0 destructor (flag 0: STLport _Destroy), the def's slot 1 on its last release.
// Layout: Ego_r.pdb CTCMultiArrow (0x4C, retail 0x48); no engine header.
// Shape: user-written dtor: IS_THING_ALIVE + Kill(true) for the bow effect, then an index loop over the ammo effects with a
//   signed bound (`test eax,0FFFFFFF8h ; jle` / `jl`: the size is converted to long) re-reading size() every pass.
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

class CSpecialAbilitiesMultiArrowDef;
class CTCMultiArrow : public CTCBase { public:             // vtable 0x0126ED7C
    CDefPointer<const CSpecialAbilitiesMultiArrowDef>   PDef;                       // +0x0C
    long                                                NumberOfShots;              // +0x10
    long                                                NumberInVolley;             // +0x14
    long                                                NumOfVolleys;               // +0x18
    long                                                SpellLevel;                 // +0x1C
    bool                                                RemovedEffect;              // +0x20
    long                                                CreateAmmoEffectCount;      // +0x24
    long                                                CreateAmmoEffectDelay;      // +0x28
    long                                                CreateAmmoEffectTimer;      // +0x2C
    std::vector<CIntelligentPointer<CThing> >           AmmoEffects;                // +0x30
    CIntelligentPointer<CThing>                         BowEffect;                  // +0x3C
    bool                                                CutSceneCancelled;          // +0x44
    virtual ~CTCMultiArrow();
};

CTCMultiArrow::~CTCMultiArrow()
{
    if (IS_THING_ALIVE(BowEffect))
        BowEffect->Kill(true);
    for (long i = 0; i < (long)AmmoEffects.size(); ++i) {
        if (IS_THING_ALIVE(AmmoEffects[i]))
            AmmoEffects[i]->Kill(true);
    }
}
