# Chunk 592 (2026-10-30): 9 of 10 engine-range released pass 9 rows landed

8 RELOCATION_MATCH + 1 MATCH, all PASS (g++ and VC7.1). Proven: GiveGameInfo (FableWin), four STLport vector rows
(CArray<C3DVector> / CLandscapeTheme / CPreTransposedBoneMatrix _M_fill_insert, CSequenceInfo _M_insert_overflow).
Descriptive (libpk / GameCODA): PKCStaticQueue<SDecoderMessage>::Pop (/EHsc), CEaxContext::SetAllParameters,
CUnprovenAudioBuffer_00C591F0::RouteShort4 (MATCH), CSoftwareVoice::AdvanceBuffer. Every chunk label was wrong.
Parked (../../pending/2026-10-27_linux_hand592/differ/): 0x00AF2640 NParticleEngine::CPSCBase::GetCurrentPosition (298v313):
retail keeps x/y on the FPU and stores transformed rows back into their own slots, then a C3DVector temporary via the copy
ctor; next: v2's float locals + v3's copy ctor writing rows back into x/y; else an x87 slot-reuse lane gap. Carries a full
named 32-slot CPSCBase vtable model (no header for CPSCBase, a vtable class).
NAME_FIXES.tsv: 20 rows (nine STLport helper placeholders -> exact std:: names; 0x00B6C700 QAE not IAE; libpk audio
misnames incl. 0x00C5CE40 landed as CGameflowAssistanceScript::Init, 0x00C64420 as CCharacter_SetActiveSkill; particle
bindings) + 3 coordinator rows (CEaxContext/CEAXContext, CAudioSound/CAudioObject one object two names, 0x00C54BD0 misnamed).
Header gaps: CLandscapeTheme, CPreTransposedBoneMatrix, CSequenceInfo, C3DVector, CMatrix3x4, CPSCBase, CParticleUpdateParameters;
CEngineLandscapeRenderer.h declares LandscapeThemes as untyped unsigned char[0xc].
