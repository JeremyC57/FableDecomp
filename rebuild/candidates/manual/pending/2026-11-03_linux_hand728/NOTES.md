# Chunk 728: 2 RELOCATION_MATCH, 2 parked, 2 not attempted

Grade with `FABLE_HDR_OVERLAY=$S/w22/chunk728/hdr`. The overlay was rebased on the Codex pass 13 working tree on 2026-11-03.
Pass 13 changed CPersistContext.h, CPlayer.h and fable_map.h. `base/` holds the repo copies the overlay was diffed against.
hdrcheck2 reports `default=same` for all 11 CHANGED headers (work/hdrcheck.txt).

Payload: 0x0074B770, plus 0x00C267E0 with `/EHsc`. vland on the final overlay gives WINS: 2, RELOCATION_MATCH, tests PASS (vland.out).
SIBLINGS.tsv covers 84 landed TUs. Each one defines a macro whose branch this overlay changes:
CIENGINE, CMAINGAMECOMPONENT, PLAYER, CTCBASE, CTCWIFE, CTHINGSEARCHTOOLS or STLP_VECTOR. CTCCREATUREGENERATOR,
CTCSHAPEMANAGER and STLP_SET have no landed includer. Graded with the overlay, all 84 are 3 MATCH + 81 RELOCATION_MATCH, tests PASS.

## Matched
- 0x0074B770 CTCCreatureGenerator::InitialiseCreatureGenerator (395 B, 14 attempts).
  - Levers: `GetParentThing().GetPos()` (the inline CTCBase accessor) fixes the register order (dl 7 -> 0).
  - Marker lookup: `GetThingSearchTools().GetAllThings<CThingMarker>(CThingFilter_HasDefGlobalIndex(index), markers)`
    (retail instance 0x0074D940).
  - The inline FableWin-only `CPlayer::GetPPlayerCharacter()`.
  - Global 0x013824AC is the descriptive `g_CreatureGeneratorNavCheckEnabled`.
- 0x00C267E0 CStreamLoadHandler::Process (577 B, libpk, /EHsc, 10 attempts).
  - It does not belong with CHandleBasedMemoryPool. It is slot 0 of the one-slot vtable 0x012B4018, which ctor 0x00C26A30 installs at
    +4. PKCWorkerThread::ProcessNextRequest 0x00C26B20 calls it for each popped request. It only calls the pool's
    GetPointerFromHandle / Lock / Unlock.
  - All libpk names are descriptive (no PDB covers 0x00C08AE0..). Twin: 0x00C26DE0.
  - Lever: `char* block = GetPointerFromHandle(h); header = *(CSampleHeader*)(block + offset)` gives retail's add order.

## Parked
- 0x006D2BD0 CTCWife::OnSerialise: DIFFER(1770v1770), dl=8, 21 attempts.
  - The catalog's "CThingNPC::TransferLoveState" is wrong; see NAME_FIXES.
  - Shape levers, in order of effect:
    1. Cached end iterators (dl 181 -> 58).
    2. The two-argument `CPersistContext::Transfer(name, value)` forwarding inline to the three-argument Transfer with
       `GetDefaultVal<T>()` (58 -> 8). VC then builds the CCharString name before it pushes the default, the value and the name.
  - Since Codex pass 13 the repo header declares both members. The row defines the bodies at TU level, as the landed
    0x00CDC070 does, so CPersistContext.h is no longer in the overlay.
  - Residue: retail stores the BooleanHusbandAppearances loop's `false` default once, before the loop (`mov [esp+0xf], bl`).
    Ours stores it inside the loop just before the call; the jmp/nop alignment padding follows from that.
  - Tried without effect:
    - GetDefaultVal bodies: non-const local, const&, const local, a bool specialisation, a `const T` return.
    - An int counter, a reference local, a named CCharString.
    - A const or non-const default local, inside or outside the loop.
    - A literal `false` third argument (dl 71: the pushes move before the name).
    - /Ox, optimize("t"), a visible Transfer body.
- 0x0074BC20 CTCCreatureGenerator::DrawDebugInfo: DIFFER(1415v1290), dl=308, 12 attempts.
  - The callees match in order up to the line primitives. That covers GetPos, resize 0x00443F90, the inline
    GFIntToWideString x2, the three wide operator+, PeekComponent 0x00686D60, the CEnginePrimitiveText ctor 0x004C3120 and
    CIEngine slot 23 SetPersistentPrimitive.
  - The text block now matches up to register choice. Levers: named `text_pos`, then `const CFontBank& font =
    PeekComponent().PeekDebugFont()`, then `unsigned __int64 id`. Those come after the text string, and the colour is a temporary.
  - Residue 1 (the size): retail calls the CEnginePrimitiveLine ctor 0x004C3090 out of line for the two open-position lines
    and expands it inline for the two hidden ones. Ours expands all four.
    - This is not an inline budget: a diagnostic with 8 sites still expanded all of them.
    - CShape::EditorDrawHighlightPoints 0x007617A0 (expanded) and NShape::CTrack 0x00761620 (called) show the same split
      inside one TU.
    - Named colour locals, FABLE_RETAIL_CRGBCOLOUR / C3DVECTOR and an inline Text ctor did not change it.
  - Residue 2 (1 line): CEnginePrimitiveHandle::Clear's release stores `Data = 0` twice in retail, once inside
    `if (Data)` and once after. The shared CIVCountedPointer::Release stores it once. Changing that Codex pass 11 header
    would touch its landed users, so it was left alone.
- 0x0074BC20 also needs the new data name `g_CreatureGeneratorDrawDebug` 0x013BAD58: a .bss bool, never written, read only
  by this row and CTCCreatureGenerator::FrameUpdate 0x0074C226.

## Not attempted (time): CTCWife::GlobalUpdate 0x006D0830 (2584 B) and FrameUpdate 0x006D1E30 (3484 B)
Names: Ego_r ?GlobalUpdate@CTCWife@@UAEX_N@Z 0x00705510 and ?FrameUpdate@CTCWife@@UAEXXZ 0x007072E0. The Ego_r order
GlobalUpdate / FrameUpdate / OnSerialise matches retail.

GlobalUpdate is a state machine over:
- WaitingForDivorce +0x29
- WaitingForFadeOut +0x27, which also needs the CMainGameComponent @0x013B86A0 +0x28 display flags +0xBC / +0xC9 / +0xC8
- WaitingForSex +0x2C
- WaitingForSexCutscene +0x2D
- WaitingForFadeIn +0x28
- WaitingForDowry +0x2B

GlobalUpdate callees:
- 0x00661FF0 on the parent: `== 2` plays the wedding movie.
- GFPlayFullScreenXBoxMovie 0x006286F0: "Data\Video\5_wedding_comp.xmv", edx 0x1B, 10 stack args.
- The script hook "Hook_Fresco_11_Wedding" through [[0x013B86A0]+0x24]+0x38, vslot 0x450/4.
- The InitWife sequence and world calls:
  - InitWifeNonpersistentState 0x006D03E0
  - InitWifeAtEndOfMarriageFrescoStartFadeIn 0x006CF3B0
  - GetWorld 0x00686A70 -> +0x1C 0x006BB720 (fade, 0.0416667f * def +0x30)
  - GetThingSearchTools 0x00686D80 -> 0x006D4600 (a vector of things); per thing GetTC 0x0040F020 (TC 0x22) -> 0x0065935E
  - PlayerCharacter GetPItem 0x00A01B50 on parent +0x168 -> 0x00666EF0(3.0f); PeekWorld 0x00686A80 -> PeekMyPlayer 0x0049D9D0
    -> 0x00487DC0
  - Two fpatan facing blocks (0.159155f = 1/2pi, wrap with fmod 0x00BFEB28) into vslot 0x108 of TC 2
  - GetWorld()+0x34 vslot 9 -> 0x0069A990
  - 0x00687540 (0x42 message, ConstantFPS 0x01375550)
  - 0x00434C90
  - SendPauseEvent 0x006CEBF0
- WaitingForSex: 0x0049319C on [0x013B8790]+0x18, then GetPlayerManager 0x00686D40 -> 0x00449970 -> AddMode 0x006345C0 (0x1B),
  then CMainGameComponent world frame 0x00416225.
- The sound / divorce branch: TC 0x10 -> 0x0066BED0 / 0x0066AE10; GetWorld()+0x60 -> 0x004AB8B0 / 0x00493B5B; TEXT_GUI_WIFE_MARRIED_RECEIVED_DOWRY.
FrameUpdate was not analysed.

## Overlay changes (hdr/)
- **New** engine/CEnginePrimitiveThemeChannelDesc.h, CEnginePrimitive.h, CEnginePrimitiveLine.h (inline PDB ctor),
  CEnginePrimitiveText.h (ctor declared, 0x004C3120), CEnginePrimitivePrepareInfo.h (0x74, Engine +0x34) and
  CThingFilter_HasDefGlobalIndex.h. Members, sizes and signatures come from Ego_r.
  - The enums EEnginePrimitiveType / EEngineInterpolationSet / EEnginePrimitiveBlendMode / EEnginePrimitiveBlendOp stay MS
    forward-declared, because no enumerator names are on disk. Line Type 6 and Text Type 5 are the values retail stores.
  - Heads-up: agent724b's overlay also created engine/CEnginePrimitive.h (holding the Line class) and
    CEnginePrimitivePrepareInfo.h. The coordinator should keep one; mine splits Line/Text/ThemeChannelDesc into per-class headers.
- engine/CIEngine.h (FABLE_RETAIL_CIENGINE): slots 17-23. Refresh, CheckPrimitiveVisibility, GetPrimitiveDrawDistanceFactor,
  AddPrimitive, ClipSphere, ClipBox and SetPersistentPrimitive (pure; CEngine 0x00B23BC0).
- engine/CTCBase.h (FABLE_RETAIL_CTCBASE): public inline GetParentThing() (QAE/QBE). Protected declarations GetWorld 0x00686A70,
  GetThingSearchTools 0x00686D80, PeekThingSearchTools 0x00686D90 and PeekComponent 0x00686D60.
- engine/CMainGameComponent.h (FABLE_RETAIL_CMAINGAMECOMPONENT): PeekDefinitionManager 0x00415FBC. PDebugFont is typed
  `CCountedPointer<CFontBank>` with the inline FableWin PeekDebugFont(). The default path keeps the opaque bytes.
- engine/CTCCreatureGenerator.h (new FABLE_RETAIL_CTCCREATUREGENERATOR): the polymorphic class at retail offsets, sizeof 0xA8.
- engine/CTCWife.h (new FABLE_RETAIL_CTCWIFE): the polymorphic class at retail offsets, sizeof 0xEC.
  It turns FABLE_RETAIL_CTCBASE and FABLE_RETAIL_DEF_CLASS_BASE on for its includers, because the landed _$E1 0x0121E210
  defines neither. Without that the branch hits CDefPointer.h's #error, and Wine cl hangs.
- engine/CTCShapeManager.h (FABLE_RETAIL_CTCSHAPEMANAGER): StatGetInterfaceType.
- engine/CPlayer.h (FABLE_RETAIL_PLAYER): the inline GetPPlayerCharacter declaration. engine/CThingPlayerCreature.h defines it.
- engine/CThingSearchTools.h (retail branch): the GetAllThings<T,Container,Filter> member template.
- stlport/fable_containers.h (FABLE_RETAIL_STLP_VECTOR): vector::reserve declaration.
- stlport/fable_map.h (new FABLE_RETAIL_STLP_SET): set begin()/end()/size() const.

## Open questions
- RELOC_CONFLICT: retail has two byte-identical CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound copies, 0x004365B0 and
  0x0040F020. InitialiseCreatureGenerator's GetTC calls 0x004365B0; the landed 0x00635080 binds the same symbol to 0x0040F020.
  One of them is probably another CVectorMap instance with an 8-byte element, and the landed binding should be checked.
- The libpk owner type is spelled CPanel in one landed binding and CPooledResourceManager here.

## GOTCHAS (one line each)
- A TU whose headers hit a `#error` (fatal C1189) hangs Wine cl just like a missing include (C1083). Example: a retail
  branch that includes engine/CDefPointer.h without FABLE_RETAIL_DEF_CLASS_BASE. Pre-check siblings with `cpp` before vland.
- VC7.1 can keep an inline constructor out of line at some call sites of a function and expand it at others. Seen with
  CEnginePrimitiveLine in DrawDebugInfo 0x0074BC20 and NShape::CTrack 0x00761620. It is not a size budget, so don't chase it
  with more inline code.
- A two-argument CPersistContext::Transfer(name, value) that inlines into the three-argument one (`GetDefaultVal<T>()`)
  is what makes VC build the CCharString name before the default/value pushes. A literal third argument puts the pushes first.
- In a DrawPreparePrimitivesForRendering-style call, a named `const CFontBank& font = PeekComponent().PeekDebugFont()` and a
  named id local move the colour-temporary stores after the pushes, as retail has them.
