# 00e36d50 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 40. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00e36d50/NAMING_EVIDENCE.md

# 00e36d50 — archery moving-dummy script

The exact original identity is `NScript::CV_ArcheryCompetitionScript::CAC_DummyMoving::Main`, emitted as `?Main@CAC_DummyMoving@CV_ArcheryCompetitionScript@NScript@@EAEXXZ`. It is a private virtual override, not a free function. Ego_r lists it at donor00e82050 and FableWin at donor03940ff0. Native child vtable012e3178 slot1 points to00e36d50; adjacent Init/GetParentScript and the static-dummy sibling agree with the PDB class and donor order. The full native extent is1471 bytes, ending at ret00e3730e followed by alignment padding.

The shared owner header retains every original data member, including all nested child classes. Ego_r ownerTPI13011 has donor size0xb8; the proven native script base reduces it to0xa4. ChildTPI13021 is0x24 bytes: protected ParentClass is a reference at+14, PMasterData a pointer at+18, and private PointsValue/Steps at+1c/+20. Original anonymous-enum values are retained (WAITING0, COUNTDOWN1, FINISHED2, STARTED3).

Both procedure records recover reverse, dummyScriptName, hitPower, startPos, targetPos, targetThingName, targetThing and count. FableWin additionally records vector, reversevector and the generated return adapter. The two text reads really are virtual GetDataString (slot3), despite the local's name. GetPos is slot6 and projectile-hit detection slot25. The script uses endpoint Z unchanged and a planar X/Y displacement, not the full 3D vector multiplied by count.

The global is the established `?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A` at0143e90c, a CDefPointer<const CScriptDef>, not a raw pointer. New CScriptDef.h retains all943 original PDB data fields. Original donor size4436 becomes native0x1088 from the real parent-size difference and50 vectors changing16 to12 bytes. Native vtable012fb5c8 slot20, GetSizeofClass00f29500, returns0x1088. The seven score/speed offsets used here are independently visible in this row:2f8,2fc,300,380,384,388,38c. No new packing scope was introduced.

The original protected static fastcall comparator is `?Compare@?$CBasicString@D@@KIJPBD0@Z` at00411570. Both PDBs give that exact spelling. Its40-byte body was independently compiled and compared to native; every byte matches. The selected empty CScriptThing destructor plus real counted-pointer member destructor compiles to the native66-byte004aa840 body, with all three relocation identities checked. Alternative probes with an explicit additional ReleaseData/Reset in the handle destructor were rejected: they made Main shorter but produced a100-byte destructor instead of the native66 bytes. They are diagnostic history only.

The text-equality overload `??8CCharString@@QBE_NPBD@Z` is present in FableWin and is corroborated by native inline null/non-null string-comparison branches. RTM outlines its fourth use in this candidate; no retail address is assigned to that extra outlined copy. The shared text-comparison declarations are behind a new opt-in, leaving existing default text unchanged. No stronger inlining declaration is assumed.

Selected attempt14, independently regraded as35, is DIFFER(1521v1471), with the correct0xa0 frame. The remaining differences are the fourth text comparison, the final handle cleanup's inlining decision, and consequent block/slot placement. A shorter object alone was not treated as a better reconstruction. All64 target relocations were reviewed by names/literals independently of divergent byte positions; seven Game virtual calls and all79 emitted CScriptThing vtable entries were checked. No byte match is claimed.

The standalone fixture pastes the selected Main. Its independent native-block event planner covers96 configurations and6272 termination scenarios, including every scoring name, unknown/empty names, two speed sets, null target positions, forward/reverse movement, endpoint Z, score/sound/animation order, delayed starts, second rounds, and lifetime balance. GNU C++03 and real VC7.1 /GR- both pass. The fixture does not rely on copy elision. Behavior success does not remove the byte residue.

## Current accepted result

The earlier parked discussion above records the previous selection, not the current result. Root40 now verifies all1,471 bytes and60 positional operands. A narrow opt-in expands the genuine original text-equality body; the newest Owner rule's conditional preference is documented in evidence/00e36d50_INLINE_RULE_REVIEW.json. The original annotation keyword remains unproven. The original debug donor independently confirms the constant-condition frame loop and explicit termination return (evidence/00e36d50_ORIGINAL_LOOP.json), eliminating the spurious implicit normal-exit cleanup. All6,272 exact-body scenarios pass both compilers.
