#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x007BD7CA now binds its PDB spelling ??1CTCShotDefinitionFireballSpell@NThingShot@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCShotDefinitionFireballSpell_Destruct@@YIXPAX@Z.
namespace NThingShot { class CTCShotDefinitionFireballSpell { public: virtual ~CTCShotDefinitionFireballSpell(); }; }
void* __fastcall CTCShotDefinitionFireballSpell_ScalarDeletingDestructor(void* spell, int, unsigned int flags)
{
    reinterpret_cast<NThingShot::CTCShotDefinitionFireballSpell*>(spell)->NThingShot::CTCShotDefinitionFireballSpell::~CTCShotDefinitionFireballSpell();
    if (flags & 1)
        operator delete(spell);
    return spell;
}