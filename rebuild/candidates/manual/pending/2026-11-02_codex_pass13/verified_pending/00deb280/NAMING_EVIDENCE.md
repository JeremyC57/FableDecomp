# 00deb280 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 25. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00deb280/NAMING_EVIDENCE.md

# Identity and extent

The selected leaf is `?Main@CVictimsBrotherHelper@CQ_RansomVictimScript@NScript@@EAEXXZ`. Both original PDB symbol streams and complete TPI records prove the private virtual override: Ego_r child12298, owner1224b; FableWin child12452, owner12405. `evidence/00deb280_types.json` preserves the original records and raw field-list bytes. This is a class, with protected parent reference+14 and master-data pointer+18, size1c; the original constructor is not manually authored.

The independent native chain is its own registration: `00de0a69` pushes `012dd074` = `VictimsBrotherHelper`; `00de0a90` stores allocator `00de1fb0`. That allocator requests1c bytes at `00de1fb3` and stores table `012dd21c` at `00de1fe4`; table slot1 at `012dd220` is `00deb280`. Neither neighboring function order nor ROW label is used as identity proof. The leaf ends with ret at `00debc5f`; the owner allocator begins immediately at `00debc60`, without padding. Extent2528 is correct.

# Layout and helpers

The complete owner has all23 original data members, ending with five16-byte scripted-thing resources. Original Ego_r fields are shifted by14 because its base is5c versus retail48. The owner allocation at00debc60 isbc, constructor00de0320 stores its own table012dce70, and this leaf accesses RescuedVictim+4b, ThanksCutsceneStarting+53, MissionSucceeded+49, HitEvilBrotherAndProtection+58, SomeoneDealingWithBeingHit+5c and seh_Brother+9c. All ten original nested children and their entire layouts are present in the shared private header. GetParentScript is const; GetMasterData is not const. Native master-data write+12d matches the existing complete CQ_SunnyvaleMasterData::RansomVictimHaveVictimKilled.

`PlayCutscene` is public, thiscall, and owns two by-value CCharString arguments in both original TPIs. Native00de6730 uses this owner's resource fields+6c/+7c/+8c/+9c/+ac, builds the role map and calls RunCutsceneMacro at00de6b25. It destroys both argument strings at00de6c00/00de6c09 then ret8 at00de6c11. The caller constructs NULL and CS_RANSOM_OUTRO_EVIL directly in the two stack argument slots. The owner-specific native body and both original public spellings jointly bind the callee.

Both concatenation overloads are independently distinguished by the native chain: at00deb684 ECX is the result slot, EDX points to TEXT_QST_B06_, and the stack holds Me.GetDataString()'s CCharString reference, proving fastcall0099f690 `(const char*, const CCharString&)`. The returned CCharString becomes EDX for0099f600 at00deb68f, with `_CHAT` on the stack, proving `(const CCharString&, const char*)`. CCharString's const-char conversion0099e4c0 follows, then char-pointer Speak007e7390 with random-no-repeat2,false,true,false. All three owned concatenation strings are destroyed in reverse order.

The original CTimer bodies are additionally checked against donor0360a120/160/190/1d0/200 and native00cd4450/4470. RegisterTimer87 stores TimerIndex; destructor deregisters88; assignment calls SetTimer89 and returns*this; comparison calls GetTime, whose body uses GetTimer90. The private TimerIndex opt-in is selected. The selected timer helpers retain ordinary inline annotations; the final pause constructor exception is documented below. Natural generated resource constructors remain natural; only the original destructor is declared for native out-of-line calls.

`evidence/00deb280_locals.json` preserves both original procedure/local scopes: seh_me, TurningTimer, speech_movie, PauseMyEntitiesPlease, Brother; FableWin additionally records _Fail_Val and HLStyleConversationIndex. The selected body uses these original names. Earlier diagnostic reference/text locals are not selected.

# Historical validation of superseded attempt19

Selected attempt19 is DIFFER(2518v2528), with framea4 but timer index caching and cleanup distribution residue. Twelve independent source families remain in immutable history. All121 actual COFF operands and27 virtual identities are reviewed by `development/00deb280_review.py`; each relocation is tied to its actual object instruction and independent native target or exact literal payload. There is no same-offset target claim after divergence. Native call argument order, owner tables and callee bodies are recorded separately from symbol existence. Existing aliases are listed in NAME_FIXES.json, not reintroduced.

The standalone C++03 fixture contains the exact selected Main body. All1558 cases pass GNU64 and native VC7.1 /GR-: seven initial flags, six hit counts, health including NaN, acquisition retries, speech delays, timer values, all revenge/dialogue arguments and1200 termination checkpoints. Behavior success is not a byte-match claim. Immutable source, complete header snapshot, actual RTM object and compiler logs are under history/00deb280/attempt_19; selected hashes and both test receipts are in SELECTION.json.

# Current verified selection

Expert23/24 restored the original constant for-frame loop, independently confirmed in donor03891480. Ordinary22 calls all three authored pause constructors; the existing justified pause expansion gate restores native framea4. No generated member body was authored. Main independently rebuilt25 on current shared headers and verified the exact Leaf,120 positional operands,27 original virtual identities, full2528 relocated bytes and own registration/factory/vtable identity. GNU64 and VC7.1 /GR- each pass1558 behavior cases; selected fixture now exercises actual original timer and pause helper bodies. Current proof is reviews/00deb280/RESULT.json, SELECTION.json and HEADER_HASHES.json, replacing the above historical mismatch.
