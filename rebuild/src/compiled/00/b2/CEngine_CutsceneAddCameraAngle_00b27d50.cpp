// CEngine::CutsceneAddCameraAngle(const CCamera& camera) -- append the CCamera to the PreloadCameras vector
// Leaf: ?CutsceneAddCameraAngle@CEngine@@UAEXABVCCamera@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B27D50, 62 bytes, thiscall, `ret 4`.
// Identity (proven): slot 57 of vtable 0x012A0F3C (RTTI .?AVCEngine@@); it appends a CCamera (copy constructor 0x0049D380, 0x38 bytes) to the vector<CCamera> at +0x3C = Ego_r.pdb CEngine::PreloadCameras (CArray<CCamera>, +60); Ego_r.pdb ?CutsceneAddCameraAngle@CEngine@@UAEXABVCCamera@@@Z (the catalogue's name for this slot).
// Callees: CCamera::CCamera(const CCamera&) 0x0049D380, vector<CCamera>::_M_insert_overflow 0x00B29840.
// Shape (family M082, representative 0x004B2850): `PreloadCameras.push_back(camera);` -- STLport push_back inline (null-checked
//   placement copy, or _M_insert_overflow(finish, x, __false_type(), 1, true) with the tag homed in the argument slot).
// Header debt: engine/CEngine.h cannot carry the STLport vector member with its methods; the class is modelled locally by
//   the member's retail offset.
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CCamera {                                              // 0x38 bytes
public:
    CCamera(const CCamera& other);                            // 0x0049D380
    unsigned long Members[14];
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
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
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x00B29840
};
}
class CEngine {
public:
    virtual void CutsceneAddCameraAngle(const CCamera& camera);
    unsigned char _pad[0x38];                         // +0x4 .. members before PreloadCameras
    std::vector<CCamera> PreloadCameras;         // +0x3c
};
void CEngine::CutsceneAddCameraAngle(const CCamera& camera)
{
    PreloadCameras.push_back(camera);
}