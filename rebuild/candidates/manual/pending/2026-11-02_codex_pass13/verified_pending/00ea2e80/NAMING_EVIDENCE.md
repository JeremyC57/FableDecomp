# 00ea2e80 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 19. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00ea2e80/NAMING_EVIDENCE.md

# 00ea2e80 identity and selected residue

Exact original symbol `?Main@CV_LostTraderScript@NScript@@UAEXXZ` occurs in both PDB name sets. Native vtable 012edce8 slot 2, the original RegisterMain neighbour 00ea2de0, and registered script V_LostTrader identify the row. Ego_r TPI 0x11aaf gives all seven owner fields and the complete three nested entities; native Init 00ea2d20 independently proves their state offsets. Header size 0x58 accounts for the native base 0x48 versus the donor debug base 0x5c.

Original FableWin procedure locals prove `HeroEnteredTheLevelFromCorrectEntrance`; the optimized Ego_r record elides it. Its native distance-result bool controls repeated entry attempts. Original strings distinguish BanditCampPath1 (quest objective) from BanditCampPath_1 (level readiness); those spellings are deliberately not unified. IsDistanceBetweenThingsUnder is exact `?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z` at 00cbe2ff, present in both donors and independently consistent with the native 114-byte alive/position/distance implementation.

The thread callback 00ea3490 is `?WatchForQuestFailure@CV_LostTraderScript@NScript@@QAEXXZ`, proven by both PDBs, the current owner's member-pointer argument, and literal WatchForQuestFailure. The old label CV_BeardyBaldyScript::WatchForQuestFinished is wrong and is reported in NAME_FIXES. Binding factories follow the three original literal/nested-class pairs.

Ten distinct source strategies preserve the closest first source: 1478 bytes versus native 1542. The early control flow, destructor expansions and frame agree apart from exit distances. Three binding constructors and one thread constructor remain out of line where native expands them. A closer length under an explicit loop break is rejected because the block/register residue grows. No match is claimed. The complete assigned span ends at 00ea3485 ret.

The exact selected Main is pasted into a self-contained call-trace behavior fixture. Its external script is built from native-block order and exercises zero to two wrong entrances, both immediate/delayed load and unload, reached-trader waits, every terminating guard and all four allocation-failure sites. All 2700 scenarios pass GNU C++03 and real VC7.1 /GR-. Behavior success does not resolve the byte mismatch.

## Expert exact constructor expansion

Actual original CEntityScriptBinding<NScript::CV_LostTraderScript> allocator ctor03b26c20 calls original binding base default ctor, stores own vtable, copies EntityScriptName, then PParentScript, ClassAllocFunc, ClassAllocValid(true), Flags; Func is untouched. Native offsets are4/8/10/14/18 while debug offsets4/c/14/18/1c reflect donor eight-byte CCharString. Original TPI marks this public authored ctor0x3. A separate callback overload and compiler-generated copy0x103 exist in TPI; no unused overload body is invented.

Actual original CSpawnedFunc<NScript::CV_LostTraderScript> ctor03b26ba0 calls CSpawnedFuncBase(name,id), then stores Func and PClass in that order. Native offsets34/38 are donor3c/40 minus base-size difference8. Original TPI marks this public authored ctor0x3 and distinct copy0x103. Current exact helper bodies are thus independently proven; native Main expands all four sites. Stronger forms use separate row-defined gates under latest Owner conditional preference, and do not claim original keyword proof.

All97 positional target identities/payloads now match. Binding own vtable012edd54, spawn012edcfc, original factories00ea34f0/00ea4ff0/00ea5150 and callback00ea3490 are checked at their actual operand sites. Main endsRET00ea3485 and ten alignment bytes precede WatchForQuestFailure00ea3490. Frame0x40 matches.

Current result supersedes the earlier parked paragraphs: main19 confirms1542 bytes and97 positional operands,2700 scenarios per compiler. Exact original keyword remains unproven; original body and type are proven, and the new Owner conditional helper preference is documented.
