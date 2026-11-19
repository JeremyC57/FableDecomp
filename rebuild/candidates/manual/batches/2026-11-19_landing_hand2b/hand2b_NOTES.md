# hand2b NOTES (w24, 2026-11-19)
Tools: mkpayload.py <addr..> builds payload.json + oracle.tsv from src/ + test/ (payload key for flags is "flags", NOT "compiler_flags":
verify_and_land.py reads c.get("flags"); with "compiler_flags" the row was graded at default flags, 613v771).

## Matched
### 0x00B6DC40 CEngineWaterRenderer::ProcessSeaLoading -- RELOCATION_MATCH + PASS (strict vland, tmp/vland.log)
- Source: fable7's q4 (Ego_d line-table shape: empty-request block `return false`; else-if (!HaveBackgroundSea || Requested != Loaded)
  block; {smap_index} arm returns true / else false; IsFinished arm with {stream} scope returns false, else true; one final return false).
  ReleaseSeaBuffers body defined above the row (same-TU plain member, Ego_d TU order; text identical to landed 0x00B6D480).
- NO FLAGS (final): the six pointer drops are `LoadingSea.Reset(NULL)` / `LoadingSeaEntry.Reset(NULL)`. FableWin.pdb's line table
  (Engine_Water_Renderer.obj, module 953, tmp/fw_lines_water.txt) gives lines 591/592, 613/614, 644/645 at 16 bytes each (push 0 +
  member address + call; ReleaseData() would be 14). With ReleaseData() the row needed /d2inlT196 (smallest N; T<=195 = 701 bytes,
  default = 613) -- that form is superseded. No header opt-ins (existing ones only), hdr/ unused.
- Test: test/00b6dc40.cpp, fixture over the row's verbatim body; 9/9 mutants killed, rerun after the Reset change (else-if guard, request-change guard, Loaded id
  store, LoadSea call, pending return, HaveBackgroundSea clear, smap return, discard call, stream-arm release).

## Open
### 0x00F3ABF0 CRangeCompressor::CalcCompressionScript -- DIFFER(605v605) ebx/ebp swap; ~33 grades + c2prio probes
- Best faithful source differ/00f3abf0.cpp (fable7 c5 shape + catalog callee model). Byte MATCH with a non-Ego_d local:
  differ/00f3abf0_d1_match_nonEgod.cpp (`unsigned long& slot = block_buffer[n]; slot = Read...`). Draft escalation with the full
  analysis: tmp/ESCALATE_00f3abf0.md (NOT sent: under 100 grades). Lever: a candidate referenced in the fill-loop head block
  (an element-address temp live across the inline read) -> this 58 -> <=56.
- Tools: gv.sh (define-variant grade), pv.sh (c2prio summary), cl.sh (direct VC7.1 compile + objdump -r), ord.sh (call order),
  nd.py (differing-instruction count).
- Name flag: Ego_r.pdb lib_range_compressor.obj holds an out-of-line CArray<CScriptEntry>::push_back (?push_back@?$CArray@UCScriptEntry
  ...); retail 0x00F3B420 (catalog: std::vector<CScriptEntry>::push_back, landed under that name) is in this TU's range and is
  push_back-shaped -- it may really be CArray<CScriptEntry>::push_back. Coordinator decision; the row uses the catalog name.
- (later) d6 = d1 as a natural for-body (`unsigned long& value = block_buffer[n]; value = Read...;`) is RELOCATION_MATCH with catalog
  bindings (vector<unsigned long>::vector(unsigned), vector<CScriptEntry>::push_back, CalcBest, _free). NOT original: FableWin.pdb's
  own record of the row has exactly Ego_d's locals (no reference local), and VC7.1 /Od /GX sizes: base 640 (+18 for the three `for`
  lines fable7 measured = 658 = FableWin = Ego_d), d6 644. Fallback only (owner decision, like 0x00680050's include order).
- FableWin.pdb line table (llvm-pdbutil dump -l -modi=1063 FableWin.4k.pdb; tmp/fw_lines_1063.txt; FableWin's library is VC10 too,
  vc100.pdb) pins the row: 728 `long count = 0;` (7), 730 if (6), 732 for (32), 734 count += (66), 735 `}` (2), 736 return count (8),
  739 if ==3 (6), 742 return A+B (64), 745 block_buffer(n) (19), 747 for (32), 749 store (39 = VC7.1 /Od of the plain
  `block_buffer[n] = ReadVarSizedUInt(block_ptr + n * element_size, block_size);`), 750 `}`, 752 script_entry (8), 753 count1 (23),
  755 if ==1, 757 push_back (12), 758 return count1, 761 sub_script (12), 763 count2 = (36), 764 count2 += (47), 766 if (8), 768/769,
  772 for (37), 774 push_back(sub_script[n]) (21), 775 `}`, 776 return count2. ReadVarSizedUInt (lines 322-336): if (==4) return (15) /
  else if (==2) return (11) / line 331 `else` (5-byte skip jmp) / 333 ASSERT (274) / 334 return *buffer (6). lib_array.hpp: CArray
  operator[] is ONE 719-byte line (asserts on one line), push_back lines 36-41 (189 bytes, real Lionhead body).
- c2prio probes with no effect (this 58): else-chain body (s2), sizeof() constants (s1), assert at top / ASSERT_ARM (a1, a2), header
  diagnostic materialising the reference inside operator[] (probe/hdrdiag, diagnostic only).
- REJECTED (coordinator, 2026-11-19): the d6 `value`-local match is a steering declaration (local in neither debug build).
  Kept only as differ/00f3abf0_value_local_REJECTED.cpp; src/00f3abf0.cpp removed. Row OPEN at differ/00f3abf0.cpp.
- FableWin instruction reading (FableWin.exe 0x03319190, lines from module 1063):
  * all three loops are `if (0) {} else for (...)` (xor ecx,ecx; je init; jmp exit) -- graded: no effect (folded early).
  * line 742: `CCS(script, block_ptr, .., 2) + CCS(script, block_ptr + 2, .., 1)` (left call first at /Od) -- adopted; same bytes.
  * ReadVarSizedUInt (lines 323..334): if 4 return / else if 2 return / else { ASSERT(byte_count == 1); return *buffer; } -- adopted; same bytes.
  * CArray<unsigned long>::operator[] = ASSERT(index < size()) + vector::operator[](index) (= INDEX_ASSERT) -- graded, alone and
    with STLP INDEX_START / ORIGINAL_FRONT: no effect. CArray(unsigned n) -> vector(n); CScriptEntry() stores Flags only (header ok).
  * frame slots: count, block_buffer, script_entry, count1, sub_script, count2, n x3 + /Od ternary/return temps -- no other local.
  * push_back callee is CArray<CScriptEntry>::push_back (second source after Ego_r for 0x00F3B420's name).
- `long left` counter (Codex shape, retail's ebx = block_size then sub 4): fixes the registers but its copy is scheduled before the
  loop-entry jle (2 diffs at 0x00F3AC0A); for-init form same; body-local `left = block_size - n` = swap. Retail's ebx is the SR temp.
- Also measured: QFE-4035 compiler (same swap), TU flags /Ox /G5 /G7 /GB /arch:SSE(2) /Ob1 /Oi- /Op /GX /Gy- /GF- (none).

### 0x00AE3C30 CParticleProgram::ReadFromFile -- DIFFER(2278v2310) (r4, faithful); draft family 2310v2310 6-8 instrs (non-original local)
- probe/ae/r4.cpp = fable7 r4 rebuilt (dead-arm ASSERT(version == 0x14d), GetByteSize x6, GetVertexPositionCount, GETASCARRAY_START,
  no `data` local). FableWin.pdb line table (module 1139, tmp/fw_lines_ps.txt, lines 953..1011) confirms every statement size of r4
  (954 version 11, 955 ASSERT 277, each array read 38, 977 NonSimCount 17, 978 if(!AllocateArrays(GetVertexPositionCount())) 28,
  982 ReadVectorArray 33, 986 texcoord read 38, 1000 for 32, 1002 name 24, 1003 index 11, 1004 Insert 22, 1006 Update 14). FableWin
  locals = Ego_d's (no `data`).
- Residue: c2 split of `this` (r4 -298: pieces ebx #46, esi #8, edi #7; draft -296: esi, edi, memory). First divergence 0x00AE4071:
  retail gives the no_texcoord ReadULONG value esi and reloads `this` at 0x00AE4096; ours keeps `this` in esi; later `this` takes an
  ebx piece so const 0 loses ebx (2278).
- Tried (no effect / worse): Ego_d &front() GetAsCArray (r5), named array reference k1/k2 (2270), data = GetAsCArray() k3/k5/k6
  (2278), named byte count k4 (2286), visible noinline resize specialisations k10 (2278), QFE compiler (2278). Draft family:
  k7 (`data = &ITC; if (size() > 0) data = begin();`) 2310v2310 with 8 differing instructions (select order only) -- non-original.
- MATCHED (k11..k15): faithful r4 + the EXISTING shared opt-in FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE (CDataInputStream::Read
  with FableWin's locals dst / no_bytes / overflow_check, whole body under `if (no_bytes > 0)`) -> RELOCATION_MATCH. GETASCARRAY_START
  and the version ASSERT dead arm are not needed (graded both ways): src/00ae3c30.cpp keeps the Ego_d &front() GetAsCArray (default)
  and no assert. Test: Codex fixture over the verbatim body (+GetByteSize, GetVertexPositionCount), 66 cases, 10/10 mutants killed.
  Strict vland RELOCATION_MATCH PASS, no relocation warnings. No new header opt-ins.

### 0x005AC10E CTCInventoryTrade::ConstructActualBuyInventoryPC -- DIFFER(3532v3532), 2 instructions -- ESCALATED
- differ/005ac10e.cpp (Codex draft) reproduces hand1's 2-instruction residue at 0x005ACB58. Swept singly: 49 ORIGINAL_* opt-ins and
  131 other callee-model opt-ins of the included headers (tmp/5a_orig.txt, tmp/5a_cand2.txt, results tmp/5a_sweep2.txt): no change.
  Escalation: tmp/ESCALATE_005ac10e.md.

- 00b6dc40 /Od measurement (VC7.1 cl /Od /GX, probe/b6): Reset(NULL) build 827 bytes, ReleaseData() build 815 bytes
  -> 12 bytes over 6 sites = 2 bytes per drop, so Reset(NULL) is 16 per line vs 14, matching FableWin 591/592, 613/614, 644/645.
  Reset(NULL) is inferred from these sizes plus the default-flag match. Line 586 (IsEmpty, 18 bytes) is not independently verified.
