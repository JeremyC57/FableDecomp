// CEnginePrimitiveAttachmentManager::~CEnginePrimitiveAttachmentManager() -- destroy every attached-primitive record and the list sentinel, then the CEngineComponent and CBaseClassNonCopyable bases
// Leaf: ??1CEnginePrimitiveAttachmentManager@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B39F70, 62 bytes, thiscall, tail `jmp ??1CBaseClassNonCopyable@@MAE@XZ` (0x0099A300).
// Identity (two sources): stores vtables 0x012A1AB0 (+0) and 0x012A1A78 (+4) first, both RTTI .?AVCEnginePrimitiveAttachmentManager@@
//   (: CBaseClassNonCopyable +0, CEngineComponent +4; rtti_bases.tsv), FableWin.pdb ??1CEnginePrimitiveAttachmentManager@@UAE@XZ; it sits
//   right after the landed constructor 0x00B39F30, and the landed ??_G 0x00B3ADC0 inlines the same body. The catalog's
//   "CTCInventoryStats::~CTCInventoryStats" is propagated.
// Shape: no user code; the member list's STLport ~_List_base (clear(), then the allocator's `if (p) free(p)` on the sentinel), then
//   the bases in reverse order.
// Callees: std::_List_base<CEngineInternalAttachedPrimitiveInfo, std::allocator<...> >::clear 0x00B3A760, free 0x00BFEA14,
//   ??1CEngineComponent@@UAE@XZ 0x00B59730, ??1CBaseClassNonCopyable@@MAE@XZ 0x0099A300.
// Header: engine/CEnginePrimitiveAttachmentManager.h (FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_LIST, new).
#define FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_LIST
#include "engine/CEnginePrimitiveAttachmentManager.h"

CEnginePrimitiveAttachmentManager::~CEnginePrimitiveAttachmentManager()
{
}