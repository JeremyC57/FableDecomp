// CThing::InitTC(CTCBase* tc) -- bring a newly added thing component up to the thing's own initialisation stage (stage 1, 2 or 3 of the TC's staged init virtuals) unless it is already marked for removal
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C73E0, 86 bytes (+ int3 padding). No direct callees: CTCBase virtual
// slots 4, 5, 6 (+0x10/+0x14/+0x18, the staged initialisers).
// Shape: CTCBase::ToBeRemoved is the bool at +8 (PDB); CThing::InitialisationStage is the 3-bit field at retail
// +0x94 (PDB +0xa4: the retail CThing is 16 bytes narrower before it), read once (`and al,7`) and compared
// with 1, 2, 3 in an if/else chain (a switch lowers to `dec`); every arm keeps its own `pop esi ; ret 4`.
struct CTCBase {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void InitialiseStage1();                 // slot 4 (+0x10)
    virtual void InitialiseStage2();                 // slot 5 (+0x14)
    virtual void InitialiseStage3();                 // slot 6 (+0x18)
    void* ParentThing;                               // +0x04
    bool  ToBeRemoved;                               // +0x08
};

struct CThing {
    void*         __vftable;                         // +0x00
    unsigned char _pad04[0x94 - 0x04];
    unsigned char InitialisationStage : 3;           // +0x94 (PDB +0xa4)
    void InitTC(CTCBase* tc);
};

void CThing::InitTC(CTCBase* tc)
{
    if (!tc->ToBeRemoved) {
        unsigned char stage = InitialisationStage;
        if (stage == 1) {
            tc->InitialiseStage1();
        } else if (stage == 2) {
            tc->InitialiseStage1();
            tc->InitialiseStage2();
        } else if (stage == 3) {
            tc->InitialiseStage1();
            tc->InitialiseStage2();
            tc->InitialiseStage3();
        }
    }
}
