# Naming pass wave 21 (2026-11-01): 165 landed rows respelled, all byte-identical (165/165 PASS: 2 MATCH + 163 RELOCATION_MATCH)

Inputs: brief naming/briefs/BRIEF_W21.md, QUEUE_W21_input.tsv (26 rows: 520 1, w20 decisions 5, c689 20), DEFERRED_W20_INPUT.tsv (456).
Index: idx/full.jsonl = HEAD (chunk 689 landed; its 8 new rows indexed in idx/delta689.jsonl and merged), idx/after.jsonl = HEAD + ov_final.
Commands (from $S/naming/w21, lane env sourced): `python3 scripts/compose21.py ov_final` (every stage on fresh repo copies; writes
runs/<fam>.rows.txt and ov_<fam>/), `scripts/ovrows.py ov_final > runs/final.rows.txt`, `scripts/mkindex.py idx/ovfinal.jsonl ov_final runs/final.rows.txt`
(merge into idx/after.jsonl), `bash scripts/recheck.sh final ov_final $PWD/runs/final.rows.txt 3`, then qscan21 -> mkdef21 -> mkq21 ->
mkbind21 (runs/rk_final.pairs.jsonl idx/after.jsonl runs/final.rows.txt) -> mkren21 idx/after.jsonl -> mklabels21 -> fixout21.

Families (commit order; runs/<fam>.rows.txt, cumulative ov_<fam>/):
- csound 12: CAudioOwner -> CSoundSystem (vtable 0x012B1E24), CSoundSystemSetup record, factory 0x00C096E0 = __stdcall CreateSoundSystem_00c096e0.
- voice 17: CPKVoiceParams (ctor 0x00C10A30, setters SetMinDistance/SetMaxDistance/SetScale 0x00C20480/0x00C204A0/0x00C20420, Load 0x00C27C10,
  SetFilter_00c20440 one name), Play 0x00C13140 = CSoundEventManager::Play (the CPKVoiceTable object), loop rows one class CSoundLoop_00c377f0,
  CPKEmitter -> CAmbientEmitter in 0x00C44D30 ClearEvents (its Release callee follows the respelled own row 0x00C61FB0).
- dbg 11: the 0x00C29FC0 callers' stand-ins -> PKCSemaphore (the own row); 0x00C355E0 stream -> CString / CDebugBreak (own rows' Leaf).
- alloc 7: StoreSound*Allocator own rows 0x00C16690/6B0/6D0 re-authored; CDefaultAllocator_00c166f0 / g_DefaultAllocator_0143bb34 (one class key).
- misc 2: 0x00CBA8E0 operator< comparator text; 0x00C62920 const unsigned short* name (wave 20 DEFERRED).
- gdst 110: ?GDefStringTable@@3VCDefStringTable@@A in every TU but 0x005DCF70 / 0x008AF850 / 0x00890F40 (DEFERRED); PDB CDefString::SetString
  0x009D5060, ??BCDefString 0x00415D70, GetStringAsCharArray 0x009AE6B0; 0x0054ED90 CCharString global; three tests respelled.
- c689 6: own rows 0x00613AE0 / 0x006196D0 (STLport _M_erase, 0x00834FD0 text), 0x00619F50 / 0x00D3AFF0 (??_G via construction helper),
  0x00AB0FC0 (__uninitialized_copy from the 0x00AB3300 text), 0x00AD30C0 (std::vector names).
HELD: setcopy (scripts/hand21/h21_setcopy.py, not in STEPS) -- DECISIONS_W21 item 1.
Outputs: BINDINGS.tsv (102 RETIRE / 47 ADD / 1 KEEP, 0 CONFLICT; bindings_multi_check 0 on runs/reloc_bindings.after.tsv), LABELS.tsv 34,
RENAMES.tsv 31, QUEUE_W21.tsv 26 (respelled 7, partial 3, already-correct 10, deferred 5, hold 1), DEFERRED.tsv 454, DECISIONS_W21.tsv 5.
