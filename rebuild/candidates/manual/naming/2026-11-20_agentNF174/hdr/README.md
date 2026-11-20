# NF174 header overlay (FABLE_HDR_OVERLAY=$S/w22/agentNF174/hdr)

Every header here is a copy of the repo working-tree header (base copy in ../orig/, byte-equal to the repo at copy time) with
`#ifdef` opt-ins only. Without the opt-in macros each header is token-identical to its base (diff shows only added #ifdef blocks).
pin_asserts.py was run after every edit.

## engine/CV_ArcheryCompetitionScript.h (base: rebuild/include/engine/CV_ArcheryCompetitionScript.h)
- Added `FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_CTOR` right after the class head: declaration of
  `CV_ArcheryCompetitionScript(CScriptDataBase*, CGameScriptInterfaceBase&)` (Ego_r / FableWin ??0...@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, 0x00E32350). Used by rows/00e32350.cpp.
- pin_asserts: pinned 0. Both asserts were already FABLE_STATIC_ASSERT_AT(150 / 151): the opt-in adds 5 lines above them, but the
  _AT form keeps the typedef names unchanged.
- Header users: 12 landed includers (grep -rlE 'CV_ArcheryCompetitionScript\.h' rebuild/src/compiled); none defines the new macro, so
  the opt-in changes none of them.
