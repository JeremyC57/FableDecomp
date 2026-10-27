// CEngineLandscapeRenderer::RegisterSubPatchForZSorting(long& index, CLandscapeBackgroundPatch* patch) -- give a background sub-patch a slot in this frame's Z-sort list the first time (index = the slot, entry added this frame, not an occlusion box, no occlusion id), else mark its existing slot as added this frame
// Grade: DIFFER(128v129) -- retail keeps the new entry's folded word 0xFFFFFFFD in esi and stores it to both the temporary and the placed element (then `mov eax,[ecx+4] ; add eax,8 ; mov [ecx+4],eax`); ours reloads the word from the temporary and uses `add [ecx+4],8` (aggregate, field-assignment and constructor forms all identical)
// Leaf: ?RegisterSubPatchForZSorting@CEngineLandscapeRenderer@@QAEXAAJPAVCLandscapeBackgroundPatch@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B67E30, 129 bytes, thiscall `ret 8`.
// Identity (proven): Ego_r.pdb / FableWin.pdb `?RegisterSubPatchForZSorting@CEngineLandscapeRenderer@@QAEXAAJPAVCLandscape
//   BackgroundPatch@@@Z`; the vector it grows is CEngineLandscapeRenderer::SortedSubPatchList (+0x5E4 in the engine
//   header, CArray<CSubPatchSorterEntry>; the callee is the bound vector<CSubPatchSorterEntry>::_M_insert_overflow
//   0x00B6C4C0), and the only caller 0x00BE5D20 is a CLandscapeBackgroundPatch member passing (&this->+0x100, this)
//   to the landscape renderer global 0x01436EA8. The catalog row is a gap-scan placeholder.
// Element: Ego_r.pdb CEngineLandscapeRenderer::CSubPatchSorterEntry { CLandscapeBackgroundPatch* Patch; long
//   AddedThisFrame:1, IsOcclusionBox:1, OcclusionID:30; } -- the new entry's word is the folded constant 0xFFFFFFFD
//   (added, not a box, id -1), the re-registration `or dword ptr [entry+4],1`.
// Callees: std::vector<CSubPatchSorterEntry>::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00B6C4C0.
// Shape: STLport push_back inline (placement copy with null test, `finish += 8`; the overflow call's tag homed on the
//   dead `patch` slot); the index < 0 test falls through to the insertion, the mark is the out-of-line tail.
#define CEngineLandscapeRenderer DonorCEngineLandscapeRenderer
#include "engine/CEngineLandscapeRenderer.h"
#undef CEngineLandscapeRenderer

inline void* operator new(unsigned int, void* p) { return p; }

class CLandscapeBackgroundPatch;

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator { public: allocator() { } };

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                         // 0x00B6C4C0
};

}   // namespace std

class CEngineLandscapeRenderer : public DonorCEngineLandscapeRenderer {
public:
    struct CSubPatchSorterEntry {                         // Ego_r.pdb, 8 bytes
        CLandscapeBackgroundPatch* Patch;                 // +0x0
        long AddedThisFrame : 1;                          // +0x4 bit 0
        long IsOcclusionBox : 1;                          //      bit 1
        long OcclusionID : 30;                            //      bits 2..31
    };
    std::vector<CSubPatchSorterEntry>& SubPatchList() { return *(std::vector<CSubPatchSorterEntry>*)SortedSubPatchList; }
    void RegisterSubPatchForZSorting(long& index, CLandscapeBackgroundPatch* patch);
};

void CEngineLandscapeRenderer::RegisterSubPatchForZSorting(long& index, CLandscapeBackgroundPatch* patch)
{
    if (index < 0) {
        index = SubPatchList().size();
        CSubPatchSorterEntry entry = { patch, 1, 0, -1 };
        SubPatchList().push_back(entry);
    }
    else
        SubPatchList()[index].AddedThisFrame = 1;
}
