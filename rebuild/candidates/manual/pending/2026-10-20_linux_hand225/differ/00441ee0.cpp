// CPrimitiveHandleSet_00441ee0::~CPrimitiveHandleSet_00441ee0() -- destructor of a second unidentified engine record (sibling of 0x00441CB0): destroy two vectors of primitive handles, two embedded primitive handles (last-handle notification, release) and the first handle vector
// Grade: DIFFER(307v297) -- register-constant hoisting: ours loads 1 into ebx for the two IsUnique compares (`mov ebx,1 ; cmp [eax+4],ebx`, retail `cmp dword ptr [eax+4],1`) and aligns the last loop head
// Leaf: ??1CPrimitiveHandleSet_00441ee0
// Retail: Fable.exe (TLC, VC7.1) @ 0x00441EE0, 297 bytes, plain `ret`.
// Label: the catalog's "CPSCDecalRenderer::~CPSCDecalRenderer" is a propagated name (the PDB CPSCDecalRenderer has its
//   handles at +0x4c/+0x58 and no vectors); the body is the member-wise destructor of a non-polymorphic record whose
//   scalar deleting destructor is 0x00442F00; the class is unidentified, so it is named after its address.
// Callees: free 0x00BFEA14, the handles' virtual destructor (slot 0, called with 0), the primitive's virtual slots 3
//   (+0x0C, OnLastHandleReleased(handle)) and 1 (+0x04, Destroy), ~CBaseClass 0x0099A2E0 (out of line).
// Layout: +0x1c vector<CEnginePrimitiveHandle>, +0x2c and +0x34 CEnginePrimitiveHandle (vtable 0x0122F598, 8 bytes),
//   +0x3c/+0x48 vector<CEnginePrimitiveHandle> (STLport 12-byte vectors).
// Shape: as 0x00441CB0 (same TU, same inline vector / handle destructors); here VC7.1 keeps one zero register (ebp) and
//   forwards the cleared counted pointer into the member destructor's test.
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

struct CPrimitiveHandleSet_00441ee0 {
    unsigned char                         _pad00[0x1c];
    CRetailVector<CEnginePrimitiveHandle> Handles1c;   // +0x1c
    unsigned char                         _pad28[0x2c - 0x28];
    CEnginePrimitiveHandle                Handle2c;    // +0x2c
    CEnginePrimitiveHandle                Handle34;    // +0x34
    CRetailVector<CEnginePrimitiveHandle> Handles3c;   // +0x3c
    CRetailVector<CEnginePrimitiveHandle> Handles48;   // +0x48
    ~CPrimitiveHandleSet_00441ee0();
};

CPrimitiveHandleSet_00441ee0::~CPrimitiveHandleSet_00441ee0()
{
}
