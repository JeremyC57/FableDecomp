// Alloc_CCombatSequence_WillPhysicalShieldNonMelee(first, second) -- the `__fastcall` factory: a 0x28-byte CCombatSequence_WillPhysicalShieldNonMelee over the two creatures, null when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B64D0, length 50 bytes.
// Callees: operator new @ 0x00BFEA1A (0x28), CCombatSequenceBase::CCombatSequenceBase @ 0x008B0AB0 (on the new block, first, second). Vtable 0x0127A3B4 =
// CCombatSequence_WillPhysicalShieldNonMelee (rtti-vtables.tsv). Shape: the landed factory family (`test ; je` on the block, the derived vtable stored after the
// base constructor, `xor eax,eax` on the null arm). The manifest label "CFirstPersonInit::Alloc" is propagated.
struct CThingCreatureBase;

struct CCombatSequenceBase {
    virtual void v00();
    unsigned char _body[0x28 - 4];                 // sizeof = 0x28
    CCombatSequenceBase(CThingCreatureBase* first, CThingCreatureBase* second);   // 0x008B0AB0
};

struct CCombatSequence_WillPhysicalShieldNonMelee : CCombatSequenceBase {   // vtable 0x0127A3B4
    virtual void v00();
    CCombatSequence_WillPhysicalShieldNonMelee(CThingCreatureBase* first, CThingCreatureBase* second) : CCombatSequenceBase(first, second) { }
};

CCombatSequence_WillPhysicalShieldNonMelee* __fastcall Alloc_CCombatSequence_WillPhysicalShieldNonMelee(CThingCreatureBase* first, CThingCreatureBase* second)
{
    return new CCombatSequence_WillPhysicalShieldNonMelee(first, second);
}