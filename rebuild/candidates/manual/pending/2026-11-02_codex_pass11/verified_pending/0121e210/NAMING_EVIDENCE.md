# Identity and binding evidence

Assigned row 0x0121E210, 80 bytes. Compiler-generated dynamic initializer for CTCWife::CourtingAdviceGroupNames.

- Ego_r name-set donor 01010cc0 names this initializer; its next donor initializer and retail .CRT$XCU successor name the same neighbouring class array. FableWin independently lists the same initializer.
- Four retail constructor arguments are exactly `TEXT_AI_COMMENT_COURTSHIP_SUGGEST_ROMANTIC_EXPRESSION`, `TEXT_AI_COMMENT_COURTSHIP_SUGGEST_ROMANTIC_GIFT`, `TEXT_AI_COMMENT_COURTSHIP_SUGGEST_IMPROVE_APPEARANCE`, `TEXT_AI_COMMENT_COURTSHIP_SUGGEST_INCREASE_RENOWN`. Destinations begin 0x013BAB30 and advance four bytes; the native consumer uses this table.
- Raw Ego_r TPI 0x1a9a9 field 0xd74 proves PRIVATE STATIC CONST CCharString[4], not the existing public mutable stand-in binding. Raw global/public symbols prove `?CourtingAdviceGroupNames@CTCWife@@0QBVCCharString@@B`.
- CCharString TPI0x27dc is class V, one protected CCharStringData* PStringData at +0, size4. Native constructor 0099ebf0 receives literal and long(-1), and array stride4 agrees. Exact PDB ctor `??0CCharString@@QAE@PBDJ@Z`; dtor `??1CCharString@@QAE@XZ`.
- 004012bc is CRT atexit (established exact _atexit binding); final pushed cleanup address is the generated destructor for this same array.
- Extent ends in pop ecx/ret at byte80, followed by next function; no arbitrary trimming.

Exact emitted initializer Leaf and all relocation/literal addends will be checked after compilation. _$E1 is a TU-local compiler name; different rows remain separate TUs.
