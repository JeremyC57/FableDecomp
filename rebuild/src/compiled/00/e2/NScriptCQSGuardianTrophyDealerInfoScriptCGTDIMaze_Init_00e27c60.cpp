// NScript::CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze::Init() -- CGTDI_Maze::Init()
// Leaf: ?Init@CGTDI_Maze@CQS_GuardianTrophyDealerInfoScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E27C60, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+8] ; lea ecx,[esi+8] ; call [eax+28h] ; fstp [esi+1Ch] ; pop esi ; ret
// Guardian Trophy Dealer maze info (quest script helper). The object embedded at +0x08 carries
// its own vtable; slot 10 (+0x28) returns a float which is stored as InitialAngle (+0x1C, PDB).
// PDB also names +0x14 ParentClass (CQS_GuardianTrophyDealerInfoScript*) and +0x18 PMasterData
// (CQ_SunnyvaleMasterData*), so the vtable-bearing subobject occupies +0x08..+0x14.
// Naming wave 35: identity reg 00e27969 "GTDI_Maze" in 00e27920; Alloc 00e27bb0; vtable 012e20a4 slot 2; PDB ?Init@CGTDI_Maze@CQS_GuardianTrophyDealerInfoScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CGTDI_Maze::Init (wave3/labels_alloc.tsv); PDB ?Init@CGTDI_Maze@CQS_GuardianTrophyDealerInfoScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Init@CGTDI_Maze@@QAEXXZ`; this source now defines the PDB member (vptr placeholder dropped (own)); same bytes.

namespace NScript { class CQS_GuardianTrophyDealerInfoScript; } using NScript::CQS_GuardianTrophyDealerInfoScript;
struct CQ_SunnyvaleMasterData;

// Vtable-bearing subobject at +0x08; only slot 10 is used here.
struct CMazeAngleSource {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24();
    virtual float GetAngle();          // slot 10 (+0x28)
    unsigned char _pad_0x04[8];        // subobject is 0x0C bytes (+0x08..+0x14)
};

namespace NScript {
class CQS_GuardianTrophyDealerInfoScript {    // the PDB owner script; only its nested entity class is modelled here
public:
    class CGTDI_Maze;
};
}
namespace NScript {
class CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze {
public:
        unsigned char                       _pad_0x04[4]; // +0x04
    CMazeAngleSource                    AngleSource;  // +0x08
    CQS_GuardianTrophyDealerInfoScript* ParentClass;  // +0x14 (PDB)
    CQ_SunnyvaleMasterData*             PMasterData;  // +0x18 (PDB)
    float                               InitialAngle; // +0x1C (PDB)
private:
    virtual void Init();                          // PDB ?Init@CGTDI_Maze@CQS_GuardianTrophyDealerInfoScript@NScript@@EAEXXZ
};
}

void NScript::CQS_GuardianTrophyDealerInfoScript::CGTDI_Maze::Init()
{
    this->InitialAngle = this->AngleSource.GetAngle();
}