// vector<CPhysicsMeshInfo>::push_back(const CPhysicsMeshInfo& x) -- copy-construct a physics-mesh entry at the end, growing through _M_insert_overflow when full
// Leaf: ?push_back@?$vector@VCPhysicsMeshInfo@@V?$allocator@VCPhysicsMeshInfo@@@std@@@std@@QAEXABVCPhysicsMeshInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2D190, 85 bytes, `ret 4`. Only caller 0x00A26EA3 (CMeshDataBank, the
//   physics-mesh info array). The catalog label "vector_push_back_generic" is descriptive.
// Element: CPhysicsMeshInfo (0x10 bytes, vtable 0x0129CD50 over CMeshDataBankEntry 0x0129CD28: Bank +4, Index +8,
//   CIVCountedPointer<CMeshData> MeshData +0xC whose pointee keeps IVRefCount at +4) -- rtti-vtables.tsv + Ego_r.pdb.
// Shape: STLport push_back inlined: placement-new null test, then the implicit copy constructor (base vtable store,
//   Bank/Index copied, own vtable store, counted pointer copied and addref'd), `add [ecx+4],10h`; else
//   `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)` (0x00A2C960, manifest "Vector_ReallocInsert_CScriptThing"
//   is propagated) with the empty tag homed on x's own (still live) argument slot. Here a bare declaration of
//   _M_insert_overflow reproduces that homing; no same-TU body is needed.
inline void* operator new(unsigned int, void* where) { return where; }

class CMeshDataBank;

class CMeshDataBankEntry {                              // vtable 0x0129CD28
public:
    virtual void v00();
    CMeshDataBank* Bank;                                // +0x04
    unsigned long  Index;                               // +0x08
};

class CIVCountedPointeeBase {
public:
    virtual void Destroy(int flags);
    unsigned long IVRefCount;                           // +0x04
};

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

class CPhysicsMeshInfo : public CMeshDataBankEntry {    // vtable 0x0129CD50, 0x10 bytes
public:
    class CMeshData;
    virtual void v00();
    CIVCountedPointer<CMeshData> MeshData;              // +0x0C
};

namespace std {
struct __false_type { __false_type() {} };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x00A2C960
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

template class std::vector<CPhysicsMeshInfo>;