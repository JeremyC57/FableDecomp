#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DC9D2 now binds its PDB spelling ??1CTCShotDefinitionProjectile@NThingShot@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCShotDefinitionQueenHornetShot_Destruct@@YIXPAX@Z.
namespace NThingShot { class CTCShotDefinitionProjectile { public: virtual ~CTCShotDefinitionProjectile(); }; }
void* __fastcall CTCShotDefinitionQueenHornetShot_ScalarDeletingDestructor(void* shot, int, unsigned int flags)
{
    reinterpret_cast<NThingShot::CTCShotDefinitionProjectile*>(shot)->NThingShot::CTCShotDefinitionProjectile::~CTCShotDefinitionProjectile();
    if (flags & 1)
        operator delete(shot);
    return shot;
}