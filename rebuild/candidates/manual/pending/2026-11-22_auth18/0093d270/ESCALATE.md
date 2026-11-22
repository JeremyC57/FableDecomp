# ESCALATE 0x0093D270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable (295 B) -- AUTH18, 2026-11-22
Leaf `?IsRunnable@CAIStateGroup_MeleeCombatNearbyBase@@MAE_NXZ`. Best rule-clean source `$S/w22/auth18/0093d270/e1.cpp` (AUTH1's
Ego_d text, unchanged) = **DIFFER(305v295)** at stock flags, residue 6 lines. (297v295 on record is hand219's direct-read local-model
bracket, not rule-clean.) Full evidence: LOG.md here (items 1-7); earlier packets auth1/auth2 ESCALATE.md, p25R NOTES.

## Residue (retail - / ours +), unchanged
```
 call GetCombatManager (0x00661ED0)
+mov eax, dword ptr [eax + 0x28]
 lea ecx, [ebx + 0x14]
-mov esi, eax
+mov dword ptr [esp + 0x10], eax        ; inline getter's return-value home
 call GetPItem (0x00A01B50)
 mov ecx, dword ptr [ebx + 4]
-mov edx, dword ptr [esi + 0x28]
-mov edi, dword ptr [ecx + 0x18]
+mov edx, dword ptr [esp + 0x10]
+mov esi, dword ptr [ecx + 0x18]
 push edx
```

## Closed this pass
- **Flip (2) closed negative.** Ego_r type stream (lane PDB): GetProximityForNavigation public, LF_MFUNCTION 0x25AAC returns float
  BY VALUE; member private float; IsAbleToNavigateFromPosToPos(const C3DVector&, const C3DVector&, float). Ego_r = Ego_d = FableWin.
- **The discriminator is reference v by-value return, not float handling** (corrects the earlier "float $T" framing). Mini m3 (row
  shape; cite it only for this, not for the row's exact bytes): by-value float -> $T; by-value **long** -> no $T but load+push still
  BEFORE GetPItem; direct member read -> a third order (GetPItem first); `const float&` -> retail's order exactly (manager kept,
  load after GetPItem). /Ob0 gives natural order (f1 and f3 printed), so the deferral comes from c2's expansion of a reference return.
  Hence no float flag can fix it: /Op and /Za (/Za == /Op on the mini; /Za cannot compile the engine headers) drop the $T but still
  load early; /arch, /G3../G7, /GB, /QIfdiv, /QI0f, /QIfist, /Gd /Gr /Gz, /GX, /Zp, /J, /Gi, /Zc:*, LTCG (ltcg_grade.py) all keep it.
- **Positive control from retail itself** (LOG item 8): 0x00AC361A in landed 0x00AC2BE0, `PartitionSpace(verts, start_tri_index,
  BoundingSphere.GetRadius())`, by-value inline float getter (FableWin QBEMXZ): retail emits the dead return-value home
  (`mov eax,[edi+2Ch]; mov ecx,eax; push ecx; ...; mov [esp+20h],eax`), exactly as our VC7.1 does. Retail's compiler treats a
  by-value inline float getter like ours; the two GetProximityForNavigation sites do not behave like one.
- **Retail**: 30 GetCombatManager call sites; exactly two read +0x28 (0x006AAA24 in thing_player_creature, 0x0093D351 here), two TUs,
  both behave as an lvalue read. The retail dead-home byte shape (25 sites) shows no by-value float getter in the 5 sources read.

## Not closed (bounded)
- Flip (1), source side (LOG item 6): no landed row found with the row's exact shape, within these limits: single-line in-class
  Get*/Peek* bodies only; 119 GetLength hits dismissed by name; call-returned objects only; 3 candidates examined.

- Compile-order inversion (callee compiled after caller): two hook tries on m2 (edge cut at the sort; moving GetP's entry to the list
  tail) did not move GetP's compile after f1 (c2/c2cut.py). Would need the post-sort compile list [0x10881D68] reordered. Even if it
  dropped the $T, the long-getter result says a by-value expansion still loads before GetPItem, so this lever is unlikely to give
  retail's order.

## Question for the advisor (yes/no)
Does the row end as outcome (A) -- terminal, reopening only on TLC-era header evidence (a TLC CCombatManager.h, or a TLC-era PDB /
map showing a reference-returning or public-member form) -- rather than landing `const float&` as an Inferred opt-in
(contradicted by FableWin QBEMXZ, Ego_d line 55 = 76 B, Ego_r 0x25AAC)?

## Terminal paragraph (for adoption as written)
> 0x0093D270 ends in (A). Retail's argument order (manager pointer held across GetPItem, +0x28 read after it) is what VC7.1 3077
> emits for a reference-returning inline (`const float&`) and for no by-value form measured: a by-value float or long getter loads
> before the next opaque call (mini m3), retail itself homes a by-value float getter's return (0x00AC361A), and no flag, LTCG, or
> c2 inline path tried moves that. All three PDBs (Ego_d, Ego_r,
> FableWin; one later VC10 revision) give `float GetProximityForNavigation() const` by value, so the matching text contradicts them
> and cannot land. Both retail uses (0x006AAA24, 0x0093D351) agree with the reference form, which points to a TLC-era header
> difference no lane source shows. Best rule-clean text e1.cpp, DIFFER(305v295). The row reopens only on TLC-era evidence for
> this inline (a TLC header or PDB), or on a landed row with this exact shape that shows retail's deferred load.
