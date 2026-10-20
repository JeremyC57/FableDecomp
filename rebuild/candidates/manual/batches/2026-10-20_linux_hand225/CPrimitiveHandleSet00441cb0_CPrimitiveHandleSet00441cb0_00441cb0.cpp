// CPrimitiveHandleSet_00441cb0::~CPrimitiveHandleSet_00441cb0() -- destructor of an unidentified engine record: free its POD vector, destroy three vectors of primitive handles (virtual destructor per element), the embedded primitive handle (last-handle notification, release) and the first handle vector
// Leaf: ??1CPrimitiveHandleSet_00441cb0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00441CB0, 296 bytes (+ int3 padding), plain `ret`.
// Label: the catalog's "CObject::~CObject" is a propagated name (none of the three PDB CObject classes has these
//   members); the body is the member-wise destructor of a non-polymorphic record whose scalar deleting destructor is
//   0x00442DA0 (`delete` helper 0x00443890); its real class is still unidentified, so it is named after its address.
// Callees: free 0x00BFEA14, the handles' virtual destructor (slot 0, called with 0), the primitive's virtual slots 3
//   (+0x0C, OnLastHandleReleased(handle)) and 1 (+0x04, Destroy), ~CBaseClass 0x0099A2E0 (out of line).
// Layout: +0x1c vector<CEnginePrimitiveHandle>, +0x5c CEnginePrimitiveHandle (vtable 0x0122F598, 8 bytes),
//   +0x64/+0x70/+0x7c vector<CEnginePrimitiveHandle>, +0x88 a POD vector (STLport 12-byte vectors).
// Shape: STLport `~vector()` = by-value `_Destroy(first, last)` (bounds cached) + `if (start) free(start)`; the handle's
//   inline destructor is `Clear()` (the 0x00AF70D0 form) then the counted pointer's own release and the base call.
extern "C" void __cdecl free(void* block);          // 0x00BFEA14

class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
};
class CBaseClass {                                 // vtable 0x01231710
public:
    virtual ~CBaseClass();                         // 0x0099A2E0
};
class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};

template <class T> __forceinline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct CRetailVector {          // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline ~CRetailVector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
};
struct CRetailPODVector {
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    __forceinline ~CRetailPODVector()
    {
        if (_M_start)
            free(_M_start);
    }
};

struct CPrimitiveHandleSet_00441cb0 {
    unsigned char                         _pad00[0x1c];
    CRetailVector<CEnginePrimitiveHandle> Handles1c;   // +0x1c
    unsigned char                         _pad28[0x5c - 0x28];
    CEnginePrimitiveHandle                Handle5c;    // +0x5c
    CRetailVector<CEnginePrimitiveHandle> Handles64;   // +0x64
    CRetailVector<CEnginePrimitiveHandle> Handles70;   // +0x70
    CRetailVector<CEnginePrimitiveHandle> Handles7c;   // +0x7c
    CRetailPODVector                      Data88;      // +0x88
    ~CPrimitiveHandleSet_00441cb0();
};

CPrimitiveHandleSet_00441cb0::~CPrimitiveHandleSet_00441cb0()
{
}
