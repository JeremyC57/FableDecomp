# Hand chunk 2036 (CTCInventory .. CTCInventoryAbilities): 1 of 2 byte-exact (1 RELOCATION_MATCH, 1 not attempted)

## What matched (1)
- **0x005841F9 CTCInventory::GetItemDescription(CWideString& text, long itemDefIndex, bool hidePrice) const** -- 2033 bytes,
  RELOCATION_MATCH at grade 4 of pass 1; behaviour test PASS (g++ and VC7.1); read-only vland: RELOCATION_MATCH + PASS, no
  RELOC_IDENTITY. Appends the price (stock def default, or the buy price of the live shop the hero stands in that stocks the
  item), then for a hair / tattoo card the attractiveness / scariness / alignment lines (hair card also "(change)" against the
  hero's worn modifiers of the same body area), then CTCInventoryItem::GetDetails.

### Start / extent (the brief asked for proof)
- 0x005841F9 is a real start: the preceding bytes end with `5b 5d c2 0c 00` (`pop ebx ; pop ebp ; ret 0Ch`) at 0x005841F4..F8, no
  padding (packed TU); the function's only reference is the CTCInventory vtable slot 78 (0x01250F1C).
- It ends at 0x005849E7 `leave ; ret 0Ch` (0x005841F9 + 2033 = 0x005849EA), and 0x005849EA starts the next function at once
  (`push ebp ; mov ebp,esp ; push ecx`). No cold code, no funclets (no EH frame). Extent 2033 confirmed; no SPLITS entry.

### Names (two sources each)
- Row: Ego_r.pdb + FableWin.pdb `?GetItemDescription@CTCInventory@@UBEXAAVCWideString@@J_N@Z`; vtable 0x01250DE4 slot 78 =
  the GetItemDescription slot (CTCInventoryBase 0x005BC6B3 in the Experience table, landed Clothing 0x005B5CF8 / Weapons
  0x005C7913 overrides); `ret 0Ch`; landed ConstructItemDescription 0x005819DD calls slot 78 with false. The catalog label
  CTCShop::BuildItemDescription is propagated and wrong (NAME_FIXES.tsv).
- Callees: every call target checked with rel.py (all "same" as reloc_bindings.tsv) except the three CTattooDef::GetTypeBalanced*
  (unbound; Ego_r spelling, LABELS.tsv). Parameter names are descriptive (the PDB keeps none): `text`, `itemDefIndex` (as the
  base declaration), `hidePrice` (true = no price line; ConstructItemDescription passes false).
- Descriptive (no PDB record): the CGameDefinitionManager +0xDC member name `PHeroStatsDef` (existing header spelling).

### How it was matched (grade log, pass 1; every grade = q.py on the full row)
| grade | source | result | what it showed |
|---|---|---|---|
| g1 | first draft (probe/v1.cpp): price block as ConstructWeaponDescription, stat blocks as Clothing GetItemDescription, `tattooCard->` calls, PeekHeroStatsDef() reference, one re-assigned `value`/`change` | DIFFER(2033v2033), 581/651 equal | only the tattoo branch reloaded `[ebp-0Ch]` per call (retail `mov edi,[ebp-0Ch] ; test edi,edi ; mov ecx,edi` x3) + frame slots |
| g2 | probe/v2.cpp: `const CTattooDef* tattoo = tattooCard.Get();` inside `else if (!IsNull)` | DIFFER(2027v2033) | tattoo in edi, but `text` lost ebx (c2prio: text 28 = value 28, value wins the tie on its key) |
| g3 | probe/a5.cpp: stats read as `GetDefinitionManager().PHeroStatsDef` (no accessor temp) + `const CTattooDef& tattoo = *tattooCard;` | DIFFER(2033v2033), 621/651 | all code equal; only the three stat values' frame word ([ebp+8] v retail [ebp-34h]) and the label / literal temporaries |
| g4 | probe/b1.cpp = src: one named value per stat (attractiveness / scariness / alignment + ...Change) | **RELOCATION_MATCH** | |
| (regrade) | src after comment-only edit, via gall.sh at the end | RELOCATION_MATCH | |

Free measurements (not grades): ~10 c2prio runs (bin/c2p, copied from agent2027) and 2 frame-packer traces (bin/pk, copied from
agent3016 pk55) plus compile-only probes (bin/cc.py). Key numbers:
- text's colouring priority: v1 29 (ebx), v2 28 (tattoo local + Get() load are two candidates in the tattoo block: K 5 -> 6),
  v3 `else if (const CTattooDef* t = tattooCard.Get())` 27, v4 (else { ptr; if (ptr) }) 27, v5 (`const CTattooDef&` after the
  IsNull test) 28; the first stat value (attractiveness) is 28 with key 369 in all of them, text's key is 2 -> text needs 29.
- The PeekHeroStatsDef() reference accessor creates an inline-return candidate (`local`, line of the stats declaration, prio 24)
  in the outer block; reading the member directly drops it: +1 for text in every variant (a1 = v3 + member read: 28; a5 = v5 +
  member read: 29 -> ebx). Modelling the member as CDefPointer<const CHeroStatsDef> (tmp/hdrx probe) keeps the accessor temp.
- Byte-neutral spellings measured without effect on the priorities: nested v flat `wornDef` tests, `if (!hairDef.IsNull())
  GetSubDef; if (!modifier.IsNull())` split, `hairCard.Get() || tattooCard.Get()`, named `current` for the second ftol,
  value/change declared at the block top.
- Frame packer (b1 trace): with one re-assigned `value` the packer sees one node w9 that is listed first among the 4-byte nodes
  and takes the dead `text` home [ebp+8]; three named values are w3 nodes that reach w3 after the label CCharString temporaries
  (w3, escaping), so the temporaries take [ebp+8] first and the values conflict and go to the [ebp-34h] slot (with hairDef, the
  GetTC key temporaries and the shop-thing copy), exactly retail's layout.

## Parked for Fable 5.1 (0)

## Not attempted (1)
- 0x005F6E40 CTCInventoryAbilities::OnSerialise (1746 bytes): not started -- the brief allows it only with well over half the
  context left after row 1, which was not the case after the measurement work. Extent checked for the next agent: it starts right
  after 0x005F6E3D `ret 0Ch` (only reference: .rdata 0x01256AD8, a vtable slot) and ends at 0x005F750F `leave ; ret 8`
  (0x005F6E40 + 1746 = 0x005F7512). The bytes from 0x005F7512 are separate tiny unreferenced accessor COMDATs (`mov eax,[ecx] ;
  ret`, `mov eax,[ecx+4] ; ret`, `mov eax,[ecx+4] ; sub eax,[ecx] ...`), so the reservation's 2113-byte estimate over-counts and
  1746 is right. Outline from dis_all.txt (verified call targets only): `CPersistContext::IsSaving` 0x004AEF50 first (bound
  ?IsSaving@CPersistContext@@QBE_NXZ), then a save path and a load path over the ability table at +0x160 (stride 8, 0xA0 bytes)
  and the vectors at +0x154 / +0x16C / +0x17C / +0x188 with the transfer-name strings 0x01256F94 / 0x01256F84 / 0x01256F74 /
  0x01256F60, ending with the base call 0x005BFD5E (bound ?OnSerialise@CTCInventoryBase@NInventory@@MAEXAAVCPersistContext@@_N@Z).

## Skipped
- none.

## Label corrections
- 0x005841F9: catalog / manifest `CTCShop::BuildItemDescription` -> `CTCInventory::GetItemDescription` (UBE const virtual).
- 0x0057FC46: catalog `CTattooDef_Methods::GetTypeBalancedAttractiveness` (landed leaf with a stand-in class and a vector without
  its allocator argument) -> `CTattooDef::GetTypeBalancedAttractiveness` (Ego_r spelling); 0x0057FC71 / 0x0057FC9C landed leaves
  are bare names (fine) but should be respelled on the real class too.
- 0x01231728 is the pooled wide literal L"\n"; reloc_bindings binds it as `_s_newLine` (a TU-static array stand-in of the landed
  CProgressDisplay::WordWrapText 0x00497A60). Our literal spelling is retail's (the Clothing description rows use it too).

## Header debt
- engine/CGameDefinitionManager.h: +0xDC hero-stats def modelled as a raw pointer with a descriptive name; FableWin's
  GetPHeroStatsDef returns CDefPointer<const CHeroStatsDef>, so the member is probably that CDefPointer (byte-identical layout).
- Overlay opt-ins (hdr/README.md): CTCInventory.h FABLE_RETAIL_CTCINVENTORY_ITEMDESCRIPTION, CTattooDef.h
  FABLE_RETAIL_CTATTOODEF_TYPEBALANCED.

## Files
- src/005841f9.cpp, test/005841f9.cpp (final). probe/v1..v5, a1, a5, b1 (= src without the final comment), c1, q1/q4/q6, s0/s2/s4
  (c2prio variants). tmp/c2p_*.txt (c2prio outputs, `b` = with --blocks), tmp/md*.txt (masked diffs of g1..g3).
- Tools copied (read-only originals untouched): bin/c2p (agent2027 c2prio), bin/pk (agent3016 pk55 run.py / phook + sim2 / qs),
  bin/md.py + bin/rel.py (agent2004), own bin/cc.py (compile-only), bin/gr.sh (grade + diff), bin/pr.sh (c2prio summary),
  bin/bk.py (per-block priority diff of two c2prio --blocks runs).
