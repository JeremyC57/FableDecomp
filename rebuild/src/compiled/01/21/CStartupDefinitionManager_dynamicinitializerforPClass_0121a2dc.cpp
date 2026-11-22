// CStartupDefinitionManager::`dynamic initializer for 'PClass'' -- register the atexit destructor of the counted pointer CStartupDefinitionManager::PClass
// Leaf: _$E1
// Retail: Fable.exe @ 0x0121A2DC, 12 bytes: push 0x01228AF1 ; call _atexit 0x004012BC ; pop ecx ; ret (the inline default ctor's two zero stores fold away: the static is zero-initialised).
// Name: FableWin.pdb 0x0100C6EC `dynamic initializer for 'CStartupDefinitionManager::PClass'' / 0x0101B611 `dynamic atexit destructor
//   for 'CStartupDefinitionManager::PClass'' (the same pair in Ego_r.pdb and Ego_d.pdb); the static is
//   ?PClass@CStartupDefinitionManager@@1V?$CCountedPointer@VCStartupDefinitionManager@@@@A (all three PDBs, protected).
// Initializer / atexit pair unit (NF234, 2026-11-22, q12647 / q12651; EXTRA_RULES "static initializer / atexit pairs"): 0x0121A2DC and
//   0x01228AF1 are the _$E1 / _$E2 VC7.1 emits for this one definition, so both rows compile the same TU (the 0x01223EC0 / 0x0122A5E0
//   NullString pattern). VC7.1 emits a static's atexit destructor only in the TU that defines the static, so both objects define PClass:
//   the pair's known LNK2005 (debt under the static-pair rule until the TU-emitter rule lands). Was a stand-in (AutoTinyThunkTarget /
//   g_AutoTinyThunkObject_013b8760 / Invoke_00433cc0, CGlobal_0121a2dc).
// Opt: /O2.
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_PCLASS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
#include "engine/CStartupDefinitionManager.h"

CCountedPointer<CStartupDefinitionManager> CStartupDefinitionManager::PClass;   // 0x013B8760
