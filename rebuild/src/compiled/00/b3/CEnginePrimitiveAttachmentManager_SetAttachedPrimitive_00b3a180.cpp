// CEnginePrimitiveAttachmentManager::SetAttachedPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, const CEnginePrimitiveAttachmentInfo&, const CEnginePrimitiveHandle&, long, unsigned long) -- unless the handle already holds an attached primitive (type 0x2e), create one, point the handle at it and record it in the manager's list; then attach it, releasing the handle when that fails
// Leaf: SetAttachedPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3A180, length 407 bytes, `ret 0x18`.
// Callees: CBase ctor 0x0099A2D0, operator new 0x00BFEA1A, CEngineInternalPrimitiveAttached ctor 0x00B3A910, malloc
//   0x00BFEA0E (list node), ~CEngineInternalAttachedPrimitiveInfo 0x00B3AB10, CEngineInternalPrimitiveAttached::Attach
//   0x00B396E0; the persistent primitive's virtual slots 1 (destroy) and 3 (last handle released).
// Layout: CEnginePrimitiveHandle / CIVCountedPointer as the landed SetChildPrimitive 0x00BBC310 (vptr 0x0122F598,
//   counted object: RefCount +4, primitive type +8); CEngineInternalAttachedPrimitiveInfo as the landed ctor 0x00B3AAE0
//   (vptr 0x012A1AB8, Handle +4); the manager's STLport list<CEngineInternalAttachedPrimitiveInfo> header node pointer
//   at +0x10 (retail body).
// Shape: the list is modelled as STLport (by-value iterator with a copy ctor, push_back -> insert(end(), x) -> create_node)
//   so the callee-saved pushes stay at entry; insert reads position->Prev into a local before linking; the attached
//   primitive is a named pointer local, so `this` for Attach is loaded before the argument pushes.
extern "C" void* __cdecl malloc(unsigned int size); // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    long Type;                                                       // +0x08
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) {}
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
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

struct CBaseClass {
    virtual ~CBaseClass();
    CBaseClass();                                  // 0x0099A2D0
};

class CEnginePrimitiveHandle : public CBaseClass { // vptr 0x0122F598
public:
    virtual void Release();
    CIVCountedPointer Data;                        // +0x04
    CEnginePrimitiveHandle() {}
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline void Set(CEnginePersistentPrimitiveBase* object)
    {
        Clear();
        Data.Object = object;
    }
    __forceinline CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other)
    {
        Clear();
        Data = other.Data;
        return *this;
    }
};

struct CEngineInternalAttachedPrimitiveInfo : CBaseClass {   // vptr 0x012A1AB8
    virtual void Release();
    CEnginePrimitiveHandle Handle;                 // +0x04
    CEngineInternalAttachedPrimitiveInfo() {}
    ~CEngineInternalAttachedPrimitiveInfo();       // 0x00B3AB10
};

class CEnginePrimitive;
class CEnginePrimitiveAttachmentInfo;

class CEngineInternalPrimitiveAttached : public CEnginePersistentPrimitiveBase {
public:
    unsigned char _body[0xdc - 0xc];
    CEngineInternalPrimitiveAttached();            // 0x00B3A910
    bool Attach(const CEnginePrimitive& primitive, const CEnginePrimitiveAttachmentInfo& info,
                const CEnginePrimitiveHandle& parent, long boneIndex, unsigned long flags);   // 0x00B396E0
};

template <class T> struct CRetailList {            // STLport list: header node pointer
    struct SNode {
        SNode* Next;
        SNode* Prev;
        T      Value;
    };
    struct iterator {
        SNode* _M_node;
        iterator(SNode* node) : _M_node(node) {}
        iterator(const iterator& other) : _M_node(other._M_node) {}
    };
    SNode* Header;
    iterator end() { return iterator(Header); }
    SNode* create_node(const T& value)
    {
        SNode* node = (SNode*)malloc(sizeof(SNode));
        new (&node->Value) T(value);
        return node;
    }
    iterator insert(iterator position, const T& value)
    {
        SNode* node = create_node(value);
        SNode* prev = position._M_node->Prev;
        node->Prev = prev;
        node->Next = position._M_node;
        prev->Next = node;
        position._M_node->Prev = node;
        return node;
    }
    void push_back(const T& value) { insert(end(), value); }
};

class CEnginePrimitiveAttachmentManager {
public:
    unsigned char _pad[0x10];
    CRetailList<CEngineInternalAttachedPrimitiveInfo> AttachedPrimitives;   // +0x10
    void SetAttachedPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive,
                              const CEnginePrimitiveAttachmentInfo& info, const CEnginePrimitiveHandle& parent,
                              long boneIndex, unsigned long flags);
};

void CEnginePrimitiveAttachmentManager::SetAttachedPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive,
                                                             const CEnginePrimitiveAttachmentInfo& info,
                                                             const CEnginePrimitiveHandle& parent, long boneIndex,
                                                             unsigned long flags)
{
    if (!handle.Data.Object || handle.Data->Type != 0x2e) {
        CEngineInternalAttachedPrimitiveInfo attached;
        attached.Handle.Set(new CEngineInternalPrimitiveAttached());
        handle = attached.Handle;
        AttachedPrimitives.push_back(attached);
    }
    CEngineInternalPrimitiveAttached* attachedPrimitive = static_cast<CEngineInternalPrimitiveAttached*>(handle.Data.Object);
    if (!attachedPrimitive->Attach(primitive, info, parent, boneIndex, flags))
        handle.Clear();
}