// COpinionReactionManager::$E_AttitudeStrings() -- dynamic initializer of the static CCharString COpinionReactionManager::AttitudeStrings[18] (the attitude names "NONE" .. "HATE", one CCharString(const char*, -1) per element), then registers the array's atexit destructor
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x0121E480, 318 bytes, in `.text$yc`, reached from the .CRT$XCU table entry 0x01374488, plain `ret`.
//   Registers the atexit thunk 0x012295F0 (the compiler's reverse-order ~CCharString loop over the 18 elements at 0x013BABD0;
//   landed under the wrong label "CFontBankStateBlock::CreateCRC" -- NAME_FIXES).
// Identity (proven, two sources): (1) Ego_r.pdb / FableWin.pdb name sets hold `dynamic initializer for
//   'COpinionReactionManager::AttitudeStrings'` (donor 0x0100CEA0) and no other 18-string attitude table; (2) the 18 literals are the
//   attitude names NONE, RESPECT, AWE, DISDAIN, FEAR, FRIENDLY, WORSHIP, RIDICULOUS, OFFENSIVE, AGREEABLE, UGLY, ATTRACTED, LOVE,
//   LOVE, LIKE, NEUTRAL, DISLIKE, HATE and the array 0x013BABD0 is the one COpinionReactionManager::GetThingAttitudeToPlayerAsString
//   0x007018B0 indexes by the attitude type (landed; bound there as the descriptive `g_OpinionAttitudeNames` -- NAME_FIXES).
//   Catalog row "_gapscan::sub_0121e480" carried no name. Not a toolchain-library object (a compiler-generated initializer of a
//   Fable static; no CRT / vendor .lib holds it).
// Shape: an aggregate initializer of a CCharString array with distinct string literals: VC7.1 emits one inline
//   `push -1 ; push offset lit ; mov ecx,&element ; call CCharString(const char*, long)` per element (the long defaults to -1,
//   the implicit conversion), element 0 first, then `push offset $E2 ; call _atexit ; pop ecx`.
// Callees: CCharString(const char*, long) 0x0099EBF0 (??0CCharString@@QAE@PBDJ@Z), _atexit 0x004012BC.
// Data: the 18 pooled literals 0x0125496C ("NONE"), 0x01264C38 .. 0x01264BB0; the static member array at 0x013BABD0 (data
//   symbol ?AttitudeStrings@COpinionReactionManager@@2PAVCCharString@@A: public static assumed, no PDB data-symbol dump).
class CCharStringData;
class CCharString { public:
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    CCharStringData* PStringData;
};
class COpinionReactionManager { public:
    static CCharString AttitudeStrings[18];              // 0x013BABD0
};
CCharString COpinionReactionManager::AttitudeStrings[18] = {
    "NONE", "RESPECT", "AWE", "DISDAIN", "FEAR", "FRIENDLY", "WORSHIP", "RIDICULOUS", "OFFENSIVE",
    "AGREEABLE", "UGLY", "ATTRACTED", "LOVE", "LOVE", "LIKE", "NEUTRAL", "DISLIKE", "HATE"
};