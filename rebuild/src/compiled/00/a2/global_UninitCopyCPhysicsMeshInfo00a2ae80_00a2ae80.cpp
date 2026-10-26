// _global::UninitCopy_CPhysicsMeshInfo_00a2ae80(const CPhysicsMeshInfo* first, const CPhysicsMeshInfo* last, CPhysicsMeshInfo* result, const __false_type&) -- STLport __uninitialized_copy over 16-byte CPhysicsMeshInfo records: placement-copy each record (CMeshDataBankEntry base, then the counted mesh data), return the advanced result
// Leaf: UninitCopy_CPhysicsMeshInfo_00a2ae80
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2AE80, length 71 bytes (+ int3 padding), `ret 8`.
// Callers: the vector<CPhysicsMeshInfo> helpers @ 0x00A2B030 (call 0x00A2B0A9), the reserve @ 0x00A2C2B0 (call
//   0x00A2C30F) and the regrowth @ 0x00A2C960 (calls 0x00A2C9B9 / 0x00A2CA1F).
// Callees: none (both copy constructors are implicit and inline). Vtables: 0x0129CD28 = CMeshDataBankEntry, 0x0129CD50 =
//   CPhysicsMeshInfo (rtti-vtables.tsv).
// Layout (Ego_r.pdb): CMeshDataBankEntry (12 bytes) = vptr +0x0, CMeshDataBank* Bank +0x4, unsigned long Index +0x8;
//   CPhysicsMeshInfo (16 bytes) = that base + CIVCountedPointer<CPhysicsMeshInfo::CMeshData> MeshData +0xC (intrusive:
//   the pointee keeps its count at +4).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` under /Gr (`__fastcall`: first in
//   ecx, last in edx, result + the tag reference on the stack). The implicit copy constructor runs the base's implicit
//   copy (base vtable store, Bank, Index), then stores the derived vtable and copies MeshData with its add-ref
//   (`inc [esi+4]`): two vtable stores because the base copies members between them.
// Label correction: the catalog's `_Uninit_copy<..CScriptThing..>` is propagated (CScriptThing is 12 bytes).
inline void* operator new(unsigned int, void* where) { return where; }

class CMeshDataBank;

class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);           // slot 0
    virtual void Delete();                     // slot 1
    unsigned long IVRefCount;                  // +0x04
};

template <class T> class CIVCountedPointer { public:  // intrusive counted pointer, 4 bytes
    T* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

class CMeshDataBankEntry {
public:
    virtual ~CMeshDataBankEntry();             // vtable 0x0129CD28
    CMeshDataBank* Bank;                       // +0x4
    unsigned long Index;                       // +0x8
};

class CPhysicsMeshInfo : public CMeshDataBankEntry {
public:
    class CMeshData : public CIVCountedPointeeBase { public: };
    virtual ~CPhysicsMeshInfo();               // vtable 0x0129CD50
    CIVCountedPointer<CMeshData> MeshData;     // +0xC
};

struct __false_type { __false_type() { } };

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

CPhysicsMeshInfo* __fastcall UninitCopy_CPhysicsMeshInfo_00a2ae80(const CPhysicsMeshInfo* first, const CPhysicsMeshInfo* last, CPhysicsMeshInfo* result, const __false_type&)
{
    CPhysicsMeshInfo* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}