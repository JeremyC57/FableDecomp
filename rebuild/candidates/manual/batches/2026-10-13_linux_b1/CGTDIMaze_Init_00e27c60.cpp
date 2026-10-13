// CGTDI_Maze::Init()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E27C60, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov eax,[esi+8] ; lea ecx,[esi+8] ; call [eax+28h] ; fstp [esi+1Ch] ; pop esi ; ret
// Guardian Trophy Dealer maze info (quest script helper). The object embedded at +0x08 carries
// its own vtable; slot 10 (+0x28) returns a float which is stored as InitialAngle (+0x1C, PDB).
// PDB also names +0x14 ParentClass (CQS_GuardianTrophyDealerInfoScript*) and +0x18 PMasterData
// (CQ_SunnyvaleMasterData*), so the vtable-bearing subobject occupies +0x08..+0x14.

struct CQS_GuardianTrophyDealerInfoScript;
struct CQ_SunnyvaleMasterData;

// Vtable-bearing subobject at +0x08; only slot 10 is used here.
struct CMazeAngleSource {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24();
    virtual float GetAngle();          // slot 10 (+0x28)
    unsigned char _pad_0x04[8];        // subobject is 0x0C bytes (+0x08..+0x14)
};

struct CGTDI_Maze {
    void*                               __vftable;    // +0x00
    unsigned char                       _pad_0x04[4]; // +0x04
    CMazeAngleSource                    AngleSource;  // +0x08
    CQS_GuardianTrophyDealerInfoScript* ParentClass;  // +0x14 (PDB)
    CQ_SunnyvaleMasterData*             PMasterData;  // +0x18 (PDB)
    float                               InitialAngle; // +0x1C (PDB)
    void Init();
};

void CGTDI_Maze::Init()
{
    this->InitialAngle = this->AngleSource.GetAngle();
}
