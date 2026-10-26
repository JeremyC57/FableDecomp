#pragma optimize("s", on)
extern void __fastcall CTCShotDefinitionFireballSpell_Destruct(void* spell);
void* __fastcall CTCShotDefinitionFireballSpell_ScalarDeletingDestructor(void* spell, int, unsigned int flags)
{
    CTCShotDefinitionFireballSpell_Destruct(spell);
    if (flags & 1)
        operator delete(spell);
    return spell;
}