# chunk 824 header overlay

All three files are fresh copies of rebuild/include/engine/<file> (diffed against the repo just before hand-back) with ONE opt-in
`#ifdef` block each; the default path is textually unchanged, no existing line reflowed, so no landed includer is affected.

| header | opt-in macro | origin | used by | evidence |
|---|---|---|---|---|
| engine/CV_TempleOfLightScript.h | FABLE_RETAIL_CV_TEMPLEOFLIGHTSCRIPT_ACOLYTEGOOD | chunk 811 (re-applied verbatim, not yet in the repo) | src/00edd170.cpp (RELOCATION_MATCH) | CTOL_AcolyteGood layout from Ego_r.pdb (ParentClass& +0x14, PMasterData +0x18, CTimer +0x1c, 0x20 bytes), entity vtable 0x012F3E94 |
| engine/CV_ChapelOfEvilScript.h | FABLE_RETAIL_CV_CHAPELOFEVILSCRIPT_ACOLYTEEVIL | chunk 814 (re-applied verbatim, not yet in the repo) | differ/00e6bc50.cpp (parked) | CCOE_AcolyteEvil layout from Ego_r.pdb (ParentClass* +0x14, PMasterData +0x18, CTimer +0x1c), entity vtable 0x012E8468 |
| engine/CTCCreatureOpinionOfHero.h | FABLE_RETAIL_COPINIONTRANSIENTOFFSET_BINARY | NEW (chunk 824) | differ/00684610.cpp (parked, 4 lines) | Ego_r.pdb / FableWin.pdb ?TransferBinaryIn@COpinionTransientOffset@@QAEXAAVCMemoryDataInputStream@@@Z (ILT thunk 0x004793F8); a `class CMemoryDataInputStream;` forward declaration inside the same opt-in |

The new block declares `void TransferBinaryIn(CMemoryDataInputStream& stream);` in COpinionTransientOffset so the row's same-TU body of
Transfer<COpinionTransientOffset> (the landed 0x00685F80 switch) can spell its binary-load arm with the PDB signature.
