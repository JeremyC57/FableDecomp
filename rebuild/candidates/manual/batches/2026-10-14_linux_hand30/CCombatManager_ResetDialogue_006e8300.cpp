// CCombatManager::ResetDialogue() -- re-initialise the combat dialogue manager, reset the attack-pattern idle interval to half a second and clear the nearest-combatant-only flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E8300, length 24 bytes.
// Callees: CombatDialogueManager::Init() @ 0x008AF2B0 (on the embedded manager at +0x38). The chunk label "CDrawQuickAccessIcons::OnHeroHit" is a
// propagated shape name (the real OnHeroHit is @ 0x0064DA5C, landed); this body sits in the 0x006Exxxx combat/AI module and its layout is
// CCombatManager's (PDB, 104 bytes: AttackPatternIdleStandardInterval +0x38, AttacksFromNearestCombatantOnly +0x3c, DialogueManager +0x40 -- retail
// 8 lower because the two map members at +4 and +0x14 are 12-byte STLport trees in the shipped build, not 16). Shape: the float literal is an
// immediate dword store, the flag a byte store, after the Init call on the member's address.
struct CombatDialogueManager {
    unsigned char _body[0x28];                     // SpeechGroupInfoMap, PDialogueDef, PCurrentTalker, CurrentTalkerTextGroupIndex, LastSpeechFrame
    void Init();                                   // 0x008AF2B0
};

struct CCombatManager {
    unsigned char         _pad[0x30];              // vptr, RegisteredCombatSequences, Combatants, flags, World, debug flags, ProximityForNavigation, EnableAINonAttackMode
    float                 AttackPatternIdleStandardInterval;   // +0x30
    bool                  AttacksFromNearestCombatantOnly;     // +0x34
    unsigned char         _pad35[3];
    CombatDialogueManager DialogueManager;         // +0x38
    void ResetDialogue();
};

void CCombatManager::ResetDialogue()
{
    DialogueManager.Init();
    AttackPatternIdleStandardInterval = 0.5f;
    AttacksFromNearestCombatantOnly = false;
}
