# h4d b10/b11 audit notes (verifier)
## Row 1 b10 0x00C77800
- hdrcheck2: all default=same (vacuous [0] because whole headers gated by PASS22); PASS22=same with real lines (7/14/37). g++ -E -P md5 identical repo vs overlay with/without PASS22 (gxxE.log).
- No FABLE_STATIC_ASSERT in the 3 headers.
- CodeView: CHLRvbParams m_fDecayTime/LF/HF ratios/RefFreqLF/HF/EchoTime all Private (cv_CHLRvbParams.txt); only public getter GetDecayTime; /Od body reads [eax+10h] etc. directly, 16 calls = expf/SetGain/Set_ShelfLfHf only (od_UpdateDecayTimes.na.txt).
- Locals: CodeView frame order -4 fDecaySamples, -8 fDecayFactorHF, -0xc i, -0x10 LF, -0x14 ratioHF, -0x18 spacing, -0x1c ratioLF, -0x20 fGain = row declaration order (README says "CodeView record order": it's the reverse of record order = ascending slot). Blocks: uiBufferSamples per loop body (-0x4c,-0x68,-0x7c), j block from 0x7af to end of i body (VC7.1 for-scope).
- Statement/operand order spot-checked: OK (fdiv then fchs; Set_ShelfLfHf pushes sps,RefHF,HF,RefLF,LF; input: shr 1, (i+1)*spacing, - MaxShort(+0x10); spacing = MaxLong(+0x14)>>2; echo fild sps * EchoTime / ds, fchs; store +0x22c).
- q (own oracle from Fable.exe, union overlay ov_final): RELOCATION_MATCH. b10 TU preprocesses identically with/without b11 friend block -> lands with b11 copy, no regrade needed (done anyway).
- Test PASS; 25/25 own mutants killed + 1 equivalent control survived (mut_b10.log).
- check_portable_spellings: clean.
## Row 2 b11
- @comp.id ReverbFns_SSE.obj = 0x00600C05 (prodid 0x0060 build 3077) = Rich entry 0x0060/3077 count 1369. PASS.
- sdkcmp: ApplyFilterOrder2_quad 681 diffs 0; caller 0x00C75CE0 = ?Process@CFloatShortReverb@@...PAMPAPAMI@Z 432 diffs 0, +0xA0 -> 0x00C88E60; its caller 0x00C6A180 = ?ProcessShortReverb@CFloatReverbDspEngine@@AAEXPAPAMI@Z 128 diffs 0, +0x76 -> 0x00C75CE0 (landed as CReverbProcessor_ProcessStage94: extra name fix). Retail E8 callers: only 0xC75D7F->C88E60, 0xC6A1F5->C75CE0.
- CFloatFilterParams public API (Dbg CodeView, 3 objs): Set_ShelfLfHf, Set_LpButterworth2, Set_HighPass, SetGain, GetGain, GetA0, GetB0; data private. No getter for A1/A2/B1/B2 -> twin needs the friend (member of CReverbFnsSSE).
- Gate: SDK member links through portable_twins_check's single-obj link line with #pragma comment(lib,"CODAApi.lib") + FABLE_VC71_LIB=<SDK lib dir>; exe holds the 681 masked bytes once (probe/linkprobe).
- /Od ApplyFilterOrder2_quad: only OutputDebugPrintf calls (no GetGain/GetB0); ASSERTs: pfDest!=0, pfDest&15==0, pfSrc!=0, pfSrc&15==0, uiSamples!=0, uiSamples&3==0.
- Release gather: c2/c6 on x87; c6 association per lane: l0,l1 (b0+b1)+b2; l2 (b1+b2)+b0 (+0x10F); l3 (b2+b0)+b1 (+0x169). Write-back lane 0..3, [0] then [1].
- Author twin bug: per-lane store before next lane's load (diverges for pfDest = pfSrc + 128). Fixed (read 4 then write 4).
- Fixed b11: src stub (toolchain_library, purity OK; lib_verify --source MATCH 4/4), twin header plain C++ only, test + rows driver. No forbidden tokens.
- Lane tool issue (narrowed): portable_twins_check passes `/I<tests dir>`, which tool_args mistranslates under wine (Z:\I\home\...); in-tree tests still find fable_test_mode.h by quoted-include lookup (test_C3DBoundingBoxSSE compiles OK), but a test OUTSIDE rebuild/tests/portable that includes it hangs cl (seen: COMPILE_FAIL timeout). Inlined the label in my test.
- dst=src+1 feedback case made data inexact (removed).
## Final (b11 fixed)
- final_b11_runs.log: vc71-asm/dual/portable PASS, gxx-portable PASS, gxx-m32 SKIPPED; vc71-row-portable OK; vc71-row-dual PASS 1002 lines.
- Retail measured (vc71-asm): PC_24 -> per-lane single model (c6 l0/l1 0x3E070B18, l2 ..12, l3 ..10; c2 0xBED815B0); PC_53 -> once-rounded (c6 0x3E070B12 all lanes, c2 0xBED815B2).
- Exes: SDK 681 masked bytes present in asm/dual test exes and the as-is driver; absent in portable builds.
- twinmut_final.log: 13 killed (incl. author-style per-lane kernel); 2 one-sided overlap edits equivalent alone; fb commute control survives.
- Friend needed (nofriend g++ access errors). lib_verify --source fixed stub MATCH 4/4.
- Extra name fix: 0x00C6A180 = ?ProcessShortReverb@CFloatReverbDspEngine@@AAEXPAPAMI@Z (128 bytes identical, +0x76 -> 0x00C75CE0), landed as CReverbProcessor::ProcessStage94.
- b11 helper header (unlanded) says "linked into Fable.exe" (wording rule); moot after fix.
