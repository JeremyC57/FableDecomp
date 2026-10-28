// CEnginePrimitiveOcclusionInfo::CEnginePrimitiveOcclusionInfo() -- count 1, no pending tests, not seen, stamped with the current occlusion frame
// Leaf: ??0CEnginePrimitiveOcclusionInfo@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2FA90, 48 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x012A1284 (RTTI .?AVCEnginePrimitiveOcclusionInfo@@ : CIVCountedPointeeBase);
//   Ego_r.pdb ??0CEnginePrimitiveOcclusionInfo@@QAE@XZ; it follows SetAsOccluded 0x00B2F9F0 / SetAsVisible 0x00B2FA40
//   in the occlusion source file and precedes its ??_G 0x00B2FB10.
// Layout (Ego_r.pdb member names, STLport 12-byte CArray): IVRefCount +4, InvisibleFrames +8, OcclusionTestHandles
//   +0x0C, Seen +0x18, TestFrame +0x1C (retail size 0x20; the PDB's is 0x24).
// Globals: 0x01436E5C the CEngineOcclusionManager (CurrentFrame +0x4010).
// Shape: retail stores the array, then TestFrame, then InvisibleFrames and Seen: TestFrame is a member initialiser
//   (declaration order puts it after the array), InvisibleFrames / Seen are assigned in the body.
#include "engine/CEngineOcclusionManager.h"

extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6 (unused here)

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
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
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __first, iterator __last)
    {
        _Tp* __i = __copy_ptrs(__last, this->_M_finish, __first, __true_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}

template <class T> class CArray : public std::vector<T> { };

class CIVCountedPointeeBase {
public:
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const;
    mutable unsigned long IVRefCount;                // +0x04
};

class CEnginePrimitiveOcclusionInfo : public CIVCountedPointeeBase {
public:
    CEnginePrimitiveOcclusionInfo();
    virtual ~CEnginePrimitiveOcclusionInfo();
    long InvisibleFrames;                            // +0x08
    CArray<unsigned long> OcclusionTestHandles;      // +0x0C
    bool Seen;                                       // +0x18
    unsigned long TestFrame;                         // +0x1C
private:
    void ClearHandles() { OcclusionTestHandles.clear(); }
};

extern CEngineOcclusionManager* g_pOcclusionManager;     // 0x01436E5C

CEnginePrimitiveOcclusionInfo::CEnginePrimitiveOcclusionInfo()
    : TestFrame(g_pOcclusionManager->CurrentFrame)
{
    InvisibleFrames = 0;
    Seen = false;
}