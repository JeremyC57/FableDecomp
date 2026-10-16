# Hand chunk 185 (CAnimComponentCombatComboChain .. CBase): 25 of 40 byte-exact

## What matched (25)
- CAnimComponentCombatHandedness/CombatMisc::TransferBinaryIn (005dba80, 005dbcd0): the landed ComboChain 0x5DBB60 Read model; bools via an inline `TransferBoolIn` (`v != 0`).
- CAnimComponentSetCreatureMode::Initialise (005dc9b0): the landed CAnimComponentFlags::Initialise parser model; `if (mode) { Mode = mode; return true; } return false;`.
- CAnimationEntry(long,long,long,long) (005ded00): three `new` components (TransitionInTime/Flags/DelayTime), value stored after the null join.
- CAnimationSet::TransferBinaryOut (005dc070): size() = ptr diff / 20 re-evaluated, unsigned loop.
- CAnimSet copy ctor (00710b80): explicit init list + five one-bit bitfield assignments give the xor/and/xor merges.
- C3DMeshInfo::operator= (00a2b3c0, label fix): box/sphere with user copy ctors (implicit op= = lea block copy); IV pointer release as a pointee member (`dec [ecx+4] ; jne`), arrays unrolled.
- NCameraModes::CBase::TransferPersistantBaseData (0084cf80): C3DVector with a user copy ctor and NO operator= -> pointer-register copies.
- NScript CArtefact::OnPredicateFail (00dcf920), CBalverineFightingGuard::OnInterrupted (00da4c90): CScriptThing pimpl model of CM_HiddenBooty::Init; the literal must convert implicitly to `const CCharString&` to be built first.
- CBankFile: SplitSymbol (retail copies the whole symbol into *pName), GetLastModifiedTime (local CDiskFileWin32, Opened cleared in the ctor BODY), GetRecoveryFilePath (named joined path), GetRandomValidationValue (CRC32 of GetSystemTime, retry while <= 1), InternalWriteEntryData, DeleteEntry (`= CRuntimeData()` temp in dead arg slot; clear() copy-down helper), FindEmptyBlockOffset (named entry local), OpenFileReadOnly, InternalResizeEntryData.
- CDefStringTable ctor (009d4db0) and CBankFileAsync ctor (009d5f80): STLport tree/list inline ctors, body-assigned flags.
- CBankFileAsync::ProcessDisposalList / DiscardAsyncData (009d6730, 009d67e0): STLport list inlines; the inner lock sits in an inline bool helper returning from inside the lock scope; CRITICAL_SECTION model must be dword aligned.
- CBankFileManager::CreateRetailBank / CloseRetailBank (009a7720, 009a78d0): `#pragma function(strcmp)`, null-first `c_str()` (`p == 0 ? "" : p->Text`).

## Parked for Fable 5.1 (15)
- 005dc650 CAnimComponentCombatComboChain::Initialise DIFFER(209v210): first parsed value homed in the text slot instead of the context slot (and the "," temp reversed) | tried: named bool, inline if, id local | next: lane gap: dead-argument-slot temporaries
- 0070c800 CAnimSet::GetInterpolatedTime DIFFER(162v168): retail `mov ax,[..] ... movsx eax,ax` after the engine chain, ours `movsx` from memory; helper arg loads scheduled differently | tried: plain index, short local, GetAnimation(short) | next: by-value inline short getter
- 00a8ff50 N3DPrimitive::CAnimatedBlock::ConvertToVBFormatNonBump DIFFER(217v231): retail X/Y via fld/fstp, Z via a 0x1c frame temp, output pointer biased +8; ours dword copies | tried: copy-ctor getters, 3-float-ctor getters, POD getters | next: getter built from three floats with only a 3-float ctor + named vertex temp (graphics row)
- 00471865 vector<pair<ulong,CAnimationEntry>>::operator= DIFFER(222v239): edx-passed helper ranges, `push 14h ; pop edi`, ebp frame | tried: Dinkumware op= under optimize("s") | next: lane gap: LTCG custom convention
- 00c72a40 CAudioSource::SetSourceProperty DIFFER(170v170): esi/edi swapped | tried: ternary lookups, if lookups, inline AL wrappers | next: lane gap: register rotation (audio row)
- 009cb960 CBankFile::OpenReadWrite DIFFER(211v218): retail keeps alignment in ebp across both arms | tried: return form, if/else, inverted test | next: lane gap: register allocation
- 009d0fc0 CBankFile::GetAllocatedBlockSize DIFFER(185v198): offset spilled to the dead index slot in retail, ebp in ours | tried: bool/int found, return in loop | next: lane gap: dead-argument-slot homing
- 009d5d30 CBankFileAsyncData ctor(buffer) DIFFER(179v179): +0x46/+0x47 flag stores scheduled around the lock pushes | tried: init list, body | next: lane gap: scheduling
- 009d5df0 CBankFileAsyncData ctor(pool) DIFFER(183v183): same as 009d5d30 | tried: init list, body | next: lane gap: scheduling
- 009d6350 CBankFileAsyncData::Abort DIFFER(191v196): retail bl result with one shared lock tail | tried: return form (194v196), || form (200v196), goto | next: result bool declared before the lock, assigned from the chain
- 009d5970 CBankFileAsyncEntry::SetupRead DIFFER(191v191): eax/ecx swap in the sync arm | tried: PDst before test, per arm, shared readSize | next: lane gap: register rotation
- 009d6100 CBankFileAsyncEntry::BeginReadDataAutoAlloc DIFFER(235v235): edx vs ecx for the bank/ctor args, SetupRead arg loads rotated | tried: named counted local | next: lane gap: register rotation
- 009cc750 CBankFileEntryUpdateData::Equals DIFFER(235v235): loop element addressing order / registers | tried: plain, crc local, #pragma function(strcmp), null-first c_str (extra) | next: named element refs before strcmp
- 009a7e20 CBankFileManager::GetDevHeaderFilePath DIFFER(172v171): retail cross-jumps two copy-ctor calls (push per arm, shared call) | tried: ternary, inline const& helper, per-name helpers | next: lane gap: cross-jumping
- 009a7ed0 CBankFileManager::GetRetailHeaderFilePath DIFFER(172v171): twin of 009a7e20 (+0x20 field) | tried: same source shape | next: same

## Skipped
- none (no row already landed; no two-function rows).

## Label corrections
- 0x00A2B3C0 "CAnimationEntry::operator=" is C3DMeshInfo::operator= (0x60 bytes).
- 0x009D4DB0 "CBankFile::GetRandomValidationValue" is CDefStringTable::CDefStringTable().
- 0x009D5F80 "CBankFile::ConstructorInitialize" is CBankFileAsync::CBankFileAsync() (vtable 0x0129B8D4).
- 0x009A7ED0 (second "GetDevHeaderFilePath") is GetRetailHeaderFilePath (CBankInfo +0xc).
- 0x00471865 "CAnimationSet::AssignAnimations" is vector<pair<ulong,CAnimationEntry>>::operator=.
- 0x0084CF80 CBase is NCameraModes::CBase; 0x00DCF920 / 0x00DA4C90 are NScript::CQ_OrchardFarmRaidScript::CArtefact / CQ_MinionClifftopChaseScript::CBalverineFightingGuard.
- Callees: 0x009CBF60 CBankFile::ReadData (not OpenMapDialog), 0x009CBB80 GetAlignment (not CWorld::DrawGetEnvironment), 0x009A8090 bank-manager register (not IsTutorialPlayed), 0x00A242C0 animation-bank GetAnimation, 0x00A23CE0 CAnimation::GetDuration, 0x009CD850 CBankFile::Close (not CDefStringTable dtor), 0x0098F350 CThreadedFile::QueueRead, 0x009D7B00 disposal-list push_back, 0x00994430 is ~CFileDataOutputStream.
- Retail CBankFile base is 0x110 bytes (CBankFileAsync members from +0x110; DisableAsync +0x149, NonCached +0x148).
