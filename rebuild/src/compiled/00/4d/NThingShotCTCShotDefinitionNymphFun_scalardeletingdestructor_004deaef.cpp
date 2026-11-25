#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DC9D2 now binds its PDB spelling ??1CTCShotDefinitionProjectile@NThingShot@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCShotDefinitionNymphFun_Destruct@@YIXPAX@Z.
namespace NThingShot { class CTCShotDefinitionProjectile { public: virtual ~CTCShotDefinitionProjectile(); }; }
void* __fastcall CTCShotDefinitionNymphFun_ScalarDeletingDestructor(void* fun, int, unsigned int flags)
{
    reinterpret_cast<NThingShot::CTCShotDefinitionProjectile*>(fun)->NThingShot::CTCShotDefinitionProjectile::~CTCShotDefinitionProjectile();
    if (flags & 1)
        operator delete(fun);
    return fun;
}