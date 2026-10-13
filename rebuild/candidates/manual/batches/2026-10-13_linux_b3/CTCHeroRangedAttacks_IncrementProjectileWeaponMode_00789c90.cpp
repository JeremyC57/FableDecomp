// CTCHeroRangedAttacks::IncrementProjectileWeaponMode()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00789C90, oracle length 39 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx+10h] ; push esi ; push edi ; mov edi,[ecx+44h] ; inc edi ; add eax,128h ; mov [ecx+44h],edi ; mov esi,[eax+4] ; mov edx,edi ; sub esi,[eax] ; sar esi,2 ; pop edi ; cmp edx,esi ; pop esi ; jne R ; jmp <reset mode> ; R: ret
// engine/CTCHeroRangedAttacks.h (PDB): +0x10 PHeroCombatDef_Object, +0x44
// ProjectileWeaponFirstPersonZoomModeIndex. Steps to the next first-person zoom mode and wraps
// (0x00789B30, manifest: ResetProjectileWeaponMode, tail call) once the index reaches the size of
// the combat def's zoom-mode vector (4-byte elements, {first,last} at def+0x128).
//
// Lane feedback: retail forms the vector pointer (`add eax,128h`) and takes `size()` with a memory-operand
// subtract: the retail-vector object with an inlined size() reproduces it. RELOCATION_MATCH.
#include "engine/CTCHeroRangedAttacks.h"

template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator { T* p; explicit iterator(T* q) : p(q) {} iterator operator+(long n) const { return iterator(p + n); } T& operator*() const { return *p; } };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

// Call-surface view of CHeroCombatDef: the zoom-mode vector sits at +0x128 of the def.
struct CHeroCombatDefCalls {
    unsigned char      _pad_0x000[0x128];
    CRetailVector<long> ZoomModes;        // +0x128 {first,last,end}
};

struct CTCHeroRangedAttacks_Methods : CTCHeroRangedAttacks {
    void ResetProjectileWeaponMode();   // 0x00789B30 (tail call)
    void IncrementProjectileWeaponMode();
};

void CTCHeroRangedAttacks_Methods::IncrementProjectileWeaponMode()
{
    // Step to the next first-person zoom mode; once the index reaches the mode count, wrap.
    CHeroCombatDefCalls* def = reinterpret_cast<CHeroCombatDefCalls*>(this->PHeroCombatDef_Object);
    ++this->ProjectileWeaponFirstPersonZoomModeIndex;
    if (this->ProjectileWeaponFirstPersonZoomModeIndex == def->ZoomModes.size())
        ResetProjectileWeaponMode();
}
