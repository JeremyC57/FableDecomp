# Identity and binding evidence

Assigned row 0x0121F280, 80 bytes. Compiler-generated dynamic initializer for CTCOracleMinigame::SymbolStrings.

- Ego_r name-set donor 01011a90 names this initializer; its next donor initializer and retail .CRT$XCU successor name the same neighbouring class array. FableWin independently lists the same initializer.
- Four retail constructor arguments are exactly `HUD_EXPR_TABLET_Y`, `HUD_EXPR_TABLET_C`, `HUD_EXPR_TABLET_A`, `HUD_EXPR_TABLET_M`. Destinations begin 0x013BAE94 and advance four bytes; the native consumer uses this table.
- Raw Ego_r TPI 0x33b4c field 0x340 proves PRIVATE STATIC CONST CCharString[4], not the existing public mutable stand-in binding. Raw global/public symbols prove `?SymbolStrings@CTCOracleMinigame@@0QBVCCharString@@B`.
- CCharString TPI0x27dc is class V, one protected CCharStringData* PStringData at +0, size4. Native constructor 0099ebf0 receives literal and long(-1), and array stride4 agrees. Exact PDB ctor `??0CCharString@@QAE@PBDJ@Z`; dtor `??1CCharString@@QAE@XZ`.
- 004012bc is CRT atexit (established exact _atexit binding); final pushed cleanup address is the generated destructor for this same array.
- Extent ends in pop ecx/ret at byte80, followed by next function; no arbitrary trimming.

Exact emitted initializer Leaf and all relocation/literal addends will be checked after compilation. _$E1 is a TU-local compiler name; different rows remain separate TUs.
