# 00f170c0 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 3. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00f170c0/NAMING_EVIDENCE.md

# 00f170c0 naming evidence

The original function is NScript::CQ_ArenaScript::CCellWhisper::Main. Both corrected original name sets contain its exact private Main mangling. Ego_r TPI0x11b55 gives the full CQ_ArenaScript owner and TPI0x11bed gives the nested 0x1c CCellWhisper. Factory00f16fe0 independently allocates0x1c, stores the owner reference at+0x14 and master-data pointer at+0x18, and installs vtable012f9458. Its slot1 is00f170c0. Empty Init00f17090 and parent getter00f17080 independently agree.

The original public long ArenaRound is at PDB+0xc0. Retail base contraction0x14 and STLport vector contraction4 yield native+0xa8. Original public bool InHitCutsceneAlready moves from+0xd4 to native+0xbc. The complete owner header contains all original members, enum and all15 nested entity classes, with access preserved. Its retail size is0x11c; CCellWhisper remains0x1c. CArenaRoundDef is only a forward declaration used by the genuine std::vector member.

PDB function-local records name seh_me, both speech_movie and PauseMyEntitiesPlease scopes, _Fail_Val, and FableWin's long RoundComment. The original donor independently confirms the for-frame loop, exact five-case switch, rand()%(ArenaRound-2), attack short-circuit, resource claim/clear/release scopes, and hit-cutscene latch. Native confirms all strings, flags, priorities, virtual call slots and constants.

Both PDB method-list records identify CScriptThing's copy constructor as compiler-generated: Ego_r TPI0x28aa and FableWin TPI0xbcd4 have attrs0x103 for the copy constructor, versus0x003 for the default constructor. The narrow FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY opt-in omits the pre-existing copy declaration and lets VC7.1 generate it. The default branch is unchanged. No authored body or stronger inline substitutes for the generated member. Native's four by-value copies store CBaseClass then CScriptThing vtables, copy PImp and increment the shared reference count, matching the generated body. CBaseClass's implicit copy and the original authored CCountedPointer copy supply that behavior.

STB and Movie special members are implicit. The ordinary authored CScriptThing destructor remains inline; Movie's CCountedPointer destructor is an external specialization declaration. VC7.1 reproduces native's mix of full, partial and out-of-line destructor expansion. The ordinary pause constructor expands in the first movie and remains a call in the second, exactly as native. No stronger helper is used.

Return00f17b4e and one-byte alignment precede the five-entry switch table00f17b50..00f17b63; twelve CC bytes follow before the next factory. The assigned2724-byte extent includes all five table entries. Exact Leaf is independently extracted from the selected COFF. Every131 relocation is verified, including the table-base reference and five local case-label targets. Nineteen Game slots and five Thing slots are independently checked. CRT rand and delete are bound through native import thunks, not catalog labels.

Evidence:00f170c0_types.json,00f170c0_enums.json,00f170c0_function_locals.json,00f170c0_COPY_MEMBER_TPI.json,00f170c0_factory.txt,RELOCATIONS.json and author review. The earlier CFlick candidate query is preserved only as discovery history.

Main03 independently rebuilt the exact source with shared original Arena owner, implicit Thing-copy gate and ordinary helpers: RELOCATION_MATCH2724, all131 operands including the five-entry switch table and24 virtual identities checked,1939 cases/compiler PASS.
