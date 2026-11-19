# Landing hand4b b8 (2026-11-19): (anonymous namespace)::CombatStressTestDebug (0x00834530, 558 bytes)

vland --land, strict headers: RELOCATION_MATCH + PASS; 8 grades. Name proven: ?CombatStressTestDebug@?A0x4ed42fe4@@YIXAAVCThingAICreature@@ABVCThingSearchTools@@PAK@Z
(catalog had a member CThingAICreature::CombatStressTestDebug). Shared-header port with the Ego_d names and FableWin statements (0x01D6D8F0). Lever: the visible
noinline GetAllThings body (ALLTHINGSBODY; without it 539v558). Helper self-grades: GetAllThings written from its Ego_d record in FableWin /Od order as new nested
opt-in CThingSearchTools.h ALLTHINGSBODY_EGOD -- RELOCATION_MATCH at its own address 0x0052B270; set<long> _M_erase RELOCATION_MATCH at 0x004A9CB0.
New opt-in CThingAICreature.h SUSPENDERS (Suspenders as std::set<void*> at +0x1CC, same size as the pad; inline SuspendAI with landed 0x00833A10's body).
Shim finding (queued, not changed): with STLP_TREE_ERASE_KEY but without _POSTINC the tree's erase(first, last) uses prefix ++ for `first++` (VC7.1 warns C4620)
-- wrong behaviour on that opt-in's default path.
Header-user recheck: pending at commit time (run after the catalog-flags recheck); result recorded in the follow-up commit.
