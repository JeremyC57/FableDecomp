// CTCHeroCentre::OnHeroWalkedThroughDoor(EHeroCentreDoorType doorType) -- the guild's main door (type 1) opens the training screen
//   when experience spending is enabled
// Leaf: ?OnHeroWalkedThroughDoor@CTCHeroCentre@@QAEXW4EHeroCentreDoorType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077DB90, 36 bytes, `ret 4`:
//   mov eax,[esp+4] ; dec eax ; push esi ; mov esi,ecx ; jne R ; call 0x00686A80 ; mov cl,[eax+0D6h] ; test cl,cl ; je R ;
//   mov ecx,esi ; call 0x0077D4B0 ; R: pop esi ; ret 4
// Identity (naming wave 25): Ego_r.pdb ?OnHeroWalkedThroughDoor@CTCHeroCentre@@QAEXW4EHeroCentreDoorType@@@Z; the callee 0x0077D4B0
//   is ?OpenTrainingScreen@CTCHeroCentre@@QAEXXZ (the catalog name; Ego_r order OnHeroCloseDoor / OpenTrainingScreen (0x100 bytes) /
//   RemoveQuestCard / OnHeroWalkedThroughDoor (0x30) = retail 0x0077D4B0 (254 bytes) .. 0x0077DB90 (36)); the world accessor is
//   ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ 0x00686A80 (DECISIONS_W24_RESOLVED item 7) and +0xD6 is CWorld::ExperienceSpendingEnabled
//   (retail offset, as the landed CWorld ctor 0x004A67D0 and 0x0077F7E0 model it). Was CTCHeroCentre_Methods with a free
//   GFGetHeroCreature() and OnHeroEnteredGuildDoor (descriptive).
// Lane feedback: retail `mov eax,[esp+4] ; dec eax ; jne` is a one-case switch, not an `==` test. RELOCATION_MATCH.
enum EHeroCentreDoorType { };
class CWorld { public:
    unsigned char _pad00[0xd6];
    bool          ExperienceSpendingEnabled;            // +0xd6
};
class CTCBase {
protected:
    const CWorld& PeekWorld() const;                    // 0x00686A80
};
class CTCHeroCentre : public CTCBase { public:
    void OpenTrainingScreen();                          // 0x0077D4B0
    void OnHeroWalkedThroughDoor(EHeroCentreDoorType doorType);
};

void CTCHeroCentre::OnHeroWalkedThroughDoor(EHeroCentreDoorType doorType)
{
    // `dec eax ; jne` is the compiler's single-case switch (case 1); `if (doorType == 1)`
    // emits `cmp dword ptr [esp+4],1` instead.
    switch (doorType) {
    case 1:
        if (PeekWorld().ExperienceSpendingEnabled)
            OpenTrainingScreen();
        break;
    default:
        break;
    }
}
