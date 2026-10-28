// CEnginePrimitiveAttachmentManager::UpdateSceneForPreload() -- for every attached primitive still referenced outside the manager, update its output primitive at time 0 and enable that output primitive
// Leaf: ?UpdateSceneForPreload@CEnginePrimitiveAttachmentManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B3A0D0, 63 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?UpdateSceneForPreload@CEnginePrimitiveAttachmentManager@@QAEXXZ; AttachedPrimitives +0x10
//   is list<CEngineInternalAttachedPrimitiveInfo> (node data +8: Info vptr, PrimitiveHandle +4 = node +0x0C, its Data
//   object node +0x10); callee ?UpdateOutputPrimitive@CEngineInternalAttachedPrimitive@@QAEXM@Z 0x00B399A0 (`push 0`
//   = 0.0f); CEngineInternalAttachedPrimitive::OutputPrimitive +0x8C (Data +0x90); CEngineInternalPrimitiveBase::Enable is
//   bit 0 of the +0x34 bit-field byte.
// Shape: STLport list iteration with end() re-read every pass; the test is the handle's inline GetReferenceCount() > 1
//   (FableWin.pdb ?GetReferenceCount@CEnginePrimitiveHandle@@ over ?GetRefCount@?$CIVCountedPointer@V
//   CEnginePersistentPrimitiveBase@@@@: null -> 0, so the conditional value lands in eax and moves to ecx for the call;
//   the plain IsNull()/IVRefCount form loads ecx directly); the handle data re-fetched after the call; the constant 1
//   held in ebx for both the unsigned compare and the bit-field `or`.
// Header debt: the engine headers flatten the list, the handles and the bit-field byte, so the members used are
//   modelled locally.
class CBaseClass {
public:
    CBaseClass();                                    // 0x0099A2D0
    virtual ~CBaseClass();                           // 0x0099A2E0
};

namespace std {
template <class _Tp> class allocator { };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _Tp* operator->() const { return &(operator*()); }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    iterator end() { return iterator(_M_node._M_data); }
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;   // 4 bytes
};
}

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    mutable unsigned long IVRefCount;                // +0x04
};
template <class T> class CIVCountedPointer {
public:
    bool IsNull() const { return Object == 0; }
    T* Get() const { return static_cast<T*>(Object); }
    T& operator*() const { return *Get(); }
    unsigned long GetRefCount() const { return Object != 0 ? Object->IVRefCount : 0; }
    CIVCountedPointeeBase* Object;
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
public:
    long Type;                                       // +0x08
};
class CEnginePrimitiveHandle : public CBaseClass {
public:
    bool IsNull() const { return Data.IsNull(); }
    CEnginePersistentPrimitiveBase& GetData() const { return *Data; }
    unsigned long GetReferenceCount() const { return Data.GetRefCount(); }
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
};

class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
    unsigned char  _pad0C[0x34 - 0x0C];              // +0x0c .. RenderLayerMask
    unsigned char  Enable : 1;                       // +0x34 bit 0
    unsigned char  NonPersistent : 1;
    unsigned char  OverrideFadeDistance : 1;
    unsigned char  PostRenderEnable : 1;
    unsigned short LastFrameRendered;                // +0x36
    unsigned char  _pad38[0x48 - 0x38];              // NextPrimitive, RefPrimitive, ...
};

class CEngineInternalAttachedPrimitive : public CEngineInternalPrimitiveBase {
public:
    unsigned char          _pad48[0x8c - 0x48];      // PrimitiveTransform +0x48, ParentType, ChildType, ParentHandle, ParentGroupIndex
    CEnginePrimitiveHandle OutputPrimitive;          // +0x8c
    void UpdateOutputPrimitive(float time);          // 0x00B399A0
};

class CEngineInternalAttachedPrimitiveInfo : public CBaseClass {
public:
    CEnginePrimitiveHandle PrimitiveHandle;          // +0x04
};

class CEnginePrimitiveAttachmentManager {
public:
    virtual ~CEnginePrimitiveAttachmentManager();
    unsigned char _pad04[0x10 - 0x04];               // +0x04 base words
    std::list<CEngineInternalAttachedPrimitiveInfo> AttachedPrimitives;   // +0x10
    void UpdateSceneForPreload();
};

void CEnginePrimitiveAttachmentManager::UpdateSceneForPreload()
{
    for (std::list<CEngineInternalAttachedPrimitiveInfo>::iterator it = AttachedPrimitives.begin();
         it != AttachedPrimitives.end(); ++it) {
        CEnginePrimitiveHandle& handle = it->PrimitiveHandle;
        if (handle.GetReferenceCount() > 1) {
            static_cast<CEngineInternalAttachedPrimitive&>(handle.GetData()).UpdateOutputPrimitive(0.0f);
            static_cast<CEngineInternalPrimitiveBase&>(
                static_cast<CEngineInternalAttachedPrimitive&>(handle.GetData()).OutputPrimitive.GetData()).Enable = true;
        }
    }
}