// CTCHeroRangedAttacks::ResetProjectileTargetingZoomParameter()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789B00, oracle length 33 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[ecx+10h] ; test eax,eax ; je D ; mov dl,[eax+136h] ; test dl,dl ; je D ; mov dword ptr [ecx+40h],0 ; ret ; D: mov dword ptr [ecx+40h],3F800000h ; ret
// engine/CTCHeroRangedAttacks.h (PDB): +0x10 PHeroCombatDef_Object (CDefPointer<CHeroCombatDef>),
// +0x40 float ProjectileWeaponZoomParameter. The zoom parameter rests at 0.0 when the combat def's
// flag byte at +0x136 says ranged aiming starts zoomed in, otherwise at 1.0.
#include "engine/CTCHeroRangedAttacks.h"

struct CHeroCombatDefCalls {
    unsigned char _pad_0x000[0x136];
    bool          ProjectileZoomStartsIn;      // +0x136 (name inferred from use)
};

struct CTCHeroRangedAttacks_Methods : CTCHeroRangedAttacks {
    void ResetProjectileTargetingZoomParameter();
};

void CTCHeroRangedAttacks_Methods::ResetProjectileTargetingZoomParameter()
{
    CHeroCombatDefCalls* def = reinterpret_cast<CHeroCombatDefCalls*>(this->PHeroCombatDef_Object);
    if (def && def->ProjectileZoomStartsIn) {
        this->ProjectileWeaponZoomParameter = 0.0f;
        return;
    }
    this->ProjectileWeaponZoomParameter = 1.0f;
}
