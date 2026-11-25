# NF454: best-guess registry pass (B1) over BEST_GUESS_v3_pass.tsv (3,672 lines)

Registry-only. No repo writes, no compiles. Every input line is in exactly one of ACCEPT / REJECT / FOUND_REAL.

## Counts (after review fixes, post.py)

| place | ACCEPT | ACCEPT_PROVISIONAL | REJECT | FOUND_REAL | total |
|---|---|---|---|---|---|
| libpk 0xC08AE0-0xCA0000 | 746 | 0 | 168 | 54 | 968 |
| data >= 0x1200000 | 756 | 0 | 138 | 96 | 990 |
| game | 366 | 115 | 490 | 447 | 1,418 |
| 0xCA0000-0xCD0000 | 3 | 0 | 62 | 231 | 296 |
| **all** | **1,871** | **115** | **858** | **828** | **3,672** |

descriptive_check --extra: ACCEPT.tsv 2,061 lines (190 registry + 1,871) 0 FAIL; ACCEPT_PROVISIONAL (4 columns, prov4.tsv) 0 FAIL;
ACCEPT + FOUND_REGISTRABLE (222) + provisional together (all_reg.tsv, 2,398 lines) 0 FAIL.

Game ACCEPT is tiered: ACCEPT.tsv keeps game lines whose class is known to a PDB name set or RTTI (only the member name is a guess) and GF* free functions; ACCEPT_PROVISIONAL.tsv (115) has game lines whose class/scope is in no PDB or RTTI name (the class itself may be an invented stand-in, e.g. CRecordWithTwoLists, CSortedRecords) and other free functions. Register the provisional file only after a per-class look.

## Files
- ACCEPT.tsv: 4-column registry lines (basis kept from the input: `style` for libpk/data/vendor, `one-source:caller-binding` for game).
- REJECT.tsv: address, symbol, reason, proposed_name, place. `needs_vendor_source` lines are here (190).
- RENAME_PROPOSALS.tsv: the 119 rejects with a proposed spelling (101 `??_G<Class>` for ScalarDeletingDestructor stand-ins, 11 data names without the address suffix, CMatrix3x4 RotateB/A/C -> RotateX/RotateY/RotateZ from the bodies, 0xC62350 -> RandomFloat, 0xC3C380 -> AudioAlloc, 0xC889F0 -> F32vec4::operator+=, the SRegionLookup* ctors).
- FOUND_REAL.tsv: address, landed_symbol, real_symbol, source, action, place.
- FOUND_REGISTRABLE.tsv: the 222 FOUND_REAL lines whose landed spelling already IS the proven name, in registry format (basis `one-source:vendor-object` = the lib_verify section symbol at high confidence with reloc_bad 0, or named by an ok/sec relocation of a vendor section; `[?]`-only and `[bad]` relocations are not counted; `one-source:egor-pdb-initializer` = 31 REACTION_MANAGER_DEF_INSTANTIATION_NAME). Not mixed into ACCEPT because they are not guesses; the coordinator decides whether the registry or the tracker should count them as proven.

## Sources checked per line (screen.py / screen2.py)
Ego_r, FableWin, Ego_d carved name sets (exact symbol and same `?name@scope@`), FSE manifest function names, the retail Fable.exe strings (ASCII + UTF-16, incl. RTTI `.?AV` names and the libpk `__FILE__` paths), vendor_signatures.tsv / library_signatures.tsv by address, and the relocation-target names inside those files (relocnames.py: 1,172 addresses named by vendor objects). No GameCODA SDK symbol list exists in the repo (only nf430 coda_match.tsv, RTTI matches), so that source could not be applied.

## Patterns accepted wholesale
- libpk (756): every class with readable audio-domain names (CEAXListener 53, CAudioContext 45, CString 38 (MFC6 member names), CEAXBuffer 32, CAudioSource, CHLRvbParams, CMixBuffer, CSoundEvent, CImaAdpcmCodec, the CFloat* reverb DSP classes, PKC* containers, CSal*, mixer free functions MixVoice*/Resample*/ScaleAccumulate16*...). Owner rule 2026-11-04 / 11-12: light descriptive names for hand-reconstructed vendor code. basis `style`.
- data (767): Lionhead-style globals and statics (g_b*/g_p*/s_*/k*, UPPER_CASE constants in anonymous namespaces, function-local statics, `$S<n>` guard variables, D3D vertex declarations, EAX/DirectSound GUID names, CPU / Windows-version flags).
- game (482): members of RTTI/PDB classes with descriptive verbs (CSoundManagerLH, CSoundEmitterLH, CASoundManager, CTC* FinalCleanup/OnCreate, CMode* ctors, CPersistContext::Transfer* templates, STLport-spelled template instances), GF* free functions, WordWrap_*, anonymous-namespace helpers.

## Reject patterns
- game: free STL helper stand-ins (Dest_val*, Cons_val*, __median, StdList_*, RBTree_GetMaxNode_2..7 ...) 135; `ScalarDeletingDestructor` stand-ins 101 (proposal `??_G`); free `Class_Method` stand-ins 66; placeholder types in the signature (Obj, Payload, Node, SymPair ...) 28; address-named classes/namespaces (C00xxxxxx, N00xxxxxx) 15; RetailUI / CRetail* / size-numbered stand-in models; vfunc_N / _0x.. / _slotNN / _A suffixes.
- libpk: size-suffixed classes CLibpkObject78 / CLibpkObject1D8 / CLibpkStateA4 (offset setters) 26; Fable-game labels propagated onto libpk bodies 15+ (SetLungedThisFrame, SetDebugCameraType, GetWorldMapNumber, Enable2DSceneRenderingOnAcceptingObject, CEngine2::ChangeCameraAngle, CCreatureAction_SummonerFlameSlice, CPixelShaderData, CIsThingAlive, CQuadric ...); Row00c41c40 / Row00c6e* / C00c* scopes; letter-suffix siblings (SetPositionB/C, CreateHardwareVoiceA/B, DtorMsgEventList_B/C).
- data: 34 EH-funclet addresses at 0x0121xxxx carrying a function name (the name belongs to the parent, not the funclet); vague names (g_FeatureA..G, g_DetailA..D, g_flag1/2, g_slotStateA/B, aa, bb, g_const, g_someFactor ...) 33; address-suffixed names 11.
- needs_vendor_source (190): vorbisfile.c / Intel ivec.h names in libpk (73), ImeUi / strmbase / vorbis / zlib / CRT data not named by any vendor relocation (60), and strmbase / libvorbis / ImeUi code in 0xCA0000-0xCB5000 without a lib_verify section (57). Most look like the real vendor names already, but nothing on this machine proves them (no FABLE_THIRDPARTY tree).

## FOUND_REAL highlights
- 443 game + 1 vendor: the PDB has the same `?name@scope@` with another signature (dtor QAE vs UAE, ctor pointer vs reference, const). These are C1 candidates, not guesses; registering the landed spelling would contradict a PDB name.
- 28 libpk + 5 vendor libvorbis/libogg sections where lib_verify proves a different decoration (e.g. `__vds_shared_init@12` -> `@_vds_shared_init@12`, `/Gr` fastcall): respells.
- 25 strmbase sections and 19 data lines spelled differently from their vendor object (`?lbase@?1??inflate_table@@9@4QBGB` -> `...@@9@9`, `?crc_lookup@@3QBIB` -> `_crc_lookup`, `?g_zero@@3MB` / `?real@...` / `_kMaxPitch` are compiler float literals `__real@xxxxxxxx`).
- 10 game D3DX import lines differ from d3dx9_25.lib names (library not on disk).

## Needs a ruling
1. FOUND_REGISTRABLE (291): register as `one-source:vendor-object` / `one-source:egor-pdb-initializer`, or have the tracker count vendor-proven spellings as proven?
2. The 443 PDB same-name-other-signature lines: route C1 (respell) rather than the registry. Is that agreed?
3. needs_vendor_source (190): a pass with the vendor trees (Xiph 1.0.1/1.1 vorbisfile, DXUT ImeUi.cpp, Intel ivec.h) would likely move most to FOUND_REAL.
4. The 34 funclet addresses at 0x0121xxxx: the tracker reports them agree_unverified with a function's name; the tracker or the binding source may need a funclet exclusion.

## Review fixes (advisor pass)
- 11 data + 1 game (md5_process) lines whose name+scope exists in a PDB set with another signature/decoration moved to FOUND_REAL as C1 candidates.
- ACCEPT notes no longer claim absence where a flag exists: FSE-manifest hits (EndCutFade, StopSound, IsSoundPlaying, GetPos, IsNull) and bare-word retail strings (Create, Release, EAXSet ...) are named in the note.
- FOUND_REAL carries the signature confidence / reloc_bad (or relocation status); a respell is unqualified only at high confidence with reloc_bad 0 or an ok/sec relocation, otherwise it is labelled 'candidate'. PDB same-name lines other than destructors are labelled 'candidate' (ctors/operators/templates may be separate overloads).
- 10 d3dx9_25.lib import-thunk lines moved to REJECT (toolchain_library thunks get no naming work).
- Offset-named setters in libpk (CListenerState::SetFloat30/34/38/SetInteger44/6C, CSoundEvent::SetParam70, CCodaPlayback::SetSourceProperty20/FC) and the libpk navigator family (CNavigatorStateMessage, CNavigatorSwitch, decided with fill_ENavigatorType) moved to REJECT.

## Resume point
All 3,672 lines are decided; nothing is left unread. Rules are mostly scripted: screen.py -> screen2.py -> datause.py -> relocnames.py -> decide_libpk.py / decide_data.py / decide_game.py {game,vendor} -> post.py. Not saved as files: three inline libpk steps (ivec/letter-suffix moves, reloc/needs_vendor_source reclassification) and the first TSV writer; post.py reads the TSVs that writer produced, so a clean re-run must redo those inline steps. Spot checks were done by sample (dmsample.py), not line by line: the game ACCEPT set is the least reviewed (482; three samples of 30-45 lines, each round tightened the rules).
