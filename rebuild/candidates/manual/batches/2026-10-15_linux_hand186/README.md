# Hand chunk 186 (CBase .. CCombatAction_GenericStrikeResponseKnockdown): 28 of 40 byte-exact

## What matched (28)
- NCameraModes::CBase::UpdateCameraOcclusion 0x0084DB30 -- line-of-sight pull-in. Shape: the observed thing as a named local before the call; the focal point as a named return copied into `from` (C3DVector user copy ctor, no operator=). Matched on the extra try.
- CBaseIntelligentPointer find 0x006E67A0 -- STLport four-way unrolled `__find` over an intelligent-pointer range.
- NEnvironmentInternal::CBlendedEnvironmentTheme::operator= 0x006B5DF0 / CThemeChannel::operator= 0x006B6180 -- compiler-generated memberwise assignment (POD colour and scaler arrays, then 3 x two CArray operator= calls). Shape: no user operator=; a dummy `a = b` caller makes VC emit it.
- CEngineEnvironment::CEngineEnvironment 0x00B8CEE0 -- wind state ctor. Shape: C3DVector with a user ctor and a user copy ctor and no operator=, so the 64-entry history loops copy through a pointer register.
- CBrainOutput::operator= 0x0088C840, CBufferedFileDataOutputStream::Write 0x00994870, CCategory HighlightNextItem/AddItem x2 (0x005BD054/0x005BF060/0x005BF5A4), CChunkedFileSuperChunk Find{First,Next}SubChunkOfType x4 (MATCH), CV_FisticuffsClubScript region reset 0x00E82010, CCombatActionBase ctor 0x00857E40, CCombatAction_GenericStrikeResponseBase copy ctor 0x006F83C0, CCombatAction_GenericStrikeResponseKnockdown ctors 0x008679C0/0x00868430 -- matched before the restart. Each source header has its shape notes.
- CBriarRose::DoOnSpeakToSpeech 0x00DA27B0 -- TEXT_QST_B08_BRIAR_ONSPEAKTO + _START/_MIDDLE/_END picked by the parent's timer. Shape: an inline `GetTime(const CTimer&)` gives the `add eax,60h ; mov eax,[eax]` read.
- CCRCSymbolMap::SaveBinary 0x00A382E0 -- memory stream to a CDiskFileWin32. Shape: CBase owns the vptr (`virtual ~CBase`), so ~CBase runs on the file's own address.
- CCharString MakeOwnCopyOfDataOrEmptyString 0x0099EB40, Format_vsnprintf 0x0099F1F0, CString::AllocBeforeWrite 0x00C0BEE0 -- matched before the restart; tests added in this session.
- CChunkCollectionIndicator::Main 0x00CFC990 -- entity-script HUD bar loop. Shape: the CCharString title as a scoped named local (a temporary argument is built after the vtable load).
- CCombatActionBase::GetWeaponTrailUsedInStrikeAction 0x00857D10 -- for_each over the strike events, then GetInterface 0x4C. Shape: the LTCG for_each (result address in ecx, callee-clean) modelled as a __thiscall member on the result functor; the functor is built in its argument slot.
- CCombatAction_FallStunned::Initialise 0x008ED950 -- AddMode(CCountedPointer<CModeBase>(new CModeDazed)). Shape: a user copy ctor on CCountedPointer makes the argument build in place; its null test folds away after the `[eax+6Ch]` store.
- CCombatAction_GenericStrikeResponseBase::FrameUpdate 0x00865C20 -- dust effect and finish-when-moving. Shape: name the creature locals before the __fastcall calls; the retail base is 0x114 bytes.

## Parked for Fable 5.1 (11)
- 00c915a0 CBitstream::ReadBits DIFFER(190v188): retail sets `or edi,-1` between the two compares, ours uses a separate `mov edi,-1 ; jmp` block | tried: if/else -1, -1 default with ||, offset local, first shift via member | next: a goto/flag form, or the value set to -1 inside the first-compare failure
- 008eda90 CCombatAction_FallStunned::CCombatAction_FallStunned DIFFER(165v169): inline CCharString==("") materialises `sete al` where retail branches straight off it | tried: inline/forceinline ==, != with swapped arms, open-coded test | next: one ternary condition with a single default arm
- 00c0e1f0 CString::Left DIFFER(210v208): dest CString in the dead count slot in retail, `sub esp,8` frame in ours | tried: Right-sibling model | next: lane gap: dead-argument-slot temporary
- 00a5b650 CCachedFile::Copy DIFFER(179v184): ebx pushed in the prologue (retail scopes it to the memmove arm); the 0x009E9900 append uses the LTCG convention | tried: direct erase, begin()/end(), CopyTrivial helper | next: lane gap: LTCG custom convention + dead-slot temporaries
- 00a5b0a0 CCachedFile::MovePosition DIFFER(232v239): ours CSEs Position+offset across the throw; retail recomputes it and keeps an 8-byte frame | tried: throw temporary, named exception, inline ThrowFileError | next: model the throw path as not noreturn, else lane gap: dead-slot temporaries
- 00a0c2b0 CCamera::PointAt DIFFER(206v208): flag test in al and the pushes before the x87 stores in retail; dl and the pushes after the branch in ours | tried: ternary ctor arg, if/else Z, ternary Z | next: lane gap: scheduling (maybe a by-value SetForward helper)
- 0099b9a0 CWideString::operator+=(wchar_t) DIFFER(212v203): retail keeps c in ebx and homes n=1 in c's dead slot; ours homes n in the frame | tried: inline append, __forceinline append, STLport length check + Max(const&) | next: lane gap: dead-argument-slot temporaries / register allocation
- 00a37060 CClassFactory::GetClassNumberFromName DIFFER(199v199): `mov esi,[ecx+4] ; add esi,10h` v `mov ecx,[ecx+4] ; lea esi,[ecx+10h]` | tried: member loop, count+info locals, guarded do-while | next: iterate a pointer at &info->Name directly
- 00761560 NShape::CClosedShapeBase::EditorDrawPreparePrimitivesForRendering DIFFER(186v186): loop copies load all three words before the stores in retail; ours interleaves | tried: user copy ctor (best), implicit copy, float-local operator= | next: lane gap: scheduling
- 00465c54 CCombatAbilityBlockDefBase::Transfer DIFFER(185v226): ebp frame with speed-style zeroing (`xor ebx`) and the saving arm first in retail; /Os `and` zeroing and inverted arms in ours | tried: optimize(s), /O2, optimize(s)+tag ctors | next: speed with `#pragma optimize("y", off)`, one vector declared before the if
- 004c9840 CThing::RemoveTC DIFFER(213v213): retail homes the discarded insert() result in the dead `type` slot (0x14 frame); ours uses a 0x18 frame | tried: base model, CTCBase +4 fix, POD result | next: lane gap: dead-argument-slot temporary

## Skipped
- 0099f2e0 CCharString::LoadFromFile -- already landed.

## Label corrections
- 0x00B8CEE0 is CEngineEnvironment::CEngineEnvironment (vtable 0x012A3C68), not CBlendedEnvironmentTheme.
- 0x006B6180 is CThemeChannel::operator= (CBlendedEnvironmentTheme without TimeOfDay).
- 0x0099B9A0 is CWideString::operator+=(wchar_t), not CCharString::InsertChar.
- 0x00C0BEE0 is the MFC-style CString::AllocBeforeWrite; 0x00C0E1F0 is CString::Left.
- 0x004C9840 is CThing::RemoveTC(long), not CBaseObject::OnPropertyChanged; 0x004CC0D0 is bitset::reset(pos), 0x004CCC80 is list::insert on g_list 0x013B8A24.
- 0x00692910 is CCreatureActionBase::GetDefinitionManager (it is the third __fastcall argument of CreateDustEffectForCreature).
- 0x00740160 is CModeDazed(CThing*); 0x0085A960 is std::for_each with CGetActiveStrikeEventThatContainsFrame (LTCG convention).
- 0x00E82010 is a CV_FisticuffsClubScript member, not ~CChunkedFile.
- Exceptions: ThrowInfo 0x01369380 = CBBBFileException (4 bytes), 0x013692F8 = CGenericException (1 byte, empty).
