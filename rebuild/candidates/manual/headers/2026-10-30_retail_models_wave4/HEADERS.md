# Wave 4 headers (class-model migration, 2026-10-30)

**No shared header was added or changed in wave 4.** `git status` in the worktree shows only `rebuild/src/compiled/` row
sources. Every migrated row moved onto a header that waves 1-3 wrote and proved:

| header (opt-in) | wave 4 rows |
|---|---:|
| engine/CGameScriptInterface.h (`FABLE_RETAIL_NSCRIPT`; vtable 0x01260F0C, 918 slots) | 636 |
| engine/CBaseClass.h (no opt-in; vtable 0x01231710, CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0) | 591 |
| engine/CParentDefClassBase.h (`FABLE_RETAIL_DEF_CLASS_BASE`, 23 of them with `FABLE_PDCB_TYPED_MEMBERS`) | 41 |
| engine/CDefClassBase.h (`FABLE_RETAIL_DEF_CLASS_BASE`) | 12 |

(52 rows took both the CBaseClass and the CGameScriptInterface header; they are counted in both lines.)

Brief item 5 (new headers for CBaseObject 0x0129A7D8, CScriptGameResourceObjectBase 0x0126008C,
CScriptGameResourceObjectScriptedThingBase 0x0127094C, CScriptGameResourceObjectMovieBase 0x01260EF4,
NScript::CIsThingConscious 0x012C2FE8, CSpawnedFunc<T> / CSpawnedFunc1<T,P>) was not started: items 1-3 used the budget.
NEXT.md has the row counts per class.

## Row-local models written by hand this wave (evidence)

These are the 11 wave 2 PDCB reverts, redone on the shared def chain. Each derived def keeps its own members row-locally.
The shared retail base is 0x39 bytes packed, so the first derived member sits at +0x3C, the next 4-aligned offset.

| row | class | what changed | evidence |
|---|---|---|---|
| 004585e0 | CHitLocationDef ctor | dropped the derived `_body[0x38]` base pad and the `v00` stand-in | PhysicalPrimitive +0x3C (retail ctor stores) |
| 00458d3f | CThingShotDef ctor | the `0x60-byte base` stand-in became Ego_r.pdb members: Speed +0x3C .. CauseDamage +0x5C, ShotDefinitionClassName (CDefString, -1) +0x60, ShotOffsetAngleRange / ShotOffsetRadiusRange (CFloatRange) +0x64 / +0x6C | Ego_r.pdb CThingShotDef offsets less 4; retail `or [esi+60h],-1` + four fldz stores at +0x64..+0x70 |
| 0045767e | CSpecialAbilitiesForcePushDef ctor | `CRetailVector<long>` + pad stand-ins became Ego_r.pdb members MaxDistanceToPush +0x3C .. Level4OffsetMagnitude +0x74, std::vector<float> RadiusOfEffect +0x78, ExtraPush / PushSpeed, ObjectDamage +0x8C, TimeDelayBetweenCastsSec +0x98 (a minimal STLport `std::vector`) | Ego_r.pdb offsets less 4 (vectors 12 bytes); retail zero stores at +0x78 / +0x8C / +0x98 |
| 0045a0ee | NUISystem::CUILocaleGraphicsDef ctor | dropped the `_pad04[0x38]` and the `v00` stand-in; the 12-byte member at +0x3C (map ctor 0x00453309) is now three pointer words, so it stays 4-aligned behind the 0x39 base | Ego_r.pdb WorldMapRegionName (std::map) +0x40 / HelpScreenGraphics (vector) +0x50, less 4 |
| 004773f1 | CEngineLocalDetailLayerDef copy ctor | dropped the `_pad` and the `v00` stand-in; both 12-byte vector members are pointer words (4-aligned) | Ego_r.pdb SpacingFromLayer +0x40 / Objects +0x50, retail +0x3C / +0x48 |
| 00458994 | CGlobalDef ctor | the `_body[0x90-4]` base stand-in became the Ego_r.pdb scalars ControlSchemePC +0x3C .. MaxFollowers +0x8C; dropped the `v01` stand-in | Ego_r.pdb CGlobalDef offsets less 4; the existing comment's +0x90.. members unchanged |
| 00458b9d | CHeroCombatDef dtor | the opaque 0x3C base became the shared chain; the derived def is `__declspec(novtable)` | retail dtor has no vtable store of its own; same model as the landed 0x0047747C |
| 00459e46 | CPlayerGuiDef dtor | the same as 00458b9d | the same |
| 004575fc | CSpecialAbilitiesBulletTimeDef dtor | `typedef long CDefString` became the 4-byte `class CDefString { long TablePos; }`; novtable derived def | as 00458b9d |
| 0045784b | CSpecialAbilitiesPhysicalShieldDef dtor | the same as 004575fc | the same |
| 004564b5 | CRegionDef ctor | the two-parameter local CVectorMap became the 3-parameter `CVectorMap<K,V,CKeyPairCompareLess<K,V> >` (Compare +0x0C, Dirty +0x0D), which is the header's forward declaration; dropped the `v00` stand-in | retail 16-byte CVectorMap (wave 3 LESSONS item 1) |

All 11 rows also got their PDB slot 18-20 overrides from `scripts/add_def_overrides.py`. That script now also tries
`Cls@Namespace` for a class inside `namespace N {` (0045a0ee). Each override is a declaration only.

Not redone: 004585bd. It models the def base as a MEMBER of an unattributed aggregate. It is CArmourDef::~CArmourDef
(NAME_FIXES.tsv). It needs a rename before it can take the novtable derived model.
