// CTCWife::$E_WifeAppearanceDialogueGroupStemNames() -- dynamic initializer of the static CCharString CTCWife::WifeAppearanceDialogueGroupStemNames[16] (the "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_<category>_" dialogue-group stems), then its array atexit destructor is registered
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x0121E260, 284 bytes, in `.text$yc`, reached from the .CRT$XCU table entry 0x0137445C, plain `ret`.
//   Sixteen CCharString(const char*, long = -1) constructions into 0x013BAB40 .. 0x013BAB7C (stride 4: the retail CCharString is one
//   pointer), then `push 0x01229480 ; call _atexit ; pop ecx` (the compiler-generated array destructor: 16 ~CCharString calls from
//   0x013BAB80 down, the `$E2` thunk; not part of this row).
// Identity (proven, two sources): (1) the .CRT$XCU neighbours are the CTCWife TU's initializers in Ego_r.pdb's order --
//   0x0121E1F0 "AM3" (CTCWife::MarriageEnabledMaleVoice, 0x013BAB2C), 0x0121E210 "TEXT_AI_COMMENT_COURTSHIP_SUGGEST_..." x4
//   (CTCWife::CourtingAdviceGroupNames, 0x013BAB30 .. 0x013BAB3C, atexit 0x012294A0), then this one; Ego_r.pdb lists exactly
//   `dynamic initializer for 'CTCWife::MarriageEnabledMaleVoice'`, `...'CTCWife::CourtingAdviceGroupNames'` and
//   `...'CTCWife::WifeAppearanceDialogueGroupStemNames'` for CTCWife; (2) the sixteen literals are the per-category stems of the
//   wife's appearance dialogue groups (EWifeAppearanceCategories, the argument of CTCWife::GetAppearanceChangedNoticed /
//   ClearAppearanceChangedNoticed) and the landed CAIStateGroup_OpinionReactionRoot::WifeClothingUpdate 0x00985F40 indexes the
//   table 0x013BAB40 by that category and appends the attitude suffix. Catalog row "_gapscan::sub_0121e260" carried no name; the
//   landed 0x00985F40 bound 0x013BAB40 as an invented `?g_WifeAppearanceLinePrefix@@3PAVCCharString@@A` (NAME_FIXES.tsv).
// Not a toolchain-library object: a compiler-generated initializer of a Fable static (no CRT/vendor .lib contains it).
// Shape: a braced array initializer of a class with a converting constructor: VC7.1 constructs each element in place with the
//   literal through the implicit CCharString(const char*, long = -1) (`push -1 ; push <literal> ; mov ecx,<element> ; call`), then
//   registers the array's atexit destructor. Callees: CCharString(const char*, long) 0x0099EBF0 (??0CCharString@@QAE@PBDJ@Z),
//   ~CCharString 0x0099EAE0 (inside the `$E2` thunk), _atexit 0x004012BC.
class CCharStringData;
class CCharString { public:
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    CCharStringData* PStringData;
};

enum EWifeAppearanceCategories {                         // the index of the stem table (16 categories)
    WIFE_APPEARANCE_GENERAL, WIFE_APPEARANCE_HEAD, WIFE_APPEARANCE_BODY, WIFE_APPEARANCE_HANDS, WIFE_APPEARANCE_LEGS,
    WIFE_APPEARANCE_FEET, WIFE_APPEARANCE_HAIR, WIFE_APPEARANCE_BEARD, WIFE_APPEARANCE_MOUSTACHE, WIFE_APPEARANCE_TATTOO,
    WIFE_APPEARANCE_FATNESS, WIFE_APPEARANCE_THINNER, WIFE_APPEARANCE_MORESCARS, WIFE_APPEARANCE_BALDNESS,
    WIFE_APPEARANCE_NAKED, WIFE_APPEARANCE_BARETORSO, NUM_WIFE_APPEARANCE_CATEGORIES
};

class CTCWife { public:
    static CCharString WifeAppearanceDialogueGroupStemNames[NUM_WIFE_APPEARANCE_CATEGORIES];   // 0x013BAB40
};

CCharString CTCWife::WifeAppearanceDialogueGroupStemNames[NUM_WIFE_APPEARANCE_CATEGORIES] = {
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_GENERAL_",      // 0x0125F608
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_HEAD_",         // 0x0125F5E0
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_BODY_",         // 0x0125F5B8
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_HANDS_",        // 0x0125F58C
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_LEGS_",         // 0x0125F564
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_FEET_",         // 0x0125F53C
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_HAIR_",         // 0x0125F514
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_BEARD_",        // 0x0125F4E8
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_MOUSTACHE_",    // 0x0125F4B8
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_TATTOO_",       // 0x0125F48C
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_FATNESS_",      // 0x0125F460
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_THINNER_",      // 0x0125F434
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_MORESCARS_",    // 0x0125F404
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_BALDNESS_",     // 0x0125F3D8
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_NAKED_",        // 0x0125F3AC
    "TEXT_AI_WIFE_OPINION_ACT_CLOTHING_BARETORSO_"     // 0x0125F37C
};