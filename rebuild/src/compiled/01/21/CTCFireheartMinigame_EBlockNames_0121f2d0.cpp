// CTCFireheartMinigame::$E_BlockNames() -- dynamic initializer of the static CCharString CTCFireheartMinigame::BlockNames[4][4] (the Fireheart minigame block marker script names "BLOCK00" .. "BLOCK33", one CCharString(const char*, -1) per element), then registers the array's atexit destructor
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x0121F2D0, 284 bytes, in `.text$yc`, reached from the .CRT$XCU table entry 0x01374620, plain `ret`.
//   Registers the atexit thunk 0x01229E20 (the compiler's reverse-order ~CCharString loop over the 16 elements at 0x013BAEB0;
//   landed under the wrong label "CFontBankStateBlock::CreateCRC" -- NAME_FIXES).
// Identity (proven, two sources): (1) Ego_r.pdb / FableWin.pdb name sets hold `dynamic initializer for
//   'CTCFireheartMinigame::BlockNames'` (donor 0x01011AE0) and its `dynamic atexit destructor` (0x0101CA90); (2) the 16 literals
//   "BLOCK<row><column>" for rows 0..3 / columns 0..3 are the marker script names the landed CTCFireheartMinigame::PlayGame 0x007E78E0
//   indexes as [row][column] at 0x013BAEB0 (the Ego_r CTCFireheartMinigame layout keeps the 4x4 shape: CThing* Markers[4][4],
//   bool BlocksActive[4][4]); bound there as the descriptive `g_FireheartMarkerNames` -- NAME_FIXES. Catalog row
//   "_gapscan::sub_0121f2d0" carried no name. Not a toolchain-library object (a compiler-generated initializer of a Fable static).
// Shape: an aggregate initializer of a 4x4 CCharString array with distinct literals: one inline `push -1 ; push offset lit ;
//   mov ecx,&element ; call CCharString(const char*, long)` per element in row-major order (the implicit conversion, length -1),
//   then `push offset $E2 ; call _atexit ; pop ecx`.
// Callees: CCharString(const char*, long) 0x0099EBF0 (??0CCharString@@QAE@PBDJ@Z), _atexit 0x004012BC.
// Data: the 16 pooled literals 0x01270AB4 ("BLOCK00") .. 0x01270A3C ("BLOCK33"); the static member array at 0x013BAEB0 (data
//   symbol ?BlockNames@CTCFireheartMinigame@@2PAY03VCCharString@@A: public static assumed, no PDB data-symbol dump).
class CCharStringData;
class CCharString { public:
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    CCharStringData* PStringData;
};
class CTCFireheartMinigame { public:
    static CCharString BlockNames[4][4];                 // 0x013BAEB0
};
CCharString CTCFireheartMinigame::BlockNames[4][4] = {
    { "BLOCK00", "BLOCK01", "BLOCK02", "BLOCK03" },
    { "BLOCK10", "BLOCK11", "BLOCK12", "BLOCK13" },
    { "BLOCK20", "BLOCK21", "BLOCK22", "BLOCK23" },
    { "BLOCK30", "BLOCK31", "BLOCK32", "BLOCK33" }
};