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
//   ~CEngineComponent 0x00B59730 (the +4 base), CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (the +0 base), operator delete 0x00BFE9BC.
// Layout: Ego_r.pdb CEnginePrimitiveAttachmentManager (28 bytes there: 16 bytes of bases, then the 12-byte Dinkumware
//   list) -- retail: CBase (vptr only) +0x00, CEngineComponent (Ego_r 12 bytes: vptr, GlobalPointerAddress,
//   RenderEnabled, KeepAlive) +0x04, the 4-byte STLport list AttachedPrimitives +0x10.
// Shape: STLport `~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }` with the allocator's
//   `if (p != 0) free(p)` (the landed list<CAnimSet> ??_G 0x0070B9B0 model), then the bases in reverse order.
// NF192 (q5164): ported onto engine/CEnginePrimitiveAttachmentManager.h (FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_LIST, the
//   0x00B39F70 opt-in: std::list<CEngineInternalAttachedPrimitiveInfo> AttachedPrimitives, STLport clear out of line), so the clear
//   call binds the 0x00B3A760 self Leaf ?clear@?$_List_base@VCEngineInternalAttachedPrimitiveInfo@@...@std@@QAEXXZ (was the stand-in
//   ?clear@CAttachedPrimitiveList@@QAEXXZ); the row-local CBaseClassNonCopyable / CEngineComponent / CAttachedPrimitiveList /
//   CEnginePrimitiveAttachmentManager models are gone.
// Fidelity: same-TU definition of ~CEnginePrimitiveAttachmentManager (0x00B39F70, the landed row's body) as at HEAD: retail expands the
//   destructor into this ??_G, so its body is visible here; the strong ??1 copy was already defined by the HEAD row.
#define FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_LIST
#include "engine/CEnginePrimitiveAttachmentManager.h"

CEnginePrimitiveAttachmentManager::~CEnginePrimitiveAttachmentManager()
{
}
