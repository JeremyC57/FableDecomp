# Landing nf165 (Account A, 2026-11-20): sound / stream names (19 rows)

Agent NF165 (round 10) handed back 21 entries (13 done, 1 done-already, 1 rejected, 6 blocked); its files are in
`naming/2026-11-20_agentNF165/`. `headdiff_HEAD_vs_final.txt` compares each row's HEAD object with the final one: the
self-renamed rows change only their own leaf, and the referenced symbols change exactly by the section-2 pairs.

- 19 rows. Self renames (7): CSoundPlayParameters::GetStreamed 0x00C1BAE0 and its ctor 0x00C1BD10, CPKVoiceParams::GetTrackBit0/1/2
  (0x00C27B70 / 80 / 90), and the free functions libpk/CStreamBlockReader.h already declares, GetStreamBlockOffset / GetStreamBlockTail
  (0x00C1E970 / 0x00C1E990). There is no PDB name for these; the header, the bodies and binder 0x00C1E9F0 agree. Section 2 has 16 caller
  respellings.
- Headers (add-only, `orig` equals HEAD): engine/CEAXListener.h (REMOVESOURCE) and libpk/CPKVoiceParams.h (TRACKBITS: TrackFlags at
  +0x44 and GetTrackBit0/1/2; its two POSITION asserts are line-pinned). Five dependency headers were copied unchanged.
- Coordinator calls:
  - 0x00665F90 reaches SetVoiceName through `(CTCSoundPlayer*)GetTC(0x10)` on its row-local model. This is the downcast retail
    code makes on GetTC's result, and it already existed at HEAD with one more cast, which this hand-back removes. Accepted.
  - The 0x00C52840 / 0x00C44170 tests take a CEAXListener* from a static buffer (test scaffolding only).
  - CSoundPlayParameters is a row-local class in 0x00C1BAE0 / 0x00C1BD10, the same shape as landed sibling 0x00C1BC30. No header
    exists for it anywhere and it has no base, so the shared-header rule does not apply.
  - q5121 rejected: the SDK names 0x00C5xxxx's body CSalSw2DVoice::CalcParams (858 bytes, as retail); catalog fix in NAME_FIXES.
- Recheck (`logs/recheck.log`): 5 MATCH + 14 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0
  created; includers 103 good, 0 bad. tablecheck: no address gains a spelling; 0x00997620, 0x00C1BC90, 0x00C5FF60, 0x00999230 and
  0x00C68F90 shrink.
- Link (`logs/link_landing.log`): 1 LNK2005 (~CDiskFileWin32), the same at HEAD (`logs/link_control_head.log`).
- Queued: the CSalSwBaseVoice family rename (q4779 / q4781, from the GameCODA SDK), the CEAXListener header renames, the
  CStreamBuffer / CSoundEventManager members (q4897-q4899), the 0x00C1BC90 setters (q5175), and 13 SDK name findings (NAME_FIXES).
