# Chunk 517 lessons

## GOTCHAS one-liners
- Wave 25 / chunk 517: if a call to an out-of-line template COMDAT (FindMostRecentMessage, GetPBestThing) has the first-word store of a bound filter in the wrong place, give the TU a same-TU `__declspec(noinline)` stand-in under `#pragma auto_inline(off)` that reads `this` and the filter's first word. VC then hoists the store as retail does (0x0092EDC0, 0x009112A0, 0x00917050).
- Wave 25 / chunk 517: a GetPBestThing stand-in whose filter has data (HasTC 0xB2) must read `filter.Type`, or VC drops the store (0x009652C0).
- Wave 25 / chunk 517: a Bind4 filter chain of temporaries only matches when written inline in the call (`MessageEventManagerOf(PeekWorld())->FindMostRecentMessage(a && b && not1(c) && d)`). A named filter local drops the temporaries' stores (0x00958770).
- Wave 25 / chunk 517: Bind2 needs a named filter local AND a named `const CMessageEventManager* manager` local to get the retail push/PeekWorld order (0x0092EDC0 / 0x0094BFC0).
- Wave 25 / chunk 517: an inline `bool HasTC` written `if (bit) return true; return false;` gives `test dword` inside an if and `shr ; and al,1` inside a return. A pick-form PeekTC (end/hit ternary) leaves the component in ecx, while the out-pointer GetInterface leaves it in eax (0x00939E70).
- Wave 25 / chunk 517: `const CCountedPointer<T>& p = obj->Member;` gives `add eax,off ; mov eax,[eax]`; a by-value copy does not (0x0094C010).
- Wave 25 / chunk 517: to get the retail 2D-distance code (X squared first), write `offset.Y*offset.Y + offset.X*offset.X` on a C3DVector difference. An anonymous-namespace `??R...@?A0x` leaf needs auto_inline(off) plus an external caller to be emitted (0x008E2E40).
- Wave 25 / chunk 517: a GFRandom switch with a zero `mode` local declared after the choice gives `xor ecx,ecx ; sub eax,ecx` (0x0090F7D0).
- Wave 25 / chunk 517: `if (A) return f(x); if (B) return g(y); return f(x);` (three plain returns) makes VC tail-merge the two f paths and hoist the shared `push` above each branch, like retail. The `!A && B` form does not (0x009E2C80 CFontManager::GetFont).
- Wave 25 / chunk 517: a `_wsplitpath(path, drive, 0, 0, 0)` row with a cookie frame needs `// Flags: /GS /Zc:wchar_t`. /GS alone mangles `G` instead of the PDB's `_W` (0x009E4C70). Old "/GS parks" may only lack /Zc:wchar_t.
- Wave 25 / chunk 517: q.py ignores `// Flags:` lines (gall.sh, finish.py and vland read them). Always pass `--flags` to q.py/gr.sh by hand.
- Wave 25 / chunk 517: STLport `__find` in the CBankFileAsync TU is `__fastcall` (/Gr) with the tag by reference. A same-TU body homes the tag in the dead argument slot but also lets VC keep `_M_finish` in edi across the call; retail reloads it. Open gap (0x009D5A30).
- Wave 25 / chunk 517: STLport `list::_M_insert_dispatch<const_iterator>` = the landed copy-ctor TU (0x009C6F70) plus iterator `*`/`++`/`!=` and `insert(pos, x)` through `_M_create_node` with a placement `_Construct`. That gives the `lea ecx,[eax+8] ; test ecx,ecx` null test (0x009C69C0, first grade).

## Review at landing
- RELOC_CONFLICT (fixed here): 0x006A4D00 was declared `?GetPosition@CThingCreatureBase@@QBEABVC3DVector@@XZ`, a symbol the landed 0x008E07F0 binds to 0x004C73D0. 0x00914600 now uses the existing `?GetPosition_006a4d00@CThing@@QBEABVC3DVector@@XZ` (landed 0x00911710). The landed 0x008E07F0 spelling for 0x004C73D0 looks wrong: the PDB name is CThing::GetPos.
- Stand-in bodies: 009112a0, 0092edc0, 0094bfc0, 00958770, 00917050, 009170b0 and 009652c0 each define a same-TU noinline template body (FindMostRecentMessage<Filter> / GetPBestThing<...>). The body calls an extern `FindMostRecentMessage_Sink` / `GetPBestThing_Sink` that has no retail address. At link, the template symbol must bind to the retail instantiation and the stand-in COMDAT body must be dropped. The retail instantiations are 0x008D46D0 (Bind2, used by 0092edc0 and 0094bfc0), 0x00911240 (Bind3), 0x00959040 (Bind4), 0x00916F30 / 0x00916FC0 (GetPBestThing<CIsFreeLogPile / CIsFreeBonfire>) and 0x00965180 (GetPBestThing<HasTC>). The same applies to 0099f3d0's same-TU `CBasicString<char>::Insert` specialisation (retail 0x009A06B0), whose placeholder body must never stand in for the real one.
- 0x006AF290 is bound as IsForThing / IsOriginatedByThing / IsTargettedAtThing. The PDB says `??0CMessageEventFilter_IsTargettedAtThing@@QAE@ABVCThing@@@Z` (used here); IsOriginatedByThing's ctor is 0x006AF2B0. vland warns RELOC_NAME.
- 0x008D46D0 (FindMostRecentMessage<Bind2>) is bound QAEPAV non-const; the PDB has QBEPBV (used here).
- 0x00939E70 is bound QBE/QAE by its landed callers; the PDB has ABE (this row emits ABE).
- 0x008FCFA0 `?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ` is labelled GetPThingWorld (RELOC_NAME).
- 0x00912700 / 0x009130D0 have invented binding spellings (ExecuteFlyingPath). This batch uses GetRandomFlightPos / AIAction_RandomFlyToPos.
- 0x0093A3C0 FindMostRecentDeed: the landed version uses `long deedType`, the PDB EOpinionDeedType.
- 0x00497890 GFROR13 is bound both YIJK and YIKK; the PDB is YIJK.
- 0x0099EBF0 CCharString(const char*, long) has several bindings (differ row 0091c0d0 only).
- 0x009C69C0: the landed 0x009C72B0 binds a non-std short spelling `?_M_insert_dispatch@?$list@UCPendingWrite@@@@...`; the landed 0x009C6F70 binds the full std template symbol this row emits.
- 0x009E2C80 has 9 spellings in reloc_bindings; this row emits the PDB `?GetFont@CFontManager@@QAE?AV?$CCountedPointer@VCFontBank@@@@ABVCCharString@@@Z`.
- 0x009E51C0 is bound `?FindSymbol@CSymbolMap@@QAE_NJ@Z`; the PDB name SymbolExists(const CCharString&) const is used here. 0x009E5120 is bound both Lookup(long) and GetSymbolSLONG (the PDB name, used here).
- 0x00A62920 CCriticalSectionLock ctor has 3+ spellings; the PDB is `??0CCriticalSectionLock@@QAE@AAU_RTL_CRITICAL_SECTION@@_N@Z` (differ 009d5a30).
- 0x009D6E80 is landed as descriptive `Find_009d6e80` with a by-value tag. Retail pushes the tag's address, and the STLport template compiles to the identical body (`??$__find@PAPAVCBankFileAsync@@PAV1@@std@@YI...ABUrandom_access_iterator_tag@0@@Z`).
- Earlier conflicts carried from this chunk: 0x00403C60 (MemCmp_Unsigned16, 0099b0f0), 0x009E9900 (CMemoryFile append copy), 0x0099A300 (`??1CBaseClassNonCopyable@@MAE@XZ` labelled CBaseClass::~CBaseClass, RELOC_NAME).
- 009e4c70 uses `__wsplitpath`, which vland flags RELOC_ALIAS (the MSVCR71 import thunk). That is expected.

## Why earlier attempts stalled
The re-queued parks were graded under the QFE lane and before these shapes were known: same-TU template stand-ins, named
filter/manager locals, inline accessors, the three-return tail-merge form, /Zc:wchar_t with /GS. Several matched on the
first RTM grade once the shape was right. The remaining 8 are register-choice, block-placement, final-bool folding or
IPA-purity residues, and an int3 span artefact.
