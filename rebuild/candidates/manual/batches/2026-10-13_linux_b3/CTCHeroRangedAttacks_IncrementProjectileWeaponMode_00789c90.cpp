// CTCHeroRangedAttacks::IncrementProjectileWeaponMode()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789C90, oracle length 39 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[ecx+10h] ; push esi ; push edi ; mov edi,[ecx+44h] ; inc edi ; add eax,128h ; mov [ecx+44h],edi ; mov esi,[eax+4] ; mov edx,edi ; sub esi,[eax] ; sar esi,2 ; pop edi ; cmp edx,esi ; pop esi ; jne R ; jmp <reset mode> ; R: ret
// engine/CTCHeroRangedAttacks.h (PDB): +0x10 PHeroCombatDef_Object, +0x44
// ProjectileWeaponFirstPersonZoomModeIndex. Steps to the next first-person zoom mode and wraps
// (0x00789B30, manifest: ResetProjectileWeaponMode, tail call) once the index reaches the size of
// the combat def's zoom-mode vector (4-byte elements, {first,last} at def+0x128).
#include "engine/CTCHeroRangedAttacks.h"

struct CHeroCombatDefCalls {
    unsigned char _pad_0x000[0x128];
    long*         ZoomModes_First;      // +0x128 vector<long> first
    long*         ZoomModes_Last;       // +0x12C vector<long> last
};

struct CTCHeroRangedAttacks_Methods : CTCHeroRangedAttacks {
    void ResetProjectileWeaponMode();   // 0x00789B30
    void IncrementProjectileWeaponMode();
};

void CTCHeroRangedAttacks_Methods::IncrementProjectileWeaponMode()
{
    CHeroCombatDefCalls* def = reinterpret_cast<CHeroCombatDefCalls*>(this->PHeroCombatDef_Object);
    ++this->ProjectileWeaponFirstPersonZoomModeIndex;
    if (this->ProjectileWeaponFirstPersonZoomModeIndex == def->ZoomModes_Last - def->ZoomModes_First)
        ResetProjectileWeaponMode();
}
