// CEngineInternalPrimitiveParticleGroup::SetChildPrimitive(unsigned long id, const CEnginePrimitive& primitive, unsigned long flags)
//   -- make child `id` show `primitive`: an unused id gets a fresh empty handle pushed on the front of the primitive list (the
//   id table grown to cover it and its iterator marked used); a child whose internal primitive has another type is released,
//   a missing one is created by the type's manager; then the child is updated from the description and enabled unless flag
//   0x20 is set.
// Leaf: SetChildPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBCF70, 476 bytes, `ret 0xc`; vtable slot 8 of CEngineInternalPrimitiveParticleGroup
//   (the landed 0x00BBC310 / 0x00BBD1A0 call it as `AddChildPrimitiveWithID`; the catalog's mangled name is this overload).
// Callees: CBaseClass::CBaseClass 0x0099A2D0 / ~CBaseClass 0x0099A2E0 (the handle temporary), malloc 0x00BFEA0E (list node),
//   CArray<CPrimitiveIterator>::Resize 0x00BBDD40, manager slot 1 CreateInternalPrimitive (table 0x01436E84), internal
//   primitive slots 1/3 (release, last-handle notice) and 5 Update.
// Layout: the retail group of the landed 0x00BBC310 / 0x00BBD1A0 / 0x00BBD200 (Primitives list +0x48, PrimitiveIDToIterator
//   +0x4c, 0x14 below engine/CEngineInternalPrimitiveParticleGroup.h's PDB offsets) -- local model; handle classes as the
//   landed list resize 0x00BBDCB0 plus the counted copy constructor.
// Shape: push_front with the predecessor read once (`prev` local) before the links; the growth test is `size() <= id`; the
//   iterator is a local with only the used byte set before the struct copy (the word copy carries the padding); child starts
//   at 0 (xor edi,edi) and takes the existing pointer inside the null test; Enable is assigned `~(flags >> 5)` (bit-field xor
//   merge with `not al`).
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

struct CEnginePrimitive {
    long Type;                                         // +0x00
};
class CEnginePrimitiveHandle;

class CEngineInternalPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    virtual void v004();
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);   // slot 5 (+0x14)
    long          RefCount;                            // +0x04
    long          Type;                                // +0x08
    unsigned char _pad0c[0x34 - 0xc];
    unsigned char Enable : 1;                          // +0x34 bit 0
    unsigned char NonPersistent : 1;
    unsigned char OverrideFadeDistance : 1;
    unsigned char PostRenderEnable : 1;
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CIVCountedPointer {
public:
    CEngineInternalPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
        Object = 0;
    }
    CEngineInternalPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* p)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = p;
        return *this;
    }
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

class CBaseClass {                                 // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                  // 0x0099A2D0
    ~CBaseClass();                                 // 0x0099A2E0
};

class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    CEnginePrimitiveHandle() { }
    ~CEnginePrimitiveHandle()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
    }
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct SHandleListNode {                           // STLport list node of list<CEnginePrimitiveHandle>
    SHandleListNode*       m_pNext;
    SHandleListNode*       m_pPrev;
    CEnginePrimitiveHandle m_value;                // +0x08
};
struct CHandleList {                               // list<CEnginePrimitiveHandle> (4 bytes)
    SHandleListNode* m_pNode;
    SHandleListNode* begin() { return m_pNode->m_pNext; }
    void push_front(const CEnginePrimitiveHandle& x)
    {
        SHandleListNode* position = begin();
        SHandleListNode* node = (SHandleListNode*)malloc(sizeof(SHandleListNode));
        new (&node->m_value) CEnginePrimitiveHandle(x);
        SHandleListNode* prev = position->m_pPrev;
        node->m_pNext = position;
        node->m_pPrev = prev;
        prev->m_pNext = node;
        position->m_pPrev = node;
    }
};

struct CPrimitiveIterator {                        // 8 bytes: in-use flag and the list node
    unsigned char    m_used;
    SHandleListNode* m_pNode;
};
struct CIteratorArray {                            // CArray<CPrimitiveIterator>: first, last, end
    CPrimitiveIterator* m_pFirst;
    CPrimitiveIterator* m_pLast;
    CPrimitiveIterator* m_pEnd;
    unsigned long size() const { return m_pLast - m_pFirst; }
    void Resize(unsigned long count);              // 0x00BBDD40
    CPrimitiveIterator& operator[](unsigned long index) { return m_pFirst[index]; }
};

struct CEnginePrimitiveManager {
    virtual void v00();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);   // slot 1 (+0x04)
};
struct CEnginePrimitiveManagerTable {
    unsigned char             _pad[0x10];
    CEnginePrimitiveManager*  Managers[1];         // +0x10, indexed by primitive type
};
extern CEnginePrimitiveManagerTable* g_pPrimitiveManagers;   // 0x01436E84

class CEngineInternalPrimitiveParticleGroup {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual unsigned long AddChildPrimitive(const CEnginePrimitive& primitive, unsigned long flags);   // slot 6
    virtual void v07();
    virtual void SetChildPrimitive(unsigned long id, const CEnginePrimitive& primitive, unsigned long flags);   // slot 8
    unsigned char  _pad04[0x44];
    CHandleList    Primitives;                     // +0x48
    CIteratorArray PrimitiveIDToIterator;          // +0x4c
};

void CEngineInternalPrimitiveParticleGroup::SetChildPrimitive(unsigned long id, const CEnginePrimitive& primitive, unsigned long flags)
{
    if (id >= PrimitiveIDToIterator.size() || !PrimitiveIDToIterator[id].m_used) {
        Primitives.push_front(CEnginePrimitiveHandle());
        if (PrimitiveIDToIterator.size() <= id)
            PrimitiveIDToIterator.Resize(id + 1);
        CPrimitiveIterator it;
        it.m_used = 1;
        it.m_pNode = Primitives.begin();
        PrimitiveIDToIterator[id] = it;
    }
    CEnginePrimitiveHandle& handle = PrimitiveIDToIterator[id].m_pNode->m_value;
    CEngineInternalPrimitiveBase* child = 0;
    if (handle.Data.Object) {
        child = handle.Data.Object;
        if (child->Type != primitive.Type)
            handle.Clear();
    }
    if (!handle.Data.Object) {
        child = g_pPrimitiveManagers->Managers[primitive.Type]->CreateInternalPrimitive(primitive.Type);
        handle.Clear();
        handle.Data = child;
    }
    child->Update(primitive, flags);
    child->Enable = ~(flags >> 5);
}
