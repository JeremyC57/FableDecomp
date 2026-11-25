#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x007BD80A now binds its PDB spelling ??1CTCShotDefinitionDrainLife@NThingShot@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCShotDefinitionDrainLife_Destruct@@YIXPAX@Z.
namespace NThingShot { class CTCShotDefinitionDrainLife { public: virtual ~CTCShotDefinitionDrainLife(); }; }
void* __fastcall CTCShotDefinitionDrainLife_ScalarDeletingDestructor(void* life, int, unsigned int flags)
{
    reinterpret_cast<NThingShot::CTCShotDefinitionDrainLife*>(life)->NThingShot::CTCShotDefinitionDrainLife::~CTCShotDefinitionDrainLife();
    if (flags & 1)
        operator delete(life);
    return life;
}