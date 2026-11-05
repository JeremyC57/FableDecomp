# Chunk 778: notes

## Results (2 matched, 3 parked)
| Row | Grade | Attempts | Key spelling / residue |
|---|---|---|---|
| 0x00631720 NUISystem::CUIDef::operator= | RELOCATION_MATCH | 3 | `CUIDef::Copy`-style body `*this = *static_cast<const CUIDef*>(def)`. Retail's operator= is the IMPLICIT one: FABLE_RETAIL_PDCB_IMPLICIT_ASSIGN drops the declared CParentDefClassBase::operator=, which VC then emits inline. The FABLE_RETAIL_CUIDEF branch also loses its 10 `_pad_` members: the implicit copy copied them (DIFFER 1619v1337). |
| 0x00615E70 CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferIn | RELOCATION_MATCH | 5 | The sibling 0x006159A0 shape: `value = CUIRegionInfo()` and `if (!(parser.ReadAsSymbol() == ",")) return;` chains, with each field read through the value-returning CPersistTraitsSLONG::TransferIn. The optional Taverns field is a nested if: `if (PeekNextItem(item) && item.IsSymbol()) { if (item.GetSymbol() == ',') {...} else if (ReadAsSymbol() == ")") Taverns = 0; }`. It needs a `__forceinline` `CCharString::operator==(char)`. |
| 0x00ACCBF0 COptimisedPrimitive::PerformSubContraction | parked DIFFER(1282v1264), 139 lines | 17 logged | vertex1 and vertex2 sit in swapped registers (ebx/edi). Retail keeps the map-loop key in ebp, loaded from node+0x10 before the refcount increment; ours reloads it. Normalise must be written in the locals form. |
| 0x00E50910 CV_BeardyBaldyScript::WatchForQuestCardConditions | parked DIFFER(1392v1328), 51 lines | 20 | Lever 4 (a loop-head do-while(0) on the outer for(;;) and on the region loop) reproduces retail's inline/out-of-line ~CScriptThing split. Residue: VC keeps ebx = 0 live into the MsgIsTalkedToBy wait (`cmp ecx,ebx` plus a `xor ebx,ebx; jmp` back-edge). That moves the shared return block and the inline-dtor tails. |
| 0x00839420 NThingHelpers::CreateEffectOnHit | parked DIFFER(1304v1308), 75 lines | 17 | Spellings that worked: `GetSquaredMagnitude2D`, `offset * radius * 0.75f` (the Z lane is eliminated as dead), `C3DVector(offset.Y, -offset.X, 0.0f) * -1.0f` in an if/else-if chain over `GetLastActionHandedness()`, `!GetPBaseDef().IsNull()`, `fromCentre.GetSquaredMagnitude() > GFSqr(def->RenderFadeDistance)`, a named CCharString for the region-def name, and the out-of-line CParticleUpdateParameters() 0x006E24D0. Residue: frame-slot order and the creatureDef release shape. |

Next ideas: see line 2 of each differ/ source.

## Header overlay (hdr/, all behind sub-macros; hdrcheck2: default=same for every file)
- engine/CParentDefClassBase.h: FABLE_RETAIL_PDCB_IMPLICIT_ASSIGN (new opt-in) leaves operator= implicit. 0x00431ED0 is its out-of-line COMDAT.
- engine/NUISystem_CUIDef.h: the FABLE_RETAIL_CUIDEF branch drops its 10 `_pad_` members and gains offset asserts. hdrcheck2 reports FABLE_RETAIL_CUIDEF=DIFF. All 11 landed FABLE_RETAIL_CUIDEF includers were rechecked with vland `--recheck-landed` (SIBLINGS.tsv): every one is RELOCATION_MATCH PASS.
- stlport/fable_map.h: FABLE_RETAIL_STLP_TREE_ERASE (`_Rb_tree::erase(iterator)`, `map/multimap::erase(iterator)`, `set::clear`) and FABLE_RETAIL_STLP_TREE_POSTINC (iterator `operator++(int)`). New opt-ins with no landed user; only the parked 0x00ACCBF0 uses them.
- fable_string.h: FABLE_RETAIL_CCHARSTRING_CHAR_EQ adds a `__forceinline bool operator==(char) const`. Sources: FableWin ??8CCharString@@QBE_ND@Z 0x020C8690, and the retail site 0x006162B4.
- engine/CParsedItem.h: FABLE_RETAIL_CPARSEDITEM_SYMBOL adds IsSymbol() inline and GetSymbol() 0x009B9870.
- engine/CStringParser.h: FABLE_RETAIL_CSTRINGPARSER_PEEKNEXTITEM adds PeekNextItem(CParsedItem&) 0x009BA8C0.
- engine/CTCInventoryMap.h: FABLE_RETAIL_CTCINVENTORYMAP_REGIONINFO_CTOR adds a CUIRegionInfo() that zeroes all 10 longs (Ego_r ??0CUIRegionInfo@CTCInventoryMap@@QAE@XZ).
- engine/CHitParameters.h: FABLE_RETAIL_CHITPARAMETERS_EFFECTONHIT adds IsCreateParticleEffectOnHit() and GetPHitMaterialDef() (both FableWin.pdb spellings) for the parked 0x00839420.
- engine/CThingCreatureBase.h: FABLE_RETAIL_CTHINGCREATUREBASE_LASTHANDEDNESS adds GetLastActionHandedness() (Ego_r/FableWin; its body reads +0x138 as long) for the parked 0x00839420.

Header debt: the CUIDef pad removal could become the default once the coordinator confirms the 11 includers; the other opt-ins carry no debt.

## Callee names established
- CWorld slot 8 is GetGameLODCentre. CDefClassBase slot 10 is GetInstantiationName (by value, a CDefString temp).
- CMaterialDef::GetBlendedSwordHitEffect(long, float, long&, long&, float&) const 0x008EE790 (Ego_r ?GetBlendedSwordHitEffect@CMaterialDef@@QBE_NJMAAJ0AAM@Z).
- CreateScaledEffect 0x006E11F0 (?CreateScaledEffect@@YIPAVCThing@@JJMABVC3DVector@@_N@Z). GFSqr is ?GFSqr@@YIMM@Z.
- 0x013756F5 is NGlobalConsole::CombatParticleEffects. 0x0129BA3C is DELTA_NOTIONAL_ZERO.
- CThing +0x91 bit 0 is Dead. +0x30 bit 0x10 of UsingComponents is HasTC(132) (the striker is berserk). CCreatureDef +0xCD is UseActualHitPosForHitEffects.

## GOTCHAS one-liners
- An implicit operator= (no declared base operator=) copies `_pad_` filler members too: drop pads from a class whose implicit copy is matched (0x00631720).
- A CPersistTraits optional trailing field is `PeekNextItem(item) && item.IsSymbol()` then a nested `GetSymbol() == ','` / `ReadAsSymbol() == ")"`; the char compare must be `__forceinline` (0x00615E70).
- A CScriptBase watcher whose wait loop puts a bool in bl can still use `cmp ecx,ebx` if VC keeps a zero in ebx across that loop; retail re-zeroes at the outer latch. It is a register-allocation residue, not a source-shape lever (0x00E50910, 20 grades).
- In a vector op whose Z lane is never read (`offset * r * 0.75f` used only for X/Y), VC drops the Z multiplies; write the full C3DVector expression and do not hand-split it (0x00839420).
- VC7.1 folds a vector built with a literal 0 Z (`C3DVector(y, -x, 0.0f) * -1.0f` stores Z = 0x80000000); retail's -0.0 Z store is that spelling, not an fchs (0x00839420).
- Call the out-of-line CParticleUpdateParameters() 0x006E24D0 where retail does: the landed CreateEffect row's `__forceinline` ctor is per-TU (0x00839420).
