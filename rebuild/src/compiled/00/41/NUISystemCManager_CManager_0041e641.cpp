// NUISystem::CManager::~CManager() -- destructor of the front-end UI manager: own vtable; clear a showing error message; hand the manager singleton empty mesh and graphics banks; then the members (+0x88 down to +0x10) and the base are destroyed
// Leaf: ??1CManager@NUISystem@@IAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041E641, length 146 bytes. Size-optimised TU: `#pragma optimize("s", on)`.
// Names (NF226, 2026-11-22, q12566): protected and non-virtual, ??1CManager@NUISystem@@IAE@XZ in Ego_r.pdb (0x00438C6F), FableWin.pdb
//   (0x01AE5392) and Ego_d.pdb; engine/NUISystem_CManager.h `protected: CManager(); ~CManager();`. The binders 0x0042FB40 / 0x0042DB20 (??_G)
//   already bind that spelling. Was a row-local model (NUISystem::CManager on a `__declspec(novtable)` CManagerBase stand-in with a virtual
//   destructor and SMember24 / 60 / 70 / 88 member stand-ins) that emitted ??1CManager@NUISystem@@UAE@XZ (public virtual).
// Model: engine/NUISystem_CManager.h (retail layout, 0xD0: banks +0x10 / +0x18, Keys +0x24, the two lists +0x5C / +0x60, Layers +0x70,
//   BastardChild +0x80, BastardChildren +0x88, m_bHasErrorMessage +0x9C) with FABLE_RETAIL_CMANAGER_ERRORMESSAGE (ClearErrorMessage
//   0x0041CE6A), _WORLDINIT (SetMeshBank 0x0041D198, by value) and the new _SETGRAPHICSBANK (SetGraphicsBank 0x0042A9B7, by value).
//   The member destructors this size TU calls out of line are the header's STLport instances, each equal to its retail owner with its
//   relocations bound alike: ~_Rb_tree for BastardChildren 0x0042CE63, CCountedPointer<CComponent>::ReleaseData 0x004291DE (the
//   FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA body), ~_Rb_tree for Layers 0x0042B7D7, ~_List_base for UIActionParams 0x0042AB1C and
//   ThingActionParams 0x0042AB73, ~_Rb_tree for Keys 0x0042B796, ReleaseData<CMeshDataBank> 0x0042918C, ReleaseData<CGraphicDataBank>
//   0x00419108, and the base NUISystem::CObservable's implicit destructor 0x0042BEC0 (Ego_r.pdb / FableWin.pdb ??1CObservable@NUISystem@@QAE@XZ),
//   the tail jump.
// Shape: the empty counted-pointer arguments are default-constructed in their argument slots, zeroed from the size-TU zero register ebx
//   (`push ecx ; push ecx ; mov eax,esp ; mov [eax],ebx`); GetInstance is evaluated after the argument.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CMANAGER_ERRORMESSAGE
#define FABLE_RETAIL_CMANAGER_WORLDINIT
#define FABLE_RETAIL_CMANAGER_SETGRAPHICSBANK
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
CManager::~CManager()
{
    if (m_bHasErrorMessage)
        ClearErrorMessage();
    GetInstance()->SetMeshBank(CCountedPointer<CMeshDataBank>());
    GetInstance()->SetGraphicsBank(CCountedPointer<CGraphicDataBank>());
}
}
