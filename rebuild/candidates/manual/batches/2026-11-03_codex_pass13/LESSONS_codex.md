# Pass13 findings

- Large script registration functions can match on their first source strategy when reconstructed with the actual CEntityScriptBinding<T> shared template and implicit CCharString arguments. Conditional temporary lifetime bits arise naturally from each new expression.
- Original nested entity ParentClass fields are protected references, despite several old neighbour examples declaring public pointers. Query original TPI before copying these examples.
- Original script WAIT form uses NewScriptFrame followed by IsActiveThreadTerminating inside the loop, plus another termination guard after it. IsRegionLoaded(...) != false materializes the bool as retail does.
- 00cdeb40 is CPersonalScript_GlobalThingsScript::CArcheryCompetitionSignpost::Alloc, not the old descriptive free CreateArcheryCompetitionSignpost. The original PDB signature and registration literal prove the correction.
- CSpawnedFunc's inherited stronger-inline annotation has no original proof. Pass13 adds a class opt-in for ordinary inline while leaving the previous default unchanged. BanditToll currently exposes an inlining residue; it is not accepted yet.

- 00d1d100: plain shared CSpawnedFunc ctor expands at all three sites; two implicit region strings construct before allocation, while the explicit empty CCharString constructs afterward. Original Gate block and Flags=1 binding arguments give 1728/1728 bytes.
# Pass13 additional findings

- 00debef0: ordinary inline bodies of the real CScriptThing destructor and IsAlive, plus the original ChiefsHouse/SecondaryHouse lexical scopes, reproduce retail's mixed inline/call decisions on the first compile (1471/1471). No special inlining region or second class view is needed.
- CSpawnedFunc<T>::SuspendableProcess must set inherited StopRunning=true after invoking Func. Original CASuspendableProcess field evidence and three native 15-byte overrides confirm this; merely calling Func omits a required completion store.
- 00eeef70: the ordinary original header bodies reproduce one expanded cleanup exit and later called cleanups, while still expanding the spawned-function constructor (1428/1428). A callback's descriptive name is not its behavior: WaitForBoasts at 00eef510 is empty; preserve that fact in both evidence and test doubles.


- 00d51520: putting a later CTimer and its string temporaries in a sibling lexical block fixes frame0x8c→0x88 and handle slot ordering. GetTime instead of operator> restores the final exit sequence. Pause constructor and timer assignment expansion remain unresolved; equal2280-byte length is not a match. Original names and full master-data layout recovered from both procedure records/TPI and native ctor/Init/Alloc.

- 00dd35d0: binding a returned CScriptThing temporary to const CScriptThing& prevents devirtualising its IsAlive call and reproduces the native mother-handle slots, changing1453v1454 to an exact relocated1454-byte match. Original local names are recovered; the reference syntax itself is inferred from native output, not directly from the donor's object-local record.

- 00e36d50: planar `endpoint + C3DVector(dx*count,dy*count,0)` restores the native X/Y spill pattern and0xa0 frame; using the entire direction vector retains unnecessary Z arithmetic. Explicit boolean exits in text equality recover clustered null/non-null comparisons, but the fourth inline site remains open. Independently grade visible callee bodies: adding an explicit release before implicit member destruction shortened Main while making the destructor100 rather than66 bytes; rejected.

- 00d9daf0 remains parked despite all four visible handle/vector context bodies reproducing native exactly. RTM still expands handle assignment only in the first creature arm, whereas retail expands all three; direct assignment layers, original same-TU lifetime bodies, lexical scopes, recovered HobbeSize and whole-arm duplication did not repair it. Correct callee identity/body is necessary but does not itself prove the whole caller matches.

- 00e32540: assigning through original `GetMasterData()->ArcheryHighScore` evaluates and preserves the receiver before rand; direct `MasterScriptData->ArcheryHighScore` loads it afterward. This fixes the ebp cache and all following scratch roles. Source still differs at three ordinary thread constructors. The genuine UpdateGameScore/UpdateHighScore context bodies independently match26/48 bytes but do not change Main's inlining. Neither helper is an additional scope completion.

- 0098acf0: original CMatrix4x4::SetIdentity must store diagonal ones first, then individual off-diagonal zero fields in ascending order; chained assignments reverse the emitted stores. Full1333-byte match with original manager layout and STLport const-pointer/vector overloads.

-00989cb0 (verified match): original C4DVector::Initialise avoids an extra attenuation temporary. The actual donor camera call passes a separately constructed C4DVector(CameraPos,0) to the three-argument transform with a default destination; this restores matrix/camera stack placement. Original unparenthesized Y arithmetic and separate UpdateFlags OR/AND statements resolve the final x87/SIB differences. Ordinary inline fails; the needed stronger original header-helper form is documented under the newer Owner rule, not claimed as a recovered keyword.

-00efcc50 (verified): scope the original CCharString Time around iteration work so it dies before the outer NewScriptFrame, while remaining alive across dialogue wait frames. This removes the only14 instruction-line differences and matches2517bytes with ordinary inlining.

-00ec5de0 (verified): original GFArcTan/GET_ROUNDED_ANGLE chain needs float comparisons and float2pi division, then original constant for loop with NewScriptFrame iteration preserves timer cleanup. Ordinary header pause constructor remains called; documented stronger expansion of its proven original body restores both native inline sites. Exact keyword is unknown; newest Owner conditional header-helper rule applies.

-00df1830 (verified): original generated for-loop and IsDead helper restore control/cleanup shape; the final two CTimer assignments need documented stronger original header-helper expansion after ordinary forms fail. Exact original keyword remains unproven. Donor0360a190 proves the assignment body and reference return.

-00f000e0 (verified): original generated for loop and question polling reproduce all but final ToutingTimer=25. The exact original timer assignment body with documented stronger header-helper expansion removes that last10-byte gap; initial assignment already expanded. Original keyword unknown, ordinary attempts preserved.

-00d51520 (verified): exact original timer ctor and dtor must BOTH be visible to preserve TimerIndex in EDI across calls. Either body alone leaves reloads. Keep real native external calls under auto_inline(off); helper bodies separately match23/20bytes. Combined with documented original pause/timer expansion, this resolves the entire2280-byte function.
-00e70bc0 (verified): stronger expansion of the proven original pause constructor fixes the last called second site and automatically restores frame84, boolean slots and cleanup. No source rewrite needed; newest Owner helper preference and unproven keyword documented.

- 00e36d50 now MATCH1471: after genuine text-equality expansion, the remaining91 extra cleanup bytes came from expressing the outer loop as while(!terminating). The original debug donor shows constant-condition for-frame iteration and an explicit terminating return inside. Restoring that form removes unreachable normal-exit cleanup and restores all slots. All60 target operands and both real helper bodies verified.

- 00ea2e80 recovered1542 exact bytes by conditional expansion of genuine original binding and spawned-function constructors. Confirm the authored TPI attributes and original initializer/store sequence first; compiler-generated copy constructors are a separate category. All97 actual operands match.

- 00d1eb80 MATCH2441: genuine Compare body must be visible but called. A plain template definition with /Ob1 lets the compiler retain/reuse string-data pointers without expanding Compare. Stronger equality annotations were removed successfully;106 operands and1031 dual-test cases verified.

- 00e479b0 MATCH1819 and00e32540 MATCH2468: same independently proven public authored spawned-function constructor expansion gate recovered both previously parked scripts. Function/member/class names unchanged;137/153 actual operands and2688/7359 dual-compiler scenarios verified. Original constructor keyword remains unproven and is documented; no invented wrapper or generated-member body was added.

- 00501450 MATCH1334: full original STLport insert_unique body fixes temporary lifetimes; original LowerBound computes count=size before begin. Main integration exposed a spill tie dependent on incomplete CClass declarations. Removing artificial layout-check methods and restoring the genuine original authored constructor fixes it without invented engine names. Declaration context can affect a tie; preserve full original types, do not add fake members.

- 00efdc40 MATCH1847 and00d43450 MATCH2168: original constant-condition for-frame loops with explicit terminating returns avoid extra normal-exit cleanup. Debug donor entry/post-body guard sites independently prove the form. ShopKeeper needs no stronger helper annotation; WillDummy also needs the separately proven original timer assignment/GetTime bodies and documented expansion choice. Root88/94 operand audits and2857/311 cases per compiler pass.

- 0121af20 MATCH2695: the original mode-info class defined before CPlayer completes naturally produces16-byte general member pointers. Full debug/native initializer mapping proves94 callbacks and two null entries, rather than guessing by address order; all384 destination stores and94 callback operands match.
- 004fd040 MATCH2589: distinguish GetAsCharArray inline null/data access from the separately called const-char conversion. World/name offsets return const C2DVector by value. Exact original getters, float formatter and STLport tree iteration yield a first-shape match.
- 00eaafe0 MATCH2598: reuse the proven real MurderTwist owner/resource helpers, but independently prove the Brother identity, six dialogue literals and event4. Do not substitute the older brother state value.
- 00e055b0 MATCH2716: after genuine pause expansion restores the frame, original null-first GetPos/NullCoord order fixes the only branch residue. Verify raw instructions when masked shifted operands make the diff appear larger.

- 00eedc20 MATCH2728: preserve the real three-channel colour constructor calling Initialise, whose ordinary byte stores give native BGRA results. Original assigned-answer loop and locals agree with donor; existing genuine pause expansion restores the final25 bytes.

- 00f170c0 Arena Whisper: original implicit CScriptThing copy plus ordinary pause helper gives mixed inline/called decisions. Retain and verify all five switch entries inside the2724-byte extent; full131-operand root audit,1939 dual cases.
- 00dc1770 Oakvale bandit: one original shared `spotted` local across detect/chase scopes fixes stack reuse; actual authored const Thing equality must expose its counted-pointer/IsEqualTo body. Stronger helper expansion is documented policy application, not original keyword proof. Original helper first parameter is non-const CScriptThing&;116 operands/1688 dual cases.
- 00e26e30 MazeAtTavern: keep native completion check outside talked-to branch but keep success actions and normal Return INSIDE original for-frame loop. This fixes cleanup sharing after first632 instructions already match. Do not follow donor's different success nesting; no invented generated resource body.2477-byte root33 match,126 operands/1580 dual cases.

- 00d21040 prisoner (unresolved): exposing the genuine non-inline CCountedPointer<CScriptThing>::operator= gives the native nested assignment call and an independently exact72-byte helper. It does not solve mixed implicit-copy expansion: predicate copy must stay called while DestMarker copy expands. Current eligible2742/2758,1055 dual semantic cases; preserve both generated-body policy and unresolved status.

- 00dc7490 SecretPassageKeeperFriend: ordinary source with four independent speech resource scopes needs only documented expansion of genuine authored pause constructor; generated resource members stay implicit/external. Include all eight detached cancellation blocks after normal return in the2745-byte extent.125-operand main03 match,1874 dual cases.

- Arena siblings00f14a90/00f15660/00f162b0: original for-frame loops, independently recovered literal/factory identities and true implicit Thing copy reproduce mixed resource expansion without stronger helpers. Flick hints0..2 reset2→1; Needle0..3 reset3→2; Shadow0..6 reset6→5. Exact2746/2876/3368; verify each table entry and each sibling's own symbol separately.
- Generated-member eligibility audit: a helper can match bytes yet still be wrongly authored. CStringParser copy raw0x103 must remain implicit or declaration-only; removing the authored body in009bb110 retained exact2340. Incomplete Thing/resource rows were similarly restored and prior ineligible sources preserved, never silently counted as valid families.

- Guard2 00f19bb0: original implicit Thing assignment plus independent cutscene/Bert waits retains correct Turned reset behavior and matches3158 bytes; ordinary helper bodies suffice.
- KeyHoarder 00e740c0: direct normal returns inside movie lifetime and a per-frame key-removal loop match2812 bytes with ordinary authored helpers and actual generated resource members.

## 00deb280: original frame loop and authored pause body

A conditional do-loop/common-cleanup form plateaued2518/2528. The independently recovered original constant for-frame with a leading termination return produced2550 under ordinary helpers, then exact2528 with only the established genuine pause constructor expansion gate. Timer helpers stayed ordinary; resource generated members stayed implicit. Expert23/24 and independent main25 agree;1558 dual-compiler cases passed. Do not infer original stronger keyword from matching output.
