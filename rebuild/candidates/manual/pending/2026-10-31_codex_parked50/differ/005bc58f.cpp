// 005bc58f NInventory::CTCInventoryBase::SetInventoryAnimRenderBox
// Codex parked-50 best attempt: DIFFER(35v35); residue: 35/35 bytes; retail computes destination in EAX before loading source into ECX; compiler assigns the opposite roles.
// Leaf: ?SetInventoryAnimRenderBox@CTCInventoryBase@NInventory@@QAEXABVC2DBoxF@@@Z
// Flags:
#include <stddef.h>
#include "rebuild_abi.h"
// This alias is an implementation-only shared-header data projection. The
// original owner is NInventory::CTCInventoryBase (class), not this generated
// global struct. Its full304-byte storage includes the flattened original base.
#define CTCInventoryBase CTCInventoryBaseFields
#include "engine/_quarantine/CTCInventoryBase.h"
#undef CTCInventoryBase
class C2DBoxF {public:float TLX,TLY,BRX,BRY;};
#include "engine/CBaseClassNonCopyable.h"
class CThing;
class CTCBase:public CBaseClassNonCopyable{
protected:CThing&ParentThing;
private:bool ToBeRemoved,IsInGlobalUpdateTCs;
};
namespace NInventory {
// Layout: original typed InventoryAnimRenderBox occupies native BC..CB. Shared
// field projection above supplies the original member offsets and total size.
class CTCInventoryBase:public CTCBase {
 unsigned char unreadBeforeBox[0xbc-sizeof(CTCBase)];
 C2DBoxF InventoryAnimRenderBox;
 unsigned char unreadAfterBox[sizeof(CTCInventoryBaseFields)-0xcc];
public:void SetInventoryAnimRenderBox(const C2DBoxF&);
};
void CTCInventoryBase::SetInventoryAnimRenderBox(const C2DBoxF&box){InventoryAnimRenderBox=box;}
}
FABLE_STATIC_ASSERT(sizeof(NInventory::CTCInventoryBase)==304);FABLE_STATIC_ASSERT(offsetof(CTCInventoryBaseFields,InventoryAnimRenderBox_TLX)==0xbc);FABLE_STATIC_ASSERT(sizeof(C2DBoxF)==16);
