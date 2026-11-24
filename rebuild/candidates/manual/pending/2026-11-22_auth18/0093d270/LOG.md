# 0x0093D270 CAIStateGroup_MeleeCombatNearbyBase::IsRunnable -- AUTH18 log (continues auth1/auth2/auth5 logs)
Baseline: e1.cpp (AUTH1 Ego_d text, copy here) = DIFFER(305v295), residue 6 lines (inline float getter return home after GetCombatManager).
1. Ego_r type stream (lane PDB; regenerate with `llvm-pdbutil dump -types $S/codex27/pdbx/Ego_r.4k.pdb | grep -n GetProximityForNavigation`, then the type record 0x25AAC; the 135 MB dump was deleted): CCombatManager
   LF_ONEMETHOD GetProximityForNavigation public, type 0x25AAC LF_MFUNCTION return float (0x0040) BY VALUE, 0 args, thiscall;
   member ProximityForNavigation float private (offset 48 in Ego_r layout); 4 copies of the class record, all equal.
   CTCCreatureNavigation::IsAbleToNavigateFromPosToPos args (const C3DVector&, const C3DVector&, float). => flip (2) closed negative.
2. LTCG (ltcg_grade.py on e1): DIFFER(295v295) by coincidence: the $T home stays (mov eax,[eax+28h] / mov [esp+10h],eax), early
   prologue reordered. No help.
3. Flag sweep on m2 mini (/G3../G5 /GB /Oy- /Oi /Ot /Gs /GA /QIfdiv /QI0f /QIfist /Gd /Gr /Gz /GX /EHsc /GR /Zp1 /Zp4 /J /Gi
   /Zc:*): $T stays. /Za drops it, but /Za == /Op on the mini (same listing), and /Za cannot compile the engine headers (forward enum
   declarations, C2599). Not a candidate.
4. Mini m3 (row shape: inline GetCreature, non-inline intelligent-pointer arrow, Nav(GetCreature().GetPos(), enemy->GetPos(),
   GetCreature().GetCM().X)):
   - by-value float getter: $T home, push before GetPItem;   - by-value long getter: no $T, load+push before GetPItem;
   - direct member read: GetPItem first, GetCM after (another order);
   - `const float&` getter: EXACT retail order (GetCM -> edi, GetPItem, load [edi+40] late, push).
   - /Ob0 (f1 and f3 printed): natural order => the deferral is c2's inline expansion (by-value return temp sequenced; reference deref deferred).
5. Retail scan: 30 GetCombatManager (0x00661ED0) call sites; only 0x006AAA24 and 0x0093D351 read +0x28; both lvalue-like.
   Retail-wide byte scan for the DEAD return-home shape (copy, push, store; tools/scan_t2.py): 25 sites (20 in landed rows, 5 unmapped).
   Source read for 5 of them (Draw2DScaledGraphic, UpdateRightStickLookAround, CTCDRumble::Create: named float locals;
   AddLightPrimitive: new-expression args; WriteToFile: struct-return GetCenter().X). The scan cannot see the row's LIVE variant.
6. Flip (1), source side (tools/flip1.py): 86 by-value inline float getter names in the headers; 153 landed uses as a non-first argument
   with a call in a left argument (119 are GetLength name collisions with non-float getters). No hit has the row's shape (inline
   by-value float getter on a CALL-RETURNED object, followed by an opaque call in a left argument). Checked: GetFOV (0x00697F50) is
   out-of-line; GetScale (0x00BA5260) sits in an inlined static; GetSamplesPerSecond (0x00A50510) has only inline left args.
   Bounded scan, limits: the header regex only catches single-line in-class Get*/Peek* bodies; the 119 GetLength hits were dismissed
   by name (only the 20 objcall lines were seen to be CMemoryDataOutputStream); the filter required a call-returned object (AUTH1's
   minis say the object kind is not a factor); 3 candidates examined. No flip found within these limits.
8. Positive control from retail (scan hit 0x00AC361A, landed 0x00AC2BE0 PartitionSpace line 41):
   `SubElements[0]->PartitionSpace(verts, start_tri_index, BoundingSphere.GetRadius())`, C3DBoundingSphere::GetRadius = by-value inline
   float getter (`float GetRadius() const { return Radius; }`, FableWin ?GetRadius@C3DBoundingSphere@@QBEMXZ). Retail: `mov eax,[edi+2Ch]
   ; mov ecx,eax ; push ecx ; ... ; mov [esp+20h],eax ; call` = the dead return-value home our VC7.1 gives. So retail's compiler homes a
   by-value inline float getter's return like ours (and here with no call in the left arguments). 0x00B0CC8B (UpdateAddParticle) not
   resolved to a source expression.
7. Compile-order inversion (c2/c2cut.py, diagnostic hook on m2, 2 tries): cutting the f1/f2 -> GetP edges at the sort (0x1070CFDF)
   and moving GetP's entry to the end of [0x1087F4C8] both still compile GetP first (start order GetP, f1, f2). The instrument does
   not control the post-sort compile list; left open (needs [0x10881D68] reordering).
(note: one read-only `git log -S defer3` was started by mistake in the repo and stopped at once; nothing written. No further git.)
