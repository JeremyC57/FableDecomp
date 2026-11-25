#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x007BF2B0 now binds its PDB spelling ??1CTCShotDefinitionBase@NThingShot@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCShotDefinitionThrowPredictive_Destruct@@YIXPAX@Z.
namespace NThingShot { class CTCShotDefinitionBase { public: virtual ~CTCShotDefinitionBase(); }; }
void* __fastcall CTCShotDefinitionThrowPredictive_ScalarDeletingDestructor(void* predictive, int, unsigned int flags)
{
    reinterpret_cast<NThingShot::CTCShotDefinitionBase*>(predictive)->NThingShot::CTCShotDefinitionBase::~CTCShotDefinitionBase();
    if (flags & 1)
        operator delete(predictive);
    return predictive;
}