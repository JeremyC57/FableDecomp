// AreAllowedToCoexist(EGameAction action1, EGameAction action2) -- whether two game actions may share one input binding: special attack / run / first-person targeting with each other, attract experience orbs / spell mode with each other
// Leaf: ?AreAllowedToCoexist@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005578A0, 147 bytes: 56 bytes of code + the jump tables (dwords 0x005578D8, byte index 0x005578E4,
//   0x4f entries), plain `ret` (Ego_r.pdb `?AreAllowedToCoexist@@YI_NW4EGameAction@@0@Z`: a free __fastcall, the
//   actions in ecx / edx).
// Callers: NUISystem::CKeyRedefiner::ClearDuplicateDefinitions 0x005580B0 (landed; call 0x005580C8).
// Enum: EGameAction values from Ego_r.pdb (ghidra_out/controller_enums_pdb.txt): GAME_ACTION_SPECIAL_ATTACK 8,
//   GAME_ACTION_ATTRACT_EXPERIENCE_ORBS 26, GAME_ACTION_RUN 31, GAME_ACTION_TOGGLE_FIRST_PERSON_TARGETING 45,
//   GAME_ACTION_ACTIVATE_SPELL_MODE 86.
// Jump table (checked by hand against retail, since relocation masking cannot pair cases with arms): the byte index is
//   action1 - 8; retail bytes 8->0, 26->1, 31->0, 45->0, 86->1, every other entry 2; dword table { 0x005578B6 (arm
//   testing action2 == 8 / 0x1f / 0x2d), 0x005578C8 (arm testing 0x1a / 0x56), 0x005578D5 (false) }. The built object
//   has the same byte index (8->0, 26->1, 31->0, 45->0, 86->1, rest 2) and dword table { $L283 = +0x16 (the 8/0x1f/0x2d
//   arm), $L286 = +0x28 (the 0x1a/0x56 arm), $L280 = +0x35 (false) }: every case lands on the matching arm.
// Shape: one switch with two grouped case lists, each arm an `||` chain returning true, `break` to the shared false.
enum EGameAction {                                      // Ego_r.pdb EGameAction (ghidra_out/controller_enums_pdb.txt), the values used here
    GAME_ACTION_SPECIAL_ATTACK = 8,
    GAME_ACTION_ATTRACT_EXPERIENCE_ORBS = 26,
    GAME_ACTION_RUN = 31,
    GAME_ACTION_TOGGLE_FIRST_PERSON_TARGETING = 45,
    GAME_ACTION_ACTIVATE_SPELL_MODE = 86,
    GAME_ACTION_COUNT = 115
};

bool __fastcall AreAllowedToCoexist(EGameAction action1, EGameAction action2)
{
    switch (action1) {
    case GAME_ACTION_SPECIAL_ATTACK:
    case GAME_ACTION_RUN:
    case GAME_ACTION_TOGGLE_FIRST_PERSON_TARGETING:
        if (action2 == GAME_ACTION_SPECIAL_ATTACK || action2 == GAME_ACTION_RUN ||
            action2 == GAME_ACTION_TOGGLE_FIRST_PERSON_TARGETING)
            return true;
        break;
    case GAME_ACTION_ATTRACT_EXPERIENCE_ORBS:
    case GAME_ACTION_ACTIVATE_SPELL_MODE:
        if (action2 == GAME_ACTION_ATTRACT_EXPERIENCE_ORBS || action2 == GAME_ACTION_ACTIVATE_SPELL_MODE)
            return true;
        break;
    }
    return false;
}