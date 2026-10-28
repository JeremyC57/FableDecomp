// CEnginePrimitiveOcclusionManager::SetAsOccluded(CEnginePrimitiveOcclusionInfo& info) const -- mark a primitive occluded for the next PrimitiveOccludeDelay+1 frames, drop its pending pixel-count tests, stamp the occlusion frame
// Leaf: ?SetAsOccluded@CEnginePrimitiveOcclusionManager@@QBEXAAVCEnginePrimitiveOcclusionInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2F9F0, 67 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?SetAsOccluded@CEnginePrimitiveOcclusionManager@@QBEXAAVCEnginePrimitiveOcclusionInfo@@@Z;
//   donor order (CheckVisibilityResult, SetAsOccluded, SetAsVisible, GetOccludedFrames, ??_G Info) matches retail
//   0x00B2F9F0 / 0x00B2FA40 (SetAsVisible: InvisibleFrames = 0, the same clear and stamp) / 0x00B2FA90 (Info ctor) /
//   0x00B2FB10 (??_G, slot 0 of the Info vtable 0x012A1284). this+0x0C is PrimitiveOccludeDelay (the manager ctor
//   0x00B2F760 stores 4 there); info +0x08 InvisibleFrames, +0x0C OcclusionTestHandles, +0x1C TestFrame (PDB).
// Globals: 0x01436E5C is the CEngineOcclusionManager (Ego_r.pdb CurrentFrame at +0x4010).
// Callees: memmove 0x00BFEAE6 (the dead arm of the inlined STLport __copy_trivial).
// Shape: the private ClearHandles() inline = OcclusionTestHandles.clear() = STLport 4.6 erase(begin(), end()):
//   copy(last, _M_finish, first) folds to __copy_trivial(_M_finish, _M_finish, _M_start), keeping retail's
//   `cmp eax,eax` and the never-taken memmove arm; destroying the unsigned long tail is empty.
#define CEnginePrimitiveOcclusionManager CEnginePrimitiveOcclusionManager_HeaderLayout
#include "engine/CEnginePrimitiveOcclusionManager.h"
#undef CEnginePrimitiveOcclusionManager
#include "engine/CEngineOcclusionManager.h"

extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

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
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
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
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const;
    mutable unsigned long IVRefCount;                // +0x04
};

class CEnginePrimitiveOcclusionInfo : public CIVCountedPointeeBase {
public:
    virtual ~CEnginePrimitiveOcclusionInfo();
    long InvisibleFrames;                            // +0x08
    CArray<unsigned long> OcclusionTestHandles;      // +0x0C
    bool Seen;                                       // +0x18
    unsigned long TestFrame;                         // +0x1C
private:
    void ClearHandles() { OcclusionTestHandles.clear(); }
    friend class CEnginePrimitiveOcclusionManager;
};

extern CEngineOcclusionManager* g_pOcclusionManager;     // 0x01436E5C

class CEnginePrimitiveOcclusionManager : public CEnginePrimitiveOcclusionManager_HeaderLayout {
public:
    void SetAsOccluded(CEnginePrimitiveOcclusionInfo& info) const;
};

void CEnginePrimitiveOcclusionManager::SetAsOccluded(CEnginePrimitiveOcclusionInfo& info) const
{
    info.InvisibleFrames = PrimitiveOccludeDelay + 1;
    info.ClearHandles();
    info.TestFrame = g_pOcclusionManager->CurrentFrame;
}