// NThingShot::CTCShotDefinitionThrow::GetShotDamage() -- the shot def's HitThingDamage scaled by the definition's DamageMultiplier
// Leaf: ?GetShotDamage@CTCShotDefinitionThrow@NThingShot@@
// Renamed (fistp pass, 2026-10-24) from CTCPhysicsBase::GetAirResistance @ 00/7b/CTCPhysicsBase_GetAirResistance_007bee10.cpp: vtable slot 31 of NThingShot::CTCShotDefinitionThrow; sole caller 0x007BDD6B is CTCShotDefinitionMultiArrow::GetShotDamage (the derived override calling its base); Ego_r.pdb CTCShotDefinitionBase PShotDef +0x3C / DamageMultiplier +0x48 = retail +0x38 / +0x44 (the usual -4 shift) and CThingShotDef::HitThingDamage +0x58 = retail +0x54
// (was: NThingShot::CTCShotDefinitionThrow::GetShotDamage() -- the float at +0x54 of the object at +0x38 times the scale at +0x44)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BEE10, oracle length 10 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov eax, dword ptr [ecx + 0x38] ; fld dword ptr [eax + 0x54] ; fmul dword ptr [ecx + 0x44] ; ret 
// Tiny-body shape `GetScaledTargetValue` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
class CThingShotDef { public:
    unsigned char _pad1[0x54];
    float HitThingDamage;                         // +0x54
};

namespace NThingShot {
struct CTCShotDefinitionThrow {
    unsigned char _pad2[0x38];
    CThingShotDef* PShotDef;                     // +0x38
    unsigned char _pad3[0x8];
    float DamageMultiplier;                          // +0x44
    float GetShotDamage();
};
}

float NThingShot::CTCShotDefinitionThrow::GetShotDamage()
{
    return DamageMultiplier * PShotDef->HitThingDamage;
}
