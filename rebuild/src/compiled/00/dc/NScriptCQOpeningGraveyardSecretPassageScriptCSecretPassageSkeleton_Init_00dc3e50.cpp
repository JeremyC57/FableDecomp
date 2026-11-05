// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton::Init() -- reset the entity script: no skeleton part returned yet, the four ceremonial-skeleton object names (sword, shield, armour, helmet) assigned, collected_all cleared
// Leaf: ?Init@CSecretPassageSkeleton@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: engine/CCountedPointer.h included first (the header's PImp is a by-value CCountedPointer<CScriptThing>, Data +0x04 / Info +0x08; the row never expands its members).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC3E50, 68 bytes (vtable entry at 0x012DA49C of the script's table).
// Callees: CCharString::operator=(const char*) @ 0x0099EFE0 (landed), four times with the literals
// "OBJECT_CEREMONIAL_SKELETON_SWORD" @ 0x012DA51C, "..._SHIELD" @ 0x012DA4F8, "..._ARMOUR" @ 0x012DA4D4,
// "..._HELMET" @ 0x012DA4B0.
//
// Shape: the `returned[4]` clearing loop collapses to ONE dword zero store (`mov dword ptr [esi+1Ch],0`),
// scheduled between the first literal push and its `lea ecx`; then the four `push lit ; lea ecx,[esi+N] ;
// call` string assignments and the closing byte store.
// Layout = Ego_r.pdb CSecretPassageSkeleton (52 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me
// +8}, ParentClass +0x14, PMasterData +0x18, bool returned[4] +0x1C, CCharString skeleton_object[4] +0x20,
// bool collected_all +0x30 (no engine header exists for the script classes).
// Naming wave 35: identity reg 00dc28dc "SecretPassageSkeleton" in 00dc27c0; Alloc 00dc3d70; vtable 012da494 slot 2; PDB ?Init@CSecretPassageSkeleton@CQ_OpeningGraveyardSecretPassageScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CSecretPassageSkeleton::Init (wave3/labels_alloc.tsv); PDB ?Init@CSecretPassageSkeleton@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CSecretPassageSkeleton@@QAEXXZ`; this source now defines the PDB member (class model unchanged); same bytes.

class CCharString { public:
    void* m_pStringData;
    const CCharString& operator=(const char* text);    // 0x0099EFE0
};

class CGameScriptInterfaceBase;
namespace NScript { class CQ_OpeningGraveyardSecretPassageScript; } using NScript::CQ_OpeningGraveyardSecretPassageScript;
struct CQ_SunnyvaleMasterData;

#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"

class CEntityScriptBase { public:
    virtual void v00();                          // entity-script vtable
    CGameScriptInterfaceBase* Game;              // +0x04
    CScriptThing              Me;                // +0x08
};

namespace NScript {
class CQ_OpeningGraveyardSecretPassageScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CSecretPassageSkeleton;
};
}
namespace NScript {
class CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton : public CEntityScriptBase {
public:
    CQ_OpeningGraveyardSecretPassageScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*                 PMasterData;   // +0x18
    bool                                    returned[4];   // +0x1C
    CCharString                             skeleton_object[4];   // +0x20
    bool                                    collected_all; // +0x30
private:
    virtual void Init();                          // PDB ?Init@CSecretPassageSkeleton@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
};
}

void NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton::Init()
{
    for (long i = 0; i < 4; ++i)
        returned[i] = false;
    skeleton_object[0] = "OBJECT_CEREMONIAL_SKELETON_SWORD";
    skeleton_object[1] = "OBJECT_CEREMONIAL_SKELETON_SHIELD";
    skeleton_object[2] = "OBJECT_CEREMONIAL_SKELETON_ARMOUR";
    skeleton_object[3] = "OBJECT_CEREMONIAL_SKELETON_HELMET";
    collected_all = false;
}