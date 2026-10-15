// CCombatAbilityMeleeAttackData::CCombatAbilityMeleeAttackData(const CCombatAbilityMeleeAttackData& other) -- compiler-generated copy constructor: base CCombatAbilityData (vtable + five words), own vtable, the two flags and the damage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00737370, length 69 bytes.
// Leaf: ??0CCombatAbilityMeleeAttackData
// Retail bytes:  mov eax,ecx ; mov ecx,[esp+4] ; mov dword ptr [eax],offset CCombatAbilityData::`vftable' ; mov edx,[ecx+4] ;
//                mov [eax+4],edx ; ... ; mov edx,[ecx+14h] ; mov [eax+14h],edx ; mov dword ptr [eax],offset CCombatAbilityMeleeAttackData::`vftable' ;
//                mov dl,[ecx+18h] ; mov [eax+18h],dl ; mov dl,[ecx+19h] ; mov [eax+19h],dl ; mov ecx,[ecx+1Ch] ; mov [eax+1Ch],ecx ; ret 4
// Manifest label "CResource::CResource" is propagated and wrong: vtable 0x01232484 is CCombatAbilityData and 0x01232498 is
// CCombatAbilityMeleeAttackData (rtti-vtables.tsv). Members from engine/CCombatAbilityData.h and
// engine/CCombatAbilityMeleeAttackData.h (the generated headers flatten the vptr, so a local polymorphic model is used).
// Shape: the base vtable store survives in a copy constructor (the source words read after it may alias `this`,
// so the store is not dead -- a default constructor drops it); the implicit copy constructor moves every member,
// floats included, with integer moves. VC7.1 emits the implicit copy constructor out of line, so the placement-new
// helper only makes the TU emit it -- it is not part of the retail body.
struct CCombatAbilityData {                  // vtable 0x01232484
    virtual ~CCombatAbilityData();
    long  PercentageChance;                  // +0x04
    float SecondsDuration;                   // +0x08
    float SecondsToWaitBeforeRepeat;         // +0x0C
    long  Anim_TablePos;                     // +0x10 CDefString::TablePos
    long  InterfaceType;                     // +0x14 ECombatAbilityInterfaceType
};

struct CCombatAbilityMeleeAttackData : CCombatAbilityData {   // vtable 0x01232498, 0x20 bytes
    bool  Blockable;                         // +0x18
    bool  Knockdown;                         // +0x19
    float Damage;                            // +0x1C
    virtual ~CCombatAbilityMeleeAttackData();
};

inline void* __cdecl operator new(unsigned int, void* where) { return where; }

void CopyMeleeAttackDataInto_00737370(void* where, const CCombatAbilityMeleeAttackData& other)
{
    new (where) CCombatAbilityMeleeAttackData(other);
}