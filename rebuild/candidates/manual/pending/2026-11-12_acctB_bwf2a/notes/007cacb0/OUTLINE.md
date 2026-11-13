# 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate -- OUTLINE (identity scout, bwf2, 2026-11-12)

Built on prior/scout/outline.md (copied below, sections 0-8). I re-read the whole retail listing (dis_all.txt, 889 insns) against it:
every block address, call target, TCI key / UsingComponents word+bit, frame slot and push count in sections 1, 3 and 4 checks out.
Corrections / additions found in this pass (they override the text below where they differ):

C1. Frame recount (verified by counting pushes): fish-arm operator+ results t1..t11 sit at +0x54,+0x50,...,+0x2C (t1 = desc+L".  " at
    +0x54, t11 = the final `+ L"."` at +0x2C); GetDescription result +0x58; GFIntToWideString(length) +0x5C; GFIntToWideString(weight)
    +0x64. Construction order = weight string, length string, description, t1..t11 (right-to-left argument evaluation of ONE expression);
    the 14 dtors run +0x2C,+0x30,...,+0x5C,+0x64. All "+x" offsets are esp after the 4 pushes (ebx/ebp/edi/esi), frame 0x74.
C2. ForceClear box copies: retail reads BOTH copies from &InitialAreaItemTook (`lea eax,[esi+3Ch] ; mov edx,eax` .. second copy `mov edx,
    [eax]`), i.e. source = Initial for both. The landed same-TU PrepareToShowItems 0x007CA670 writes this as two statements
    `CurrentAreaItemTaking = InitialAreaItemTook; OldAreaItemTaking = InitialAreaItemTook;` -- try that FIRST (same TU, matched); the
    scout's chained `Old = Current = Initial` probe gives the same order and is the fallback.
C3. 0x004BBE10 is ?Init@CRightHandedSet@@QAEXABVC3DVector@@0@Z (Ego_r 0x004BF6E0 + FableWin 0x01D0D780/0x018002BE; NO PDB has a
    CRightHandedSet::Set). Landed file CRightHandedSet_Set_004bbe10.cpp (leaf "Set") and the local `Set` of PrepareToShowItems
    0x007CA670 are NAME_FIX candidates. PrepareToShowItems (same TU) EXPANDS it, GlobalUpdate CALLS it: inline body + VC7.1's
    per-function inline budget (GlobalUpdate is 2974 B). Same story for GFLimit<float,float,float> 0x0048DAE0 (inline template in
    engine/GFLimit.h, called here), CSoundInit() 0x0041C930 and the first IS_THING_ALIVE(PContainer) 0x004CC340. Write them inline
    first and let the budget decide; only if c2 expands one, switch to the declaration-only spelling (hdr_prior CRIGHTHANDEDSET_INIT,
    ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE_NOINLINE is per-TU and would make BOTH PContainer sites calls -- wrong).
C4. operator+(const CWideString&, const wchar_t*) 0x0099BF30: Ego_r spells ??H@YI?AVCWideString@@ABV0@PB_W@Z; the landed row's leaf is
    ??H@YI?AVCWideString@@ABV0@PBG@Z (unsigned short). Use what fable_string.h declares (the build's wchar_t setting decides); flag at
    landing, do not fight it in the row.
C5. FableWin (debug, later Dinkumware build) COMDAT order right after GlobalUpdate 0x02726CD0: ?IsReceivingItems@CTCHeroReceiveItems@@QBE_NXZ
    0x027290B0, SetAsEnabled CTCDiggingSpot 0x027290D0, SetAsEnabled CTCFishingSpot 0x027290F0, and GetTC<CTCFishingSpot> 0x0272A710 /
    GetTC<CTCDiggingSpot> 0x0272A770 -- these helpers' first user is GlobalUpdate: the opening test is `if (!IsReceivingItems()) return;`
    (or the member read; same bytes), and both spot arms are `GetTC(&spot)` + `spot->SetAsEnabled(false)`.
C6. Display block: `OldAreaItemTaking = CurrentAreaItemTaking;` is an unconditional statement BEFORE `if (AmountThroughForAreaResize <
    1.0f)` (its copy is scheduled between the fcomp and fnstsw). GFGetInterpolatedValue calls 1/2 read interp straight from the member
    (`fmul [edi]`); calls 3/4 use the by-value copies at +0x64 (0x007CB746 / 0x007CB793) because the GetRenderTargetDimensions call
    intervenes. C2DExtentsI result slot +0x5C (push &+0x5C before each call, `ret 4` hidden-return thiscall).
C7. Header overlay: $W/hdr/ is EMPTY (the coordinator's re-base was not produced). $W/hdr_prior/ (older repo state) holds the needed
    opt-ins: CTCHeroReceiveItems.h _GLOBALUPDATE, CThingObject.h _HASPROPERTY, CRightHandedSet.h _INIT, CPlayerGuiDef.h _CONTAINERITEM,
    CTCInventoryItem.h _DESCRIPTION, CTCHeroStats.h _RENTMONEY, new CTCFish.h / CTCFishingSpot.h / CTCDiggingSpot.h / CTCGold.h /
    CTCHeroOnlineScoreboard.h. Re-base each onto the CURRENT repo header (e.g. repo CTCHeroReceiveItems.h now has _VSLOTS which
    hdr_prior lacks) -- full current copy + `#ifdef` additions only.

---------------------------------------------------------------------------------------------------------------------------------------
# 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate -- outline (scout, 2026-11-12)

Sources: retail listing `dis_all.txt` (889 insns, 2974 B); PDB name sets `ghidra_out/egor_pdb_names.tsv` (E) / `fablewin_pdb_names.tsv` (F);
landed rows under `rebuild/src/compiled/` (L); `rebuild/manifest/functions.tsv` (M). "PROBE" = compiled by me under VC7.1 RTM in
`scout/probe/` with `scout/pc.py` (compile-only, prints the leaf's disassembly). UNVERIFIED = not checked.

## 0. Identity / extent (verified)
- E 0x007C9230 + F 0x02726CD0 `?GlobalUpdate@CTCHeroReceiveItems@@UAEX_N@Z` (virtual, public, one bool arg, `ret 4`). Retail vtable
  0x0126EBFC slot 26 (rtti-vtables.tsv; xref: only reference is .rdata 0x0126EC64) = CTCBase slot 26 `virtual void GlobalUpdate(bool)`
  (rebuild/include/engine/CTCBase.h:105). Leaf: `?GlobalUpdate@CTCHeroReceiveItems@@UAEX_N@Z`.
- Extent right: int3 at 0x007CACA0..AF, last insn `ret 4` 0x007CB84B, int3 0x007CB84E/F, next fn 0x007CB850 SetItemAsPickedUp (landed).
  Start 16-aligned. One function, no cold code after the ret (every branch target is inside 0x007CACB0..0x007CB84D).
- Sizes: retail 2974 B / 889 i; E 3230 B / 939 i; F (debug) 7339 B / 1646 i (naming_stragglers/*.signatures.tsv).

## 1. Prologue / flags
- `sub esp,74h ; push esi ; mov esi,ecx` then `mov al,[esi+25h] ; test ; je 0x007CB847` (ReceivingItems), THEN `push ebx ; push ebp ;
  push edi` (shrink-wrapped: the early exit jumps to the main epilogue's `pop esi ; add esp,74h ; ret 4`).
- Saved order: esi (before the test), ebx, ebp, edi. Epilogues: `pop edi ; pop ebp ; pop ebx ; pop esi ; add esp,74h ; ret 4`.
- No /GS cookie, no EH frame (no `push -1 / fs:0` although CWideString / CCharString locals have dtors), no `and esp`, esp frame (/Oy).
- => no `// Flags:` line, no pragma. Same as every landed CTCHeroReceiveItems sibling (none has a Flags line or optimize pragma:
  grep of rebuild/src/compiled/00/7c/CTCHeroReceiveItems_*.cpp).
- Arg `bool` at [esp+88h] (after the 4 pushes) is never read. Frame 0x74 > 0x70, so no dead-param-home reuse (r3002 lesson).
- Three epilogue copies: 0x007CAF1B (ForceClear-and-empty exit), 0x007CAF93 (list-empty exit; its two flag stores are scheduled
  between the pops), 0x007CB844 (main).

## 2. Members (retail offsets = Ego_r minus 12 from PCurrentItem on; landed ctor 0x007CA830 / dtor 0x007CA8E0 / OnKill 0x007CA630 model all of them)
Items list +0x0C (node: next +0, prev +4, pair.first CIntelligentPointer<CThingObject> +8, pair.second bool +0x10), PickedUpItems +0x10,
PCurrentItem +0x1C, PersistCurrentItem +0x24, ReceivingItems +0x25, DisplayingItems +0x26, ForceClear +0x27, SendMessageW +0x28 (PDB
spelling), PContainer CIntelligentPointer<CThing> +0x29 (1-packed), AmountThroughForAreaResize float +0x34, FullSizeTimer long +0x38,
InitialAreaItemTook C2DBoxF +0x3C, OldAreaItemTaking +0x4C, CurrentAreaItemTaking +0x5C, OldItemRHSet CRightHandedSet +0x6C,
CurrentItemRHSet +0x84, ItemPrimitiveHandle CEnginePrimitiveHandle +0x9C, BackgroundPrimitiveHandle +0xA4; sizeof 0xAC.
Header debt: repo engine/CTCHeroReceiveItems.h models only Items/PickedUpItems (opt-in FABLE_RETAIL_CTCHERORECEIVEITEMS_PICKEDUPITEMS);
the worker needs an overlay opt-in with the rest (copy the member list from the landed OnKill / dtor rows; the handles must be the real
engine/CEnginePrimitiveHandle.h type with FABLE_RETAIL_CENGINEPRIMITIVEHANDLE so `.Clear()` expands).

## 3. Frame map (offsets = esp after the 4 pushes; "+x" below. Frame words +0x10..+0x83)
| slot | holds (first/last use) |
|---|---|
| +0x10 | `CWideString text` (default ctor 0x0099AED0 at 0x007CB013; `+=` / `=`; SetText arg; dtor 0x007CB6B6). Live only in the new-item block. |
| +0x14 | fish arm: CWideString weightUnits (GetTextBySymbol result 0x007CB154, dtor 0x007CB331). gold arm: `CTCHeroStats* stats` (GetTC out, NOT initialised: written only on HasTC hit 0x007CB49D, read 0x007CB573 / 0x007CB5D5). |
| +0x18 | fish arm: the 4 `CCharString` symbol temps (ctor 0x0099EBF0 `(sym,-1)`, dtor 0x0099EAE0 after each lookup), PeekTC key temps 0xDC / 0x1A, then the early copy of `fish->GetLength()` (0x007CB171 store, 0x007CB186 fld). gold arm: `long gold` (=0 at 0x007CB451, = GetGold() 0x007CB4EE, read 0x007CB577 / 0x007CB5D9). |
| +0x1C | fish arm: CWideString weightText (result 0x007CB124, dtor 0x007CB336) |
| +0x20 | fish arm: CWideString lengthUnits (0x007CB0F4, dtor 0x007CB33F) |
| +0x24 | fish arm: CWideString lengthText (0x007CB0C4, dtor 0x007CB348). gold arm: CCharString "TXT_OBJT_GOLD" temp (0x007CB5EE..0x007CB625). |
| +0x28 | c2 spill of ebx = `const CPlayerGuiDef& def` (stored 0x007CACD0, reloaded 0x007CAFF3 (sound arg) and 0x007CB6BB); shared with the GetTC key temps 0xB0 / 0xEF in the ForceClear-empty block (def dead there). |
| +0x2C..+0x54 | fish arm: the 11 `operator+` result temps t11..t1 (t11 +0x2C ... t1 +0x54, 4 bytes each) |
| +0x58 | fish arm: `inv->GetDescription()` result |
| +0x5C | fish arm: GFIntToWideString(length). gold arm: new RentMoney copy for `fild` (0x007CB589). display block: GFLimit lower `0.0f` temp, then the C2DExtentsI result (8 B, +0x5C..+0x63) of both GetRenderTargetDimensions calls. |
| +0x64 | the multi-use 0x20-byte word group (+0x64..+0x83): ForceClear: CIntelligentPointer<CThingObject>(NULL) temp (8 B); C3DVector(0,0,0) for AddMessage (12 B); new-item: CSoundInit (0x20 B, out-of-line ctor 0x0041C930, GainFaderBits +0x78 = 0x8010); fish arm: GFIntToWideString(weight), CRightHandedSet result (24 B); inventory arm: GetDescription result then CRightHandedSet result; gold arm: all GetTC/PeekTC key temps (4, 0xCC, 0x33, 0xD5, 0x10D), the scoreboard GetTC out (uninitialised: miss path reads [esp+64h] = the last key, 0x007CB5BC), GetTextBySymbol result, C3DVector(0,0,-1); display block: GFLimit upper `1.0f` temp and the by-value `interp` param copies of the 3rd/4th GFGetInterpolatedValue (0x007CB746, 0x007CB793). |

Long-lived registers: esi = this (except ForceClear-empty exit: `add esi,0A4h` = &BackgroundPrimitiveHandle); ebx = &def (then spilled),
= message manager (ForceClear), = &PCurrentItem from 0x007CAFB2 to the SetText tail, = stats in the gold arm, = &def again from 0x007CB6BB;
ebp = 0 (ForceClear zero for the float store and both Reset(0) stores) / &PCurrentItem / &PContainer / fish TC / 0x4000000 constant /
hero / &CurrentAreaItemTaking / FullSizeTimer; edi = various (&handle, ConstantFPS, &PContainer, lookup ends, inventory TC, &Amount).

## 4. Block-by-block pseudo-C++ (retail order)
Names: `GetPItem` = CBaseIntelligentPointer::GetPItem 0x00A01B50 (`Get()` / `operator->` of CIntelligentPointer); `LB` = CVectorMap<
ETCInterfaceType,CTCBase*,..>::LowerBound non-const 0x0040F020 (=> GetTC) / const 0x004365B0 (=> PeekTC). HasTC bit = UsingComponents
word +0x20+4*(t>>5), bit t&31.
```
0x007CACB0  if (!ReceivingItems) return;                                    // je 0x007CB847
0x007CACC4  const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();            // 0x00438570 static __fastcall; ebx, spilled +0x28
0x007CACCB  if (ForceClear) {                                               // else -> 0x007CAF25
0x007CACDA    OldAreaItemTaking = CurrentAreaItemTaking = InitialAreaItemTook;   // PROBE chain.cpp: chained form copies src->Current then src->Old (retail order)
              ForceClear = false;  AmountThroughForAreaResize = 0.0f;      // stored from the xor ebp,ebp zero
0x007CAD10    ItemPrimitiveHandle.Clear();                                  // inline: refcount==1 -> Data->vtbl[3](this) (NotifyReleased); Release(dec,+vtbl[1]); Data=0 twice
0x007CAD4C    DisplayingItems = false;
0x007CAD55    if (IS_THING_ALIVE(PCurrentItem)) {                           // EXPANDED: GetPItem x2 (null, +0x91 bit0)
0x007CAD6E      AddItemToInventory(*PCurrentItem, PersistCurrentItem);      // 3rd GetPItem; 0x007CABD0 private `ret 8`; bool as `xor ecx,ecx ; mov cl,[esi+24h]`
0x007CAD83      Items.pop_front();                                          // inline STLport erase(begin()): prev->next, next->prev, ~CBaseIntelligentPointer(node+8) 0x00A01C10, free 0x00BFEA14
0x007CADA3      PCurrentItem = NULL;                                        // temp CIntelligentPointer<CThingObject>(0): base ctor 0x00A01B10, vptr 0x0125A858, SetPItem(0) 0x00A01B90; operator= = GetPItem(temp)+SetPItem; ~ 0x00A01C10
              }
0x007CADD9    if (Items.empty()) {                                          // cmp [hdr],hdr ; jne 0x007CAF25 (falls into the normal path)
                DisplayingItems = false;                                    // second store (0x007CADE9)
0x007CADE4      if (SendMessageW && IS_THING_ALIVE(PContainer))             // OUT OF LINE: call 0x004CC340 (__fastcall, ecx=&PContainer)
0x007CADFD        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_HERO_REWARDED_FROM_CONTAINER /*65*/,
                      CMainGameComponent::GetConstantFPS(), C3DVector(0,0,0), 0, 0, 0.0f, 0.0f, (long)PContainer.Get());
                      // GetWorld 0x00686A70, mgr = World+0x60 (inline F GetMessageEventManager), AddMessage 0x00687540; FPS read into edi
                      // first, vector stores X,Y,Z at +0x64.. before the GetWorld call; PContainer.Get() is the LAST (8th, `long`) arg (pushed first)
0x007CAE44      if (IS_THING_ALIVE(PContainer)) {                           // EXPANDED this time (2 GetPItem). Alt: !IS_THING_DEAD(PContainer) (both templates exist, E+F)
0x007CAE66        CTCFishingSpot* fs; if (PContainer->GetTC(&fs)) fs->SetAsEnabled(false);   // TCI_FISHING_SPOT 176 (+0x34 bit16); LB non-const, key at +0x28; Enabled +0x24
0x007CAEA7        else { CTCDiggingSpot* ds; if (PContainer->GetTC(&ds)) ds->SetAsEnabled(false); }  // TCI_DIGGING_SPOT 239 (`mov ecx,[eax+3Ch] ; test ch,ch ; jns`); Enabled +0x0C
                }                                                           // each arm re-calls GetPItem (one per GetTC)
0x007CAEE4      BackgroundPrimitiveHandle.Clear();                          // same inline Clear, esi := &handle, zero in edi
0x007CAF1B      return;                                                     // epilogue copy 1
              }
            }
0x007CAF25  if (!IS_THING_ALIVE(PCurrentItem)) {                            // expanded (2 GetPItem, ecx=edi=&PCurrentItem); alive -> 0x007CB6BF. Alt: IS_THING_DEAD(PCurrentItem)
0x007CAF47    for (it = Items.begin(); it != Items.end(); ) {               // first test vs cached header (eax), loop test `cmp edi,[esi+0Ch]` (end() re-read) -- OnKill shape
0x007CAF50      CThingObject* item = it->first.Get();                       // ONE GetPItem per node (ebx = node+8) => pointer local, OnKill's IS_THING_ALIVE(item) on CThingObject*
                if (IS_THING_ALIVE(item)) ++it;                             // 0x007CAF67 mov edi,[edi]
0x007CAF6B      else it = Items.erase(it);                                  // inline erase: next->ebp, prev, relink, ~ptr 0x00A01C10, free, it=next
              }
0x007CAF8C    if (Items.empty()) { ReceivingItems = false; DisplayingItems = false; return; }   // epilogue copy 2
0x007CAFA5    PCurrentItem = Items.front().first;                           // GetPItem(front+8) ; SetPItem on ebx=&PCurrentItem (ebx keeps it)
0x007CAFBD    PersistCurrentItem = Items.front().second;                    // byte [front+0x10]
0x007CAFC8    if (PSoundManager) {                                          // 0x013B8394 (?PSoundManager@@3PAVCASoundManager@@A), loaded once into edi
0x007CAFD2      CSoundInit init; init.GainFaderBits = 0x8010;               // ctor call 0x0041C930 (header default = declaration only)
0x007CAFE3      PSoundManager->Get2DEmitter().PlaySoundEvent(CMainGameComponent::Get()->GetSampleBank(), def.ReceiveItemsSoundCriteria, true, init, false);
                // slot 9 (+0x24) first, then pushes 0,&init,1,&def+0x644,PMainGameComponent->+0x10; emitter slot 4 (+0x10). def reloaded from +0x28.
              }
0x007CB00F    CWideString text;                                             // +0x10
0x007CB018    if (PCurrentItem->HasTC(TCI_FISH /*220*/)) {                  // word +0x38, `mov edi,10000000h ; test edi,ecx` (constant reused below)
0x007CB031      const CTCFish* fish = NULL; PCurrentItem->PeekTC(&fish);    // xor ebp,ebp; GetPItem; HasTC; LB const key 0xDC at +0x18 -> ebp
0x007CB068      const CTCInventoryItem* inv = NULL; PCurrentItem->PeekTC(&inv);   // xor edi,edi; LB const key 0x1A at +0x18 -> edi
0x007CB0A2      CWideString length_text  = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_FISH_LENGTH");        // +0x24
0x007CB0D2      CWideString length_units = ...GetTextBySymbol("TEXT_GUI_FISH_LENGTH_UNITS");                                            // +0x20
0x007CB102      CWideString weight_text  = ...GetTextBySymbol("TEXT_GUI_FISH_WEIGHT");                                                  // +0x1C
0x007CB132      CWideString weight_units = ...GetTextBySymbol("TEXT_GUI_FISH_WEIGHT_UNITS");                                            // +0x14
                // each: CCharString temp at +0x18 (implicit const char* -> CCharString, or CCharString("..")), `mov eax,[13B86A0h] ; mov ecx,[eax+14h]`
0x007CB162      text += inv->GetDescription() + L".  " + length_text + L": " + GFIntToWideString((long)fish->GetLength()) + length_units
                      + L".  " + weight_text + L": " + GFIntToWideString((long)fish->GetWeight()) + weight_units + L".";
                // PROBE fishtext.cpp (A v B): the INLINE accessors GetLength()/GetWeight() give retail's start `fld [ebp+14h]` +
                // `mov ecx,[ebp+10h] ; mov [esp+..],ecx` early length copy; direct `fish->Length` reads it late (wrong).
                // __ftol2 0x00BFEA70 x2, GFIntToWideString 0x00405F00 x2 (fastcall ecx=result, edx=long), operator+(W,const ushort*) 0x0099BF30 x5,
                // operator+(W,W) 0x0099BE70 x6, GetDescription 0x005D9070, operator+= 0x0099B8D0, then 14 ~CWideString 0x0099B510 (+0x2C..+0x5C, +0x64)
0x007CB2D2      OldItemRHSet = CurrentItemRHSet = inv->GetStartingRightHandedSet();   // 0x005D92F0 result at +0x64; PROBE chain.cpp: chained form = src->Current then src->Old (retail)
0x007CB32A      // ~weight_units, ~weight_text, ~length_units, ~length_text (declaration order reversed)
0x007CB351    }                                                             // jmp 0x007CB689
0x007CB356    else if (PCurrentItem->HasProperty(10) || PCurrentItem->HasTC(TCI_GOLD /*204*/)) -> gold arm 0x007CB44F
                // GetProperties = virtual slot 48 (+0xC0) 0x00702FF0 (E/F ?GetProperties@CThingObject@@UBEKXZ); retail `shr eax,0Ah ; test al,1 ; jne`.
                // PROBE prop.cpp/prop2.cpp: `& 0x400`, `(x>>10)&1` in an if, `& (1<<10)` all give `test ah,4`; `((x >> bit) & 1) != 0` (inline
                // helper with constant bit, or written with `!= 0`) gives retail's shr/test. F has ?HasProperty@CThingObject@@QBE_NK@Z (inline;
                // F 38 B) -> body FITTED, UNVERIFIED. 0x400 = gold property (agent2039 matched row 0x006ACA30 `case 0x400: // gold`).
                // GOLD test: `mov ecx,[eax+38h] ; test ch,10h ; jne`.
0x007CB383    else if (PCurrentItem->HasTC(TCI_INVENTORY_ITEM /*26*/)) {    // `mov ebp,4000000h ; test ebp,ecx` (constant reused); else -> 0x007CB689
0x007CB39A      const CTCInventoryItem* inv = NULL; PCurrentItem->PeekTC(&inv);   // xor edi,edi; LB const key at +0x64
0x007CB3D0      text += inv->GetDescription();                              // result +0x64, += 0x0099B8D0, ~ 0x0099B510
0x007CB3EF      OldItemRHSet = CurrentItemRHSet = inv->GetStartingRightHandedSet();   // result +0x64
0x007CB44A    }                                                             // jmp 0x007CB689
0x007CB44F    [gold arm, laid out LAST, falls into the tail]                // PROBE order.cpp: plain `if/else if (A||B)/else if (C)` already gives this layout
              { CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPPlayerCharacter();   // 0x00686D40, 0x00449970 (QAE), inline `lea ecx,[eax+34h] ; GetPItem` -> ebp
                long gold = 0;                                              // [+0x18]=0 scheduled before the GetPlayerManager call
0x007CB46F      CTCHeroStats* stats; hero->GetTC(&stats);                   // `test byte [ebp+20h],10h` (TCI 4); LB non-const key +0x64 -> [+0x14]; uninitialised
0x007CB4A1      const CTCGold* gold_tc = NULL; PCurrentItem->PeekTC(&gold_tc);   // LB const key 0xCC; result only in ecx
0x007CB4DC      if (gold_tc) {                                              // `test ecx,ecx ; je` (HasTC miss jumps straight past)
0x007CB4E7        gold = gold_tc->GetGold();                                // 0x007CBDC0 (QBEJXZ)
0x007CB4F7        CTCOwnedEntity* owned; if (PCurrentItem->GetTC(&owned)) { // TCI 51 (+0x24 bit19); LB non-const key 0x33; no null test
0x007CB529          CTCBuyableHouse* house; if (owned->GetPOwner()->GetTC(&house) && house->GetRented()) {   // POwner +0x0D (GetPItem); TCI 213 (+0x38 bit21); Rented +0x12
0x007CB573            stats->AddRentMoney(gold);                            // RentMoney +0x54 (inline F AddRentMoney); new value in ecx, copied to +0x5C for fild
0x007CB583            CTCHeroOnlineScoreboard* sb; hero->GetTC(&sb);        // TCI 269 (hero word +0x40, `test ah,20h`); uninit out = +0x64 (miss reads the key slot)
0x007CB5C0            sb->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)16, (float)stats->GetRentMoney(), false);   // 0x0055F3B0
                    } } }                                                   // jmp 0x007CB5D9 (skips the `mov ebx,[esp+14h]` reload at 0x007CB5D5)
0x007CB5D9      stats->AddMoney(gold, true);                                // 0x0057B327 (unconditional, gold may be 0)
0x007CB5E7      text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TXT_OBJT_GOLD");   // CCharString temp +0x24, result +0x64, operator= 0x0099B7D0
0x007CB62A      NGameText::ConstructString(text, gold);                     // 0x009C7B70 fastcall (ecx=&text, edx=gold), result dropped
0x007CB635      OldItemRHSet.Init(g_ItemUp, C3DVector(0.0f, 0.0f, -1.0f));  // 0x004BBE10 (see names); vector at +0x64 stored X,Y,Z; g_ItemUp 0x01383A5C
0x007CB661      CurrentItemRHSet = OldItemRHSet;                            // interleaved 2-register copy from edi=&Old
              }
0x007CB689  CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().GetSubtitleRenderer().SetText(PeekComponent().PeekCutSceneMainFont(), text, 0x62, 0.0f, true, true);
              // PeekComponent 0x00686D60 first, font = [eax+1614Ch]; singleton 0x013B8790 +0x18; SetText 0x00492E4B (ret 18h); then ~text 0x007CB6B2
0x007CB6BB  }  // ebx = def reloaded from +0x28 (the alive path 0x007CAF41 arrives here with ebx still = def)
0x007CB6BF  if (DisplayingItems) {                                          // else -> 0x007CB844 epilogue
0x007CB6CA    OldAreaItemTaking = CurrentAreaItemTaking;                    // the fld/fcomp of the next test is scheduled above this copy
0x007CB6CA    if (AmountThroughForAreaResize < 1.0f) {                      // fcomp [0x0122DED8]=1.0f ; fnstsw ; test ah,5 ; jp 0x007CB7E4
0x007CB6FF      AmountThroughForAreaResize += 1.0f / (CMainGameComponent::GetConstantFPS() * def.NumberOfSecondsForContainerItemToReachFullSize);  // fild FPS ; fmul [ebx+8C0h] ; fdivr 1.0 ; fadd [edi]
0x007CB730      GFLimit(AmountThroughForAreaResize, 0.0f, 1.0f);            // ??$GFLimit@MMM@@YIXAAMABM1@Z 0x0048DAE0 out of line; 0.0f temp +0x5C, 1.0f temp +0x64
0x007CB735      CurrentAreaItemTaking.TLX = GFGetInterpolatedValue(InitialAreaItemTook.TLX, def.ContainerItemDisplayAreaTLPos_X, AmountThroughForAreaResize);
0x007CB754      CurrentAreaItemTaking.TLY = GFGetInterpolatedValue(InitialAreaItemTook.TLY, def.ContainerItemDisplayAreaTLPos_Y, AmountThroughForAreaResize);
0x007CB766      CurrentAreaItemTaking.BRX = GFGetInterpolatedValue(InitialAreaItemTook.BRX,
                    (float)GPDisplayManager->GetRenderTargetDimensions().X + def.ContainerItemDisplayAreaBRPos_X - 640.0f, AmountThroughForAreaResize);
0x007CB79C      CurrentAreaItemTaking.BRY = GFGetInterpolatedValue(InitialAreaItemTook.BRY,
                    (float)GPDisplayManager->GetRenderTargetDimensions().Y + def.ContainerItemDisplayAreaBRPos_Y - 480.0f, AmountThroughForAreaResize);
                // engine/GFGetInterpolatedValue.h `(new-old)*interp+old`, by-value float params: interp (rightmost) is evaluated BEFORE the
                // dims call of the 3rd/4th calls -> the +0x64 copies (0x007CB746 hoisted, 0x007CB793). 640.0f 0x01238168, 480.0f 0x01238874;
                // the same formula as landed NPlayerGui::GFAnchorToBottomRight 0x00643D0C (`(float)Width + pos.X - 640.0f`), written per
                // component here (X call first; the out-of-line GFAnchorToBottomRight would do Y first). GetRenderTargetDimensions 0x009BEDC0 on
                // GPDisplayManager 0x013B8390, C2DExtentsI result at +0x5C.
0x007CB7CA      FullSizeTimer = (long)(CMainGameComponent::GetConstantFPS() * def.NumberOfSecondsForContainerItemToStayAtFullSize + 1.0f);  // fild ; fmul [ebx+8D8h] ; fadd 1.0 ; __ftol2
              }
0x007CB7E4    OldItemRHSet = CurrentItemRHSet;                              // ecx = &CurrentItemRHSet kept as Rotate's this
              --FullSizeTimer;                                              // `mov ebp,[esi+38h]` hoisted first, `dec ebp ; mov [esi+38h],ebp` mid-copy (order vs the copy UNVERIFIED)
0x007CB818    CurrentItemRHSet.Rotate(g_ItemUp, -(1.0f / (CMainGameComponent::GetConstantFPS() * def.NumberOfSecondsForContainerItemToRotate)));
              // fild ; fmul [ebx+8D4h] ; fdivr 1.0 ; fchs (a `-1.0f/x` literal would use the -1.0f constant 0x0122DEE0 instead); `push ecx` = slot for the float
              // Rotate 0x00A22F80 ?Rotate@CRightHandedSet@@QAEXABVC3DVector@@M@Z
0x007CB83A    ParentThing->SetAsRenderedThisFrame(4);                       // 0x004C7590 ?SetAsRenderedThisFrame@CThing@@QAEXK@Z
            }
0x007CB844  epilogue (main)
```

## 5. Callees / globals (address -> PDB spelling; sources; declared where)
| addr | spelling | 2 sources | declared |
|---|---|---|---|
| 0x00438570 | ?PeekGuiDef@CPlayerGui@@SIABVCPlayerGuiDef@@XZ | E 0x0044B230 + reloc_bindings (6x) | landed row (local); CPlayerGui.h UNVERIFIED |
| 0x00A01B10/B50/B90/C10 | CBaseIntelligentPointer ctor / GetPItem / SetPItem / dtor | E + engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER) | header |
| 0x007CABD0 | ?AddItemToInventory@CTCHeroReceiveItems@@AAEXAAVCThing@@_N@Z | E 0x007C8F40 + F + landed | add to overlay header |
| 0x00BFEA14 | _free | M + landed dtor | STLport allocator |
| 0x004CC340 | ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z | F 0x01CCA150 + landed (descriptive leaf ?IsThingAlive_004cc340@@ -- NAME_FIX candidate) | engine/ThingAlive.h FABLE_RETAIL_IS_THING_ALIVE (+ CIntelligentPointer COMPARE_NULL_INT for `!= 0`) |
| 0x00686A70 | ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ | E 0x006B1E60 + F | CTCBase.h:160 |
| 0x00687540 | ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z | E 0x006B2740 + landed leaf | CMessageEventManager.h:13 |
| 0x0040F020 / 0x004365B0 | CVectorMap::LowerBound non-const / const | landed rows + F | engine/CVectorMap.h via CThing GetTC/PeekTC |
| 0x0041C930 | ??0CSoundInit@@QAE@XZ | E 0x00518C60 + F + landed | engine/CSoundInit.h (default = decl) |
| vtbl | CASoundManager slot 9 Get2DEmitter (descriptive) / CASoundEmitter slot 4 PlaySoundEvent | CASoundManager.h:141 / CASoundEmitter.h:39 | headers |
| 0x0099AED0 / 0x0099B510 | ??0CWideString@@QAE@XZ / ??1CWideString@@QAE@XZ | E + reloc_bindings | fable_string.h |
| 0x0099EBF0 / 0x0099EAE0 | ??0CCharString@@QAE@PBDJ@Z / ??1CCharString@@QAE@XZ | E + reloc_bindings | fable_string.h |
| 0x009C95E0 | ?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z | E + reloc_bindings | NGameText_CDataBank.h (UNVERIFIED which opt-in) |
| 0x00405F00 | ?GFIntToWideString@@YI?AVCWideString@@J@Z | E 0x00404F90 + landed | no shared declaration (grep of rebuild/include: none) -> declare `CWideString __fastcall GFIntToWideString(long);` |
| 0x0099BE70 / 0x0099BF30 | ??H@YI?AVCWideString@@ABV0@0@Z / ??H@YI?AVCWideString@@ABV0@PB_W@Z | E + reloc_bindings | fable_string.h:247-253 |
| 0x0099B8D0 / 0x0099B7D0 | ??YCWideString@@QAEABV0@ABV0@@Z / ??4CWideString@@QAEABV0@ABV0@@Z | E + reloc_bindings / landed | fable_string.h:148,150 |
| 0x00BFEA70 | __ftol2 | M + landed | compiler |
| 0x005D9070 / 0x005D92F0 | ?GetDescription@CTCInventoryItem@@QBE?AVCWideString@@XZ / ?GetStartingRightHandedSet@CTCInventoryItem@@QBE?AVCRightHandedSet@@XZ | E 0x00559FC0 / 0x0055A250 + landed | CTCInventoryItem.h has only the static overloads -> overlay |
| 0x00702FF0 (vtbl +0xC0) | ?GetProperties@CThingObject@@UBEKXZ | E 0x005524F0 + landed | CThingPhysical.h:18 |
| 0x00686D40 | ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ | E + F + landed | CTCBase.h:152 |
| 0x00449970 | ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ | E 0x004450F0 + landed leaf | CPlayerManager.h |
| inline | ?GetPPlayerCharacter@CPlayer@@QBEPAVCThingPlayerCreature@@XZ | F 0x01A5B1A0 + CPlayer.h:183 | header |
| 0x007CBDC0 | ?GetGold@CTCGold@@QBEJXZ | E 0x007CA450 + landed | no repo header: agent2039/hdr/engine/CTCGold.h |
| 0x0055F3B0 | ?UpdateScoreForActivity@CTCHeroOnlineScoreboard@@QAEXW4EOnlineScoreboardActivity@1@M_N@Z | E 0x005D86B0 + reloc | agent2039/hdr/engine/CTCHeroOnlineScoreboard.h |
| 0x0057B327 | ?AddMoney@CTCHeroStats@@QAEXJ_N@Z | E + landed | agent2039/hdr CTCHeroStats.h FABLE_RETAIL_CTCHEROSTATS_RENTMONEY (also AddRentMoney/GetRentMoney, F 0x01DF38C0/F0) |
| 0x009C7B70 | ?ConstructString@NGameText@@YI_NAAVCWideString@@J@Z | E 0x00B15970 + landed | NGameText_CDataBank.h:23 |
| 0x004BBE10 | landed as `CRightHandedSet::Set` (descriptive; M label "CRightHandedSet::CRightHandedSet"); E 0x004BF6E0 + F 0x01D0D780 `?Init@CRightHandedSet@@QAEXABVC3DVector@@0@Z` have the same (const C3DVector&, const C3DVector&) signature and copy body. No reloc binding. NAME_FIX candidate (address match UNVERIFIED). CRightHandedSet.h FABLE_RETAIL_CRIGHTHANDEDSET_SET has an INLINE body; retail calls it out of line -> needs a declaration-only spelling |
| 0x00A22F80 | ?Rotate@CRightHandedSet@@QAEXABVC3DVector@@M@Z | E 0x00B8BA40 + landed | CRightHandedSet.h FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE |
| 0x00686D60 | ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ | E + F | CTCBase.h:169 opt-in |
| 0x00492E4B | ?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCWideString@@KM_N2@Z | E 0x0049C103 + landed leaf | CSubtitleRenderer.h |
| 0x0048DAE0 | ??$GFLimit@MMM@@YIXAAMABM1@Z | F 0x01A6F480 + landed leaf | engine/GFLimit.h has an INLINE template body; retail CALLS it here -> needs the call kept out of line (declaration-only / noinline spelling; check how landed callers of 0x0048DAE0 do it) |
| 0x009BEDC0 | ?GetRenderTargetDimensions@CDisplayManager@@QAE?BVC2DExtentsI@@XZ | E 0x00B242A0 + CDisplayManager.h:63 | header |
| 0x004C7590 | ?SetAsRenderedThisFrame@CThing@@QAEXK@Z | E 0x004C9150 + landed leaf | CThing.h:383 under FABLE_RETAIL_CTHING |
| 0x00438570 note | PeekGuiDef | | CPlayerGui.h:212 under FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF |
| data | 0x013B8394 PSoundManager; 0x013B86A0 CMainGameComponent::PMainGameComponent; 0x01375550 CMainGameComponent::ConstantFPS; 0x013B8390 GPDisplayManager; 0x013B8790 CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass; 0x01383A5C g_ItemUp (descriptive, landed 0x007CA670; value (0,-1,0)); vtbl 0x0125A858 CIntelligentPointer<CThingObject> | ghidra_out/labels_*.tsv + landed rows | |
| strings | 0x0126ECD4 "TEXT_GUI_FISH_LENGTH", 0x0126ECB8 "..._LENGTH_UNITS", 0x0126ECA0 "TEXT_GUI_FISH_WEIGHT", 0x0126EC84 "..._WEIGHT_UNITS", 0x0126EC6C "TXT_OBJT_GOLD"; wide 0x0126EC7C L".  " (dot + 2 spaces), 0x01236A50 L": ", 0x0124C4A8 L"." | rstr.py + raw bytes | |
| floats | 0x0122DED8 1.0f, 0x01238168 640.0f, 0x01238874 480.0f | raw bytes | |

CPlayerGuiDef (repo header is flattened): ReceiveItemsSoundCriteria CCharString +0x644 (needs a typed overlay opt-in; landed CPlayerGuiDef ctor
rows 0x00459E46 / 0x00459BB6 type it CCharString), NumberOfSecondsForContainerItemToReachFullSize +0x8C0, ContainerItemDisplayAreaTLPos
+0x8C4/+0x8C8, BRPos +0x8CC/+0x8D0, ToRotate +0x8D4, ToStayAtFullSize +0x8D8 (offsets asserted in rebuild/include/engine/CPlayerGuiDef.h:1246-1252).
New classes needed (no repo header): CTCFish (E: PDef +0x0C, Length +0x10, Weight +0x14; F inline ?GetLength@CTCFish@@QBEMXZ /
?GetWeight@CTCFish@@QBEMXZ; TCI_FISH 220), CTCFishingSpot (E Enabled +0x24; TCI 176; F ?SetAsEnabled@CTCFishingSpot@@QAEX_N@Z inline),
CTCDiggingSpot (E Enabled +0x0C, Hidden +0x0D; TCI 239; F ?SetAsEnabled@CTCDiggingSpot@@QAEX_N@Z). FableWin GetTC/PeekTC instantiations
confirm every lookup form: F ??$GetTC@ for CTCFishingSpot, CTCDiggingSpot, CTCHeroStats, CTCOwnedEntity, CTCBuyableHouse,
CTCHeroOnlineScoreboard; ??$PeekTC@$$CB for CTCFish, CTCInventoryItem, CTCGold -- exactly the non-const / const LowerBound split in retail.

## 6. Inline expansions and where they already match
- CEnginePrimitiveHandle::Clear x2 (0x007CAD10..0x007CAD4C, 0x007CAEE4..0x007CAF1A): engine/CEnginePrimitiveHandle.h FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
  (+ FABLE_RETAIL_CIVCOUNTEDPOINTER for the in-test release store); matched by CPSCLight::Kill 0x00B19C20 / PreparePrimitives 0x00B19C60 (header note).
- STLport list pop_front / erase(it) / front / empty / begin / end: stlport/fable_containers.h FABLE_RETAIL_STLP_LIST (lines ~848-878); pair
  under `#pragma pack(1)` like the landed OnKill / GiveItem rows. Loop shape = landed OnKill 0x007CA630.
- CThing GetTC / PeekTC / HasTC (forceinline, CVectorMap LowerBound out of line): engine/CThing.h FABLE_RETAIL_CTHING_GETTC_EXPANDED, as matched
  in agent3009 TurncoatSpell 0x007C6E50 and agent2039 0x006ACA30 (the latter has the SAME rent/gold sub-block: `const CTCGold* gold = NULL;
  thing->PeekTC(&gold); if (gold) ...; owned_entity->GetPOwner()->GetTC(&house) && house->GetRented(); stats->AddRentMoney(..);
  GetTC(&scoreboard); scoreboard->UpdateScoreForActivity((..)16, (float)stats->GetRentMoney(), false);` -- read-only model, RELOCATION_MATCH).
- IS_THING_ALIVE: engine/ThingAlive.h (mixed expanded / called sites like TurncoatSpell; here call at the 1st PContainer site only).
- CIntelligentPointer ctor(T*) / operator= / Get: engine/CIntelligentPointer.h (_ASSIGN for operator=); ctor(T*) expanded here.
- CMainGameComponent::Get / GetConstantFPS / GetTextBank / PeekCutSceneMainFont / GetSampleBank: CMainGameComponent.h opt-ins _TEXTBANK,
  _SAMPLEBANK_WORLDSEED (or the landed RewardHeroWithItems 0x0074EE90 local model, which has the same SetText / GetTextBySymbol tail).
- CSoundInit default ctor out of line + member store: landed NInventory PlayInventoryMovementSound 0x005BD33C.
- GFGetInterpolatedValue: engine/GFGetInterpolatedValue.h (landed CGameCameraBase::GetRenderCamera 0x006FD5E0).
- No fistp, no SSE, no inline asm: both float->long conversions are `__ftol2` calls (plain casts). No ASM_RULES site.

## 7. FableWin counterpart (F 0x02726CD0, 7339 B / 1646 i) -- helpers retail expands inline (F debug size B / insns)
GetTC<CTCFishingSpot> 74/27, GetTC<CTCDiggingSpot> 74/27 (both right after this TU's list COMDATs, F 0x0272A710/770 -> used by this TU),
SetAsEnabled CTCFishingSpot 22/10 & CTCDiggingSpot 22/10 (F 0x027290F0/D0, directly after GlobalUpdate), IS_THING_ALIVE<CIP<CThingObject>> 56/23,
IS_THING_ALIVE<CIP<CThing>> 56/23 (called once in retail), Clear@CEnginePrimitiveHandle 73/27, GetMessageEventManager 20/9, GetConstantFPS 10/5,
PeekTC<const CTCFish> 71/27, PeekTC<const CTCInventoryItem> 74/27, PeekTC<const CTCGold> 74/27, GetTC<CTCHeroStats> 74/27, GetTC<CTCOwnedEntity>
74/27, GetTC<CTCBuyableHouse> 74/27, GetTC<CTCHeroOnlineScoreboard> 71/27, GetLength 17/9, GetWeight 17/9, GetRented 17/9, GetSampleBank 17/9,
GetTextBank 22/10, PeekCutSceneMainFont 25/10, GetSubtitleRenderer 17/9, CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get 15/6,
GFGetInterpolatedValue 28/12, IsDead 22/10, HasTC 28/12, HasProperty@CThingObject 38/16 (F 0x01D860F0), list empty/front/pop_front/erase/begin/end
(F list COMDATs 0x027298C0.. are Dinkumware -- different STL; retail is STLport 4.6). GetPPlayerCharacter: F name 0x01A5B1A0, absent from
the signatures file. Called (not inline) in retail: GFLimit<MMM> (F 78/33), CSoundInit ctor (F 87/27), GetGold (F 235/68), Init/Set 0x004BBE10
(F Init 60/24), GetMainPlayer (F 264/79).

## 8. Unusual points / open questions for the worker
1. The two IS_THING_ALIVE(PContainer) sites: 1st called (0x004CC340), 2nd expanded. Try plain IS_THING_ALIVE at both first; if c2 inlines
   both, the 2nd may be `!IS_THING_DEAD(PContainer)` (a different template, E has an out-of-line IS_THING_DEAD<CIP<CThing>> 0x0062E159).
2. HasProperty body (shr/test form) -- FITTED; see §4 probe results. Alternative: a local `bool` with `!= 0`.
3. Uninitialised outs: stats (+0x14) and scoreboard (+0x64) must stay uninitialised (frame reads on miss); fish / inv / gold_tc are `= NULL`.
4. +0x28 is a c2 spill of `def`, not a named local; the key temps 0xB0/0xEF land on it.
5. `--FullSizeTimer` position relative to `OldItemRHSet = CurrentItemRHSet` and whether the `if (ReceivingItems) {..}` wrapper vs early
   return matters (shared exit either way) -- UNVERIFIED.
6. AddMessage gets PContainer.Get() as the trailing `long` (pushed first) and 0 / 0 for the two CThing* -- bytes are unambiguous.
7. ForceClear-and-empty exit leaves ReceivingItems set (only DisplayingItems cleared, twice) -- as retail.
8. Tooling: `scout/gr.sh <src> <note>` (copy of agent2048's gr.sh pattern for this row, smoke-tested; copy it to agent2057/gr.sh,
   it calls `scout/md.py`), `scout/pc.py <src> <leaf-substring>` (compile-only probe disassembly into scout/tmp/pc).

## 9. Structure summary (bwf2 scout)
- No jump tables, no cold blocks after the epilogue, no SEH (no fs:0) despite CWideString/CCharString locals (TU has no /EHsc), no /GS
  cookie, no frame pointer (/Oy). Three epilogue copies (0x007CAF1B, 0x007CAF93 with the two flag stores scheduled between pops,
  0x007CB844); the ReceivingItems early-out jumps to 0x007CB847 (`pop esi` only: shrink-wrapped, esi pushed before the test).
- Top-level shape: `if (!ReceivingItems) return; const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef(); if (ForceClear) { ...; if
  (Items.empty()) { ...; return; } } if (!IS_THING_ALIVE(PCurrentItem)) { purge loop; if (Items.empty()) { ReceivingItems = false;
  DisplayingItems = false; return; } take front; sound; CWideString text; fish / gold / inventory arms; SetText; } if (DisplayingItems)
  { area + rotation update; ParentThing->SetAsRenderedThisFrame(4); }`
- Arm layout: fish (0x007CB031..0x007CB351, jmp tail), property/gold tests (0x007CB356), inventory (0x007CB383..0x007CB44A, jmp tail),
  gold arm LAST (0x007CB44F..0x007CB686) falling into the SetText tail 0x007CB689.

---------------------------------------------------------------------------------------------------------------------------------------
## 10. Re-run verification (identity scout, 2026-11-12)
- Verified again against dis_all.txt: prologue/early-out (0x007CACB0..0x007CACD4), ForceClear box copies (both from &Initial: 0x007CACDA
  `lea eax,[esi+3Ch] ; mov edx,eax`, Current first 0x007CACE1, Old 0x007CACFA reading [eax]), property/gold/inventory tests 0x007CB356..
  0x007CB394, inventory arm (no null test on inv before GetDescription 0x007CB3D7), the whole gold arm 0x007CB44F..0x007CB686 (miss paths
  all land on 0x007CB5D5 `mov ebx,[esp+14h]`; scoreboard miss 0x007CB5BC reads the key slot +0x64), SetText tail, display block and
  Rotate tail. No corrections to sections C1-C7 / 0-9.
- Header drift and the GFLimit / CRightHandedSet::Init out-of-line spellings: see DOSSIER.md section 8 (repo CRightHandedSet.h now has an
  INLINE Init under FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE -- do not define it; use hdr_prior's declaration-only _CRIGHTHANDEDSET_INIT).
