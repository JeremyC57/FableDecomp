// CClass_00880f50::vfunc_2(CObject_00883ae0*) -- calls CObject_00883ae0::CEnvironmentSpellEffectInterface_UpdateAllEffects on the argument; this unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00880F50, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx,[esp+4] ; call CObject_00883ae0::CEnvironmentSpellEffectInterface_UpdateAllEffects ; ret 4
// Virtual forwarder: the one argument becomes `this` of a __thiscall call @ 0x00883AE0 with no
// further arguments; this object is not read. Callee names are kept from the manifest only when the
// label is unique. One of 11 bodies of this shape.
struct CObject_00883ae0 {
    void CEnvironmentSpellEffectInterface_UpdateAllEffects();                         // 0x00883AE0
};

struct CClass_00880f50 {
    virtual void v00();
    virtual void vfunc_2(CObject_00883ae0* object);
};

void CClass_00880f50::vfunc_2(CObject_00883ae0* object)
{
    object->CEnvironmentSpellEffectInterface_UpdateAllEffects();
}
