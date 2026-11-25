struct Pair; struct Alloc;
class CThingAICreature;
class CThing;
// Naming (NF439 gen, 2026-11-25): callee 0x008648A0 binds its PDB spelling ?IsWillingToAttackTarget@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@@Z (T1 target; was the free stand-in ?cons_construct_008648a0@@YIXPAXPAUPair@@@Z).
namespace NCreatureAI { bool __fastcall IsWillingToAttackTarget(CThingAICreature const* dst, CThing const* val); }
void __fastcall Cons_val(Alloc* self, Pair* dst, Pair* val)
{
    (void)dst;
    NCreatureAI::IsWillingToAttackTarget((CThingAICreature const *)(*reinterpret_cast<void**>(self)), (CThing const *)val);
}