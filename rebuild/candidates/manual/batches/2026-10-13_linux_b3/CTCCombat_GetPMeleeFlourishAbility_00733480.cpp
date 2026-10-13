// CTCCombat::GetPMeleeFlourishAbility()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733480, oracle length 41 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[ecx+38h] ; push esi ; mov esi,[ecx+3Ch] ; sub esi,eax ; mov eax,[ecx+44h] ; cdq ; sar esi,2 ; idiv esi ; mov eax,[ecx+38h] ; pop esi ; mov edx,[eax+edx*4] ; test edx,edx ; mov eax,[esp+4] ; mov [eax],edx ; je R ; inc dword ptr [edx+4] ; R: ret 4
// engine/CTCCombat.h (PDB): AbilityList +0x2C, UsableAbilityList +0x3C, CurrentAbilityIndex +0x4C.
// In retail the two vectors are 12 bytes each, so UsableAbilityList's {first,last} sit at
// +0x38/+0x3C and CurrentAbilityIndex at +0x44. Returns a CDefPointer (copy = AddRef) to the
// usable ability selected by CurrentAbilityIndex modulo the list length; the hidden result
// pointer is the first stack slot.
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.

struct CDefPointeeBase {
    void* __vftable;        // +0x00
    long  RefCount;         // +0x04
};

struct CDefPointer_MeleeCombatAbilityDef {
    CDefPointeeBase* Object;
    CDefPointer_MeleeCombatAbilityDef(CDefPointeeBase* object) : Object(object) { if (object) object->RefCount++; }
};

struct CTCCombat_Retail {
    unsigned char     _pad_0x00[0x38];
    CDefPointeeBase** UsableAbilityList_First;   // +0x38 (PDB vector at +0x3C)
    CDefPointeeBase** UsableAbilityList_Last;    // +0x3C
    unsigned char     _pad_0x40[4];
    long              CurrentAbilityIndex;       // +0x44 (PDB +0x4C)
    CDefPointer_MeleeCombatAbilityDef GetPMeleeFlourishAbility();
};

CDefPointer_MeleeCombatAbilityDef CTCCombat_Retail::GetPMeleeFlourishAbility()
{
    long count = this->UsableAbilityList_Last - this->UsableAbilityList_First;
    return CDefPointer_MeleeCombatAbilityDef(this->UsableAbilityList_First[this->CurrentAbilityIndex % count]);
}
