# NF265 notes (2026-11-22, round 26): L129 DeleteTexture unit BLOCKED (family); nothing offered for landing

Handed back after working one L129 unit to a terminal state (context). The unit (0x00BE5C30 + 0x00BE6090 with 0x00BDA110 onto
`?DeleteTexture@CEngineLandscapeMemoryManager@@QAEXPAVCTexture@@@Z`) was drafted and graded (3 rows RELOCATION_MATCH + PASS, header opt-ins
equiv_check-clean), then found to collide with two landed rows. It is `blocked: family`; the graded drafts are held in work/held/ (rows/,
hdr_engine/, orig/, recheck_draft.log, equiv.tsv) for the family pass. rows/ and hdr/ are empty; RENAMES / TABLE_CHANGES are headers only.
Repo root (`ls -A`) before / after: identical. No git, no sleep.

## What blocks it: the queue and destroy-now trios are named the wrong way round in five landed rows
- Retail has two trios of CEngineLandscapeMemoryManager members (xdis):
  - destroy-now: 0x00BD9C00 / 0x00BD9C20 (`call [vtbl+0]` flag 0, then FreeVirtual) and 0x00BD9CC0 (`CTexture::Uninitialise 0x009F9F70`,
    then FreeVirtual); not catalogued, no E8 caller (inlined into ProcessDisposalList / FreeDisposalList);
  - queue: 0x00BDA090 / 0x00BDA0D0 / 0x00BDA110 (push the resource on Index / Vertex / Texture DisposalList[CurrentDisposalListIndex]).
- Names (two sources each):
  - destroy-now = the PRIVATE `Delete<X>BlockWhileBusy`: retail FreeVirtual 0x00BD9B90 / 0x00BD9C00 / 0x00BD9C20 / AllocVirtualAligned
    0x00BD9C40 / FreeVirtualAligned 0x00BD9CA0 / 0x00BD9CC0 is Ego_r's six-function run FreeVirtual 009c2300 / DeleteVertexBufferBlockWhileBusy
    009c2370 / DeleteIndexBufferBlockWhileBusy 009c23a0 / AllocVirtualAligned 009c23d0 / FreeVirtualAligned 009c2430 /
    DeleteTextureBlockWhileBusy 009c2450, in order; both name sets carry the three names (FableWin 02cb73d0 / 02cb7410 / 02cb7390).
    VB / IB for 0x00BD9C00 / 0x00BD9C20 rests on Ego_r order only (the bodies are byte-identical).
  - queue = the PUBLIC `Delete<X>`: Ego_r DeleteIndexBuffer 009c25e0 / DeleteVertexBuffer 009c2640 / DeleteTexture 009c26a0 (the same IB / VB /
    Tex order as retail 0x00BDA090 / 0D0 / 110), FableWin 02cb7350 / 02cb7450 / 02cb7490; every catalogued caller of the queue trio (22 E8
    sites) is another class's member, and neither name set has any other texture- or buffer-taking manager member, so no inline forwarder
    can stand between the outside callers and a private member.
  - (Withdrawn: the earlier "FableWin order + 0x40 spacing" argument; FableWin spaces all seven members 0x40 apart.)
- The landed rows say the opposite: the self rows 0x00BDA090 / 0x00BDA0D0 / 0x00BDA110 are named `Delete<X>BlockWhileBusy` (AAE; 0x00BDA110's
  Identity line "proven"), and ProcessDisposalList 0x00BDA150 / FreeDisposalList 0x00BD9CE0 define same-TU helpers named `DeleteTexture` /
  `DeleteVertexBuffer` / `DeleteIndexBuffer` (QAE) with the destroy-now bodies (their comments call 0x00BD9CC0 "DeleteTexture" from Ego_r
  adjacency to FreeDisposalList). Those helpers are out-of-class, non-inline definitions: strong. Measured: HEAD 0x00BDA150's object
  defines all three `?Delete<X>@CEngineLandscapeMemoryManager@@QAE...` symbols, and linking it with the 0x00BDA110 draft gives
  `LNK4006 ?DeleteTexture@CEngineLandscapeMemoryManager@@QAEXPAVCTexture@@@Z` (work/lnk). 0x00BD9CE0 was read from source only.
- So the draft unit would make `?DeleteTexture@...` (and, for 0x00BE6090, `?DeleteIndexBuffer@...` / `?DeleteVertexBuffer@...`) name two
  different functions. The recheck, pk.py, the three-object link and the table grep could not see this (the helpers are bound by no
  relocation and have no table line); a symbol-side grep of the new spellings over rebuild/src/compiled found it.

## Family plan (one unit; locks: 0x00BDA150 is NF265 now, the rest unclaimed)
1. 0x00BDA150 / 0x00BD9CE0: respell the three same-TU helpers in place to the private `Delete<X>BlockWhileBusy` (AAE); regrade both (a member
   rename can move c1 symbol ordinals: grade, do not assume); fix their Identity comments; tests if they name the helpers (0x00BDA150's test
   models its own `DeleteTexture`: a self-contained host model).
2. 0x00BDA090 / 0x00BDA0D0 / 0x00BDA110: self renames onto the public names. Recipe proven on 0x00BDA110 (work/held/rows/00bda110.cpp: port
   onto the retail manager branch + a declaration opt-in, RELOCATION_MATCH PASS, header-based test; the in-place variant also passes); the
   held overlay already has all three declaration opt-ins.
3. Binders: 0x00BE5C30 / 0x00BE6090 (held drafts, graded), and the other landed binders of 0x00BDA090 / 0x00BDA0D0, which today use
   other stand-ins: 0x00BFD480 (AppendFreeEdgeNode), 0x00BEAA00 (FreeBlock), 0x00BEAAA0 (InsertNode@LandscapeLayerList), 0x00BEB4D0,
   0x00BFDD40, 0x00BFDE20 (several are L129 rows with their own 0x01436EA8 stand-ins: DAT_01436ea8, g_asyncRoot ...). Uncatalogued call
   sites 0x00BE3807 / 0x00BE382B / 0x00BFD205 / 0x00BFD530 / 0x00BFD8C4 / 0x00BFDAD0 are in no landed row.
4. Census after: 0x00BDA110 3 -> 1; 0x00BDA090 3 -> 1 and 0x00BDA0D0 4 -> 1 if every binder moves; 0x01436EA8 table 12 -> 11 or fewer.
- What the held drafts showed (reusable): 0x00BE5C30 and 0x00BE6090 objects are section-for-section equal to HEAD after the renames (only
  `.drectve` gains `/DEFAULTLIB:"libcp"` from stlport/fable_containers.h); 0x00BE6090's two vtables vtcheck-identical to HEAD; link of
  the three drafts: 0 duplicates (control 1). Header proof: work/held/hdr_README.md (equiv_check 1,470 rows, 0 differ, live control).

## Other findings (NAME_FIXES.tsv)
- 0x00BE5C30's leaf: `?ClearTextures@CLandscapeBackgroundPatch@@QAEXXZ` (both PDBs + Ego_r donor order); no other row defines or binds it
  (0x00BF4220 calls CWaterPatchMesh::ClearTextures; 0x00C5E3D0 left it in the vendor wave). Draft held with the family.
- 0x00B677A0's self `long*` v the PDB `long&`; 0x00BE6090's self stand-in `??1Patch@Native@@UAE@XZ` v `??1CLandscapeBackgroundPatch@@UAE@XZ`
  (0x00BE8E60 binds the latter: a HEAD split).
- Comment-only leftover: 0x00BD9CE0's line 10 names "the landed DeleteTextureBlockWhileBusy 0x00BDA110".

## Not started: LocalDetailEngine 0x00BFAAE0 / 0x00BFB590, 0x00B684F0, 0x00BEB4D0's DAT_01436ea8 (now also a family binder), the
## CEngineGlobal rows, entry 2 0x009B0AC0 (NF260 plan stands).

## Decisions for the coordinator
- D1: the family above. May the 0x00BDA090 / 0x00BDA0D0 self renames (and 0x00BDA150 / 0x00BD9CE0 helper respells) land while some of their
  other binders keep their existing, different stand-ins (count-reducing at every address, no new split), or must every binder move in the
  same pass (then the family includes six L129 rows)?
- D2: accept the trio identities (destroy-now = private BlockWhileBusy, queue = public Delete<X>) against the landed 0x00BDA110 / 0x00BD9CE0
  Identity lines?
- D3: for the family pass, port 0x00BDA110-style (header) or respell in place (both graded on 0x00BDA110)?

## Flags / funclets / descriptive names / locks / slips
- Flags: none. Funclets / path includers of the drafted rows: none. Descriptive names: none.
- Locks (rows.txt, NF265): 0x00BE5C30, 0x00BE6090, 0x00BDA110 (drafts held), 0x00BEAE50, 0x00BE5C60 (witnesses), 0x00BDA150 (measurement).
  Release, or hand 0x00BDA110 / 0x00BE5C30 / 0x00BE6090 / 0x00BDA150 to the family pass.
- Order: every compile ran after its lock was NF265 and its clang precheck passed (new and HEAD sources and tests), in a later tool call,
  under an outer `timeout 900` on the launch line. Clang-only precheck failures reproduced by two-line VC7.1-header controls and graded:
  0x00BE6090's HEAD test (`<assert.h>`), 0x00BEAE50's test (`<cwchar>`). pre.sh and absrefs.py were checked live with controls.
- Reading: the chained briefs NF263 back to NF215 and NF214's read-first list (NF92 brief / lessons / notes, NF44 brief, NF90 / NF89 / NF88
  lessons, ASM_RULES, EXTRA_RULES, COORDINATION), the nf263 / nf262 / nf260 READMEs and hand-backs. GOTCHAS.md: a partial read (slip): lines
  1-217 in full, lines 360-502 by grep of their naming-era lines, lines 218-359 not read.
- Tools: NF263's tools repointed; sc.sh grades the HEAD side without the overlay (and takes HLEAF); gr.sh keeps the HEAD object; new rf2.sh
  (vland with the overlay), objsec.py (section diff with renames), eqrows.py (equiv_check row closure).
- Slips: the partial GOTCHAS read; the collision was found only after the drafts were graded (the census was address-side only).
