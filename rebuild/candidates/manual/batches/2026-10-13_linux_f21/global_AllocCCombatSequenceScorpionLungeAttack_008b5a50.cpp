// Alloc_CCombatSequence_ScorpionLungeAttack(CThingCreatureBase*, CThingCreatureBase*) -- factory: new CCombatSequence_ScorpionLungeAttack(first, second), constructor inlined over CCombatSequenceBase(first, second)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B5A50, oracle length 50 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebx ; push esi ; push edi ; push 0x2c ; mov edi,edx ; mov ebx,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; push ebx ; mov ecx,esi ; call CCombatSequenceBase::CCombatSequenceBase ; pop edi ; mov dword ptr [esi],offset CCombatSequence_ScorpionLungeAttack::`vftable' ; mov eax,esi ; pop esi ; pop ebx ; ret ; N: pop edi ; pop esi ; xor eax,eax ; pop ebx ; ret
// A __fastcall factory (ecx = first, edx = second): allocates sizeof(CCombatSequence_ScorpionLungeAttack) = 0x2c bytes
// (operator new @ 0x00BFEA1A, landed-trusted), null-checks the block and runs the inlined
// constructor, which only forwards both arguments to the base constructor @ 0x008B0AB0 and
// installs this class's vtable (0x0127967C, named through rtti-vtables.tsv). Argument types
// follow the base constructor's manifest prototype. One of 62 bodies of this shape.
struct CThingCreatureBase;

struct CCombatSequenceBase {
    virtual void v00();
    unsigned char _body[0x2c - 4];    // sizeof(CCombatSequenceBase) = sizeof(CCombatSequence_ScorpionLungeAttack) = 0x2c
    CCombatSequenceBase(CThingCreatureBase* first, CThingCreatureBase* second);   // 0x008B0AB0
};

struct CCombatSequence_ScorpionLungeAttack : CCombatSequenceBase {
    virtual void v00();
    CCombatSequence_ScorpionLungeAttack(CThingCreatureBase* first, CThingCreatureBase* second) : CCombatSequenceBase(first, second) { }
};

CCombatSequence_ScorpionLungeAttack* __fastcall Alloc_CCombatSequence_ScorpionLungeAttack(CThingCreatureBase* first, CThingCreatureBase* second)
{
    return new CCombatSequence_ScorpionLungeAttack(first, second);
}
