# Chunk 2042 header overlay (grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2042/hdr`)

Row 0x007F2DC0 CTCLightningNode::FrameUpdate (parked). Every change to an existing header is an additive opt-in: with the new macros
undefined each copied file preprocesses to its source text. Landing note: re-copy HEAD first and re-apply the blocks.

## Copied from the pending hand-back 2026-11-11_acctB_r3002 (row 0x007F26B0, same TU), then extended
| header | origin | change here |
|---|---|---|
| engine/CTCLightningNode.h | r3002 (new header) | opt-in FABLE_RETAIL_CTCLIGHTNINGNODE_FRAMEUPDATE: `virtual void FrameUpdate()` (Ego_r / FableWin ?FrameUpdate@CTCLightningNode@@UAEXXZ, vtable 0x0124337C slot 9), inline `StatGetInterfaceType()` = TCI_LIGHTNING_NODE (267: RemoveTC(0x10B), bitset word +0x40 bit 11), and the nested `CTCLightningNode::CLightningBranch` (Ego_r / FableWin ??0CLightningBranch@CTCLightningNode@@QAE@AAVCThing@@@Z; members DESCRIPTIVE PTarget +0 / PEffect +8 / Timer +0x10 from the landed ctor 0x007F3AD0 and FrameUpdate; sizeof 0x14 = `push 14h` new) |
| engine/CTCDParticleEmitter.h | r3002 (CREATE opt-in) | nested opt-in FABLE_RETAIL_CTCDPARTICLEEMITTER_UPDATEPARAMETERS: SetEmitterUpdateParameters(const CParticleUpdateParameters<unsigned long,unsigned long>&) 0x006E11A0 + guarded forward declaration |
| engine/NGraphicAppearance_CTCGraphicAppearance.h | r3002 (FORCERESEND) | none (FrameUpdate uses the existing FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL GetFocalPos 0x004C1060) |
| engine/C3DVector.h | r3002 (ROTATE_ANGLES) | none |

## Copies of repo headers with new opt-ins
| header | opt-in | change | evidence |
|---|---|---|---|
| engine/CHitParameters.h | FABLE_RETAIL_CHITPARAMETERS_ADDDAMAGE | AddDamageWithAttribute(EDamageAttribute, float) 0x00868CA0 declaration (+ EDamageAttribute.h include under the opt-in) | Ego_r / FableWin spelling = the bound one; FrameUpdate pushes DamagePerCycle, 2 (0x007F3455..0x007F3465) |
| engine/CThingFilter_EnemyCreature.h | FABLE_RETAIL_CTHINGFILTER_ENEMYCREATURE_CTOR | inline body `: SourceThing(p0) {}` for the PDB ctor | FrameUpdate stores the source pointer straight into the filter (0x007F2EDB), no call |
| engine/C2DBoxI.h | FABLE_RETAIL_C2DBOXI_CENTRE_RADIUS_OUTOFLINE | the existing out-of-line `C2DBoxI(const C2DCoordI&, long)` declaration without FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE (which also changes CTCGraphicAppearance's layout) | call 0x00434740 at 0x007F2EAD |
| engine/CVectorMap.h | FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE (nested in _INSERT) | the same Insert(value_type) body, FABLE_FORCEINLINE (Inline-budget device) | retail expands both inserts; plain inline scores 191 (trace); Ego_r has no out-of-line Insert; precedent FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE |

## New headers
| header | content | evidence |
|---|---|---|
| engine/CLightningDef.h | `CLightningDef : CParentDefClassBase` (RTTI bases), Transfer / Copy, Range +0x3C .. NodeDefIndex +0x54, sizeof 0x58 (Ego_r names/order, offsets -4 for the 0x39-byte retail base) | FrameUpdate reads +0x3C/+0x40/+0x44/+0x4C/+0x50/+0x54; quarantined generated copy agrees on 0x58 |
| engine/CThingFilter_In3DRange.h | ctor (const C3DVector&, float) inline, operator() 0x006A7780 declared, Pos / Range | Ego_r layout (16 bytes), FableWin ctor spelling, landed operator() |
| engine/CTCElectrocutedEffect.h | StatGetInterfaceType = TCI_ELECTROCUTED_EFFECT (138), Reset 0x007CE7A0 | Ego_r / FableWin names; inline GetTC key 0x8A at 0x007F348D |
| engine/NParticleEngine_CParticleUpdateParameters.h | template CParticleUpdateParameters<K,V> : CBaseClass with the nested comparators, VectorParams +4 / UpdateParams +0x14 (CVectorMap), inline ctor, virtual dtor 0x006E2500, Update 0x0076C750; CParticleUpdateParamDesc | Ego_r / FableWin names, landed 0x006E24D0 / 0x0076C750 / 0x00854B90 models |
