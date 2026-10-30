# Hand chunk 592 (PKCStaticQueue<SDecoderMessage>::Pop .. vector<CPreTransposedBoneMatrix>::_M_fill_insert): 9 of 10 byte-exact

9 rows are byte-exact in src/: 8 RELOCATION_MATCH and 1 MATCH (0x00C58740, which has no relocations). One row,
0x00AF2640, is parked in differ/ after 4 grades. Every src/ test prints `<addr>_TEST PASS` under g++ (tall.sh) and
VC7.1 (tall.py). Every Leaf line was checked with `objdump -t` against its object. No `__asm`, `naked`, `_emit` or
`volatile` appears in src/, test/ or differ/, and there are no retail_asm rows.

The read-only vland run (tmp/vland.log) reports WINS: 9, with no RELOC_IDENTITY and no DIFFER. Its only warnings are
RELOC_NAME, raised on catalog labels that NAME_FIXES.tsv corrects. lv/ has been deleted.

## What matched (9)
- **0x00C48E00 PKCStaticQueue<SDecoderMessage>::Pop()** (288v288, 2 grades, `/EHsc`; descriptive libpk method name).
  - Identity: slot 0 of vtable 0x012B8024, the class of the landed ctor 0x00C48F80, dtor 0x00C48DB0 and Push 0x00C49010.
    The PKCStaticQueue.h line-67 trace is the same one the landed Take rows use.
  - The record is returned by value. The callee 0x00C48770 is the +4 sub-record's copy ctor (it calls CHeapBlock's copy
    ctor 0x00C16900), so the return slot is copy-constructed, not assigned.
  - The fix (grade 1 was 228v288): SDecoderMessage needs a `__forceinline` memberwise user copy ctor. The implicit one
    was emitted out of line.
- **0x00C67AE0 CEaxContext::SetAllParameters(const EAXCONTEXTPROPERTIES*)** (288v288, 1 grade; descriptive).
  - It validates against the EAX 4.0 SDK ranges and the FXSlot0..3 / NULL GUIDs, then calls the four landed
    single-field setters in order.
  - SetDistanceFactor 0x00C645D0 has a same-TU `__declspec(noinline)` body so VC7.1 knows ecx survives the call.
- **0x00CBF6DC GiveGameInfo(const std::vector<CCharString>&)** (290v290, 3 grades). The name is proven: it is the only
  FableWin.pdb global taking that argument, and the body passes the vector's elements as GiveHeroYesNoQuestion's question.
  - Shared headers: CBaseObject, CScriptGameResourceObject(Movie)Base, CGameScriptInterface (`FABLE_RETAIL_NSCRIPT`) and
    CSuspendableProcessMicrothread (g_pScriptRunner).
  - The TU is a frame-pointer size TU: `#pragma optimize("y", off)` + `#pragma optimize("s", on)`.
  - The fix (grade 1 was 301v290): each CCharString argument must be a named local in its own block. Bare temporaries
    passed the ctor's returned `this`.
- **0x00BCDCA0 std::vector<CArray<C3DVector> >::_M_fill_insert** (308v308, 1 grade).
  - STLport 4.6 vector.c, verbatim.
  - Same-TU noinline helpers: `__uninitialized_copy` 0xBCD8A0 and `__copy_backward` 0xBCD850. `__copy_backward_ptrs`
    is inline.
- **0x00B6C700 std::vector<CLandscapeTheme>::_M_fill_insert** (309v309, 1 grade; QAE).
  - Same split as 0xBCDCA0: `__copy_backward` 0xB6B7E0 out of line, `__copy_backward_ptrs` inline.
- **0x00B7CAB0 std::vector<CPreTransposedBoneMatrix>::_M_fill_insert** (314v314, 1 grade).
  - The opposite split: `__copy_backward_ptrs` 0xB7AF60 is out of line (noinline) and `__copy_backward` is inline. This
    follows the landed 0x00BBD950.
- **0x00AB2CE0 std::vector<N3DAnimationBuilder::CSequenceInfo>::_M_insert_overflow** (310v310, 1 grade).
  - The landed 0xB6C220 body with the element swapped for the 52-byte CSequenceInfo.
- **0x00C58740 CUnprovenAudioBuffer_00C591F0::RouteShort4(const short*, const unsigned short*)** (309v309 MATCH, 1 grade;
  descriptive). It is the 16-bit sibling of the landed RouteFloat4 / RouteInt4, with per-plane gains: `>> 2` mono,
  `>> 1` stereo, unchanged for four channels.
- **0x00C5B300 CSoftwareVoice::AdvanceBuffer(CAudioSound*)** (309v309, matched on grade 2 of 4; descriptive). Its callers
  are 0x00C5BA05 and 0x00C5BC32 in slot 9 (0x00C5B7C0) of the CSoftwareVoice vtable 0x012B5C60.
  - The fix (grade 1 was 299v309): the loop-bit tests need an inline `bool IsLooping() const { return (m_flags >> 8) & 1; }`
    (`shr edx,8 ; test dl,1`). The raw expression gave `test dh,1`.
  - The bool argument stays the raw `(m_flags >> 8) & 1` (`and 0FFFFFF01h`), as in CCodaVoice::Play.
  - The first queue read is the inline `capacity ? items[tail] : 0`. The second is a call to GetCurrentEntry 0x00C35D10.
  - Grades 3-4 only respelled ReportQueueInfo as its bound `CAudioObject::` spelling (through a cast). The row still
    matched.

## Parked for Fable 5.1 (1)
- 00af2640 NParticleEngine::CPSCBase::GetCurrentPosition DIFFER(298v313): frame 0x3C+edi v 0x54; retail keeps x/y on the FPU (`fld [esp] ; fld [esp+4]`, later `fmul st(3)/st(2)`) and stores each transformed row into x/y's home slots, then builds a C3DVector temp (copy ctor) before `pos = T` | tried: float locals with an inline row-by-row transform (v1 183, v2 298), a memberwise `__forceinline` CMatrix3x4::operator* with a user C3DVector copy ctor and `pos = C3DVector(v)` (v3 234, v3b 341: got the return copy and the T temp but kept a separate r temp) | next: v2's float locals plus v3's user C3DVector copy ctor, with the rows written back into x/y from the FPU-held originals; otherwise lane gap: x87 operand/register order

## Skipped
- None.

## Label corrections
Every chunk label was a propagated or descriptive guess:
- 0x00C48E00: "EventQueue_HandleCircleEvent" is really PKCStaticQueue<SDecoderMessage>::Pop.
- 0x00C67AE0: "ScriptCommand_SetHeroProperties" is really CEaxContext::SetAllParameters.
- 0x00CBF6DC: "UI_PlayMovieWithSkipOption" is really GiveGameInfo.
- 0x00BCDCA0: "std_vector_insert_at_position_12byte" is really `_M_fill_insert`.
- 0x00B6C700: "Vector_Resize_Struct0x38" is really `_M_fill_insert`.
- 0x00C58740: "AudioFormat_DecodeWithChannelShift" is really RouteShort4.
- 0x00C5B300: "GameflowAssistance_OnNotify" is really CSoftwareVoice::AdvanceBuffer.
- 0x00AB2CE0: "Vector_Reallocate_0x34Bytes" is really `_M_insert_overflow`.
- 0x00AF2640: "Particle_AddToBoundingSphere" is really CPSCBase::GetCurrentPosition.
- 0x00B7CAB0: "Vector_InsertWithCount" is really `_M_fill_insert`.

Callee labels that are wrong are listed in NAME_FIXES.tsv:
- 0x00C48F20 (HoverHornetQueen)
- 0x00C48770 (an operator= that is really a copy ctor)
- 0x00C35D10 (CResource::GetUnusedFrames / CEntryTable_00c35d10)
- 0x00C5CE40 (CGameflowAssistanceScript::Init)
- 0x00C3C4A0 (GetAnimFilter / C_00c3c4a0::M)
- the STLport fill / uninitialized_fill_n / __copy_backward / __uninitialized_copy placeholders, including 0x00B7AF30
  (`_Copy_impl<CActionInputControl*>`)
- 0x00C64420 (CCharacter_SetActiveSkill, which is really CEaxContext::SetPrimaryFXSlotID)
- 0x00AF2250 / 0x00AF2780 / 0x00AF3080

## Names: proven vs descriptive
**Proven (two sources):**
- GiveGameInfo: FableWin.pdb, plus the body's vector<CCharString> use.
- The four STLport vector rows: the Ego_r.pdb element classes and resize/push_back callers, plus STLport 4.6 vector.c.
- CPSCBase::GetCurrentPosition: Ego_r.pdb, plus GetSystemTransform applied to a local system.

**Descriptive:**
- Libpk / GameCODA has no PDB or RTTI, so these names are descriptive: PKCStaticQueue::Pop (and OldestIndex),
  CEaxContext::SetAllParameters, RouteShort4 on CUnprovenAudioBuffer_00C591F0, CSoftwareVoice::AdvanceBuffer, and the
  callees CircularQueue::GetCurrentEntry, CAudioRingBuffer::Clear and CAudioObject::ReportQueueInfo.
- CPSCBase vtable slots 4/5 (CUnprovenConstSlot04/05) are unproven.

## Header debt
- No shared engine header exists for the value types CLandscapeTheme, CPreTransposedBoneMatrix,
  N3DAnimationBuilder::CSequenceInfo, C3DVector, CMatrix3x4, or for CPSCBase / CParticleUpdateParameters. CPSCBase is a
  class with a 32-slot vtable, so it should have one. These rows use local models taken from Ego_r layouts.
- CEngineLandscapeRenderer.h holds `LandscapeThemes` as `unsigned char[0xc]`. That is the right STLport size, but untyped.
- The libpk audio classes (CSoftwareVoice, CAudioObject/CAudioSound, CircularQueue, CAudioRingBuffer, CEaxContext) have no
  headers. Each landed row carries its own local model, and they disagree on spellings (see LESSONS "Review at landing").
