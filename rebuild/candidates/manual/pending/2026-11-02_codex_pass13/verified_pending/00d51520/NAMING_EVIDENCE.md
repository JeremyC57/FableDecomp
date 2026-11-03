# 00d51520 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 28. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00d51520/NAMING_EVIDENCE.md

Current status: VERIFIED_MATCH2280bytes. Earlier PARKED discussion below is historical and superseded by the final main review.

# 00d51520 identity, original types and local names

The exact Leaf is `?Main@CQ_GuildTrainingPreMeleeScript@NScript@@UAEXXZ`, present in both original PDB name sets. Native owner vtable012cf788 slot2 points to00d51520; the registered quest, original nested binding literals and RegisterMain neighborhood independently establish identity. The assigned2280-byte span ends at00d51e07 ret and is followed by eight alignment bytes before00d51e10. No extent recut is needed.

Original Ego_r TPI0x122f7 proves CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>, seven public fields and nested classes CTheRealGuildmaster, CPreMeleeDummy, CPreMeleeWhisper. Native owner size0x58 uses the0x48 base, rather than donor0x5c. Fields are MazeArrived+48, HeroSleeps+49, WhisperCutsceneFinished+4a, PreMeleeMode+4c, DummyHits+50, GuildmasterTeleport+54 and WhisperStopFollowing+55. Every child field is retained with original access; ParentClass and PMasterData are protected. Original Alloc methods return CCountedPointer<CEntityScriptBase> by value, static fastcall, and match their binding literals/factory addresses00d51e20/00d51f00/00d52230.

Both original procedure records recover Maze, Whisper, GuildMaster, seh_Maze, seh_Whisper, seh_GuildMaster, seh_Hero, mymap, speech_movie, PauseMyEntitiesPlease and ExpressionTimer. FableWin also retains the generated TRetVoid _Fail_Val and a lexical scope containing the four resources, map, movie and pause guard. These are actual recovered variable names. The later timer sibling scope is a source-shape reconstruction that fixes native stack reuse; it is not claimed as literal recovered source text.

The full original NScript::CQ_SunnyvaleMasterData is shared in a new header, not reduced to a local layout. Ego_r TPI0x11e3a gives all fields and original enums. Native Init00cdba10 verifies their ordering; native ctor00cdb9e0 and allocator00cdbd20 prove native0x144 bytes, including12-byte BoastRegions at134 and SingingStonesInSync at140. Main accesses GuildWarningOccuring atc0. CScriptDataBase is the original four-byte CBaseClassNonCopyable-derived type (TPI0xe5ea), with OnPersist and Init in its original slots.

The map is the genuine four-argument STLport4.6 map<CCharString,CScriptGameResourceObjectScriptedThingBase,...>, not CCutsceneCastMap. Actual STLport _map.h96-99 defines a distinct default constructor and comparator constructor. Native00cdbf70 allocates a36-byte node;00cdbfb0 destroys the tree. The default constructor's full name is present in FableWin; the map value and class keys are also in both procedure TPI records. RunCutsceneMacro has its exact original two-map-pointer signature and fastcall convention at00cbfb7d.

Resource PImp fields are private in original TPI0xe629/0x2869, at+8 with count info+0xc. Both classes use their real implicit construction/destruction and original base class, with no alternate views. CTimer's TimerIndex is private at+0; exact GetTime/GetIndex/operator names come from the original type/name records, with native SetTimer/GetTimer slots89/90. The canonical pDSTGame spelling is owner-settled; original global type evidence and its sole writer SetDSTGame00cbe0c0 prove it. Native pause constructor00cbe09a and destructor00cbe0b3 bracket original PauseAllNonScriptedEntities calls. Both PDBs require a reference constructor argument; the stale pointer binding is reported in NAME_FIXES.

The selected source is behavior-correct in its dependency fixture and equal in length, but remains PARKED. Its pause constructor and timer assignment do not expand as native does. All21 grades,12 conservative strategy families, best source and two useful alternatives remain available. Neither equal length nor successful behavior tests are presented as byte identity.

Main27/28 recovers all2280bytes. After original pause/timer helper expansion, TimerIndex still reloaded; making both genuine timer constructor and destructor bodies visible under auto_inline(off) let VC cache the value in EDI. Each body alone failed. Both helpers separately reproduce their native23/20-byte bodies and global operands (constructor alignment bytes excluded). These are truthful original bodies, not stand-ins, and no out-of-scope completion is claimed. All137 COFF operands and16 virtual identities rechecked;5120 scenarios pass both compilers. The fixture now uses original ctor/dtor service calls through instrumented game methods.
