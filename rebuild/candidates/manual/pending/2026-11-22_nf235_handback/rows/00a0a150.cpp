// CMeshBankManager::~CMeshBankManager() -- release every bank pointer held in the bank list, then destroy the list and the CBase part
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0A150, 112 bytes (padding included).
// Leaf: ??1CMeshBankManager@@UAE@XZ
// Shape: the class vtables for both subobjects (+0 0x0129C98C, +4 0x0129C980) are stored, then the
// body walks MeshDataBanks (+8, STLport list, sentinel re-read every step) and sets each entry's
// CCountedPointer<CMeshDataBank> to null: inline release (count block: references +0, __fastcall
// deleter +4 run on the object +8, block freed with operator delete 0x00BFE9BC) and the two zero
// stores block-then-object. The list member's destructor follows: out-of-line clear() (0x00A0A720,
// manifest "remove_if" is a byte-twin label) and the sentinel freed when present (0x00BFEA14); the
// CBaseClassNonCopyable destructor (0x0099A300) is the tail jump. Layout per the landed ctor 0x00A0A110.
// Header port (NF235, 2026-11-22, q12657): the local SCountBlock / CCountedPointer / CMeshDataBankInfo / CBankInfoList / CBaseClassNonCopyable /
//   CMeshBankListener / CMeshBankManager models replaced by engine/CMeshBankManager.h (CTOR_DTOR), engine/CCountedPointer.h (Reset(0) over the
//   Data-first RELEASEDATA body: the dead first Data store folds, leaving Info then Data zeroed, as retail 0x00A0A193 / 0x00A0A196; new
//   CMESHDATABANK_RELEASEDATA_SIZE_BODY / _RESET_SIZE_BODY + DELETEDATA_DECL_ONLY: the pick-any copies equal owners 0x0042918C / 0x0042FC86) and stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE:
//   the list dtor calls _List_base::clear out of line, ?clear@?$_List_base@VCMeshDataBankInfo@CMeshBankManager@@V?$allocator@VCMeshDataBankInfo@CMeshBankManager@@@std@@@std@@QAEXXZ,
//   the self leaf of 0x00A0A720; was ?clear@CBankInfoList@@QAEXXZ). The vtables now carry their PDB / RTTI names
//   ??_7CMeshBankManager@@6BCBaseClassNonCopyable@@@ / @@6BCDeviceResetCallback@@@ (was 6BCMeshBankListener) and their retail slots.
//   Entry field names from Ego_r.pdb (CMeshDataBankInfo: BankHandle +0, MeshDataBank +4).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CMESHDATABANK_RELEASEDATA_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_CMESHDATABANK_RESET_SIZE_BODY
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CMESHBANKMANAGER_CTOR_DTOR
#include "engine/CMeshBankManager.h"

CMeshBankManager::~CMeshBankManager()
{
    for (std::list<CMeshDataBankInfo>::iterator it = MeshDataBanks.begin(); it != MeshDataBanks.end(); ++it)
        it->MeshDataBank.Reset(0);
}
