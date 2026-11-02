# retry11a: Codex pass 11 leftovers, rows 1–23 of 46

Result: 2 RELOCATION_MATCH (00520670, 00521620), 20 DIFFER parked after at least 10 distinct source-shape probes each, and
1 BLOCKED on layout (005eb9e0, research only). `STATUS.tsv` has one line per row. `probe/<addr>.log` lists every attempt.
The header overlay is described in `hdr/README.md`.

The final check was the read-only vland, run from the repo root with `FABLE_HDR_OVERLAY=$W/hdr LANDVERIFY_WORK=$W/lv`. Both
rows gave RELOCATION_MATCH and their tests gave PASS. `--land` was never passed. With the overlay, the landed includers of
the two extended branches (00b3efa0, 00a4fe60, 00430900, 00631c60) all still give RELOCATION_MATCH PASS under
`--recheck-landed`.

## The two matched rows

### 0x00520670 `?LoadHeroStateBinary@CThingManager@@QAEXAAVCPersistContext@@@Z`

- Size: 1524 bytes (ret 4 at 0x00520C61, then int3 padding to 0x00520C64).
- Identity: in both Ego_r.pdb and FableWin.pdb. In Ego_r it is at 0x0052A690, between LoadHeroStateScript 0x0052A5B0 and
  OnThingKill 0x0052AC10.

### 0x00521620 `?SaveToGameStateBinary@CEntityRuntimePersistence@NThingManager@@QAEXAAVCPersistContext@@@Z`

- Size: 1129 bytes. Compiled with `/Gr`.
- Identity: in both PDBs (Ego_r 0x0052C6C0, FableWin 0x01D42CC0). Its caller is 0x0052444F. The manifest label
  `CEditWorldMap_LoadRivers` is wrong.

### What both rows needed

1. **Same-TU callee bodies under `#pragma auto_inline(off)`** (GOTCHAS 163). The rows need the
   `CPersistTraits<long/unsigned long/unsigned __int64>::TransferBinaryIn` bodies. Retail has them at 0x0040FE60,
   0x0040F8A0 and 0x0044B890, and calls them out of line.
   - Declared only: 00520670 gives 1604v1524 (probe h06).
   - One generic out-of-line template: 00520670 gives 1450v1524 (h08).
   - Only explicit non-inline bodies match. VC7.1 uses its knowledge of what these bodies clobber when it allocates
     registers in the caller.
2. **Explicit inline specializations of `CPersistContext::Transfer<long/unsigned long/(u64)>`.** Each one is
   CheckCRC(""), then a traits local, then a switch on Mode.
   - A single generic `Transfer(T&)` body fails to compile. h07 instantiates `CPersistTraits<CCharString>` and gets C2440.
     h10 adds an explicit-specialization declaration for `Transfer<CCharString>`, which must stay out of line at
     0x00431143, and that triggers a VC7.1 internal compiler error (C1001).
3. **CDataInputStream has no user-declared destructor.** Retail calls `~CDataStream` 0x00993B10 directly.
4. **`CArray(n)` forwards to `vector(n)`.** This is the FABLE_RETAIL_CARRAY branch.
5. **00521620 only: an explicit specialization of STLport's `std::__uninitialized_copy<CPersistedDeadEntity*>` under
   auto_inline(off),** using the STLport body. The final source is h11. A `__declspec(noinline)` form (h03, h04) also
   matches. Without the specialization (h02) the result is 1193v1129. Under /Gr this template is fastcall (YI) at
   0x005269B0.

### Local names

The local names come from the Codex pass 11 extraction of the Ego_r locals. I could not re-read Ego_r.pdb on this machine.
The source comments now say so.

### Callee 0x00431143 (the name-review hold)

Spelling: `??$Transfer@VCCharString@@@CPersistContext@@QAEXAAVCCharString@@@Z`.

1. It is already the only spelling in reloc_bindings for this address (landed caller CTattooDef::Transfer 0x004529AE).
   This pass adds no second spelling.
2. It belongs to the same one-argument `Transfer<T>(T&)` template family as the landed 0x00431020, 0x00431061 and
   0x00431102. T=CCharString is proven by its callees (the CCharString read and write paths).

Caveat: both PDBs only list the named two-argument `Transfer(const CCharString&, T&, ...)` forms, because the debug builds
inlined this one-argument form. NAME_FIXES_QUEUE c624 also covers the CWideString twin at 0x004310A7. I treat the name as
settled. The coordinator can lift the BYTE_MATCH_NAME_REVIEW hold.

## NAME_FIXES (see NAME_FIXES.tsv)

- Six spellings are proven in both Ego_r and FableWin and fix wrong landed bindings: OpenMemorySource (PBX, not PAX),
  GetRequiredWorkspaceSize (SIKXZ), Initialise (AAE, private), ReadNullTerminatedString (UAE, virtual),
  CWorldMap::GetMapUID (QBEKJ) and ~CDataStream 0x00993B10.
  - 0x00993B10 currently has 4 spellings bound to it.
  - 0x004FCE00 has `CRegionLookup_004fce00::GetRegionForLevel`, which is an invented stand-in class.
- 0x009BAE50 `??0CPersistContext@@QAE@AAVCDataOutputStream@@_N@Z` had no binding before this pass.
- **0x005269B0 gets a second spelling.** My row 00521620 binds it as
  `__uninitialized_copy<NThingManager::CEntityRuntimePersistence::CPersistedDeadEntity*>`. The landed caller 0x0052B030
  binds it as the placeholder `CUnprovenElement_005269b0`. The PDBs are Dinkumware, so they carry no STLport name. The
  evidence is the STLport 4.6 signature, fastcall under /Gr, and the 16-byte element of 00521620's `dead_entities` vector.
  The landed caller needs to be respelled.
- Every external symbol in both matched objects was checked with `objdump -t` against the PDB name lists and
  reloc_bindings.
  - All are in both PDBs except four.
  - `??_G` of CCompressorZlib and CMemoryDataInputStream are in Ego_r only.
  - CACompressor's Compress, Decompress and SetQuality are in FableWin and Ego_d, and `rtti_port_compare.tsv` reports them
    as "agree".
  - `??3@YAXPAX@Z` and `PMainGameComponent` are bound and established.

## 005eb9e0 `?FrameUpdate@CTCTavernGame@@UAEXXZ` (BLOCKED, decision needed)

- **Extent and identity are correct:** 1472 bytes. That is 1414 bytes of code, 2 pad bytes, and a 14-entry switch table at
  0x005EBF68..0x005EBFA0.
- **The blocker:** retail reads `CTCOwnedEntity::POwner` (a `CIntelligentPointer<CThing>`) at **+0x0D**. The instruction
  at 0x005EBACD is `lea edi,[eax+0xd]`, then `call 0x00A01B50` (CBaseIntelligentPointer::GetPItem). The next member,
  OwnerUID (ulong64), is at +0x18.
  - POwner at +0x0D means the pointer class has alignment 1.
  - OwnerUID at +0x18 rather than +0x15 means CTCOwnedEntity itself is not packed.
  - Ego_r has 60 `CIntelligentPointer<...>` members at offsets that are not multiples of 4, so the packing belongs to the
    pointer classes.
- **Proposal:** an opt-in `FABLE_RETAIL_CINTELLIGENTPOINTER` branch that declares CBaseIntelligentPointer and
  `CIntelligentPointer<T>` between `#include <pshpack1.h>` and `#include <poppack.h>`, with the evidence above.
  - The owner rule of 2026-11-02 (GOTCHAS 1785) allows SDK boundary headers and forbids an authored `#pragma pack`.
  - The C2DVector branch from da932e8cc used `#pragma pack(push,1)`. The coordinator may want to align the two.
  - Before landing, check the TPI packed-property bit for CBaseIntelligentPointer in Ego_r and FableWin. That needs a
    machine with Ego_r.pdb; it is not here.
  - There is no shared CIntelligentPointer header in rebuild/include today, and the template is used across many
    classes. That makes this a header-architecture decision (the cons11b CThing family), not a per-row edit, so I wrote
    no FrameUpdate source.

## The 20 parked rows

The recurring blocker is **the inline budget**. Retail inlines an inline member at some call sites of a function and calls
it at others. No source shape I tried reproduces that split.

| Row | Retail split | Closest probe |
|---|---|---|
| 00ee8390 | The one-argument resize(n) wrapper is inlined, but its element ctor, resize(n,x) and element dtor are calls. ~CScriptThing is inlined at the teleport temporary. | Explicit two-argument resize matches the teleport site but inlines the dtor at the resize site (p05). Declaring the dtor only leaves only the teleport diff (diagnostic d03). |
| 00d6d080 | Same pattern. Retail uses `resize(count, T())` and calls ctor 0x007E72A0 and dtor 0x007E74D0 (GOTCHAS 310). | The explicit form inlines the dtor and gives 1580. |
| 00a15890 | push_back is a call at all three start-node sites. | This build inlines the first one. |
| 005e8220 | Growth of the defs, experience and gold vectors is a call; renown growth is inline. | Merging two map walks gets the call sequence right but at the wrong size. With four walks, gold growth inlines too (1428). |
| 00524290 | Transfer<long/u64> is inline. | Stays out of line even with explicit inline specializations. All variants gave identical output. |
| 006c9490 | `CCharString::operator==(const char*)` is inline at every symbol test, and so is the final `CPersistTraits<long>::TransferIn`. | Inlined at the first three only. |
| 00409730 | The CDataInputStream readers and ~CDiskFileWin32 are inline. | All stay calls; 641v1078. |
| 0069a2a0 | The camera-mode ctors and counted-pointer conversions are inline. | They stay calls. |

Rows that match in length (1-for-1 size) and are left with scheduling or register-role residue: 008185b0 (one x87 factoring),
009bca20 (one receiver reload), a58130 (one scheduling group), 774d20, ac92c0, 0055f710 (retail `xor edx,edx; mov dx`
zero-extension; a ushort member or getter, /G5 and named locals did not reproduce it).

Frame rows:
- 0048e0a2: 12 bytes of locals are missing.
- 00f3c990: retail has an 8-aligned /GS frame (`and esp,-8`); the 8-aligned local is not found.
- 00645853 and 00839420: slot ranking.
- 00a4efc0 and 00ac8c80: add vs lea, and the register role of `this`.

**Differ context debt:** every `differ/<addr>.cpp` grades against its Codex row context
(`$S/codex11_dl/x/rows/<a>/selected_context/hdr`), not rebuild/include plus this overlay. Line 4 of each file says so.
A retry should rebase first.

## Decisions for the coordinator

1. Lift the name hold on 00520670 and 00521620, and land them with this overlay. Their sources define FABLE_RETAIL_CARRAY,
   FABLE_RETAIL_SCOPED_PTR, FABLE_RETAIL_CDATAOUTPUTSTREAM, FABLE_RETAIL_CMEMORYDATAOUTPUTSTREAM, FABLE_RETAIL_CWORLDMAP,
   FABLE_RETAIL_CCLASSFACTORY, FABLE_RETAIL_CACOMPRESSOR and FABLE_RETAIL_CCOMPRESSORZLIB (all new macros), plus the
   existing CCHARSTRING, CDATAINPUTSTREAM, CMEMORYDATAINPUTSTREAM, CWORLD and CPERSISTCONTEXT.
2. Respell landed caller 0x0052B030 to the `__uninitialized_copy<...CPersistedDeadEntity...>` spelling, and apply the
   rest of NAME_FIXES.tsv.
3. Decide on 005eb9e0 packing: a `pshpack1.h` branch for the intelligent-pointer classes, plus the TPI bit check.
4. Reconcile `engine/CMainGameComponent.h` and `engine/CGameComponent.h` (new here) with the older model in `fable_game.h`.
   Merge `engine/CThingManager.h` and `engine/CMainGameComponent.h` into the cons11b CThing-family work.
