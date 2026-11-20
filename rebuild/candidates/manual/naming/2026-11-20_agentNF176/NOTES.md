# NF176 notes (round 10, long-tail engine / sound DSP / navigation / misc), handed back partial at an entry boundary

Counts (from APPLIED.tsv / STATUS.tsv): 55 entries: 10 done, 13 done-already, 5 rejected, 8 blocked, 4 deferred, 15 open (not
started; each APPLIED line carries its census). Rows: 18 ported (all in rows/), 19 not-a-binder, 17 blocked (incl. the q3968 drafts and 0x0051BCD0, drafts in
work/). recheck_final.log: one vland --recheck-landed run over the 18 ported rows: 18 RELOCATION_MATCH + PASS. EH funclet 0x012119E5
(child of 0x00C14DD0) regraded separately against the new parent: work/funclet_012119e5.log, RELOCATION_MATCH PASS.

Census: work/probe.tsv (one compile of every landed REFS row of every entry except q4978/q5012/q5013/q5064/q6492/q11718/q12068;
369 row-target lines, all MATCH / RELOCATION_MATCH at HEAD), per entry in work/summ.txt (its "OK" column compares with the ENTRIES
correct_symbol column, which for q4178 and q6188 onward holds the landed spelling, not the claim: read the claim from the evidence).
Self leaves: work/selfleaf.txt. CBaseIntelligentPointer spellings (one witness each): work/cbip_00a01b50.tsv, work/cbip_00a01b90.tsv.

## Self renames (RENAMES section 1)
- 0x00A26C50 CMeshDataBank::Close (q3886 binder, outside my entries): ?CMeshDataBank_Close@@YIXPAVCMeshDataBank@@@Z -> ?Close@CMeshDataBank@@UAEXXZ
  (both PDBs; exe scan: no landed referencer).
- 0x0088FE00 CGameScriptInterface::AddLogBookEntry (q4255 binder, outside my entries): QBEXJJJJ@Z -> the Ego_r / FableWin virtual const
  ?AddLogBookEntry@CGameScriptInterface@@UBEXABVCWideString@@00W4ECategory@CHeroLogBookEntry@CHeroLogBook@@@Z (exe scan: no landed referencer).
- 0x00C5F150 CImaAdpcmCodec::CalculateError -> CalcDifference (q4181; Microsoft 2001 imaadpcm.h in the shared header + the only binder
  0x00C5FA50 already binding it). Catalog name changes. Vendor (SDK) name.
- 0x00A3A840 CCompressorZlib_Retail::DecompressStream -> CCompressorZlib::DecompressStream (q5003; both PDBs; all 4 referencers move
  in this hand-back). Catalog name changes.
None of the four self-renamed rows has EH funclets.

## Descriptive names
- 0x00C2A890 CNavigatorSwitch::SendStateAndReset (libpk, no PDB): kept as queued; 0x00C14DD0 now binds it.
- 0x01375748 GDefaultToUsingPhysicsInCutsceneAnimations (q6503): descriptive (no PDB data symbol), already the one landed spelling.
No new made-up names.

## New local models (header-less classes; landed sibling has the same shape)
- 0x00C14DD0: `struct CNavigatorSwitch { void SendStateAndReset(); }` replaces the address stand-in CFirst_00c14dd0 (no header under
  rebuild/include; the self row 0x00C2A890 and 0x00C2A7A0 model it the same way, no base).

## In-place respells of existing local models (accepted form; defined-symbol diffs checked with objdump)
- 0x00502170 (q3886): `virtual` on the existing local CBankFileAsync::Close. COORDINATOR FLAG: the defined-symbol set is unchanged, but
  the row's pick-any `??_7CWADBank@@6B@` COMDAT (a non-retail stand-in vtable already at HEAD) grows 4 -> 8 bytes (Close slot). If not
  acceptable, drop rows/00502170.cpp: q3886 is then partial with 00502170 left (full port onto engine/CWADBank.h + CBankFileAsync.h).
- 0x00547C90 (CVideoSys::Init second parameter CTexture*), 0x0071D3B0 / 0x00581B33 (static __fastcall CUserProfileManager::Get; the
  CTBaseSingleton template stand-in removed), 0x0040E510 / 0x007E3820 (ReadAsStringUntilString moved off the CStringParserReader stand-in
  onto the row's own CStringParser), 0x006FCC80 (CSymbolInfo nested, the wave-35c shape of sibling 0x006FC8E0), 0x00495890 (callee in
  namespace NResourceDirectoryNames).
- WITHDRAWN: 0x0051BCD0 (q4455). Making its same-TU __destroy_aux body the STLport template graded RELOCATION_MATCH + PASS, but the row then
  emits the self leaf of 0x00519EF0 as a pick-any COMDAT whose body (0x24 bytes, checksum 0x063ED249) differs from the self row's and
  sibling 0x0051C1F0's (0x5C, checksum 0xA7C6DD96, comdat 2 in all three): a one-definition risk. Draft kept in work/0051bcd0_blocked.cpp.

## Ports onto shared headers
- 0x00A26C50 (CMeshDataBank.h + new opt-in), 0x0083C800 / 0x0083CA90 (CPlayer.h + CInitBaseClass.h), 0x00408A70 (fable_string.h +
  CHeroLogBook.h + CPlayer.h; new pair 0x0129AAF4 ?NullChar@CCharString@@1DB, already bound by 19 rows), 0x0088FE00 (CHeroLogBook.h +
  CPlayer.h), 0x00C5F150 (CImaAdpcmCodec.h; local `typedef __int64 LONGLONG` added, a winnt.h typedef the header lacks), 0x00A3A840 /
  0x009949F0 / 0x00994A40 (CCompressorZlib.h).
- Tests: rewritten on the header only for 0x00A26C50 (links the row; fixtures for CloseMeshes / CBankFileAsync::Close). The other
  changed rows' tests are standalone behaviour copies that do not link the row; they still PASS and were left as they are (0x00C5F150's
  test only had its method respelled: the member is private, so it cannot be called through the header).

## Needs header claim
- engine/CGameScriptInterface.h:354 declares `virtual void AddLogBookEntry(const CWideString&, const CWideString&, const CWideString&, long) const`;
  both PDBs take `CHeroLogBook::CHeroLogBookEntry::ECategory`.

## Blocked / deferred (details per row in STATUS.tsv, per entry in APPLIED.tsv)
- q3968: family. Drafts work/009f0240.cpp (self, static -> `desc`), work/009f02c0.cpp, work/009f02f0.cpp all RELOCATION_MATCH on the
  PDB spelling, NOT handed back: 0x009F03B0 still binds 0x009F0240 as CodeSections::CCodeSectionManager::GetSection, and moving it needs
  its namespace stand-in removed, i.e. its own leaf renamed (PDB order suggests DeregisterSection) with its referencer 0x00CB5B90
  (binds 0x009F03B0 as ?RemoveMorph@CParticleMorphManager@@QAEXVCCharString@@@Z).
- q4261: 0x00810F60 is CIsHitEventHitByExplosion::operator() (NAME_FIXES.tsv), so the binder fix is a re-author + self rename that
  0x0094E530 (GetEvent stand-in template row) must follow.
- q4312 (partial), q4357 (authoring), q4455 (partial), q4572 (authoring), q4901 (class identity), q4920 (CRegionMap ctor/dtor family).
- q4404 deferred (owner: CAudioContext -> CSalOpenAL, 50 rows); q4978 / q5012 / q5013 deferred (2717 / 1256 referencers, census only).

## NAME_FIXES.tsv
- 0x00810F60 (CIsHitEventHitByExplosion::operator(), q4261) and 0x00F3EA50 (manifest module _global -> CTriangleStripifier2, q4572).

## Rule slips
- Queue order: q5009 was skipped (recorded open) while q5022..q6503 were recorded and q5003 was worked; q5009 is the first open entry.
- One Bash call exited non-zero (a `cat` of a lock file that did not exist, q4611); no effect.
- I wrote two shell waits that matched their own command line (pgrep -f "tools/probe.py"); killed my own waiter by PID.
