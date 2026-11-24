# NF392 hand-back (Account A naming agent, 2026-11-24): the CEngineTextureMorph family (rulings R5 / R6)

No repo writes, no git. Work dir $S/w22/agentNF392. ~72 VC7.1 compiles (00728d10: 6 probes + 2 controls + vland 2x2;
census 30 + 5 owners; 00728 stand-ins 3 HEAD + 7 new; 00709d60 HEAD + 1). Locks: 00728d10 kept (NF392); 00728a40 / 00728b10 /
00728b50 / 00709d60 claimed and released.

## Step 1: 00728d10 -> batches/b1, HOLD_RULING (R8 only)
- The cheapest respell worked in 2 compiles. The old local model declared `CArray<T>` as a 12-byte class with its own out-of-line
  copy / dtor. Now `CArray<T> : public std::vector<T>` has no special members of its own, and the local vector declares
  `vector(const vector&)` (0x005BBDF1) and `~vector()` (0x005BC0D3). This is the model the landed same-TU sibling 00728df0
  (_M_fill_insert) already uses. The element copy now binds `??0?$vector@VCEngineTextureMorph@@...@QAE@ABV01@@Z`.
  - Without the `~vector` declaration the grade was DIFFER(203v216) (n1).
  - The PASS22 user-written inline copy / dtor (n3) does not stop the CArray COMDATs (VC7.1 emits them anyway).
- Gates, all pass:
  - vland HEAD + new at one path (work/wd_b1h / wd_b1n): RELOCATION_MATCH PASS both. The RELOC_NAME warning for the CArray line
    is gone in N. The SHARED_HEADER_AVAILABLE warnings (CArray, CTCAppearanceMorphBase) are the same as HEAD.
  - objsec by COMDAT symbol on the one-path vland objects wd_b1h / wd_b1n (evidence/objsec/00728d10.comdat.txt): every HEAD section EQ under CArray->vector, .drectve EQ.
    NEW_ONLY:
    - `??0?$CArray@VCEngineTextureMorph@@@@QAE@ABV0@@Z` (19 B)
    - `??1?$CArray@VCEngineTextureMorph@@@@QAE@XZ` (5 B)
    - Both call or jump straight to the vector bodies, the retail shape.
  - pk_all + pvr HEAD v new: only the CArray copy changes. It reads NEQ v the line on the shipped table and NOADDR with
    TABLE_CHANGES applied (scratch root work/troot, evidence/pk/T_*). The pre-existing NEQ `__destroy_aux` copy v 0x00722130 is
    unchanged (reach y/y). Reach flips 0.
  - --addrs: assignment A (0x00709A30 / 0x005BC0F6) and assignment B (swapped) both read EQ (TAA / TAB). Byte equality cannot
    choose (R9).
  - apply_table --dry: delete 1, 0 misses, multi-target 1 -> 1, none new.
  - /FORCE link of 00728d10 + 005bbdf1 005bc0d3 00728b10 00728a40 00722130 + definers 00728df0 00707b10: duplicates 0 -> 0,
    unresolved set identical.
- Census (VC7.1, evidence/census_fam.tsv, the 30 rows whose source names CEngineTextureMorph and CArray):
  - No landed object other than HEAD 00728d10 has a UND `??0?$CArray@VCEngineTextureMorph@@@@QAE@ABV0@@Z`.
  - 00707b10 and 00728df0 already define it pick-any (both also define `??1?$CArray`). They read NOADDR after the delete instead
    of NEQ. Measured with pk_all on both definer objects, shipped table and the applied table (evidence/definers/): the copy is
    leafref n, reach n, address source b only, so neither definer is a witness of the deleted line.
- Why held: R5/R7 says an extra must come from a shared header. These two come from the local model. They are otherwise
  nf387-clean: exact FableWin names, EQ at a retail address (given R9), HEAD sections EQ, and already emitted by landed rows.
  Ruling R8.
- Shared-header alternative: held/rows/00728d10_s1_sharedheader.cpp. It reaches RELOCATION_MATCH in one compile, but it has more
  residue (held/evidence/00728d10_H_v_s1.comdat.txt):
  - a new `??$__destroy<CMeshMorphSetParts*>` pick-any;
  - HEAD `allocate(n)` becomes STLport `allocate(n, const void*)`, with different bytes (`ret 4` -> `ret 8`);
  - `_Destroy<T>` is respelled `_Destroy<T*>`.

## Step 2: identity facts (own PE reader tools/pe392.py, objdump disasm tools/disasm.sh, python3 -I; evidence/step2/)
- (a) Dtor thunks:
  - 0x005BC0F6 and 0x005BC1AB are standalone 5-byte functions, `jmp 0x005BC0D3`. Each starts right after a `ret` / `ret 14h`, with
    no padding (/Os TU).
  - Neither is in the catalog. Both are manifest gaps (manifest-gaps.tsv hosts 0x005BC0D3 / 0x005BC0FB).
  - A whole-image rel32 / abs32 scan finds no reference to either.
  - 0x005BC272 is not a function start. It is the tail jmp of the 8-byte function 0x005BC26F `add ecx,0Ch ; jmp 0x005BC0D3`
    (manifest gap after 0x005BC237, unreferenced), which is `??1?$pair@VCMeshInfo@NGraphicAppearance@@VCEngineTextureMorphSet@@@std@@QAE@XZ`
    (FableWin name). Two pieces of evidence:
    - the member it destroys is at +0x0C;
    - the next function 0x005BC277 is that pair's `??_G` (table + FableWin; `__destroy_aux<pair*>` 0x005BC29E calls it with flag 0 at
      stride 0x18).
  - So the two thunk names (`??1?$CArray@VCEngineTextureMorph@@@@QAE@XZ`, `??1CEngineTextureMorphSet@@QAE@XZ`) have two distinct
    addresses: 0x005BC0F6 and 0x005BC1AB.
  - All 19 rel32 references to 0x005BC0D3 (opcode-checked): 14 calls and 5 jmps. The jmps are the three above plus 0x007220E3 and
    0x00722123. Each of those two sits inside an 8-byte function, 0x007220E0 and 0x00722120: `add ecx,4 ; jmp 0x005BC0D3`, two
    byte-identical CMeshMorphSetParts-shaped destructors (side finding, not resolved: HEAD 00728d10 already emits
    `??1CMeshMorphSetParts` 8 B NOADDR).
- (b) Copy forwarders:
  - 0x00709A30 and 0x00709BB0: rel32 at +8 lands exactly on 0x005BBDF1 for both. The bytes are identical; neither is referenced or
    catalogued. They are the only two 19-byte forwarders among the 20 rel32 references to 0x005BBDF1 (all `call`). The others are inside
    pair ctors (0x00709BD0 / 0x00709C00 / 0x00709C30 / 0x00709C60), the CMeshMorphSetParts copy 0x00728A00, the __fastcall
    `_Construct<CMeshMorphSetParts>` 0x00728A20 (both in the 0x00728 slice, uncatalogued) and the uninit/overflow bodies.
  - The two candidate names are `??0?$CArray@VCEngineTextureMorph@@@@QAE@ABV0@@Z` and `??0CEngineTextureMorphSet@@QAE@ABV0@@Z`
    (both in FableWin.pdb). Ego_r layouts show no other class with either type at +0.
- Assignment (inferred, ruling R9):
  - 0x00709A30 = CArray copy, 0x00709BB0 = CEngineTextureMorphSet copy.
  - 0x005BC0F6 = ~CArray, 0x005BC1AB = ~CEngineTextureMorphSet.
  - Evidence:
    - VC7.1 emits CArray<CEngineTextureMorph>'s generated members before CEngineTextureMorphSet's in every object measured (n2,
      controls ctl_AS / ctl_SA with the users in either source order).
    - Each retail pair lies in one TU slice with the first one earlier.
    - Two assumptions, not measured here: the linker keeps object section order, and each pair came from one object (both members
      sit between functions of one TU slice).
    - 0x00709BB0 sits directly before the pair ctor 0x00709BD0 that copies a CEngineTextureMorphSet.
  - Not a PDB address: do not treat it as proof without the ruling.

## Step 3: rest of the family
- R6 constness reads (no compile), from the retail callers:
  - 0x00709C90: only from _M_insert_overflow 0x00709D60 (x2). Non-const (I,O) form.
  - 0x00709CE0: only from 0x00709D60.
  - 0x00728B10: from 00728d10 x2 and 00728df0 x2. All non-const.
  - 0x00728A40: from 00728d10 only. 0x00728B50: from 00728df0 only.
  - No address has callers of both kinds. Names in NAME_FIXES.tsv are marked vendor-source-derived, no PDB (STLport 4.6
    `_uninitialized.h` signatures, as modelled in stlport/fable_containers.h; the vendor tree itself is not in this checkout).
- Retail slices:
  - 0x00728A40 / 0x00728B10 / 0x00728B50 / 0x00728D10 (+ landed 0x00728DF0) are one TU slice (the CTCRandomAppearanceMorph
    neighbourhood).
  - 0x00709C90 / 0x00709CE0 / 0x00709D60 are a different slice (the CEnginePrimitiveMorphedAnimatingMesh TU).
- 00728a40 / 00728b10 / 00728b50 (held/rows, built on the 00728d10 binder model):
  - Each reaches RELOCATION_MATCH first time, with the self spelled exactly as the table line.
  - Correction to NF390: `_Construct<CMeshMorphSetParts>` DOES have a retail copy. It is 0x00728A20, a 21-byte `__fastcall`
    (ecx = p, edx = value, null test). The models' cdecl `_Construct` was a convention misspelling. With `__fastcall` on the model's
    `_Construct` (u3), every extra of all three rows reads EQ at a retail address via pk_all --addrs (held/evidence/*_u3.addrs.pkall.tsv,
    addrs_728.tsv):
    - `??0CMeshMorphSetParts` copy = 0x00728A00 (29 B, unique);
    - `_Construct` = 0x00728A20;
    - CArray / CEngineTextureMorphSet copy and dtor per R9;
    - 00728b50's pick-any `??$__uninitialized_fill_n` = 0x00728A40.
  - The extras are local-model born (R8). 00728b50 also keeps `??0__false_type@std@@QAE@XZ` (3 B, NOADDR) as residue.
  - Selves go NoDuplicates -> Any (template instances).
  - Follow-up for b1: HEAD 00728d10 emits the cdecl `_Construct` (NOADDR). Switching it to `__fastcall` would make that section
    retail-EQ (0x00728A20), but it changes a HEAD section, so b1 leaves it as is.
- 00709d60: one probe of the R6 respell (held/rows/00709d60.cpp): STLport (I,O) / (I,N,T) / (I) templates plus `CArray<T> : vector<T>`
  in its local model. It reaches RELOCATION_MATCH and emits exactly the (I,O) / (I,N,T) spellings above, plus `__destroy_aux<pair*>` in
  the STLport I-form.
  - HOLD on residue (held/evidence/00709d60_H_v_u1.comdat.txt, 00709d60_u1.addrs.pkall.tsv):
    - a new `_Destroy<pair>(p)` (12 B, NOADDR);
    - `__destroy_aux` (36 B, NOADDR on the shipped table; retail 0x005BC29E is 29 B and calls `??_G` with flag 0);
    - `??_G pair` NEQ v 0x005BC277 (it was already a different body in HEAD);
    - `_Construct<pair>` (cdecl, NOADDR);
    - several HEAD sections change bytes. Some become retail-EQ: `~pair` = 0x005BC26F, `pair(const pair&)` = 0x00709C00, and the
      `~CEngineTextureMorphSet` thunk.
  - The table also has two more names at 0x00709D60's neighbourhood to retire with the unit:
    - the T-form `??$__destroy_aux@U?$pair...` -> 005bc29e (witness 00709d60);
    - `?_M_insert_overflow@?$CRetailVector@...` -> 00709d60 (witness 00709e60, a second name for 0x00709D60).
  - The landed 00709d60 self is public `QAE` (STLport's is protected `IAE`). This was not changed.
- 00709ce0 / 00709c90: not re-run. NF390's b2 / held candidates stand.
  - NF390's b2 00709ce0 and held 00709c90 still stand.
  - With b1's delete, 00709ce0's CArray copy reads NOADDR instead of NEQ, so its R5 NEQ blocker is gone. Its shared-header extras
    then need only R9.
  - The unit owner is 00709d60. Respell its local model to `CArray<T> : vector<T>` (as b1) and to STLport (I,O) / (I,N,T)
    templates. Then TABLE_CHANGES are:
    - replace the two T-form lines at 0x00709C90 / 0x00709CE0 (witness 00709d60) with the (I,O) spellings;
    - delete `??0CEngineTextureMorphArray@@QAE@ABV0@@Z` -> 005bbdf1 (UND only in 00709ce0 / 00709d60);
    - rewitness `??0CEngineTextureMorphArray@@QAE@ABU0@@Z` (also UND in 00709e60, census_fam) via HAND_TABLE_CHANGES unless
      00709e60 joins.
- Side identity: 0x005BC277 landed as `??_GSRecord_005bc277` is `??_G?$pair@VCMeshInfo@...@VCEngineTextureMorphSet@@@std@@QAEPAXI@Z`
  (NAME_FIXES).

## Rulings needed: RULINGS_NEEDED.tsv (R8 local-model extras for b1; R9 inferred name->address for the four generated members).

## Tools (tools/)
- pe392.py: PE dump, rel32, xref.
- hostof.py: catalog host of a site.
- disasm.sh: objdump of retail bytes.
- secorder.py: COMDAT section order.
- comdatrel.py: per-COMDAT bytes and relocs.
- Copied and renamed from NF390: nf392.py, probe392.py, census392*.py, comdats.py, pk374.py, pkt.py, runpair.sh, others.
