# Chunk 1018 notes (agent1018)

**Every grade ran with `FABLE_HDR_OVERLAY=$A/hdr` exported** (see `hdr/README.md`). gall.sh must be run the same way.
Compile-only sweeps (`work/probe.py`, /d2inlS / /d2inlT values) are diagnostics and are not counted as grades.

## Result: 5 RELOCATION_MATCH, 9 parked
| row | grade | grades |
|---|---|---|
| 0x00476E3E TransferVectorOfSubComponents<CEnvironmentThemeDef> | RELOCATION_MATCH, /d2inlS55 | 3 |
| 0x00477CD2 TransferVectorOfSubComponents<CEngineLocalDetailObjectDef> | RELOCATION_MATCH, /d2inlS98 | 3 |
| 0x00475DFA GFSerialiseVectorBinaryIn<C2DVector> | RELOCATION_MATCH, /d2inlS18 | 3 |
| 0x00632B90 GFSerialiseMapBinaryIn<CCharString,unsigned long> | RELOCATION_MATCH | 2 |
| 0x00F334E0 TransferVectorOfSubComponents<CConversation>(const char*, vector&) | RELOCATION_MATCH (cut 380) | 2 |
| 0x0067E290 CDegradableDef::Transfer | DIFFER(273v273, cut 273) | 3 |
| 0x00465C54 CCombatAbilityBlockDefBase::Transfer | DIFFER(203v226) | 4 |
| 0x00405220 TransferIMEColour | DIFFER(290v290) | 1 |
| 0x006C9350 CPersistTraits<pair<long,COpinionDeed*> >::TransferIn | DIFFER(314v312) | 2 |
| 0x005D8978 TransferMap<long,unsigned long> | DIFFER(754v754) prior draft | 1 |
| 0x0047042A CSoundMap::TransferBinaryIn | DIFFER(716v716) prior draft | 1 |
| 0x005C0A98 CPersistTraits<NInventory::CItem>::TransferBinaryOut | DIFFER(365v369, cut 369) prior draft | 1 |
| 0x006DC6A0 CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn | DIFFER(437v447, cut 447) prior draft | 1 |
| 0x009E81E0 CPersistTraits<C2DVector>::TransferIn | DIFFER(701v750) prior draft | 1 |

The grade budget is 10 per row. The last five rows got only one grade each, a re-grade of the prior draft. Their drafts still
use local padded models, so each needs a port to the shared headers before more grades are worth spending (see below).

## Identities proven (two sources each)
- **0x00476E3E** = `CPersistContext::TransferVectorOfSubComponents<CEnvironmentThemeDef>`.
  - Ego_r/FableWin template instance, PDB form `(const char*, vector&)`.
  - TLC's release persist macro drops the name: CheckCRC on the pooled "" 0x0122D70E, `ret 4`. This is the same convention as
    landed 0x004F9C07.
  - Stride 0xC4.
  - Element vtable 0x01236244 (RTTI).
  - Caller CEnvironmentThemeDaySetDef::Transfer 0x00476DC3 (call 0x00476DD1).
- **0x00477CD2** = `TransferVectorOfSubComponents<CEngineLocalDetailObjectDef>`.
  - Stride 0x88.
  - Vtable 0x0123634C.
  - Caller CEngineLocalDetailLayerDef::Transfer 0x00477CB1 (call 0x00477CC9).
- **0x00475DFA** = `GFSerialiseVectorBinaryIn<C2DVector>`.
  - Ego_r mangled name.
  - Per-element call to Read2DVector 0x00995030 (Ego_r name).
- **0x00632B90** = `GFSerialiseMapBinaryIn<CCharString,unsigned long,std::less<unsigned long> >`.
  - Ego_r mangled name.
  - Callees: 0x0040F8A0 CPersistTraits<unsigned long>::TransferBinaryIn, 0x00630FE0 tree clear, 0x00632C90 insert_unique.
- **0x00F334E0** = `TransferVectorOfSubComponents<CConversation>(const char*, vector&)`.
  - Named overload, Ego_r.
  - Stride 0x5C.
  - Wrapper vtable 0x01301C2C = Size / Resize / AreDefaultValsApplied / SetDefaultValsApplied / Transfer (Ego_r names).
  - Caller 0x00F2F77A passes "BookReactions".
  - The catalog name `Transfer<signed_char>` is wrong.
  - Real length 380 (code plus the 20-byte jump table at 0x00F33648). The four wrapper virtuals follow at 0x00F33660 / 0x00F33680 /
    0x00F336A0 / 0x00F336D0 (SPLITS.tsv).
- **0x0067E290** = `CDegradableDef::Transfer`.
  - Vtable slot 18.
  - Ego_r name.
  - Length 273.
- **0x00465C54** = `CCombatAbilityBlockDefBase::Transfer`.
  - Vtable 0x01232E04 slot 18.
  - Ego_r name.
- **0x006C9350** = `CPersistTraits<std::pair<long,COpinionDeed*> >::TransferIn`.
  - Ego_r/FableWin.
  - The body calls CPersistTraitsSLONG::TransferIn into +0 and CPersistTraits<COpinionDeed*>::TransferIn 0x006C9490 on +4.
  - The catalog label CAtmosBankEntry is propagated and wrong.
- **0x006DC6A0** = `CPersistTraits<CAttackHistoryCombo>::TransferBinaryIn`.
  - Ego_r + FableWin.
  - Only caller is the vector loop 0x006DD580 (call 0x006DD5F2).
  - The catalog label CHeroStatIncreaseData is wrong.
  - Length 447. A new start at 0x006DC860 (56 bytes) is an unnamed binary map-transfer wrapper: mode 2 calls 0x006DBA50, mode 3 calls
    GFSerialiseMapBinaryOut<CDefString,long> 0x006DAE40.
- **0x009E81E0** = `CPersistTraits<C2DVector>::TransferIn`.
  - Ego_r name.
  - Strings verified in the exe: "C2DCoordF" 0x0129C288, "Unknown command for C2DCoordF" 0x0129C268, "X" / "Y".
  - The catalog label CBrainBehaviour is wrong.
- **0x005C0A98** = `CPersistTraits<NInventory::CItem>::TransferBinaryOut`.
  - Ego_r + FableWin.
  - Ends `leave; ret 8` at 0x005C0C05, so the length is 369.
  - It is followed by a 19-byte 5-dword zeroing ctor at 0x005C0C09 and an 8-byte thunk at 0x005C0C1C (owners unproven).
- **0x005D8978** = `TransferMap<long,unsigned long>`. Ego_r + FableWin `??$TransferMap@JK@...ABJ@Z`; the catalog `<long,long>` is wrong.
- **0x0047042A** = `CSoundMap::TransferBinaryIn`. Ego_r + FableWin.

Callee name fixes are in NAME_FIXES.tsv: 0x00995030, 0x00475E92, 0x00452B4D, 0x00630FE0, 0x00F336F0, 0x00F33730, 0x009E2590.

## Unmatched rows: residue and what each needs next
- **0x0067E290 (differ/0067e290.cpp)**
  - Only the expression-temporary rotation in the GraphicType / bool arms and the sort push remains (c2prio `--rotation`).
  - Retail behaves as if the enum save value took eax by hint.
  - A named `CIsHealth1GTHealth2 compare;` local removed a zero store.
  - Next: a different enum save spelling (traits object vs direct WriteSLONG), or the PDB `const bool&` traits form.
- **0x00465C54 (differ/00465c54.cpp, /d2inlS20)**
  - The set range ctor / insert / operator= inline split differs from retail at every S value.
  - Next: c2 trace of the `_M_range_initialize` and `insert_unique(first,last)` costs.
- **0x00405220 (differ/00405220.cpp)**
  - Only the CRGBColour return packing remains: retail reloads the dword and splits it with `movzx edx,ch ; movzx ecx,cl`.
  - Next: return-by-value spelling of IMEColour2RGB / the RGB2IMEColour argument.
- **0x006C9350 (differ/006c9350.cpp)**
  - Retail expands CCharString::operator==(const char*) at all three symbol checks.
  - But at the "," check it calls the inner CBasicString<char>::operator== 0x004115A0 out of line. The bool sits in a frame
    byte, so the context stays in ebx.
  - VC7.1 expands all three. /d2inlT <= 55 drops the outer operator first; /d2inlS has no effect in this speed TU.
  - Next: a scoped `#pragma inline_depth` around the "," check (ladder step 3), or the PDB's operator!= spelling at that site.
- **0x005D8978, 0x0047042A, 0x005C0A98, 0x006DC6A0, 0x009E81E0** (differ/ = the prior drafts, re-graded unchanged)
  - **Header debt:** they model CCharString, CStringParser, CPersistContext, streams and vectors locally.
  - Port each to the shared headers first: fable_string.h, engine/CStringParser.h, engine/CPersistContext.h,
    engine/CMemoryDataInputStream.h, engine/CDataOutputStream.h, engine/CItem.h, engine/CVectorMap.h and stlport/.
  - Then work the residue named on each file's line 2:
    - frame slots (0x005D8978)
    - SIB operand order `[eax+ecx+4]` (0x0047042A)
    - register allocation (0x005C0A98, 0x006DC6A0)
    - member-name test jump threading (0x009E81E0)
  - The landed 0x00756D00 TransferMap<pair<long,long>,CCharString> body is the template to port 0x005D8978 from.

## Flags and inline budget
- /d2inlS is the VC7.1 size-mode inline threshold (expand iff cost < S). It is non-monotonic per TU.
- The matched values came from compile-only sweeps:
  - 0x00476E3E expands for S55..S89 and S98.
  - 0x00477CD2 expands for S98..S120.
  - 0x00475DFA expands both decisions from S18.
- Each src file carries its `// Inline-budget:` note on line 2.
