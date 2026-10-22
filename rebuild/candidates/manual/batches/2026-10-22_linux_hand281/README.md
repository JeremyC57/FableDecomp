# Hand chunk 281 (CBSGateGuard .. CCombatActionBase): 24 of 40 byte-exact

All 24 matched rows grade RELOCATION_MATCH under RTM cl 13.10.3077. Each has a behaviour test that passes under g++ (`tall.sh`) and VC7.1 (`tall.py`).

## What matched (24)
- **CBSGateGuard::Main 0x00CDFB00** -- the gate guard's entity-script loop; the shared CScriptThing / filter models fit on the first pass.
- **CBalvMarker::Main 0x00EFEB60** -- uses the inline-once destructor idiom: dtor-less local views, plus a `__forceinline` Destroy at the first return. Retail inlines the handle dtor only once.
- **CBanditAttacker::Main 0x00D1F610** -- the filter member's ScriptThingCopy is out of line, and the CScriptThing inline dtor is emitted once.
- **CBanditFirstWave::Main 0x00D1FA00** -- an inline CScriptThing copy ctor goes through the counted-pointer member's copy ctor, which gives retail's `[esi+0xc]` addressing.
- **CBankFile::ReadRecoveryFile 0x009D0380 (/GS)** -- written as an outer `if (PathExists) {...} return false;`. The signature, value and entry-word reads sit in separate `{}` blocks, so the two trailing words reuse the signature's frame slot and the dead `validation` argument slot.
- **CBankFile::UpdateEntry 0x009D1320** -- landed by advisor AdvB3 from my ue2 probe, with three changes:
  - `unsigned long aligned = Align(dataSize);` named before `WriteEntryData(GetEntryDataOffset(index), ...)`;
  - the CArray<char>(n, v) fill goes through a local start pointer (uninitialized_fill_n);
  - the CBankFileEntryUpdateData model gets pad bytes +0x11..+0x13.
- **CBankFileAsync::~CBankFileAsync 0x009D6490** -- matched on the first try from the landed ctor/disposal models (label fixed).
- **CQ_EndGameScript::CheckRegionLeaving 0x00D27950** -- `? true : false` produces the setne (label fixed).
- **CBordelloEntrance::Main 0x00E3B4A0** -- standard entity-script model.
- **CBountyHuntActivationChecker::Main 0x00E7F410** -- standard entity-script model.
- **CBountyHuntActivationChecker::RespawnFishermen 0x00E7F7F0** -- the thing accessor is inline everywhere, and named locals in the if-block share the temp slots.
- **CBowerstoneIntroGuard::Main 0x00D246C0** -- three things were needed:
  - inline-once views;
  - a `__forceinline` movie ctor (VC7.1 otherwise calls it out of line);
  - an explicit `__forceinline` handle `operator=` (base first, then the member's out-of-line PImp op= 0x008AB1E0).
- **CBriarRose::UpdateTarget 0x00DA2A20** (CQ_MinionCampScript) -- the implicit `const char*` -> CCharString conversion builds the temporary early.
- **CCOE_Victim::Main 0x00E6B400** -- three inline vector dtors; an inline `operator!=` wrapper fixes eax/edx.
- **CCS_OakValeRevisitedScript::OakValeFire 0x00EE8870** -- a `__forceinline` CScriptThing `operator=`; the index loop uses `*(begin()+i)`.
- **CCS_PlayCutsceneScript::Main 0x00F01480** -- inline STLport map ctor/dtor (malloc'd 0x24 head node, erase 0x00CD344A, free). The CCountedPointer `operator=` reads `other.Info` before `other.Data`.
- **CCameraModeDef::Transfer 0x00456564** -- needs `#pragma optimize("s")`, as in the landed Transfer siblings.
- **CCham::Main 0x00F14770** -- arena model; the handle dtor is inlined once, at the first return.
- **NUISystem::CScrollableList::Initialise 0x00555EB0** -- built from the landed CRedefinerList::Initialise 0x005566A0 model, with CAction (0x10 bytes) and a CActionParamLong (vtable 0x012366F4, type 3). The inline AddUIActionParam list insert must name the old tail before storing the links (label fixed).
- **CChunkedFileLoader::LoadChunkedFile 0x00AA79C0** -- four parts to the match:
  - CChunkedFileException is an empty class with a user-declared empty ctor, so the thrown copy reads the uninitialised byte from the dead stream slot;
  - the id compares are 4-byte intrinsic memcmp;
  - the version and root size are read through an inline `ReadULong()`, so they land in the dead argument slot;
  - the root-type check uses a temporary counted-pointer view with an inline release. The root call uses an implicit (memberwise) copy plus a manual reference bump, with the chunk pointer taken before the bump.
- **CCombatActionBase::UpdateFacingDirection 0x00856B20** -- an inline GetPhysics accessor, the float fabs overload, and the target==creature compare order.
- **CCombatActionBase::ProcessPossibleCreatureStrikeEvent 0x00858130** -- assigning the tip pointer between the first two push_backs keeps it in ebp.
- **CCombatActionBase::ProcessPossibleShotStrikeEvent 0x00858590** -- an `extern const` epsilon keeps `eps*eps` unfolded.
- **CCombatActionBase::ProcessPossibleObjectStrikeEvent 0x008587B0** -- matched on the first try from the Shot model: four finite-line tries as an `if (!hit)` chain.

## Parked for Fable 5.1 (16)
- 00d90290 CBanditCommentMarker::Main DIFFER(711v702): retail keeps 0 in ebp (`push ebp` for 0.0f/false, `cmp eax,ebp` in the inline dtor) and orders the string-arg/vtable loads differently | tried: named/unnamed string temps, inline-once views, filter forms | next: lane gap: constant-zero register caching
- 009ccef0 CBankFile::FindIndicesByFilename DIFFER(620v592): retail keeps `this` in ebp and the node index in edi, and homes temporaries in dead argument slots | tried: map find returning node*, LowerBound stand-in, named iterators | next: lane gap: register allocation + dead-arg-slot temporaries
- 009ce370 CBankFile::WriteFileInformation DIFFER(622v620): the strength-reduced RuntimeData[i] offset temp sits at [esp+0x14] (retail: [esp+0x28], above the stream, fresh `xor eax,eax`), and the loop tail caches UpdateData.begin in ebx | tried: my 5 forms plus AdvB3's 5 (named const&, direct m_pFirst[i] indexing, nested ifs, while loop) | next: lane gap: optimiser-created preheader temporary slot
- 009ce5e0 CBankFile::AddEntryMultipleFiles DIFFER(726v741): ebx/ebp swap (retail: symbol in ebx, index in ebp); VC7.1 tail-merges the two InitEntry calls that retail keeps as two copies | tried: node* map find, separate exits written out, if/else arms with full calls | next: break the tail merge with distinct argument expressions per arm
- 0084d910 CBase::UpdateCameraStretchAroundHero DIFFER(525v542): retail evaluates GetStretchHeight before the radial scale and keeps it on the x87 stack until the final Z add; the lane hoists the radial scale and the X/Y sums above the virtual call | tried: three float locals, C2DVector flat, inline scale, `position.Z += height`, rebuilt position | next: lane gap: x87 scheduling around a virtual call
- 0084dc10 CBase::FrameUpdate DIFFER(950v966): retail packs the zOffset copy, lookAt and the blend float into one frame vector (0x10), previous/blended/direction into another, and the operator* temp with the occlusion hit; ours keeps separate slots (frame 0x5c v 0x50) | tried: user-copy-ctor vs POD C3DVector, `*=`/`+=`, a named blended local (fixes previous/blended sharing), an uninitialised target local | next: `{}` scopes around lookAt and the hit/PointAt temporaries; check whether UpdateFixLookDirection's unused `target` argument is an uninitialised local
- 0084e590 CBase::UpdateTrackMeleeOpponents DIFFER(593v593): four spots -- vector begin/end through eax/ecx then copied (retail straight into edi/ebp), GetCentre vtable fetch not hoisted above the Max-Min subtraction (retail `mov eax,bounds ... mov ecx,eax`), the scheduler-commuted X-sum fadd (`fld [eax] ; lea eax ; fadd [edi]`), the vector free through eax (retail edi) | tried: my 15 forms (block-scoped filters/rect fixed the frame; named focal, Dot member, `pos + GetCentre()` order, named sum, free operator+, reference filters, dtor local, same-TU GetThingsInRect stand-in 510/573) + AdvB3 10 forms (enemies-first filters fixed spot 1; it-before-end, do-while, member loop, no named bounds, operand swap) | next: lane gap: register colouring/scheduling (see probe/advisor/0084e590/NOTES.md untried list)
- 00a38c20 CCRCSymbolMap::LoadBinary DIFFER(564v551): retail caches the constants 0 (esi) and 4 (ebp) across the whole body | tried: ternary/conditional malloc, count copied to a register local, reference/assignment element stores | next: lane gap: constant register caching
- 00a5b370 CCachedFile::Read DIFFER(533v534): size/destination in ebx/ebp swapped; throw-path message temporaries use the dead argument slots differently | tried: cf_a..cf_d (the GetPosition-before-GetLength order loses the file-pointer caching) | next: lane gap: register allocation + dead-arg slots
- 00a0bf20 CCamera::CCamera DIFFER(517v517): one x87 scheduling swap -- retail emits `mov esi,0x17c` (the second Normalise) before `fld [esp+0x20]` (up.Y) | tried: init-list MakeRHSet, RHSet ctor, body stores in 7 orders, 6 up-vector spellings | next: lane gap: x87/integer scheduling
- 00d08790 CCampHostage2::Main DIFFER(766v730): retail caches 0 in ebp, and the timer index in ebx across the loop's calls | tried: immediates vs named zero, index locals | next: lane gap: register caching across calls
- 00d08b40 CCampHostageDoor::Main DIFFER(778v778): one jne displacement -- the pre-guard early return targets the inline-dtor block's epilogue (0xd08de2) instead of the final epilogue after the out-of-line exit (0xd08e42) | tried: goto/label/nesting/forceinline-helper variants, plus AdvB3's 10 grades (probe/advisor/00d08b40/NOTES.md) | next: lane gap: epilogue cross-jump choice (same family as 0096ee90)
- 00a567a0 CCardinalSpline::GetSplinePosition DIFFER(790v766): retail rounds with an inline-asm fistp helper (ebp frame, fistp/fild [ebp-8]) | tried: one structural model (fmod wrap, neighbour pick, Hermite 0x00A56670 __fastcall) | next: lane gap: inline-asm fistp rounding
- 0099c1e0 CWideString::Replace DIFFER(610v609): retail keeps result.PStringData in ebp and the prefix data in ebx, homes the search text in the dead `what` slot, and gives AllocStringData an empty-object `this` in the `with` slot | tried: inline appends, out-of-line `operator+=` tail, inline assignment/dtors with the instance counter | next: lane gap: register/frame allocation
- 00a60d40 CChecksumCacheFile::Open DIFFER(621v623): retail caches the CDiskFileWin32 vptr 0x0122D06C in esi and the read size 4 in edi; the loop-body CCharString temp sits below the file (retail: above it) | tried: inline file ctor/dtor, named size, scoped loop temporaries | next: lane gap: constant register caching
- 00aa8830 CChunkedFileSuperChunk::ReadFromFile (/GS) DIFFER(918v889): our build caches 0 in ebp/edi for the whole function and its frame is 0x38 vs retail 0x28. The chunk size does not take the dead stream slot, and the growth temporaries do not share slots. Retail inlines `_M_insert_overflow` in branch 1 and keeps it out of line (0x00AA94E0) in branch 2 | tried: 5 forms -- ctor-less counted pointer with a manual Construct, `Max()` by reference, plain Read vs ReadULong, while vs for, an STLport-shaped inline insert_overflow | next: a `__forceinline` copy of STLport `_M_insert_overflow` with a fill_len parameter slot shared by new_start; a raw-pointer chunk local in branch 1

## Skipped
None. Every row holds one function.

## Label corrections
- 0x00D27950 is CQ_EndGameScript::CheckRegionLeaving (catalogue: CBeggar::Main).
- 0x0099C1E0 is CWideString::Replace(const CWideString&, const CWideString&) (catalogue: CCharString::ReplaceSubstring).
- 0x009D6490 is CBankFileAsync::~CBankFileAsync (catalogue: CBankFile destructor).
- 0x00555EB0 is NUISystem::CScrollableList::Initialise (catalogue: CChangingStateComponent::Initialize; the landed CRedefinerList::Initialise 0x005566A0 calls it as its base).
- 0x00D90290's parent is CQ_HobbeCaveScript.
- 0x00E7FD50 is a CV_FishingCompetitionScript method.
- 0x006625C0 / 0x006625E0 are creature def-float getters (manifest "CTCWeapon::GetAnimationSpeedValue" is propagated).
- 0x009CE180 is CBankFile::~CBankFile.
- 0x009D7DB0 is list::clear.
- 0x009D7430 is set::_M_erase.
- 0x00A62960 is CCriticalSectionLock::Unlock.
- 0x0042A34E is _Rb_tree_rebalance_for_erase.
- 0x00A60A00 is the CChecksumCacheFile map `_M_erase` (labelled BuildTreeArray).
- 0x009D2B50 / 0x0042AD34 are UpperBound / LowerBound.
- 0x00CBFB7D is RunCutsceneMacro: `void __fastcall (const CCharString&, map<CCharString,Handle>&, map<CCharString,bool>*, map<CCharString,CCharString>*, bool, bool)`. The landed 0x00E2A900 calls it RunScriptedCutscene with a wrong 4th-parameter type.
- 0x009CC0B0 GetRandomValidationValue is a thiscall member (PDB AAEKXZ), not static as a landed comment says.
- 0x007E74D0 is the scripted-thing handle dtor (labelled ~CScriptGameResourceObjectMovieBase).
- 0x00AA94E0 (labelled Vector_InsertN_uint64Entity) is CArray<CCountedPointer<CChunkedFileChunk>>::_M_insert_overflow.
- 0x0053FDE0 (labelled std::list::push_back) is a CAction member taking (target subobject, const CCountedPointer<CAction>&).
- 0x0084D0E0 (labelled CThingCreatureBase_ComputeMeleeMetrics) and 0x0084E390 (labelled StdVector_PushBack) are CAxisPointList_0084f4d0 Compute / Add.
