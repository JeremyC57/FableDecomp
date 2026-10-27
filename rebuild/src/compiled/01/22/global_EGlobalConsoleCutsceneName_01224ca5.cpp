// _global::$E_GlobalConsoleCutsceneName() -- dynamic initializer of GlobalConsoleCutsceneName: construct the empty CCharString, then register its destructor with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224CA5, 29 bytes, in `.text$yc` of a size-optimised TU (packed, unaligned
//   address), reached from the .CRT$XCU table, plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb `dynamic initializer for 'GlobalConsoleCutsceneName'`, the last of the
//   ScriptActiveStatuses / HACK_GameInfoString / pGlobalScriptDef / GlobalConsoleCutsceneName run, which the PDBs also place at
//   unaligned addresses (/Os); its predecessor here is the pGlobalScriptDef atexit-only initializer 0x01224C99. The string
//   0x0143E91C is read by NScript::CCS_PlayCutsceneScript::Main 0x00F01480 (the console cutscene to play) and by
//   GetNumberFromDataMarker 0x00CD5090; the registered thunk 0x0122AE74 is `mov ecx,0x0143E91C ; jmp ~CCharString`.
// Label: the manifest's "sub_01224ca5" is a gap-scan placeholder; the landed thunk 0x0122AE74 is named
//   _global::GlobalMethodTailThunk_0122ae74 / AtexitDtor_CCharString_at_0143e91c.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0 (through the thunk), _atexit 0x004012BC.
//   Data: the TU's own "" literal 0x0122D70E.
#pragma optimize("s", on)

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
};

CCharString GlobalConsoleCutsceneName("");                // 0x0143E91C