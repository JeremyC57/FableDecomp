// CEngineInternalPrimitiveGroup::ClearChildPrimitive(unsigned long index) -- when child slot `index` holds a primitive, release its handle (last-handle notification + counted release) and recompute the render-layer mask
// Grade: DIFFER(97v97)/(106v106) (retry wave 2, improved; same length now) -- a reference local to the handle's Data
//   inside the inline Clear() keeps the slot address in esi as retail; residue: IsUnique copies Data into ecx before its
//   null test (retail tests eax and copies for the call). Tried: handle pointer, early-return and ternary IsUnique
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC62D0, length 97 bytes (the viewport body 0x00BE2120 is the same code with the vector at +0x94, 106 bytes).
// Callees: the primitive's virtual slots 3 (+0x0C) and 1 (+0x04); UpdateRenderLayerMask @ 0x00BC6180 (out of line, `mov ecx,edi`).
// Shape: handle vector (STLport, 8-byte CEnginePrimitiveHandle = vptr + counted Data) at +0x50; `index < size()` unsigned with the first
// pointer re-read for `*(begin()+n)` (the iterator object is what makes the size/first loads match); IsValid + the agent-61 handle Clear.
// Tried: reference local, pointer local, iterator local, a bool ClearIfSet() helper -- all fold the first Data load.
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

class CEnginePrimitiveHandle {                     // 8 bytes: vptr (0x0122F598) + counted Data
public:
    virtual void v000();
    CIVCountedPointer Data;                        // +0x04
    bool IsValid() const { return Data.Object != 0; }
    __forceinline void Clear()
    {
        CIVCountedPointer& data = Data;
        if (data.IsUnique())
            data->OnLastHandleReleased(this);
        data = 0;
    }
};

struct CHandleArray {                              // CArray<CEnginePrimitiveHandle>: first, last, end (retail 12 bytes)
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    struct iterator {
        CEnginePrimitiveHandle* m_p;
        iterator(CEnginePrimitiveHandle* p) : m_p(p) { }
        iterator operator+(unsigned long n) const { return iterator(m_p + n); }
        CEnginePrimitiveHandle& operator*() const { return *m_p; }
    };
    iterator begin() const { return iterator(m_pFirst); }
    CEnginePrimitiveHandle& operator[](unsigned long n) const { return *(begin() + n); }
};

struct CEngineInternalPrimitiveGroup {
    unsigned char _pad00[0x50];
    CHandleArray  Primitives;                      // +0x50
    void UpdateRenderLayerMask();                  // 0x00BC6180
    void ClearChildPrimitive(unsigned long index);
};

void CEngineInternalPrimitiveGroup::ClearChildPrimitive(unsigned long index)
{
    if (index < Primitives.size()) {
        CEnginePrimitiveHandle& handle = Primitives[index];
        if (handle.IsValid()) {
            handle.Clear();
            UpdateRenderLayerMask();
        }
    }
}
