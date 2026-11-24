# Codex pass 25: three VERIFIED_MATCH rows held at landing (2026-11-21, Account A)

Codex returned these three as VERIFIED_MATCH, and each leaf grades RELOCATION_MATCH + PASS under strict shared headers
(`../../../batches/2026-11-21_landing_p25/logs/vland_dry_strict.log`). They are held under the pick-any rule (EXTRA_RULES
2026-11-20, "pick-any copies of functions that have their own landed row"): each object also defines pick-any (COMDAT) copies of
functions that other landed rows own, and those copies differ from Fable.exe at the owner's address. The comparison zeroes
relocation fields on both sides (`pickany_vs_retail_*.txt`; tool: Account A scratch `c25land/tools/pickretail.py`).

The sources here are the landing-ready texts:
- `0064fdb4.cpp` adds a `// Retail-asm:` line for the inlined GFFloatToLongFloor block at 0x0064FF92; the purity gate needs it.
- The two spell sources include `engine/CThingFilter_CanBeHitByEpicSpell.h` in place of a row-local copy of that class, as the
  shared-header rule requires.
- They need the header patches in `PATCHES_HELD.json`: Codex's hdr/PATCHES.json minus the two that landed (CQuaternion.h,
  EAXNativeTypes.h) and minus the open-row ones (`../PATCHES_OPEN.json`). Apply them in order to the include tree.

## 0x0064FDB4 NPlayerGui::CDrawQuestInfo::CQuestInfoElementBar::DrawAt (`/Os /d2inlS200`)

The size-mode TU emits plain-inline helpers as pick-any COMDATs in size form:
- `CEnginePrimitiveHandle::Clear`: 58 bytes against the owner 0x0041BE70's 57 (`and dword ptr [esi+4],0` against `mov ...,0`).
  The one attempt the advisor allowed fixed this. `CEnginePrimitiveHandle_clear_speed_body.diff` is a scaffolding opt-in,
  `FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_SPEED_BODY`, that compiles that one Clear under `optimize("t", on)`. With it, the leaf
  and the other 38 functions are unchanged and Clear equals 0x0041BE70. The source here defines the macro; the header change did
  not land.
- Two more copies differ, so the row is held:
  - `?HasTC@CThing@@QBE_NW4ETCInterfaceType@@@Z`: an 8-byte tail jump to bitset::test, against Fable.exe 0x004118C0's 58 bytes. The
    leaf does not reference it. 0x004118C0 has no landed row; the two spell objects' copies equal retail.
  - `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...`: 61 bytes, against Fable.exe 0x004365B0. The leaf references it,
    and HEAD already binds the symbol to both 0x0040F020 and 0x004365B0.
- A route to landing: give those two the owners' speed-mode bodies the same way (CThing.h HasTC; the CVectorMap LowerBound), each
  with the three size-pragma conditions checked, or find the TU's real flag set. B's earlier residue for this row was the frame
  (0x164 v 0x16C); Codex's `/Os` closes that.

## 0x0078FD60 CTCDivineWrath::FrameUpdate and 0x00795300 CTCUnholyPower::FrameUpdate

Each leaf calls these COMDATs at retail sites. The copies differ from Fable.exe, and the bindings are new with these rows (HEAD's
table has no line for these spellings; the landed owners use other spellings):
- `GetAllThingsInRoughArea<CThingCreatureBase, ...>` at 0x00792E50 (579 bytes)
- `GetAllThingsInRoughArea<CThingObject, ...>` at 0x007930B0 (579 bytes)
- `map<CThingCreatureBase*, PerCreatureInfo>::operator[]` at 0x007934F0 (DivineWrath) and 0x00798330 (UnholyPower) (161 bytes)
- `vector<PerEffectInfo>::push_back` at 0x007935D0 (DivineWrath) and 0x00798410 (UnholyPower) (59 bytes). Retail inlines
  PerEffectInfo's copy constructor; this copy calls `??0PerEffectInfo@...@QAE@ABU01@@Z` out of line.

The rule ("a row that binds the copied symbol itself at a retail site, with different bytes, is held") holds both rows. Eight more
copies differ but are not leaf-bound, and HEAD already binds them: GetDef x2, CHitParameters, the EDamageAttribute map family,
`_Rb_global<bool>::_M_increment`. Those are debt.

A route to landing: respell the landed owner rows (0x00792E50, 0x007930B0, 0x007934F0 / 0x00798330, 0x007935D0 / 0x00798410) onto
these template spellings with bodies equal to retail, then reland. That is naming-queue work (paused), so it is queued, not done.

## 0x00797740 / 0x00797750: uncatalogued code after 0x00795300's leaf

The leaf ends at 0x00797734. After the int3 padding come two small functions that no manifest row covers: 0x00797740 (14 bytes,
`mov eax,[ecx]; mov ecx,[eax+8]; mov eax,[esp+4]; mov [eax],ecx; ret 4`) and 0x00797750 (11 bytes). 0x00797760 is catalogued.
Logged in FINDINGS_LOG.
