// CMeshBankManager::CMeshBankManager() -- default constructor.
// Leaf: ??0CMeshBankManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0A110, length 57 bytes.
// Manifest label "CAIStateGroup_FlyingThingBase" is propagated; the vtables stored are 0x0129C98C (+0) and
// 0x0129C980 (+4), both CMeshBankManager (rtti-vtables.tsv): a class with two polymorphic bases.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (first base, out of line), malloc @ 0x00BFEA0E (the list head node).
// The second base's constructor is inline and only stores its vtable 0x0122DE50 at +4.
//
// Shape: base #1 ctor, base #2 vtable, then both derived vtables, then the member at +8: a VC6-era
// Dinkumware std::list whose constructor nulls the head, allocates a 0x14-byte sentinel node through
// the allocator (malloc) and links it to itself (`next = prev = node`); the size counter is not stored
// here (the list's element type is 12 bytes: 0x14 - two links).
// Header port (NF235, 2026-11-22, q12657): the local CBaseClassNonCopyable / CMeshBankListener / CRetailList0x14 / CMeshBankManager models
//   replaced by engine/CMeshBankManager.h (new CTOR_DTOR opt-in: bases CBaseClassNonCopyable + CDeviceResetCallback, the STLport
//   list<CMeshBankManager::CMeshDataBankInfo> MeshDataBanks +8). The vtables now carry their PDB / RTTI names: the inline
//   CDeviceResetCallback ctor's ??_7CDeviceResetCallback@@6B@ (0x0122DE50; FableWin / Ego_r / Ego_d publics, RTTI COL 0x0132CAD8; was
//   ??_7CMeshBankListener@@6B@), ??_7CMeshBankManager@@6BCBaseClassNonCopyable@@@ (0x0129C98C) and ??_7CMeshBankManager@@6BCDeviceResetCallback@@@
//   (0x0129C980; was 6BCMeshBankListener), slots OnPreDeviceReset 0x00A09D90 / OnPostDeviceReset 0x00A09D80 under their PDB spellings.
//   The list's sentinel comes from the shared STLport _List_base ctor (Data 0 stored first, then the 0x14-byte malloc'd node linked to itself).
//   FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE: _List_base::clear stays declared only (retail 0x00A0A720, out of line in this class TU), so the
//   TU carries no clear / CMeshDataBankInfo / CCountedPointer<CMeshDataBank> pick-any copies.
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CMESHBANKMANAGER_CTOR_DTOR
#include "engine/CMeshBankManager.h"

CMeshBankManager::CMeshBankManager()
{
}
