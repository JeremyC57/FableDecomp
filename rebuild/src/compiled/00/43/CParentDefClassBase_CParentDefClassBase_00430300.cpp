// CParentDefClassBase::~CParentDefClassBase() -- the compiler-generated parent-def destructor: free the SubDefInfoMap storage, then the inline ~CDefClassBase / ~CDefPointeeBase (CDefPointeeBase vptr) and the out-of-line ~CResource tail
// Leaf: ??1CParentDefClassBase@@UAE@XZ
// Retail: Fable.exe @ 0x00430300, 33 bytes.
// Name (agentNF3, 2026-11-10): Ego_r.pdb / FableWin.pdb ??1CParentDefClassBase@@UAE@XZ; engine/CParentDefClassBase.h (retail branch)
//   places it here: "~CParentDefClassBase 0x00430300 (one TU with the ctor 0x00430370; the deleting dtor 0x004303F0 auto-inlines the
//   dtor body; derived deleting dtors call 0x00430300)", the out-of-line COMDAT of the implicit destructor (FABLE_RETAIL_CPARENTDEFCLASSBASE)
//   called from 122 retail sites (landed binders spell it so). Was landed as a behavioural local CDefWithBuffer::~CDefWithBuffer
//   (catalog label CTCInventoryItem::OnDie, propagated).
// Body: `if (data) free(data)` for the SubDefInfoMap CVectorMap's STLport vector at +0x29 (malloc allocator; free = 0x00BFEA14),
//   then the inline ~CDefPointeeBase stores its vptr (0x01230BA0) and tail-jumps to the out-of-line ~CResource (0x009FC550). The
//   CParentDefClassBase and CDefClassBase levels store no vptr (implicit destructors).
// Shape: the implicit destructor; the qualified call in the emission helper below makes VC7.1 emit its COMDAT (as retail's
//   deleting destructors call it). The helper is not a retail function.
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_PDCB_TYPED_MEMBERS
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CParentDefClassBase.h"

void DestroyParentDefClassBase(CParentDefClassBase* p)          // emission helper, not a retail function
{
    p->CParentDefClassBase::~CParentDefClassBase();
}
