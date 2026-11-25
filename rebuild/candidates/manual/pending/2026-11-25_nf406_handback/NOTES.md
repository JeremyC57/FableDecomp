# NF406 hand-back (2026-11-25): T3 `nf402 0x004D31C0` and `nf402 0x004DDA94`

## Summary
- Two batches, laid out like NF402's b1 (RESULTS, SELF_SYMBOLS, TABLE_CHANGES, UNITS, rows.txt, rows/, tests/, orig/rows, orig/tests, head/payload.json + oracle.tsv, logs/, evidence/).
- **batches/b1: 4 ACCEPT.** The four GetClassTypeName callers, respelt from extern "C" `__stdcall` stand-ins to `?GetClassTypeName@<Class>@@UBE?AVCCharString@@XZ`:
  - 0x004DDA94 CTCTargetingAI
  - 0x004D8154 CTCDCameraPoint
  - 0x004E07C6 CTCDHeroSuit
  - 0x004E3148 CTCDExplosion

  Each row is its own unit. TABLE_CHANGES has one `add`: `?StatGetClassTypeName@CTCTargetingAI@@SI?AVCCharString@@XZ -> 004d2a74`, witness 004dda94.
- **batches/b2: 1 ACCEPT.** 0x004D31C0 CTCVillage::StatGetClassTypeName, respelt to `?StatGetClassTypeName@CTCVillage@@SI?AVCCharString@@XZ`, with its literal and callee fixed. TABLE_CHANGES has one `delete`: `?ctor@CCharString@@QAEPAV1@PBDH@Z -> 0099ebf0`, witness 004d31c0.
  - **The definition form departs from NF402 b1 because of the duplicate gate.** The static is defined in the class body (pick-any), and an anonymous-namespace scaffolding helper takes its address. This is the nf86 / nf88 form (landing nf387 ruling), not NF402's out-of-line form. The reason is in "0x004D31C0" below.
  - The `delete` travels with b2: HEAD's 0x004D31C0 text is the line's only user.
  - Alternate: the NF402 b1 out-of-line text is in b2/evidence/004d31c0_alternate_nf402form.cpp.txt. It grades RELOCATION_MATCH PASS (b1 logs/vland_bn.log, row 004d31c0) and links duplicates 0 -> 1 against the binder (b2/evidence/link_b2_control_nf402form.txt). The coordinator can pick either form without a round trip.
- All 5 locks are kept as NF406. No HOLD rows, no header edits, no FLAGS change, no RULINGS_NEEDED.
- CTCObjectAugmentations has **no** GetClassTypeName caller row.
  - In a raw E8/E9 scan of Fable.exe, 0x004D3CDE has exactly one caller: RegisterTCs, at 0x004F6051.
  - The class's GetClassTypeName is 0x0076C3B0 (vftable 0x0126A3A4 slot 22). It expands the body and calls nothing, so it is not in this batch. It goes in NAME_FIXES and T3 because it is a `_Methods` stand-in.

## Names and sources

### Four GetClassTypeName callers (b1)
| row | name | source 1: PDB | source 2: retail vftable slot 22 | source 3: callee |
|---|---|---|---|---|
| 0x004DDA94 | CTCTargetingAI::GetClassTypeName | Ego_r.pdb + FableWin.pdb name sets: `?GetClassTypeName@CTCTargetingAI@@UBE?AVCCharString@@XZ` | 0x012411FC (.?AVCTCTargetingAI@@): 0x01241254 -> 0x004DDA94 | call 0x004D2A74 at 0x004DDA98 = CTCTargetingAI::StatGetClassTypeName (NF402 b1) |
| 0x004D8154 | CTCDCameraPoint::GetClassTypeName | both name sets: `?GetClassTypeName@CTCDCameraPoint@@UBE?AVCCharString@@XZ` | 0x0123EC3C: 0x0123EC94 -> 0x004D8154 | call 0x004D39CF at 0x004D8158 |
| 0x004E07C6 | CTCDHeroSuit::GetClassTypeName | both name sets: `?GetClassTypeName@CTCDHeroSuit@@UBE?AVCCharString@@XZ` | 0x01242004: 0x0124205C -> 0x004E07C6 | call 0x004D4232 at 0x004E07CA |
| 0x004E3148 | CTCDExplosion::GetClassTypeName | both name sets: `?GetClassTypeName@CTCDExplosion@@UBE?AVCCharString@@XZ` | 0x01242A54: 0x01242AAC -> 0x004E3148 | call 0x004D5CE6 at 0x004E314C |

- The name sets are keyed by donor address, so they are used here as name sets only. The address binding comes from the vftable slot.
- Slot 22 is CTCBase's pure `GetClassTypeName` (engine/CTCBase.h, vtable 0x0125BE0C). Slot 21 is GetInterfaceType in every one of these vtables.
- A raw retail scan finds each row's address only as an abs32 in its `.rdata` vftable slot. There are no direct callers.
- **Calling convention and return slot.** The retail bytes are `8b4c2404 e8xxxxxxxx 8b442404 c20400`:
  - a `__thiscall` whose `this` (ecx) is unused;
  - the hidden CCharString result pointer is the one stack argument;
  - that pointer is forwarded in ecx to the static `__fastcall` (`SI`), and eax returns it;
  - `ret 4`.

  This is exactly `UBE?AVCCharString@@XZ`: public virtual const thiscall, returning by value. VC7.1 emits it from `return StatGetClassTypeName();`.

### 0x004D31C0 CTCVillage::StatGetClassTypeName (b2)
- **Source 1:** the FableWin.pdb name set has `?StatGetClassTypeName@CTCVillage@@SI?AVCCharString@@XZ` (tc_register.obj).
- **Source 2:** the retail literal 0x0123A5CC is "CTCVillage" (read from the exe), which is the class name.
- **Source 3:** the landed binder CTCBase::RegisterTCs 0x004EE23F already binds this spelling (reloc_bindings.tsv). Its call at 0x004F00E3 is the only retail caller.
- **Literal check:** the literal was right but unchecked. HEAD cast the address `(const char*)0x123a5cc`, which carries no relocation and so escaped both the grader and the literal check (NF388 queue). The row now pushes the string literal `"CTCVillage"`, and vland's literal check passes (no WARN).
- **Callee:** `??0CCharString@@QAE@PBDJ@Z` (checked by NF406: one entry each in fablewin_pdb_names.tsv and egor_pdb_names.tsv; reloc_bindings.tsv binds it to 0099ebf0, witness 00636e90), the same spelling the NF402 rows use (fable_string.h under FABLE_RETAIL_CCHARSTRING). It replaces the made-up `?ctor@CCharString@@QAEPAV1@PBDH@Z`.
- **Census for the table delete:**
  - No row in rebuild/src spells a `class CCharString` member `CCharString* ctor(const char*, int)` other than HEAD 0x004D31C0 (grep of `ctor(const char`, `->ctor(`, `.ctor(`).
  - The table line's only witness is 004d31c0.
  - The 0x008FD4BC test still models it, but tests bind nothing (see T3).
  - apply_table --dry: 1 delete, 0 misses, multi-target 1 -> 1, nothing new.

## Row texts and headers
- **CTCTargetingAI and CTCDExplosion:** the layout-rename form of CTCBerserk 0x004D4A86.
  - The class adds `static CCharString __fastcall StatGetClassTypeName();` and `virtual CCharString GetClassTypeName() const;`.
  - The preludes are NF402's 0x004D2A74 and 0x004D5CE6 preludes, with `FABLE_RETAIL_CTCBASE` defined first.
- **CTCDCameraPoint and CTCDHeroSuit:** there is no engine header, so each uses a local class over its RTTI direct base `engine/CTCDriverBase.h` (rtti_bases.tsv: `.?AVCTCDCameraPoint@@` / `.?AVCTCDHeroSuit@@ : CTCDriverBase`).
- **The virtual is a real override, not a second vptr.** Every row uses the polymorphic `FABLE_RETAIL_CTCBASE` branch, so CTCBase slot 22 is the pure virtual (EXTRA_RULES: never stack a virtual on the flat branch's `__vftable` member).
  - Proof (evidence/polymorphic_ctcbase_check.txt): each row compiles with `sizeof(Row class) == sizeof(its layout / base)` asserted.
  - Control: the CTCDCameraPoint row with `FABLE_RETAIL_CTCBASE` dropped fails exactly that assert (C2118).
- `#pragma optimize("s", on)` is kept from HEAD and placed after the includes, directly before the definition.
- **Objects** (evidence/object_symbols.txt): each new b1 object defines only its leaf (16-byte .text) plus one UNDEF, the static. There is no vftable, `??_G` or header COMDAT.
- **0x004D31C0** keeps the CTCVillage.h layout-rename (flattened default branch).
  - Its object holds: the leaf as COMDAT selection 2 (19 bytes, checksum 0xc7bfbbff); the literal; and the 6-byte scaffolding helper `?ReferenceStatGetClassTypeName@?A0x...` (marked `Fidelity: scaffolding, not retail`).

### Why 0x004D31C0 is pick-any (b2), and the duplicate debt
- RegisterTCs 0x004EE23F defines every registered TC's static in the class body (`TC_REGISTRATION_INTERFACE`).
  - Its object holds a pick-any (selection 2) `?StatGetClassTypeName@CTCVillage@@...` that is byte-identical to the row: checksum 0xc7bfbbff, 0x13 bytes (evidence/binder_004ee23f_static_comdats.txt).
- The NF402 out-of-line form (selection 1), linked /FORCE with the binder, adds one LNK4006 duplicate, 0 -> 1 (evidence/link_b2_control_nf402form.txt). Landing nf382 counts this NoDuplicates v Any clash as a new duplicate.
- The pick-any form links 0 -> 0 (evidence/link_b2.txt).
- Retail agrees that the original is an inline:
  - RegisterTCs expands the static at its first 41 registrations and calls the out-of-line copy after that;
  - CTCVillage::GetClassTypeName 0x0066122E is that static expanded.
- The same clash already exists at HEAD for NF402's out-of-line statics: 4 measured (b1 link_b1.txt, HEAD 4 -> new 4, unchanged by b1). The other same-spelling selves are queued in T3. This is existing debt, not a hold.

## Tests
- **b1:** each HEAD test was a bare printf. Each new test:
  - links the row object and supplies the class's static as a stub;
  - CCharString has a user ctor, so it returns through a hidden slot, as in the row;
  - calls the row through a const reference;
  - asserts one static call, the class-name text, and that the static constructed into the caller's own hidden slot (`g_slot == &r`).

  The catalog pass patterns are kept (`PASS_<addr>`).
- **b2:** the test asserts the retail text "CTCVillage", length -1, one ctor call and construction into the hidden slot. Pattern `004d31c0_TEST PASS`.
- **Mutation controls** (evidence/test_mutation.txt in each batch):
  - for b1, the static called twice makes every test FAIL (`calls=2`);
  - for b2, the literal changed to "DoCreatureAction" makes the test FAIL;
  - the real rows PASS.
- HEAD 0x004D31C0 had the right literal (as a cast), so its HEAD side has no LITERAL_DIFF control. The mutation run is the literal control.
- The first b1 test draft used a POD CCharString, which returns in eax. It failed against the row's hidden-slot ABI, so the model was fixed before the gated run. That was one extra vland pair.

## Gates
- **vland**, HEAD/new at one LANDVERIFY_WORK path per pair (b1: work/wd_b, logs/vland_bh.log + vland_bn.log; b2: work/wd_c, logs/vland_ch.log + vland_cn.log):
  - HEAD: all RELOCATION_MATCH PASS, with WARN RELOC_NAME (`?Init_<addr>@T@@` / `?ctor@CCharString`), STANDIN_LEAF (31c0), and GENERIC_STRUCT (dda94, 3148).
  - New: all 5 RELOCATION_MATCH PASS. No LITERAL_DIFF / LITERAL_UNCHECKED, RELOC_NAME, STANDIN_LEAF or GENERIC_STRUCT.
  - No grade drop.
  - The b1 logs come from a run that still held 31c0 in its out-of-line form (also RELOCATION_MATCH PASS). The b2 logs are 31c0's final form.
  - WARN SHARED_HEADER_AVAILABLE prints on dda94, 3148 and 31c0. It is the verifier false positive already queued (T3 `nf402 tools`). Under LANDVERIFY_STRICT_HEADERS=1 these rows would be rejected, like the NF402 siblings.
- **pk_all + pickany_vs_retail**, HEAD (repo table) v new (work/troot_<b>: the batch's TABLE_CHANGES and SELF_SYMBOLS applied): 0 NEQ everywhere, 0 reach flips (evidence/pk, pk_summary.txt, reachflip.txt).
  - b1 objects have no copies besides the leaf.
  - b2's one extra copy is the scaffolding helper: strong, NOADDR, reach n. This is the per-TU helper section the nf387 ruling accepts.
- **/FORCE link** (VC7.1 link, /DLL /NOENTRY /NODEFAULTLIB, NF398's link377.py):
  - b1 set: the 4 rows, the 4 NF402 statics and the binder 0x004EE23F. Duplicates 4 -> 4 (the same pre-existing four).
  - b2 set: the row and the binder. Duplicates 0 -> 0.
  - Nothing HEAD resolved becomes unresolved. The only RESOLVED_TO_GONE names are the old stand-in selves, which no table line or row references. GONE_UNRESOLVED: `?Init_004d2a74/39cf/4232/5ce6@T@@QAEXXZ`, `?ctor@CCharString@@QAEPAV1@PBDH@Z`.
- **Binders of respelt names:**
  - The old selves and `?Init_*` names have no table lines.
  - `?ctor@CCharString@@QAEPAV1@PBDH@Z` is deleted in b2.
  - The new callee spelling `?StatGetClassTypeName@CTCTargetingAI@@SI` gets its first table line in b1. The other three statics were already bound by 0x004EE23F, and the 31c0 spelling too.
  - The new `?GetClassTypeName@<Class>@@UBE` names are referenced only by retail vftables (not landed), and no other source or table line spells them (fixed-string grep of rebuild/src, rebuild/oracles, rebuild/include, rebuild/tests).

- **Coordinator checkers:**
  - reserved_check.py (RESERVED_REFS empty, so no git; logs/reserved_check.txt in each batch): reserved 0, mentions 0.
  - spellcheck.py runs `git diff`, so I did not run it. A git-free equivalent over the 5 rows, the 4 statics and 0099ebf0 found:
    - no table line binds any new name to another address;
    - no other spelling sits on the rows' or statics' addresses.
  - 0099ebf0 carries about 40 stand-in spellings beside the PDB ctor (pre-existing); b2 removes one.
  - The other coordinator scripts are landing-side (land_chunk, records, apply_*, recheck payloads) and need no hand-back input.

## Budget
- vland pairs: b1, 2 (the first found the test ABI bug); b2, 1; one HEAD context run of 2a74 / 39cf / 4232 / 5ce6 / ee23f for the link sets.
- Direct compiles: 5 prechecks, 4 size asserts plus 1 control, 1 pick-any precheck.
- Mutation links.
- No wineserver -k. Only my own PIDs.

## Hand-back files
- batches/b1, batches/b2: see Summary.
- NAME_FIXES.tsv: 0x0066122E and 0x0076C3B0, two GetClassTypeName overrides landed as `_Methods` stand-ins, with vftable slot plus both PDB name sets.
- T3.txt:
  - both nf402 lines done;
  - the StatGetClassTypeName v binder duplicate family;
  - 0x0066122E and 0x0076C3B0;
  - the `_CTC*_GetClassTypeName@4` (148) and `_Methods` (139 + 13) stand-in families;
  - the stale 0x008FD4BC test.
