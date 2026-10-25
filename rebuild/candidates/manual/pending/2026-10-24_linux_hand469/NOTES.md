# Hand chunk 469 (CCreatureAction_KrakenHighAttackLoop .. DynamicListEntry): 29 of 40 byte-exact

All 29 grade RELOCATION_MATCH (gall.sh, grades.txt). Every test prints `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py).
Grade counts per row are in STATUS.tsv and probe/track.txt.

## What matched (29)

**CPersistTraits binary readers (CMemoryDataInputStream)**
- 00466c8e / 0046ba4b CPersistTraits<CBrainUpdateZone>::TransferBinaryIn (byte-identical twins): reads three longs. Shape: the landed 0x004671CA size-TU reader. Read is inlined, the widened `newPosition` is stored back, and each inline long-traits local is homed in the dead stream slot [ebp+8].
- 0046b545 / 004e9e0b CPersistTraits<CMusicEntry>::TransferBinaryIn (twins): reads ID, a def string, Gain and a byte bool. Shape: the same size-TU reader, with the float memcpy'd through a GPR and the bool normalised with `cmp byte ; setne`.
- 004e9642 CPersistTraits<CObjectAugmentationParticleSet>::TransferBinaryIn: reads a float, four handles and two def-string arrays. Shape: the same size-TU reader with five inline reads.
- 0067d830 CPersistTraits<CEngineGraphic>::TransferBinaryIn: speed-TU reader. Shape: the Type long is truncated to a byte store, and the bool fast path keeps its own `pop ; ret 8` tail.
- 0067df60 CPersistTraits<CDegradableInfo>::TransferBinaryIn. Shape: the engine graphic is one raw 14-byte Read into the member, so the stream fields are reloaded because of possible aliasing. The trailing check word goes into a dead local.
- 006dbe00 CPersistTraits<CComboMultiplierData>::TransferBinaryIn: reads two floats, then count × (attribute, multiplier) map inserts. Shape: a `!= 0` down-counter, the key and multiplier locals in the two dead argument slots, and the pair built right before each insert.
- 006782b0 CPersistTraits<list<CReactionMatch*>>::TransferBinaryIn. Shape: inline STLport clear and push_back (malloc'd 12-byte node), plus an inline factory switch and a virtual read.
- 006c88d0 CPersistTraits<COpinionDeed*>::TransferBinaryIn. Shape: `*value = new COpinionDeed(...)`, then value-returning inline reads assigned as `(*value)->X = Read()` so that `*value` is re-read after each read.
- 00614af0 CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferBinaryIn. Shape: `*value = CUIMiniMapMarker()` (a default temporary), then six inline reads, each local in the dead value slot.
- 00614da0 CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferBinaryIn. Shape: a zeroed record copied with `rep movsd`, then ten inline long reads with duplicated `store ; pop ; ret 8` tails.
- 006ba8b0 CPersistTraits<CFaction>::TransferBinaryIn: reads a text blob and re-parses it with TransferIn. Shape: an inline long-read helper with its own local keeps the length in a register. A CArray<char> zero-filled through STLport uninitialized_fill_n (begin copy spilled over the `rep stos`) gives the end pointer.
- 009950f0 CDataInputStream::Read3DVector (catalog label is propagated): the landed Read2DVector with a third coordinate.

**CPersistTraits text readers / writers (CPersistContext)**
- 00403ff0 CPersistTraits<bool>::TransferIn. Shape: `token == "TRUE"`. The implicit CCharString temporary is bound to the case-insensitive operator== and built conditionally under each `||`; the conditional destruction is tracked in flags 1/2/4/8.
- 00580194 CPersistTraits<CHeroMarriageInfo>::TransferIn. Shape: a zeroed temporary copied with `rep movsd` and a block-scoped empty bool traits object. A same-TU noinline stand-in for 0x00403FF0 makes `this` share the parser slot with the closing symbol temporary.
- 00686240 CPersistTraits<CAppearanceModifierGraphics::CEntry>::TransferIn. Shape: a block-local float traits object whose real TransferIn body is in the TU (VC7.1 then homes its `this` on the value slot), and an inline by-value TransferInLong helper.
- 006ba480 CPersistTraits<CFaction>::TransferIn. Shape: `if (!parser.NextItemIs("..."))` early returns through implicit (lea-passed) temporaries. Each count and its loop are a block, so their frame words are reused. The discarded 8-byte insert result gives `and esp,-8`.
- 0040fa20 CPersistTraits<CActionInputControl>::TransferOut, 00614080 CUIMiniMapMarker::TransferOut, 00614290 CUIRegionInfo::TransferOut, 006c7960 COpinionDeed*::TransferOut. Shape: the landed 0x0057F78F text writer. Each long goes through an inline long-traits TransferOut copied into the dead `out` slot, and each float through a float traits temporary in that slot. In 006c7960 the bools use one merged `+= "TRUE"/"FALSE"` call.

**CombatDialogueManager**
- 008af2b0 Init: fetches the def, then inserts every speech group with a zero NextAllowedFrame. Shape: the def name is an implicit temporary. Each `{group,{0}}` pair is built in one 8-byte slot (zero in ebx), giving `and esp,-8`.

**AI / creature actions**
- 0091a9b0 CAIStateGroup_WorkBase<CAIStateGroupBase>::AcquireSeller: gathers shop-keepers (TC 0x90) within 20 units and draws random free ones into a type-8 interaction. Shape:
  - A same-TU noinline FindThingsInRoughArea stand-in keeps the list header in esi.
  - A C3DVector user copy ctor makes the filter copy reload from the frame.
  - The box is a named local built before the filter temporaries.
  - The list ctor self-links a local node before storing it.
  - The random index uses a two-arm `index = r % range` / `index = 0`.
- 00924720 CCreatureAction_KrakenLowChargeLoop::FinishAction (catalog vfunc_4). Shape: the landed siblings, with the target fetched into a local first (retail's fetch order).
- 008ef400 Coin::UpdateSound. Shape: the GetTC out pointer is neither null-initialised nor tested, and CBaseIntelligentPointer is packed to 11 bytes.

**UI**
- 00530b00 / 00530da0 NUISystem::CComponent::MakeUnavailableDynamically / MakeAvailableDynamically (catalog "DynamicListEntry"). Shape: the qualified MakeUnavailable/MakeAvailable call made `__forceinline` (VC7.1 does not auto-inline the loop), then the colour fades.

**Entity scripts**
- 00efab40 Cwelcome_guy::Main: takes Me under scripting, walks it to the hero, then each frame waits until the hero is within squared 2D distance 3 and turns Me to face the hero. Shape:
  - The CMeleeThunder handle model with an implicit plain-inline handle dtor.
  - Both wait-loop bodies end in an explicit `continue`, so the dtor expands at every exit; in-body `#pragma inline_depth(0)` outlines the two hero-wait returns.
  - Closeness is an inline `GetDistanceSquared2D(const C3DVector&, const C3DVector&)` with dx/dy locals.

## Parked for Fable 5.1 (11)
- 00924930 CCreatureAction_KrakenHighAttackLoop::FinishAction DIFFER(346v344): retail homes the looked-up kraken in the LowerBound key word (`sub esp,120h`) and keeps the alive-test 1 in cl; ours gives it its own word (`sub esp,124h`, `mov ecx,eax ; mov al,1`) | tried: sibling model, direct out var, key-param address (3 grades) | next: a block-scoped GetInterface key shared with the kraken out var (same-TU noinline LowerBound stand-in)
- 009e84d0 CPersistTraits<C3DVector>::TransferIn DIFFER(867v917): retail jump-threads each member-name null test into two compare chains (`cmpsb ; je`, `test al,al ; je`) and keeps the discarded ")" compare; ours materialises `sete al`, re-tests and drops the ")" result | tried: plain inline CCharString::operator==, noinline operator== stand-in (845, slots swap), ternary operator== (881) (3 grades) | next: lane gap: inline budget / jump threading of CCharString::operator== (as 004112a0)
- 009e81e0 CPersistTraits<C2DVector>::TransferIn DIFFER(701v750): same as 009e84d0. Retail keeps name.PStringData in esi and jump-threads the null test; ours keeps it in edx and re-tests a `sete` result | tried: nested if (695), early-return "." (701, frame matches), const name, plain inline == (693) (4 grades) | next: lane gap: inline budget (CCharString::operator==)
- 0057f9e9 CPersistTraits<CHeroMarriageInfo>::TransferBinaryIn DIFFER(409v413): first-UID and State chunk-length checks: retail `mov ecx,[ebx+14h] ; cmp ecx,N`, ours `cmp dword ptr [ebx+14h],N` (the second UID and the bool agree) | tried: traits form, a named default local, value-returning reads (3 grades) | next: lane gap: register-vs-memory compare choice (likely a compiler-build artefact); last idea: a per-read `long remaining` local
- 006dc6a0 CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn DIFFER(437v447): retail keeps `value` in ebp and caches the two element reads in edi/ebx (pre-loop loads from their arg-slot homes, reload after the overflow call); ours keeps value in ebx and re-reads the elements from their slots | tried: traits form, value-returning reads, same-TU noinline _M_insert_overflow 0x006DB8C0 stand-in (frame now matches), loop-carried read targets (4 grades) | next: a register-held `vector&` or an inline push_back helper taking the vector by reference
- 004112a0 CPersistTraits<CActionInputControl>::TransferIn DIFFER(584v705): retail expands CCharString::operator== → CBasicString::operator== (down to a Compare call) at the first three symbol tests and calls 0x004115A0 for the closing ")"; ours expands only two and then calls operator== out of line | tried: plain inline with Compare body (628), forceinline CCharString== (610), forceinline both (751), Compare declared only (584) (4 grades) | next: lane gap: VC7.1 inline budget
- 005c0a98 CPersistTraits<NInventory::CItem>::TransferBinaryOut DIFFER(365v369): register allocation only. Retail keeps value in ebx and the augmentation index in edi, reloads stream from its slot, and homes the presence bool and name in the dead value slot; ours keeps stream in edi and the index in ebx, and homes them in [ebp-8] | tried: flat body (377), nested CCountedPointer traits (380), nested CPerItemData traits (365) (3 grades) | next: the 005c0e81 `long itemType` local (1 grade left)
- 005c0e81 CPersistTraits<NInventory::CItem>::TransferOut DIFFER(585v587): loop element address only. Retail CSEs the byte offset (`mov ecx,i ; shl ecx,2 ; cmp [ecx+eax],0 ... push [ecx+eax]`); ours CSEs the whole address (`lea eax,[eax+ecx*4]`) | tried: g1 (584), long index, `long itemType` local (fixed the tail) (3 grades) | next: index through an inline `operator[]` returning a reference, or a base-pointer reload per access
- 007ea2f0 NTargeting::GetTargetingPositionOnThing DIFFER(351v411): return-block layout. Retail keeps a separate copy + epilogue for helper hits 2 and 3, and hits 1 and 4 jump to the block the fallback stores into; ours merges all four hits into one copy block and forwards the fallback into the hidden result (thing ebp, result ebx / interp edi vs retail result edi / interp ebx) | tried: four `if (GetHelperPoint(...)) return pos;` (natural form), `||` single return (339, fallback temps exact), 2 more return shapes (4 grades) | next: per-hit distinct named result locals, or a helper returning bool with an out C3DVector
- 008af990 CombatDialogueManager::GetBestSpeechForCreature DIFFER(687v688): (1) the target's CTCEntityEvents pointer is coloured eax (`mov cl,[edi+28h] ; xor eax,eax`, then `mov ecx,eax`) vs retail ecx (`mov al,[edi+28h] ; xor ecx,ecx`, `mov ecx,[eax+4]`); (2) both GetEvent calls put the out pointer in the dead find-iterator word and the empty CEventFilter_None in the dead `target` slot, where retail puts the out pointer in the `target` slot and overlaps the empty filter on the live `creature` slot; (3) the will-seen-used block swaps ecx/edx/eax | tried: about 30 ungraded variants (named, wrapper and helper filters; out-var scoping and order; zero-init; split `&&`; manual vs inline first IsSpeechGroupAllowed; the real GetEvent body as a same-TU noinline stand-in, which moves the filter to the creature slot but grows the frame to 10h/14h; named message filter plus manager local; list-ctor shapes) (1 grade) | next: give the same-TU find 0x008AEF20 and both GetEvent instantiations real noinline bodies together, so the slot packer sees the whole escape picture
- 00c95580 vorbis_book_init_decode DIFFER(923v910) (libvorbis 20030909 sharedbook.c; catalog "CTCWound::CWound::Initialize"): retail passes bitreverse 0x00C954F0 its argument in eax (`mov eax,[ecx] ; call`); ours must use a VC7.1 convention (`mov ecx,[ecx]`), which re-colours every call site and shifts the frame words by one | tried: the sharedbook.c body verbatim as `__fastcall` with MSVCR71 imports (1 grade) | next: lane gap: LTCG custom convention (register argument)

## Skipped
- None. 00c95580 was first set aside as LTCG. It is now parked with a verbatim libvorbis reference body: one function, no second body inside the row.

## Label corrections
These manifest/catalog labels are propagated and wrong:
- 009950f0 is CDataInputStream::Read3DVector, not CPersistTraits<CBrainUpdateZone>::TransferBinaryIn.
- 009e81e0 is CPersistTraits<C2DVector>::TransferIn, not CPersistTraits<CBrainBehaviour>.
- 004112a0 is CPersistTraits<CActionInputControl>::TransferIn, not CPersistTraits<CSimVoice>.
- 006dc6a0 is CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn, not CPersistTraits<CHeroStatIncreaseData>.
- 00686240 is CPersistTraits<CAppearanceModifierGraphics::CEntry>::TransferIn (the entry, not the container).
- 00530da0 / 00530b00 are NUISystem::CComponent::MakeAvailableDynamically / MakeUnavailableDynamically, not "DynamicListEntry::MakeAvailableWithCallbacks / MakeUnavailableDynamic".
- 00924720 / 00924930 "vfunc_4" are CCreatureAction_KrakenLowChargeLoop / KrakenHighAttackLoop::FinishAction.
- 007ea2f0 is NTargeting::GetTargetingPositionOnThing, not Creature::GetTargetFirePoint.
- 00c95580 is libvorbis vorbis_book_init_decode, not CTCWound::CWound::Initialize.
  - Its neighbours: 0x00C94EE0 _ilog, 0x00C94FD0 _make_words, 0x00C951B0 _book_unquantize, 0x00C95440 vorbis_book_clear, 0x00C954A0 vorbis_book_init_encode, 0x00C954F0 bitreverse (LTCG eax), 0x00C95560 sort32a.
- Callee identities found in this chunk:
  - 0x008AF020 / 0x008AF0A0 are CTCEntityEvents::GetEvent<NEntityEvents::CEventHitThing, CEventFilter_None> / GetEvent<CEventExpressionPerformedTo, CEventFilter_None>. The manifest has "GetEvent<CEventHitBy,...>".
  - 0x006A4E40 is the creature health fraction (the manifest has CParticleSystem::GetNormalisedTime).
  - 0x00701840 is COpinionReactionManager::GetThingAttitudeToPlayer.
  - 0x008AFD00 is map<unsigned long, TSpeechGroupInfo>::insert.
  - 0x0091A750 is CThingSearchTools::FindThingsInRoughArea<CFilter_Bind2<CThingFilter_HasTC, CThingFilter_In2DRange>> into list<T*>.
  - 0x007E7300 is the scripted-thing handle's MoveToThing.
