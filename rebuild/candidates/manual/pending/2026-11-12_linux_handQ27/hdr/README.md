# Chunk Q27 header overlay

Each file is a full copy of the current repo header (`rebuild/include/<path>`) plus `#ifdef FABLE_RETAIL_*` opt-ins only; the default
path is unchanged. Grade with `FABLE_HDR_OVERLAY=$S/w22/chunkQ27/hdr`. Nothing was copied from the Q25 overlay.

| Header | Opt-in | Adds | Used by |
|---|---|---|---|
| engine/CQ_FireHeartScript.h | FABLE_RETAIL_CQ_FIREHEARTSCRIPT_TRIGGERPROPHETCOMMENT | `void TriggerProphetComment(const CCharString&)` (0x00D38400, landed) | 00d39960 (MATCH) |
| engine/CDefendingCombatantInfo.h | FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_WHEELINIT | CCombatRing::Reset 0x006E8B60, CCombatRing::AssignAttackersToRing 0x006EBF70, CCombatWheel::Init 0x006EC850, CCombatWheel::AddRing 0x006EC7D0, inline nonconst GetRing (body = landed 0x006E84E0), forward decls | 006ec850 |
| engine/CTCCombat.h | FABLE_RETAIL_CTCCOMBAT_UNSHEATHINGTYPE | inline `GetUnsheathingWeaponType() const` | 006ec850 |
| stlport/fable_map.h | FABLE_RETAIL_STLP_TREE_ERASE_INLINE | inline `_Rb_tree::erase(iterator)` body calling `_Rb_global::_Rebalance_for_erase` (0x0042A34E) | 006ec850 |
| engine/CHandleBasedMemoryPool.h | FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL_LOCK | Lock 0x00A026F0 / Unlock 0x00A02710 | 00a8ad40 |
| engine/C3DMeshStats.h | FABLE_RETAIL_C3DMESHSTATS_LOADBINARY | LoadBinary 0x00A2F2D0; MemoryPool protected under the opt-in | 00a8ad40 |
| engine/C3DMeshMaterial.h | FABLE_RETAIL_C3DMESH2_LOADBINARY | LoadBinary 0x00ABF100 | 00a8ad40 |
| engine/C3DPrimitive2.h | FABLE_RETAIL_C3DMESH2_LOADBINARY | LoadBinary 0x00A92900 | 00a8ad40 |
| engine/C3DMesh2.h | FABLE_RETAIL_C3DMESH2_LOADBINARY | retail branch `class C3DMesh2 : public C3DMeshStats` with LoadBinary 0x00A8AD40, InitialisePointers 0x00A896A0, InitialiseBoneNamePointers 0x00A892E0 (UnmodelledBase line split under `#ifndef`) | 00a8ad40 |
| engine/CAIStateGroupBase.h | FABLE_RETAIL_CAISTATEGROUPBASE_INTERACTWITH | AIAction_InteractWith 0x00981410 | 0090e5d0 |
| engine/CAIStateGroup_BanditGossip.h | FABLE_RETAIL_CAISTATEGROUP_GOSSIP | retail class branch: Critical +0x18, WasCarryingWeapon +0x24, virtual Update | 0090e5d0 |
| engine/CAIStateGroup_MinionGossip.h | FABLE_RETAIL_CAISTATEGROUP_GOSSIP | same retail branch for the sibling | 0090e5d0 |
| engine/CCreatureAction_UnsheatheWeapons.h | FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS | `: CCombatActionBase`, ctor 0x008B8590 | 0090e5d0 |
| engine/CMessageEventFilters.h | FABLE_RETAIL_CTHINGFILTER_IN2DRANGE_CTOR | CThingFilter_In2DRange(pos, radius) ctor | 0090e5d0 |
