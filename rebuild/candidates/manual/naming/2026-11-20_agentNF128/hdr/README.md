# NF128 overlay (round 10, PRIORITY slice 7 third remainder)

Every header here is a copy of the CURRENT repo working-tree header (base copy in ../orig/, byte-equal to the repo at copy time) plus
`#ifdef` opt-ins only. Without the opt-in macro the default path is token-identical to the repo header, except that pin_asserts.py
re-pinned each moved `FABLE_STATIC_ASSERT` to its original line (`FABLE_STATIC_ASSERT_AT(<orig line>, ...)`).

## engine/NCreatureMode_CModeStrafe.h (base: rebuild/include/engine/NCreatureMode_CModeStrafe.h)
- `FABLE_RETAIL_NCREATUREMODE_CMODESTRAFE_ONACTIVATE` (inside the PASS24 class): `virtual void FrameUpdate();` (slot 3, 0x008EA220) and
  `virtual void OnActivate();` (slot 5, 0x008E9A30). Ego_r.pdb 0x0090FFC0 / FableWin.pdb 0x02AD8350 ?OnActivate@CModeStrafe@NCreatureMode@@UAEXXZ.
  Declarations only, no layout change. Used by rows/008e9a30.cpp.
- pin_asserts: pinned 1 (`sizeof(CModeStrafe) == 0x70`, line 36).
- Landed includers: none (`grep -rlE 'NCreatureMode_CModeStrafe.h' rebuild/src/compiled` is empty).

## engine/CAnimFilterOfGroup.h (base: rebuild/include/engine/CAnimFilterOfGroup.h)
- `FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE`: inline `void SetCurrentGroupName(const CCharString& name) { Group = name;
  GroupCRC = name.GetCRC(); }` (FableWin.pdb ?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z 0x01DD3450, no Ego_r copy; out of
  line in retail at 0x0073DFF0, expanded in place in 0x008E9A30). Needs FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE. No layout change.
  Used by rows/008e9a30.cpp. One-definition caveat: an includer's object emits the COMDAT of this inline while the landed owner 0x0073DFF0
  defines the symbol strongly (NOTES, decisions: nf86 / nf88 scaffolding on the owner).
- pin_asserts: pinned 1 (`sizeof(CAnimFilterOfGroup)==12`, line 33).
- Landed includers: 81 rows (`grep -rlE 'CAnimFilterOfGroup.h' rebuild/src/compiled`). None defines the new macro, so the opt-in changes none of them.

## engine/CThingCreatureBase.h (base: rebuild/include/engine/CThingCreatureBase.h)
- `FABLE_RETAIL_CTHINGCREATUREBASE_GETDEFHOVERSPEEDS` (public, before the UPDATELASTATTACKINGCREATURE block): `float GetDefHoverSlowSpeed() const;`,
  `GetDefHoverMediumSpeed`, `GetDefHoverFastSpeed` (Ego_r.pdb 0x0053CF60 / 70 / 80 + FableWin.pdb 0x01D9AF70 / A0 / D0; retail 0x006620E0 / F0 /
  0x00662100). Declarations only, no layout change. Used by rows/008ea500.cpp.
- pin_asserts: pinned 0 (no plain FABLE_STATIC_ASSERT in the header: its asserts were already _AT-pinned).
- Landed includers: 559 rows (`grep -rlE 'CThingCreatureBase.h' rebuild/src/compiled`). None defines the new macro, so the opt-in changes none of them.

## engine/NCreatureMode_CModeSitFloor.h (base: rebuild/include/engine/NCreatureMode_CModeSitFloor.h)
- `FABLE_RETAIL_NCREATUREMODE_CMODESITBASE_ANIMUPDATE` (in CModeSitBase, before NF124's ONACTIVATE block): `virtual void AnimUpdate(CAnimateThing& anim);`
  (Ego_r.pdb 0x00817E40 / FableWin.pdb 0x02841E30 ?AnimUpdate@CModeSitBase@NCreatureMode@@UAEXAAVCAnimateThing@@@Z, slot 2, 0x008D6AC0).
  Declaration only, no layout change. Used by rows/008d6ac0.cpp.
- pin_asserts: pinned 0 (no plain FABLE_STATIC_ASSERT in the header).
- Landed includers: 17 rows (`grep -rlE 'NCreatureMode_CModeSitFloor.h' rebuild/src/compiled`). None defines the new macro, so the opt-in changes none of them.

Sibling overlays (agentNF123 / NF127 / NF129 hdr/engine) re-listed at hand-back: none carries any of these four headers.
