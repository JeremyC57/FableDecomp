// CEnginePrimitiveAttachmentManager::`scalar deleting destructor'(unsigned int flags) -- the inlined destructor (both vtables reset, the AttachedPrimitives list cleared and its sentinel freed, then the CEngineComponent and CBase base destructors) and the flag-tested operator delete
// Leaf: ??_GCEnginePrimitiveAttachmentManager@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3ADC0, 83 bytes. __fastcall, `ret 4`, returns this.
// Identity: slot 0 of CEnginePrimitiveAttachmentManager's primary vtable 0x012A1AB0 (rebuild/manifest/rtti-vtables.tsv;
//   the CEngineComponent-in-manager vtable 0x012A1A78 is the second store); FableWin.pdb names
//   `??_GCEnginePrimitiveAttachmentManager@@UAEPAXI@Z` / `??1CEnginePrimitiveAttachmentManager@@UAE@XZ`. The catalog
//   label "CGuiControlTreePane_Destructor" is propagated.
// Callees: list<CEngineInternalAttachedPrimitiveInfo>::clear() 0x00B3A760 (STLport _List_base::clear: each node's
//   element destroyed through its virtual destructor, node freed, sentinel relinked; its catalog label
//   "CGuiControlTreePane::SortTreeRecursively" is propagated), free 0x00BFEA14 (the sentinel), CEngineComponent::
//   ~CEngineComponent 0x00B59730 (the +4 base), CBase::~CBase 0x0099A300 (the +0 base), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CEnginePrimitiveAttachmentManager (28 bytes there: 16 bytes of bases, then the 12-byte Dinkumware
//   list) -- retail: CBase (vptr only) +0x00, CEngineComponent (Ego_r 12 bytes: vptr, GlobalPointerAddress,
//   RenderEnabled, KeepAlive) +0x04, the 4-byte STLport list AttachedPrimitives +0x10. Bases modelled locally (real
//   virtual destructors are needed for the vtable stores; the generated headers model vptrs as data).
// Shape: STLport `~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }` with the allocator's
//   `if (p != 0) free(p)` (the landed list<CAnimSet> ??_G 0x0070B9B0 model), then the bases in reverse order.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)

class CBase {
public:
    virtual ~CBase();                                   // 0x0099A300
};

class CEngineComponent {
public:
    virtual ~CEngineComponent();                        // 0x00B59730
    void**        GlobalPointerAddress;                 // +0x04
    bool          RenderEnabled;                        // +0x08
    unsigned char KeepAlive;                            // +0x09
};

class CEngineInternalAttachedPrimitiveInfo;
struct SAttachedPrimitiveNode;                          // STLport _List_node<CEngineInternalAttachedPrimitiveInfo>

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
}

class CAttachedPrimitiveList {                          // list<CEngineInternalAttachedPrimitiveInfo> (retail 4 bytes)
public:
    std::_STLP_alloc_proxy<SAttachedPrimitiveNode*, SAttachedPrimitiveNode, std::allocator<SAttachedPrimitiveNode> > _M_node;
    void clear();                                       // 0x00B3A760
    ~CAttachedPrimitiveList()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
};

class CEnginePrimitiveAttachmentManager : public CBase, public CEngineComponent {
public:
    CAttachedPrimitiveList AttachedPrimitives;          // +0x10
    virtual ~CEnginePrimitiveAttachmentManager();
};

CEnginePrimitiveAttachmentManager::~CEnginePrimitiveAttachmentManager()
{
}