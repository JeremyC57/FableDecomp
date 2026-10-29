// CTCRandomAppearanceMorph::~CTCRandomAppearanceMorph() -- random appearance morph TC dtor: unless the parent thing is dead, unregister this appearance component from the parent's graphic appearance TC; then the member dtors (primitive handle vector, mesh morph set counted pointer), the CAGraphicAppearanceComponent base and ~CTCBase
// Leaf: ??1CTCRandomAppearanceMorph@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00727B40, 120 bytes, tail jump to the base destructor.
// Identity: the vtable stores 0x0126653C / 0x01266520 both carry RTTI `.?AVCTCRandomAppearanceMorph@@`; rtti_bases:
//   CTCRandomAppearanceMorph : CTCAppearanceMorphBase (+0) : CTCBase, NGraphicAppearance::CAGraphicAppearanceComponent (+0xC);
//   FableWin.pdb has `??1CTCRandomAppearanceMorph@@UAE@XZ`, and the scalar deleting dtor 0x00728590 (primary slot 0) calls
//   this body. The catalog label is right.
// Callees: NGraphicAppearance::CTCGraphicAppearance::UnregisterComponent 0x004C0000 (FableWin.pdb
//   `?UnregisterComponent@CTCGraphicAppearance@NGraphicAppearance@@QAEXAAVCAGraphicAppearanceComponent@2@@Z`: by reference; the
//   two bound spellings take a pointer), free 0x00BFEA14 (PrimHandles block), operator delete 0x00BFE9BC (the counted pointer's
//   info block), CTCBase::~CTCBase 0x00686830 (tail jump; CTCAppearanceMorphBase's dtor is implicit, no vptr store). The
//   CAGraphicAppearanceComponent dtor is inline: only its vptr store 0x0125A170 remains (RTTI
//   `.?AVCAGraphicAppearanceComponent@NGraphicAppearance@@`; slots 0/1 are _purecall).
// Layout: Ego_r.pdb CTCRandomAppearanceMorph (0x30) with the retail STLport vector (0xC): the component base +0x0C,
//   PMeshMorphSet +0x10 (CCountedPointer: Data, Info), Seed +0x18, PrimHandles +0x1C, PTCSkeletalMorph +0x28. CThing:
//   PTCGraphicAppearance +0x64 and the Dead bit (bit 0 of +0x91) are the Ego_r offsets minus 0x10 (STLport CVectorMap/list).
//   No engine header exists for these classes; engine/CTCBase.h flattens the vptr (header debt).
// Shape: user body `if (!ParentThing->Dead) ParentThing->PTCGraphicAppearance->UnregisterComponent(*this);`, then the implicit
//   member dtors in reverse order and the bases in reverse order (component base vptr store, then the CTCBase chain).
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

namespace std {
template <class _Tp> class allocator { public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public:
    _Tp _M_data;
};
struct __true_type { __true_type() { } };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _Tp> inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
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
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __true_type());
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};

} // namespace std

class CBaseClass { public:
    virtual ~CBaseClass();
};
class CBaseClassNonCopyable : public CBaseClass { public:
protected:
    virtual ~CBaseClassNonCopyable();
};
class CThing;
class CTCBase : public CBaseClassNonCopyable { public:
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    virtual ~CTCBase();                                     // 0x00686830
    void RemoveFromGlobalTCs();                             // 0x00686DF0
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

namespace NGraphicAppearance {
class CTCGraphicAppearance;
class CGraphicAppearanceDrawInfo;
class CMeshInfo;
class CPrimitive;
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
    unsigned char                               _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance*   PTCGraphicAppearance;   // +0x64
    unsigned char                               _pad68[0x29];
    unsigned char                               Dead : 1;               // +0x91 bit 0
};

class CTCAppearanceMorphBase : public CTCBase { public:
    class CMeshMorphSet;
};
class CTCSkeletalMorph;

class CTCRandomAppearanceMorph : public CTCAppearanceMorphBase, public NGraphicAppearance::CAGraphicAppearanceComponent { public:
    CCountedPointer<CTCAppearanceMorphBase::CMeshMorphSet>  PMeshMorphSet;      // +0x10
    unsigned long                                           Seed;               // +0x18
    std::vector<NGraphicAppearance::CPrimitive*>            PrimHandles;        // +0x1C
    CTCSkeletalMorph*                                       PTCSkeletalMorph;   // +0x28
    virtual ~CTCRandomAppearanceMorph();
    virtual bool OnAppearanceDraw(NGraphicAppearance::CTCGraphicAppearance& appearance, const NGraphicAppearance::CGraphicAppearanceDrawInfo& info);
    virtual void OnAppearanceClear();
    virtual void GetMeshesUsed(std::vector<NGraphicAppearance::CMeshInfo>& meshes) const;
};

CTCRandomAppearanceMorph::~CTCRandomAppearanceMorph()
{
    if (!ParentThing->Dead)
        ParentThing->PTCGraphicAppearance->UnregisterComponent(*this);
}