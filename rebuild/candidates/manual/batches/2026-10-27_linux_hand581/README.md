# Chunk 581 (2026-10-30): 8 of 10 released pass 9 rows landed; first chunk entirely on the wave-1 retail NScript headers

All RELOCATION_MATCH + PASS (g++ and VC7.1). Script rows: CTonyTestScriptsScript::Main, CV_HeroDuelScript::WatchForLeaving,
CQR_EscortTraderScript::DoMission, CQS_GuardianSisterInfo2_SisterInBanditCampScript::Main, CV_RandomPopulationSimScript::
CRandomPopulationCreature::Init. STLport rows: vector<CFeatDef> / <CArenaCreatureDef> / <CFireHeartPatternDef>::_M_fill_insert.
Parked (../../pending/2026-10-27_linux_hand581/differ/): vector<CArenaWaveDef>::_M_fill_insert 0x00F281E0 (558v481) and
vector<CHeroSoulsCreatureDef>::_M_fill_insert 0x00F277F0 (601v493): the implicit element copy ctor is inlined at all three
sites by VC7.1 but retail inlines it only in x_copy and calls it out of line (0x00F25C60 / 0x00F27060) in the raw-end loops;
inline_depth 0-4, auto_inline off, user inline / out-of-line copy ctors, _Construct variants and LTCG all tried. Next: the
real STLport inline chain or QFE-4035.
Every script row uses FABLE_RETAIL_NSCRIPT (CScriptBase / CScriptThing / CEntityScriptBase / interface headers); the def rows
use the corrected 0x25 CDefClassBase. Remaining local models (NOTES.md): CScriptDef and CQ_SunnyvaleMasterData opaque data
(retail vptr classes with no header yet), CSpawnedFunc<T> (the template every spawning row defines locally).
pGlobalScriptDef is declared `CScriptDef*` (the bound spelling); Ego_r has `CDefPointer<const CScriptDef>`; a CDefPointer
model changed register allocation (537v540) -- revisit when the wave-2 CDefPointer<T> header lands.
NAME_FIXES.tsv: 6 rows queued (three landed script members in the global namespace, 0x00EC1B70 class name,
0x00CB7940 / 0x00CB7E50 junk bindings).
