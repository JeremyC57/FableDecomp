// CEngineInternalPrimitiveParticleGroup::SetChildPrimitive(unsigned long id, CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags) -- adds the primitive under child id `id` (virtual AddChildPrimitiveWithID) and hands back the stored handle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBC310, length 139 bytes (`ret 10h`); vtable 0x012A59D4 slot 7.
// Callees: virtual slot 8 (+0x20, AddChildPrimitiveWithID(id, primitive, flags) @ 0x00BBCF70), then the inlined
// CEnginePrimitiveHandle::operator= (out-of-line copy @ 0x0043F9B0) and the primitive's slots 3/1.
// Layout: the retail group (0x14 bytes narrower than the PDB, see GOTCHAS 24h) keeps a 4-byte STLport list at +0x48 and the
// CArray<CPrimitiveIterator> as three pointers at +0x4C; each iterator holds a used byte and the list node, whose value (+8) is the
// 8-byte handle. Modelled locally with the retail offsets (engine/CEngineInternalPrimitiveParticleGroup.h has the PDB offsets).
// Shape: `PrimitiveIDToIterator[id].m_pNode->m_value` (element load folded, `add edi,8`), then `handle = value` with every handle
// helper `__forceinline` and the counted release on the pointee (see 0x0043F9B0).
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

class CIVCountedPointer {                          // CIVCountedPointer<CEnginePersistentPrimitiveBase>
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
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CEnginePersistentPrimitiveBase* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

class CEnginePrimitiveHandle {                     // 8 bytes: vptr (0x0122F598) + counted Data
public:
    virtual void v000();
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other)
    {
        Clear();
        Data = other.Data;
        return *this;
    }
};


class CEnginePrimitive;

struct SHandleListNode {                           // STLport list node of list<CEnginePrimitiveHandle>
    SHandleListNode*       m_pNext;
    SHandleListNode*       m_pPrev;
    CEnginePrimitiveHandle m_value;                // +0x08
};

struct CPrimitiveIterator {                        // 8 bytes: in-use flag and the list node
    unsigned char    m_used;
    SHandleListNode* m_pNode;
};

struct CIteratorArray {                            // CArray<CPrimitiveIterator>: first, last, end
    CPrimitiveIterator* m_pFirst;
    CPrimitiveIterator* m_pLast;
    CPrimitiveIterator* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
    CPrimitiveIterator& operator[](unsigned long index) { return m_pFirst[index]; }
};

class CEngineInternalPrimitiveParticleGroup {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual unsigned long AddChildPrimitive(const CEnginePrimitive& primitive, unsigned long flags);                          // slot 6
    virtual void SetChildPrimitive(unsigned long id, CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags);   // slot 7
    virtual void AddChildPrimitiveWithID(unsigned long id, const CEnginePrimitive& primitive, unsigned long flags);          // slot 8
    virtual void GetChildPrimitiveHandle(unsigned long id, CEnginePrimitiveHandle& handle);                                   // slot 9
    unsigned char    _pad04[0x44];
    SHandleListNode* m_pPrimitives;                // +0x48 list<CEnginePrimitiveHandle> (4-byte STLport list)
    CIteratorArray   PrimitiveIDToIterator;        // +0x4c
};

void CEngineInternalPrimitiveParticleGroup::SetChildPrimitive(unsigned long id, CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags)
{
    AddChildPrimitiveWithID(id, primitive, flags);
    handle = PrimitiveIDToIterator[id].m_pNode->m_value;
}
