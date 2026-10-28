# Hand chunk 517 (CIsUsableIciclePoint .. CMemoryFile): 32 of 40 byte-exact

4 MATCH, 28 RELOCATION_MATCH, 0 retail_asm, 8 parked, 0 skipped. Every src/ row PASSes its test under g++ (tall.sh) and the
lane VC7.1 cl (tall.py); the 8 differ/ rows have differ/<addr>.test.cpp tests that also PASS under both. The vland dry run
reports WINS 32 with no RELOC_IDENTITY (one RELOC_CONFLICT on 0x006A4D00 was fixed, see 0x00914600).

## What matched (32)
- **CIsUsableIciclePoint** 008e2e40 (MATCH): the anonymous-namespace predicate `??RCIsUsableIciclePoint@?A0x...`. It returns
  false when an icicle's 2D offset is inside the limit. Shape: `C3DVector offset = it->Position - point; offset.Y*offset.Y +
  offset.X*offset.X` (VC squares X first). The body needs `#pragma auto_inline(off)` plus an external caller
  (IsUsableIciclePoint_Use), or VC never emits the anonymous leaf.
- **CMap** 008f6f90 (MATCH): FindUnusedThemePaletteEntryIndex. Index counter in the loop, then
  `if (index > 0 && !used) return (unsigned char)index;`.
- **CAIStateGroup_ScorpionKing** 00903177: the default ctor (the base ctor, two intelligent pointers, an empty STLport vector).
- **CAIStateGroup_MinionSitAtSchool** 0090f7d0: AllocChairSittingMode. An inline GFRandom (LCG 0x24A1/0x24DF, `__fastcall`
  GFROR13, `%`) inside a switch. Case 0 is `new CModeSitChairStudious(*creature)` and case 1 calls the UsesSeat base.
- **Message-event finders** 009112a0, 0092edc0, 0094bfc0, 00958770. A same-TU noinline FindMostRecentMessage template
  stand-in reads `this` and the filter's first word, which makes VC hoist the Bind first-pointer store as retail does.
  Bind2 needs a named filter local plus a named `manager` local. Bind3 (009112a0) builds through operator&& from the bound
  operands. Bind4 (00958770) only matches when written inline in the call, through an inline `MessageEventManagerOf(world)`;
  a named filter of temporaries drops their stores.
- **ThrowLogsOnFire** 00917050 / 009170b0, **RecoverStealableItem** 009652c0: the same-TU noinline GetPBestThing stand-in. A
  creature local declared first orders the `nearest = 0` store. For a filter that has data (HasTC 0xB2), the stand-in must
  read filter.Type or VC drops the store.
- **BirdStartled** 00914600: Update. A creature local plus `if (!found) { Terminate(); return; }`.
- **MinionGuardReactToPlayerWithWeapon** 00939e70: IsPlayerCarryingWeapon. HasTC written as an if-form inline bool
  (`if (bit) return true; return false;` gives `test dword`), then a pick-form PeekTC (end/hit ternary, component into ecx).
- **SellToBuyer** 0094c010: IsRunnable. A `const CCountedPointer<CCreatureInteraction>&` local gives the
  `add eax,off ; mov eax,[eax]` load; list size() is an inline loop.
- **MinionReactToCrimeFromSleep** 00958f00: IsRunnable. Same shape as the landed 0x00960F00 (a bool copied from the TC,
  then the base IsRunnable clears it).
- **CIsRunnableModuleCarryingProjectileWeapon** 00982890 (MATCH): a by-value CDefPointer temporary from an inline
  GetPBaseDef(); its release is `dec [ecx+4] ; jne`.
- **CDataOutputStream** 00995a20 / 00995a80: the WriteStringNewLine / WritePresizedString string writers (inline WriteULONG).
- **CArray** 009972e0 (`<char>`), 009b4ae0 (`<unsigned char>`), 009b5fb0 (`<pair<unsigned long, unsigned long> >`):
  DeallocateMemory = `if (capacity()) { clear(); CArray<T>().swap(*this); }`.
- **CWideString** 0099b0f0: operator!= = `!(*this == other)` with a `__forceinline` operator==.
- **CCharString** 0099f2a0 Truncate, 0099f3d0 Insert (the latter through a same-TU noinline CBasicString<char>::Insert;
  see Review at landing).
- **std::list<CFileInstaller::CPendingWrite>** 009c69c0: `_M_insert_dispatch<const_iterator>`. This is the landed 0x009C6F70 TU
  with the iterator `*`/`++`/`!=` and STLport's `for (; first != last; ++first) insert(pos, *first);`. Its
  `_M_create_node` uses a placement `_Construct`, which gives the `lea ecx,[eax+8] ; test ecx,ecx` null test.
- **CFontManager** 009e2c80 GetFont: three plain returns (static if found; else streaming if found; else static). VC
  tail-merges the two static paths and hoists the `push edi`. 009e2ce0 GetFreeVertexBuffer (MATCH):
  `CurrentVB = (CurrentVB + 1) % VertexBuffers.Size(); return *VertexBuffers[CurrentVB];`.
- **CDriveManager** 009e4c70: GetDriveFromPath. `wchar_t drive[3]; _wsplitpath(path, drive, 0, 0, 0); return drive[0];`.
  Needs `// Flags: /GS /Zc:wchar_t`: the cookie frame, plus native wchar_t for the PDB `_W` mangling. The old /GS park only
  lacked the flag pair.
- **CMemoryFile** 009e9330 DumpToFile, 009e9390 Close, 009e9760 Copy, 009e97b0 ctor (local retail-layout model).

## Parked for Fable 5.1 (8)
- 0091c0d0 CAIStateGroup_NymphFloatAround::IsRunnable DIFFER(81v81): retail threads the PBrain->Creature loads through eax (`mov edx,[esi+4]` before the pushes, `mov ecx,[edx+18h]` after); ours uses ecx/edx chains | tried: GetCreature accessor, creature local, named string, bool local, __forceinline HasTC | next: lane gap: register allocation
- 00929f10 CAIStateGroup_ApprenticeHero::IsRunnable DIFFER(82v87): retail `mov dl,[eax+24h] ; mov cl,8 ; test cl,dl` for the 0x23 bit and `mov eax,[eax+4] ; pop esi ; mov ecx,eax` for the component; ours `test byte ptr [eax+24h],8` | tried: masked-word, byte, shr, bool-local HasTC forms, out-pointer GetInterface | next: a char-typed bit-mask local fed to an inline test helper
- 0093dae0 CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending DIFFER(48v48): the pushed creature loads through edx, retail ecx | tried: GetCreature, local, `||`, if, bool local | next: lane gap: register allocation
- 00946330 CAIStateGroup_PrepareMeal::IsStillRunnable DIFFER(77v76): the last test (pot Dead bit) folds to `mov eax,1 ; test al,dl`; retail `test byte ptr [eax+91h],1 ; jne false ; mov eax,1` | tried: &&, if, early return, IS_THING_ALIVE, bool bitfield | next: lane gap: final-bool tail folding (GOTCHAS chunk 240)
- 0095e430 CAIStateGroup_ReactToDeadLeader::IsStillRunnable DIFFER(63v64): the frame-budget compare folds to `setl al`; retail `jge false ; mov al,1` | tried: 17 return / && / bool-local / IS_THING_ALIVE forms | next: lane gap: final-bool tail folding
- 00994b10 CMemoryDataOutputStream::SetPosition DIFFER(46v45): byte-exact through `call _CxxThrowException`; VC7.1 emits an int3 after the noreturn call, which the row cutter stripped | tried: 2 grades (also the 2026-10-14 hand57 park) | next: land against the 46-byte span
- 009ace90 CDefClassBase::UnloadResource DIFFER(68v68): retail places the `return false` block (refcount > 2) last, behind `ja`; ours puts it right after `jbe` | tried: early return, inverted if, bool local, result local, else, reference, goto | next: lane gap: block placement
- 009d5a30 CLoadingMemoryFailureHandler::UnregisterBank DIFFER(92v89): ours `mov edi,[esi+8]` before the find, `cmp eax,edi` after (plus push/pop edi); retail `mov edx,[esi+8]` before and `mov ecx,[esi+8]` reloaded after | tried: extern find (gives the reload but a separate tag slot `lea eax,[esp+7]`, `sub esp,0Ch`), same-TU noinline find before and after the caller, the real STLport `std::__find` template with /Gr, the QFE-4035 compiler (same) | next: a find body that VC cannot prove store-free yet still sees the tag as unused (the tag homing and the purity come from the same same-TU knowledge)

## Skipped
None (no row covers two functions).

## Names
### Proven
008e2e40 (anonymous namespace)::CIsUsableIciclePoint::operator()(const C3DVector&) const
008f6f90 CMap::FindUnusedThemePaletteEntryIndex()
00903177 CAIStateGroup_ScorpionKing::CAIStateGroup_ScorpionKing()
0090f7d0 CAIStateGroup_MinionSitAtSchool::AllocChairSittingMode() const
009112a0 CAIStateGroup_MinionGuardReactToBodyFoundMessage::FindBodyFoundEvent()
00914600 CAIStateGroup_BirdStartled::Update()
00917050 CAIStateGroup_ThrowLogsOnFire::GetPNearestFreeLogPile() const
009170b0 CAIStateGroup_ThrowLogsOnFire::GetPNearestBonfire() const
0091c0d0 CAIStateGroup_NymphFloatAround::IsRunnable()
00929f10 CAIStateGroup_ApprenticeHero::IsRunnable()
0092edc0 CAIStateGroup_MinionReactToPlayerInteraction::FindPlayerInteractionEvent() const
00939e70 CAIStateGroup_MinionGuardReactToPlayerWithWeapon::IsPlayerCarryingWeapon() const
0093dae0 CAIStateGroup_MeleeCombatNearby2::IsCombatSequenceActionPending() const
00946330 CAIStateGroup_PrepareMeal::IsStillRunnable()
0094bfc0 CAIStateGroup_SellToBuyer::GetBuyersOrder()
0094c010 CAIStateGroup_SellToBuyer::IsRunnable()
00958770 CAIStateGroup_MinionReactToCrime::GetSomeoneElseGettingHelpMessage(long)
00958f00 CAIStateGroup_MinionReactToCrimeFromSleep::IsRunnable()
0095e430 CAIStateGroup_ReactToDeadLeader::IsStillRunnable() const
009652c0 CAIStateGroup_RecoverStealableItem::FindPTargetPoint() const
00982890 NCreatureAI::CIsRunnableModuleCarryingProjectileWeapon::IsSatisfied()
00994b10 CMemoryDataOutputStream::SetPosition(unsigned long)
0099b0f0 CWideString::operator!=(const CWideString&) const
0099f2a0 CCharString::Truncate(unsigned long)
0099f3d0 CCharString::Insert(long, char)
009ace90 CDefClassBase::UnloadResource()
009b4ae0 CArray<unsigned char>::DeallocateMemory()
009b5fb0 CArray<std::pair<unsigned long, unsigned long> >::DeallocateMemory()
009c69c0 std::list<CFileInstaller::CPendingWrite>::_M_insert_dispatch<const_iterator>(iterator, const_iterator, const_iterator, const __false_type&)
009d5a30 CLoadingMemoryFailureHandler::UnregisterBank(CBankFileAsync*) -- Ego_r name set; called on the old handler by the landed SetLoadingMemoryFailureHandler 0x009D66A0 before RegisterBank 0x009D6420
009e2c80 CFontManager::GetFont(const CCharString&)
009e2ce0 CFontManager::GetFreeVertexBuffer() -- Ego_r 0xB2C250 in the CFontManager block; the only PDB member that fits the layout
009e4c70 CDriveManager::GetDriveFromPath(const wchar_t*)
009e97b0 CMemoryFile::CMemoryFile(const CWideString&, FileAccessType, unsigned long)
009e9390 CMemoryFile::Close()
009e51c0 (callee) CSymbolMap::SymbolExists(const CCharString&) const -- one argument, bool; Ego_r donor order GetSymbolSLONG 0xB36AD0 < SymbolExists 0xB36B80 matches 0x9E5120 < 0x9E51C0
009e5120 (callee) CSymbolMap::GetSymbolSLONG(const CCharString&) const
### Probable
00995a20 CDataOutputStream::WriteStringNewLine(const CCharString&)
00995a80 CDataOutputStream::WritePresizedString(const CCharString&)
009972e0 CArray<char>::DeallocateMemory() (no caller; one of the two 1-byte instantiations the PDBs hold)
009e9330 CMemoryFile::DumpToFile()
009e9760 CMemoryFile::Copy(const CMemoryFile&)
006a4d00 (callee) position getter, byte twin of CThing::GetPos 0x004C73D0; no PDB name proves it
00912700 (callee) CAIStateGroup_FlyingThingBase::GetRandomFlightPos(const C3DVector&, float, C3DVector&, bool) const
009130d0 (callee) CAIStateGroup_FlyingThingBase::AIAction_RandomFlyToPos(const C3DVector&) (Ego_r donor order; the bindings say ExecuteFlyingPath)
00982890 RangedWeaponDef at retail +0xE0 (Ego_r +0xE8)
009d6e80 (callee) std::__find<CBankFileAsync**, CBankFileAsync*>(..., const random_access_iterator_tag&), __fastcall (/Gr). My compile of the STLport template is byte-identical to retail; it is landed under the descriptive Find_009d6e80
### Descriptive
006a4d00 CThing::GetPosition_006a4d00 (the existing binding from the landed 0x00911710, reused)
(no address) MessageEventManagerOf(const CWorld&): an inline stand-in for CCountedPointer::operator-> in 00958770
(no address) PeekTC<T>: an inline component pick in 00939e70
(no address) FindMostRecentMessage_Sink / GetPBestThing_Sink: externs inside the same-TU stand-in template bodies (see Review at landing in LESSONS.md)
(no address) IsUsableIciclePoint_Use: the external caller that keeps 008e2e40 emitted

## Label corrections
- 008f6f90 CMusicManager::ResetMusicEntriesOffsets -> CMap::FindUnusedThemePaletteEntryIndex (Ego_r donor order 0x8545D0..0x854830; only caller 0x8F70F0 AddThemeDefIndexToPalette)
- 00903177 CAIStateGroup_Guildmaster::CAIStateGroup_Guildmaster -> CAIStateGroup_ScorpionKing::CAIStateGroup_ScorpionKing (vtable 0x0128D5B0)
- 00917050 GetPNearestBonfire -> CAIStateGroup_ThrowLogsOnFire::GetPNearestFreeLogPile (donor order; callee 0x916F30)
- 0094bfc0 FindPlayerInteractionEvent -> CAIStateGroup_SellToBuyer::GetBuyersOrder (donor order; only caller 0x94C010)
- 0099b0f0 CCharString::operator== -> CWideString::operator!=
- 009972e0 CArray<unsigned_char> -> CArray<char>; 009b5fb0 CArray<unsigned_short> -> CArray<std::pair<unsigned long, unsigned long> >
- 009d5a30 CBankFile::PopLoadErrorQueue -> CLoadingMemoryFailureHandler::UnregisterBank
- 009e2c80 CMessageEventFilter_CanBeSeenOrHeard operator() -> CFontManager::GetFont (labels_rename1 already had it)
- 009c69c0 CActiveFile::CopyList_20byte -> std::list<CFileInstaller::CPendingWrite>::_M_insert_dispatch (already corrected in chunk 220; the chunk label is stale)
- callee 009e51c0 bound ?FindSymbol@CSymbolMap@@QAE_NJ@Z -> CSymbolMap::SymbolExists(const CCharString&) const

## Header debt
- engine/CMemoryFile.h, CMemoryDataOutputStream.h, CAIStateGroupBase.h, CAIStateGroup_*.h: flat Ego_r layouts. They have a
  16-byte CArray, the vptr as a data member, and no bases or methods. The retail STLport layouts are modelled locally.
- engine/CDriveManager.h, CThingSearchTools.h, CTCCreatureOpinionOfHero.h, CDefClassBase.h: vland warns
  SHARED_HEADER_AVAILABLE. Each row uses a small local model whose offsets are the retail ones (CDriveManager: vptr +0,
  SystemManager +4).
- CFontManager, CLoadingMemoryFailureHandler, CActiveFile: local models at retail offsets (the PDB CArray is 16 bytes, retail 12).
