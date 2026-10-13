// CAIStateGroup_CreatureUseCoverBase::IsLowCover(CThing*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00970760, oracle length 33 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[esp+4] ; mov ecx,[eax+70h] ; fld [ecx+5Ch] ; fcomp [__real@1.2] ; fnstsw ax ; test ah,5 ; jp F ; mov al,1 ; ret 4 ; F: xor al,al ; ret 4
// `this` is unused. PDB CThing +0x70 = CTCPhysicsBase* PhysicsTC; +0x5C of that component (past
// the 0x40-byte CTCPhysicsBase, i.e. in the concrete physics subclass) is the thing's height.
// Cover lower than 1.2 units (0x0129749C) counts as low cover (crouch instead of stand).

struct CThingPhysicsCalls {
    unsigned char _pad_0x00[0x5C];
    float         Height;              // +0x5C (name inferred; subclass of CTCPhysicsBase)
};

struct CThingCalls {
    unsigned char       _pad_0x00[0x70];
    CThingPhysicsCalls* PhysicsTC;     // +0x70 (PDB CThing::PhysicsTC)
};

struct CAIStateGroup_CreatureUseCoverBase_Methods {
    bool IsLowCover(CThingCalls* coverThing);
};

bool CAIStateGroup_CreatureUseCoverBase_Methods::IsLowCover(CThingCalls* coverThing)
{
    if (coverThing->PhysicsTC->Height < 1.2f)
        return true;
    return false;
}