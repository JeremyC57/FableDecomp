#ifndef FABLE_ENGINE_EMINIGAMETYPE_H
#define FABLE_ENGINE_EMINIGAMETYPE_H
// Original enum (p25B, row 0x00EEBDF0; new header): Ego_d.pdb LF_ENUM 0xCEC4 `EMiniGameType` (int; field list 0xCEC3) and the
// installed game's misc_def_types.h (ghidra_out/installed_game/header_enums.csv) agree on every enumerator. Taken by
// CGameScriptInterfaceBase slot 342 DisplayMiniGameInfo(bool, EMiniGameType).
enum EMiniGameType {
    MINIGAME_NONE=0,
    MINIGAME_FISHING=1,
    MINIGAME_DIGGING=2,
    MINIGAME_PICKPOCKET=3,
    MINIGAME_PICKLOCK=4,
    MINIGAME_STEAL=5,
    MINIGAME_TROPHY=6,
    MINIGAME_ORACLE=7
};
#endif
