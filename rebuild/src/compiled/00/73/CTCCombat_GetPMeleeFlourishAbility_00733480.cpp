// CTCCombat::GetPMeleeFlourishAbility()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733480, oracle length 41 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
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

// Lane feedback: `first[index]` keeps the vector base in a register and emits 40 bytes; indexing
// through the retail-vector iterator object reloads it after the modulo like retail. MATCH.

template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator { T* p; explicit iterator(T* q) : p(q) {} iterator operator+(long n) const { return iterator(p + n); } T& operator*() const { return *p; } };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

class CDefPointeeBase { public:
    void* __vftable;        // +0x00
    long  RefCount;         // +0x04
};

struct CDefPointer_MeleeCombatAbilityDef {
    CDefPointeeBase* Object;
    CDefPointer_MeleeCombatAbilityDef(CDefPointeeBase* object) : Object(object) { if (object) object->RefCount++; }
};

struct CTCCombat_Retail {
    unsigned char                   _pad_0x00[0x38];
    CRetailVector<CDefPointeeBase*> UsableAbilityList;      // +0x38 (PDB vector at +0x3C)
    long                            CurrentAbilityIndex;    // +0x44 (PDB +0x4C)
    CDefPointer_MeleeCombatAbilityDef GetPMeleeFlourishAbility();
};

CDefPointer_MeleeCombatAbilityDef CTCCombat_Retail::GetPMeleeFlourishAbility()
{
    // The usable ability selected by the current index, wrapped modulo the list length (signed
    // division: `idiv`), handed back as a counted def pointer (copy = AddRef).
    return CDefPointer_MeleeCombatAbilityDef(
        this->UsableAbilityList[this->CurrentAbilityIndex % (long)this->UsableAbilityList.size()]);
}