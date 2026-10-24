# Hand chunk 331 (CPersistTraits<CDefIndex> .. CPersistTraits<bool>): 38 of 40 byte-exact

All 38 graded RELOCATION_MATCH (gall.sh, RTM cl 13.10.3077); every test prints `<addr>_TEST PASS` under g++ (tall.sh)
and the lane's VC7.1 (tall.py).

## What matched (38)

- **Binary writers, `CPersistTraits<T>::TransferBinaryOut(CDataOutputStream*, const T*)`** (thiscall, traits `this`
  unused): CHeroMarriageInfo 0057a3cd (engine header; `WriteUInt64` @0x993EC0 for the two UIDs), CMusicEntry
  0046b46e / 004e9d34 (ID through WriteULong in one TU, WriteLong in the other), CSoundBankEntry 0046aaf0,
  CObjectAugmentationParticleSet 004e959d (two 12-byte def-string vectors through GFSerialiseVectorBinaryOut
  0x4E95F5), CEngineGraphic 0067d7d0, CUIRegionInfo 005fa610 (TatooShops written before TitleShops),
  CIntelligentPointer<CThing> 006c52d0 (UniqueID +0x18 or 0), COpinionDeed* 006c5310 (inline
  `IsStillHappening ? GFGetCurrentFrame() : FrameHappened`, then a local intelligent-pointer traits whose
  TransferBinaryOut is inlined), CPhysicalPrimitiveInit 0067f7d0 (type via virtual GetType, then the primitive's
  own writer), CReactionFrequencyTraitsArray 00677840 (2 x 79 nested loop). Shape: `push dword ptr [esi+N]`
  rows are `#pragma optimize("s", on)`; `mov reg ; push reg` rows are the default speed TU.
- **Binary readers (inline CDataInputStream::Read)**: CWeaponTrailGraphicSet 00469d47 / 004e9021, CFloatRange
  00458f89 (Low then High), CPhysicalPrimitiveInit 0067f870 (type into the dead stream slot, then
  SetPPrimitiveFromType + virtual reader), CSoundBankEntry 0046abd6 (three CDefString::TransferBinaryIn calls and
  a byte read as `!= 0`), bool 00403eb0, CReactionFrequencyTraitsArray 006778a0 (the type must come from an
  inline by-value `ReadLong()`: VC7.1 then caches it in ebx from its home slot before the loop).
- **POD vector persist (data/size idiom of the landed 0x006808C0)**: CReplaceableMeshes 004ea26b / 004ea2c9
  (CEngineGraphic packed to 14 bytes per the PDB; `__forceinline` size()/data() or the size TU keeps data() out of
  line), and the vector-member forms 0046fe8a / 0046fee9 / 00701e40 (`this` = the vector). Readers use
  `if (bytes > 0) Read(data, bytes)` with the `size <= remaining` test (landed 0x00757990 idiom); 0046fee9 resizes
  with a value-initialised 0x18-byte temporary (`rep stosd`).
- **Text persist**: CWideString TransferOut 0040e840 and CDefString TransferOut 007e3930 (one
  `*out += CCharString("") + "\"" + text + "\"";` expression; the def-string text is a named local first),
  CRGBColour TransferOut 009e8110 (nine appends), CDefIndex TransferIn/Out 00a3e0d0 / 00a3e180 (named
  `CDefinitionManager* manager = context->PDefinitionManager;` local puts `mov ecx,[esi]` before the argument
  pushes; inline CDefString::GetString(); nested-if CCharString == "" inline), pair<EReactionSpeechType,long>
  TransferIn 00755f40 (labelled CSimVoice; longs through an inline by-value TransferInLong helper so they home in
  the dead parser/context slots), EInputKey-labelled TransferIn 00679910 (long via 0x9E2590, enum store),
  CFaction TransferBinaryOut 006ba030 (text-save into a string through a CPersistContext, then length + bytes;
  CCharString accessors written null-test-first).
- **Mislabelled rows**: ReadC2DVector 00995030 (hidden-pointer C2DVector), water-patch creation 00bf5a30
  (`new (owner) CWaterPatchMesh(map, stream, &offset)`), NUISystem::CMouseCursor::GetPosition 0055c5d0 (named local
  + user copy ctor), unoptimised list end() 009f7970 / 009f79a0 and back() 009f7c20 (`#pragma optimize("", off)`),
  Kraken holder dtor 00c75440 (`/EHsc`: the `eh vector destructor iterator` over two elements after two inline
  Clear() calls).

## Parked for Fable 5.1 (2)

- 0046c3b9 CPersistTraits<CObjectFamilyEntry>::TransferBinaryIn DIFFER(119v119): `lea ecx,[ebp+0Ch]` (the long-traits temporary's `this` in the value-argument slot) vs ours `[ebp+0Bh]` (1-byte empty temp) / `[ebp+8]` (4-byte traits) / `[ebp-4]` / `[ebp-1]` (named locals) | tried: empty temporary, 4-byte DefaultVal temporary, named 1- and 4-byte locals, no-ctor empty class | next: a traits passed by reference from an inline wrapper, or the Object read through a `CPersistTraits<long>` reference member; else lane gap: frame-slot choice for an empty temporary
- 0040e710 CPersistTraits<CWideString>::TransferIn DIFFER(213v237): retail falls through into the opening-quote Error arm (`push -1` hoisted above `je`, msg ctor, `jmp` into the closing-quote Error tail), ours branches `jne` to a merged tail | tried: early `return`, if/else, negated condition, named `quote` delimiter in a block (fixed the chain order) | next: an error helper / `goto` shaped tail, or a do { } while (0) with break; else lane gap: block layout

## Skipped

None. No row holds two functions; several catalog lengths include trailing int3 padding (oracle lengths trimmed by
finish.py: 009f7970/009f79a0 38, 009f7c20 47, 00c75440 39, 0055c5d0 34, 006c52d0 52, 0067f7d0 57, 00995030 188).

## Label corrections

- 00995030 "CPersistTraits<CFloatRange>::TransferBinaryIn" -> CDataInputStream::ReadC2DVector() (landed callers 0x00800B90, 0x00410120).
- 00bf5a30 "CPersistTraits<CFloatRange>::TransferBinaryIn" -> water-patch creation: reads C2DCoordI offset, `new (owner) CWaterPatchMesh(map, stream, &offset)` (the out-of-line copy of the code CEngineLandscapePatch::LoadForegroundPatch inlines at 0x00BF4088); named CWaterPatchMesh::CreateFromStream (descriptive).
- 0055c5d0 "CPersistTraits<EInputKey>::TransferIn" -> NUISystem::CMouseCursor::GetPosition() const (vtable 0x0124C22C slot 16; base CSprite::GetPosition 0x0052E890).
- 009f7970 / 009f79a0 "~CPersistTraits<CReactionFrequencyTraitsArray>" -> unoptimised list end(); 009f7c20 "CPersistTraits<CObjectFamilyEntry>::TransferBinaryOut" -> unoptimised list back().
- 00c75440 "CPersistTraits<NInventory::CItem>::TransferBinaryIn" -> destructor of the 0x24-byte holder at CTCKraken+0x25C (ctor 0x00C75410, `??_G` 0x00C754A0; landed Kraken ctor 0x00C6A650 names it).
- 0046fe8a / 0046fee9 / 00701e40 "CPersistTraits<CReplaceableMeshes>::TransferBinaryOut/In" -> vector members (`this` = the vector; 0x46FE40 calls them as value->TransferBinaryOut/In); element types unknown (0x18 / 4 bytes).
- 00755f40 "CPersistTraits<CSimVoice>::TransferIn" -> CPersistTraits<pair<EReactionSpeechType,long>>::TransferIn ("(type,value)").
- 00679910 "CPersistTraits<EInputKey>::TransferIn": called only from CPersistTraits<list<CReactionMatch*>>::TransferIn (0x678760); the enum type is the label's, unverified.
- 0x0040FE60 "CPersistTraits<float>::TransferBinaryIn" moves a long (used for CObjectFamilyEntry::Object); 0x0044FC00 "CPersistTraits<float>" is CDefString::TransferBinaryIn; 0x006B91A0 "Faction_ExportToString" is CPersistTraits<CFaction>::TransferOut; 0x0049D870 "GFGetPreMainMemoryUsed" is GFGetCurrentFrame here.
