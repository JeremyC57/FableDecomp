// CEngineInternalPrimitive2DViewport::ClearChildPrimitive(unsigned long index) -- releases child slot `index`: drops its primitive handle (last-handle notification + counted release) and empties the matching per-child POD vector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF70D0, length 147 bytes; same body as 0x00AF7170 with the members at other offsets. No xrefs found.
// Callees: the primitive's virtual slots 3 (+0x0C, OnLastHandleReleased(handle)) and 1 (+0x04, Destroy when the count reaches zero),
// memmove @ 0x00BFEAE6 (the STLport __copy_trivial inside vector::erase).
// Label: the manifest name is propagated; the owner is NOT the PDB CEngineInternalPrimitive2DViewport (its Primitives sit at +0x94 and
// there is no parallel vector), so the owner is modelled locally: a handle vector at +0x90 (8-byte CEnginePrimitiveHandle = vptr + counted
// Data) and, at +0x84, a vector of 12-byte STLport vectors (start, finish, end) of POD bytes indexed in step (`lea ecx,[edi+edi*2]`).
// Shape: `size() > index` (size on the left gives `cmp eax,edi ; jbe`); the handle Clear is the agent-61 CEnginePrimitiveHandle release
// (`IsUnique` through operator->, `Data = 0` = release + a second zero store, both `__forceinline`); the inner clear() is STLport
// `erase(begin(), end())` whose `__copy_trivial(last, finish, first)` compares finish with itself (`cmp eax,eax`) and calls memmove with a zero
// count, then stores the new finish in both arms.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

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
};

class CEnginePrimitiveHandle {
public:
    virtual void v000();
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

struct CPODVector {
    unsigned char* m_start;
    unsigned char* m_finish;
    unsigned char* m_endOfStorage;
    unsigned char* erase(unsigned char* first, unsigned char* last)
    {
        unsigned char* i = (unsigned char*)CopyTrivial(last, m_finish, first);
        m_finish = i;
        return first;
    }
    void clear() { erase(m_start, m_finish); }
};

struct CHandleVector {
    CEnginePrimitiveHandle* m_start;
    CEnginePrimitiveHandle* m_finish;
    CEnginePrimitiveHandle* m_endOfStorage;
    unsigned long size() const { return m_finish - m_start; }
    CEnginePrimitiveHandle& operator[](unsigned long n) { return m_start[n]; }
};

struct CVectorArray {
    CPODVector* m_start;
    CPODVector* m_finish;
    CPODVector* m_endOfStorage;
    CPODVector& operator[](unsigned long n) { return m_start[n]; }
};

struct CEngineInternalPrimitive2DViewport {
    unsigned char _pad00[0x84];
    CVectorArray  m_childData;                     // +0x84
    CHandleVector m_childHandles;                  // +0x90
    void ClearChildPrimitive(unsigned long index);
};

void CEngineInternalPrimitive2DViewport::ClearChildPrimitive(unsigned long index)
{
    if (m_childHandles.size() > index) {
        m_childHandles[index].Clear();
        m_childData[index].clear();
    }
}