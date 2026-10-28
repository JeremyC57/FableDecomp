// C3DSkeletalMorph::AddMorphParameter(const CCountedPointer<C3DSkeletalMorphParameter>& parameter) -- point the parameter back at this morph, append it to MorphParameters and drop the cached combined morph (NotifyParamUpdate)
// Leaf: AddMorphParameter
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A45470, length 117 bytes (+11 int3 pad), `ret 4`.
// Callers: CTCSkeletalMorph::ForceSkeletalMorph 0x00786530 and CalculateSkeletalMorph 0x007867EB (landed; it declares
//   this address as AddMorphParameter). Ego_r.pdb: ?AddMorphParameter@C3DSkeletalMorph@@QAEXABV?$CCountedPointer@...@@@Z.
// Callees: 0x00A46CB0 vector<CCountedPointer<C3DSkeletalMorphParameter>>::_M_insert_overflow (the full-storage arm), the
//   count block's destroy (`mov ecx,[blk+8] ; call [blk+4]`), 0x00BFE9BC operator delete.
// Label correction: catalogued as `CSkeletalMorph_SetMorphTarget`.
// Layout (retail, as the landed ctor/dtor 0x00A44E80 / 0x00A44EB0 and Reset 0x00A45410): vptr +0, MorphParameters +4
//   (STLport vector, 12 bytes: _M_start, _M_finish, _M_end_of_storage), CombinedMorph +0x10 (CCountedPointer: Data
//   +0x10, Info +0x14). C3DSkeletalMorphParameter keeps its owning SkeletalMorph at +8.
// Shape: STLport `push_back` inlined (end-of-storage loaded with the finish pointer, placement copy with its null test,
//   `_M_insert_overflow(finish, x, __true/false_type(), 1, true)` with the empty tag homed in the dead `parameter` slot);
//   the counted pointer's user copy ctor add-refs the block; NotifyParamUpdate is the inline release `--Info->RefCount;
//   if (Info->RefCount == 0)` (re-read of Info) that nulls Info then Data (same body as the out-of-line 0x00A45330).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)
void __cdecl operator delete(void* p);               // 0x00BFE9BC

namespace std {
struct __false_type { __false_type() { } };          // user constructor: no zero-initialised tag temporary
}

class CCPPointerInfo { public:                              // count block, PDB: RefCount +0, DeleteFunc +4, Data +8
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;                            // +0x00
    CCPPointerInfo* Info;                            // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    T* operator->() const { return Data; }
    void Clear()
    {
        if (Info != 0) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Info = 0;
        Data = 0;
    }
};

namespace std {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp>
class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00A46CB0
    public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

class C3DSkeletalMorph;
class C3DSkeletalMorphTarget;
class C3DSkeletalMorphParameter {
public:
    virtual void v00();
    float             CurrentValue;                  // +0x04
    C3DSkeletalMorph* SkeletalMorph;                 // +0x08
};

class CBaseClassNonCopyable {
public:
    virtual void v00();
};

class C3DSkeletalMorph : public CBaseClassNonCopyable {
public:
    std::vector<CCountedPointer<C3DSkeletalMorphParameter> > MorphParameters;   // +0x04
    CCountedPointer<C3DSkeletalMorphTarget>                     CombinedMorph;     // +0x10
    void NotifyParamUpdate() { CombinedMorph.Clear(); }
    void AddMorphParameter(const CCountedPointer<C3DSkeletalMorphParameter>& parameter);
};

void C3DSkeletalMorph::AddMorphParameter(const CCountedPointer<C3DSkeletalMorphParameter>& parameter)
{
    parameter->SkeletalMorph = this;
    MorphParameters.push_back(parameter);
    NotifyParamUpdate();
}