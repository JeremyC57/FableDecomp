# Hand chunk 330 (CPersistContext_TransferableComponent<float> .. CPersistTraits<CDefIndex>): 37 of 40 byte-exact

All 37 are RELOCATION_MATCH under the lane's RTM cl 13.10.3077 (gall.sh, with each file's `// Leaf:` line). Their
tests print `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py). Most manifest labels in this chunk were
propagated and wrong. The real identities come from vtable slots, callers and element strides; see Label corrections.

## What matched (37)

- **CPersistContext_TransferableComponent<T> TransferOut/TransferIn (4).** 0x006859B0 is Out and 0x00686200 is In,
  for T = COpinionTransientOffset (vtable 0x0125BAB0). 0x00800430 is Out and 0x00800400 is In, for T = C2DVector
  (vtable 0x01272208). Shape: the landed CRGBColour model (0x006421C0), i.e. `CPersistTraits<T> traits;
  traits.X(context, out, TransferableValue)`. The traits default comes from T's inline default constructor:
  - COpinionTransientOffset zeroes its members through eax in the order +0, +4, +8, +10, +14, +0C, so its body
    assigns them in that order.
  - C2DVector's `X(0.0f), Y(0.0f)` gives immediate stores scheduled after the pushes.
- **CPersistContext_TransferableVector<T> TransferOut/TransferIn (12).** Rows: 0x006D3810 (pair<CCharString,long>),
  0x005C0E41/0x005C20D8 (NInventory::CItem), 0x0057F754/0x00580159 (CHeroMarriageInfo),
  0x0040F9D0/0x00411250 (CActionInputControl), 0x00614230/0x00615E10 (CUIRegionInfo),
  0x00614000/0x00615920 (CUIMiniMapMarker) and 0x0057F6E9 (a 4-byte enum, TransferIn).
  - All but the enum row use `CPersistTraits<T> traits; traits.X(context, arg, &(*TransVector)[index]);`. The element
    constructor sets the zero-store order and form. The CItem counted pointer has an out-of-line destructor
    (0x005C04BF), so it is the only destructor call. In CUIMiniMapMarker, `UIDefName("")` builds the string in place
    between the Region store and the immediates.
  - CHeroMarriageInfo uses engine/CHeroMarriageInfo.h, with a traits constructor that assigns the members (the dword
    zeroes, then the GayMarriage byte).
  - The enum TransferIn reads `T* first = TransVector->m_pFirst;` before the __fastcall reader. Its local lands in the
    dead `parser` slot.
- **TransferableVector Resize (3).** 0x007B5310 (CCrimeWaitingForBribe) and 0x006C9DE0 (8-byte COpinionDeedRecord)
  only match as STLport `resize(n) { resize(n, T()); }` with BOTH resizes `__forceinline`. A named default local, or
  calling `resize(count, T())` directly, schedules the zero stores before the size computation. A plain inline leaves
  resize(n, x) out of line. 0x006D4AA0 (pair<CCharString,long>) builds the traits object first, then runs the inline
  resize(n, traits.DefaultVal).
- **CPersistTraits text writers (4).** 0x009E7F00 C2DVector writes "C2DCoordF(x,y)" and 0x009E7F90 C3DVector writes
  "C3DCoordF(x,y,z)", both through `*out += CCharString::FormatDouble(6, v)`. 0x007B4060 CCrimeWaitingForBribe writes
  "(handle,type)" through GFIntToCharString. 0x0040E630 CCharString is `*out += CCharString("") + "\"" + *value + "\"";`.
  The unused context slot holds the temporaries.
- **CPersistTraits text readers (2).** 0x0040E510 CCharString and 0x007E3820 CDefString both read a quoted string.
  - Each early-error `return` is tail-merged into the second Error call.
  - 0x007E3820 calls `value->SetString(text)` (0x009D5060 is a void member, not a constructor). Its first message is
    "Unable to find quoted string".
  - The parser is used through a method view over engine/CStringParser.h.
- **CPersistTraits binary readers (4).** 0x004671CA CBrainBehaviour, 0x007B42D0 CCrimeWaitingForBribe, 0x00755D00
  pair<long,long> and 0x0046B0D6 pair<CDefIndex,CDefString>.
  - CMemoryDataInputStream::Read is inlined, through a method view over engine/CMemoryDataInputStream.h.
  - Two members read in one body only share the dead stream slot when each read goes through an inline
    `CPersistTraits<long>::TransferBinaryIn(stream, &member)`. Two block locals add a `push ecx` frame word, and one
    reused local swaps eax/ecx.
  - 0x004671CA's Read stores `position = newPosition` (the `add eax,4` form).
  - 0x0046B0D6's CDefIndex is a class, so the read is a `movsd` copy.
- **CPersistTraits<CBrainBehaviour>::CPersistTraits() 0x004670F5.** The "" temporary dies after the zero stores, so it
  is the argument of `DefaultVal("")`, with __forceinline CBrainBehaviour(const CCharString&) and
  CDefString(const CCharString&) constructors. This is a size TU, so plain inline constructors stay out of line.
- **_STL::copy 0x00661287.** A __fastcall template with a hidden result in ecx and the __false_type tag homed in a dead
  argument slot. back_insert_iterator needs STLport's user constructor; as an aggregate it returns in eax.
- **_STL::__linear_insert (3).**
  - 0x00A33800 works on pair<unsigned long, C3DDummyObject> with an unsigned key. copy_backward is inlined to
    __copy_backward with the tag in the dead comparator slot. pair needs a user copy constructor
    `first(p.first), second(p.second)` for the memberwise by-value copy.
  - 0x00758E00 and 0x0081BE30 work on 0x2C- and 0x28-byte records with a signed long key, and call copy_backward out
    of line.
- **LHHttp2::ResetDocumentRecv 0x0083EB90.** Three zero stores through ebx, two intrinsic memsets, InChunkMode = false,
  then a tail jump to ResetDocumentParts.
- **No-RTTI constructors 0x00C70120 / 0x00C70B00** (vtables 0x012BEDCC / 0x012BEDE0, 64- and 160-float blocks).
  - The aligned scratch pointer store precedes the vptr store, so that buffer is a second, non-polymorphic base with
    an inline constructor.
  - The two blocks are POD members, memset and pointed to from the derived constructor (a block constructor addresses
    them off edx/ebx).
  - The counters are assigned +664/+668 before +654/+658.

## Parked for Fable 5.1 (1)
- 0057f70f CPersistContext_TransferableVector<ECreatureGeneratorGenerateType>::TransferOut DIFFER(36v36): retail `mov ecx,[ebp+10h]` (index) comes before `mov eax,[eax]` (first); ours loads first, then index | tried: `TransVector->m_pFirst[index]` (y off + s on), the inline traits form (TransferOut left out of line, 33 bytes), no "s" pragma, `*(index + first)` | next: the named-local form of the landed 0x005663F0 (`long* first = TransVector->m_pFirst; long value = first[index];`), whose comment says it puts the index load in the dead `this` register first. Its TransferIn twin 0x0057F6E9 matched with the named `first` local.

## Skipped
- 007fae34: wrong bounds, not a function. It is the `depth_limit == 0` arm of the SGI `__introsort_loop` at 0x007FAC80
  (`je 0x007FAE34` at 0x007FACA6). That arm calls 0x007FAC00 (by the SGI shape, partial_sort(first, last, last, comp)) with a copied 0x34-byte
  CThingCompare_BestForTargeting2 comparator. It pops edi/esi/ebp/ebx, which it never pushed. The real function is
  0x007FAC80..0x007FAE54 (468 bytes, `ret 3Ch`).
- 0080bfec: wrong bounds, the same arm of the `__introsort_loop` at 0x0080BD30 (`je 0x0080BFEC` at 0x0080BD66). The
  28-byte elements show as the 0x92492493 divide. The real function is 0x0080BD30..0x0080C012 (738 bytes, `ret 3Ch`).

## Label corrections
- 0x006859B0 / 0x00686200: TransferableComponent<COpinionTransientOffset> TransferOut / TransferIn, not `<float>`
  TransferIn. The callee 0x00686240 is CPersistTraits<COpinionTransientOffset>::TransferIn, not
  CAppearanceModifierGraphics.
- 0x00800400 / 0x00800430: TransferableComponent<C2DVector> TransferIn / TransferOut. The callee 0x009E81E0 is
  CPersistTraits<C2DVector>::TransferIn, not CBrainBehaviour.
- 0x006D3810: element pair<CCharString,long>, not CCharString. 0x006C9DE0: COpinionDeedRecord (vtable 0x0125E9DC),
  not CCrimeWaitingForBribe.
- 0x0040F9D0 / 0x00411250: CActionInputControl, not `<long>`. The callee 0x004112A0 is not CSimVoice.
- 0x0057F754 / 0x00580159: CHeroMarriageInfo, not `<long>`. 0x0057F78F is the CHeroMarriageInfo traits writer, not
  `Inventory_FormatItemTransferDetails`.
- 0x00614230 / 0x00615E10: CUIRegionInfo, not `<long>`. 0x00614000 / 0x00615920: CUIMiniMapMarker, not
  pair<CCharString,...>. 0x006159A0 is its traits reader, not `TransferOut_HeroTraits`.
- 0x0057F70F / 0x0057F6E9 share vtable 0x0124F33C (one enum instantiation). The ECreatureGeneratorGenerateType and
  ECategory labels can't both be right.
- 0x007E3820 is CPersistTraits<CDefString>::TransferIn (the landed 0x00F33480 tail-jumps to it), not CCharString.
- 0x00755D00 is CPersistTraits<std::pair<long,long>>::TransferBinaryIn (called from GFSerialiseMapBinaryIn 0x007565E0),
  not CCrimeWaitingForBribe.
- 0x0046B0D6 is CPersistTraits<pair<CDefIndex,CDefString>>::TransferBinaryIn (8-byte element, traits default {0,-1}).
- 0x0044FC00 is CDefString::TransferBinaryIn, not CPersistTraits<float>. 0x009D5060 is CDefString::SetString (or
  SetDefaultString), not CRegion::SetMiniMapRegionExitTextOffsetX.
- 0x00661287 is _STL::copy<_Rb_tree_iterator<ulong>, back_insert_iterator<vector<ulong>>>. 0x0066124B is __copy_aux
  (landed as `ConstructString`). 0x006611F7 is __copy (input_iterator_tag), not
  `MemoryAllocator_IterateAllocatedAreas`.
- 0x00758E00 / 0x0081BE30 / 0x00A33800 are _STL::__linear_insert, and 0x0083EB90 is LHHttp2::ResetDocumentRecv. None
  of the four is a TransferableVector<CReverbEnvironmentEntry>::Resize.
- 0x00C70120 / 0x00C70B00 are constructors of the no-RTTI classes with vtables 0x012BEDCC / 0x012BEDE0, not Resize.
  The base constructor 0x00C6E6A0 is not CConsoleCommandParameters.
- 0x007FAE34 / 0x0080BFEC are not functions (see Skipped).
