# 2026-11-12 landing do -- naming agent FAM10: 104 engine `sub_<addr>` rows named, queue dl done, CText slot order fixed

Source: `naming/2026-11-12_agentFAM10/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, NAME_FIXES, ROWS, QUEUE, hdr README,
HELD_BACK.tsv, held_back/).

**Family (251 engine `sub_` rows): 104 named, 3 held back, 144 left.**
- 85 + 12 are `CCountedPointer<T>::DeleteData` rows. T comes from the Reset or ctor that stores the function as DeleteFunc,
  cross-checked against the FableWin.pdb DeleteData names, the Ego_r member types and the RTTI vtable of the object passed in. Three
  are held back: they added new local models of CAEnvironmentInteraction / CScriptConversation / CPSCBase (queued land1011do).
- Other renames: 0x0040E240 / 0x0040E260 CPersistContext::GetDefaultVal<bool> / <unsigned long>; 0x004538EB
  COpinionSet::COpinionSet (new test); 0x00487120 / 0x00487150 CPlayer::UnfreezeControls / SpecialAbilityUnfreezeControls. Their
  binders 0x007A75F0 and 0x007AA370 now use engine/CPlayer.h instead of a local CPlayer model.
- Left (NOTES): 13 DeleteData rows whose T has one source only, gapscan helpers, std rows that need full STLport leaves, and about
  127 tiny getters/forwarders with no proven class.

**Queue dl:** 0x004C24C0 is now ~CCountedPointer<const C3DMeshStats>, on the shared headers. 0x006C1250 and 0x00B4A700 are recorded
only. NF26's stale Leaf lines on 0x00416056 / 0x004160CC are fixed. The five IsInMeleeAsAttacker stand-in binders are left (class
unproven).

**NUISystem_CText.h default changed (deliberately):** slots 147-150 now default to retail's order (SetTextWindow 0x0054FFB0,
GetTextWindow 0x0054FFE0, ForceWidth 0x0054FB10, GetAlignment 0x0054FFF0, read from the bodies). This is the one header whose default
path changes. All 34 landed rows that include it, directly or through another header (`CTEXT_INCLUDERS_rechecked.txt`), were
rechecked in this landing and stay MATCH / RELOCATION_MATCH, test PASS.

**Other headers** (opt-ins only, macros new to the landed tree): `<CLASS>_VIRTUAL_DTOR` for CFontManager, CUserProfileManager and
19 generated manager/bank headers (`virtual ~T();` in place of the flattened `void* __vftable`, same layout);
NPlayerGui_CDrawBase.h IMPLICIT_DTOR, CArray.h SIZE_CTOR_FORCEINLINE, stlport/fable_containers.h STLP_VECTOR_FILL_OUTOFLINE,
CPlayer.h PLAYER_FREEZECONTROLS.

**Verification:** 140 rows rechecked: FAM10's 106 changed rows plus the 34 CText includers. Results: 89 MATCH and 51
RELOCATION_MATCH, every test PASS (`logs/recheck.log`). naming/rebind.py: removed 319, added 317. bindings_multi_check stays at its
baseline, and funclet_includes_check finds 1317 checked, 0 broken.
**Name fixes** (FAM10's NAME_FIXES.tsv, 24: CCountedPointer<T>::Reset stand-in binders, FreezeControls / SpecialAbilityFreezeControls,
RemoveMode / AddMode stand-ins, the vector<float> fill ctor) went straight to naming agent NF27, which is running.
