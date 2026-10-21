// CPlayerManager::InitialisePlayerCharacterAs(long, const CCharString&) -- makes the given player's character the named creature type
// Leaf: InitialisePlayerCharacterAs@CPlayerManager
// Retail: Fable.exe @ 0x00449B20, 25 bytes, `ret 8`. Catalog label NUISystem::CSpellContainerList::AddChild is wrong: the body is
// GetPlayer(number) (0x004498C0, ?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z, ret 4) then CPlayer::InitCharacterAs(name)
// (0x0048A070, ?InitCharacterAs@CPlayer@@QAEXABVCCharString@@@Z, ret 4); the name argument is pushed before the GetPlayer call.
// Only caller CTCCoopSpirit::SwapToHero 0x0066FF89 (on the thing's player manager). Ego_r.pdb
// ?InitialisePlayerCharacterAs@CPlayerManager@@QAEXJABVCCharString@@@Z is the matching member.
class CCharString;
class CPlayer {
public:
    void InitCharacterAs(const CCharString& name);       // 0x0048A070
};
class CPlayerManager {
public:
    CPlayer& GetPlayer(long number);                     // 0x004498C0
    void InitialisePlayerCharacterAs(long number, const CCharString& name);
};
void CPlayerManager::InitialisePlayerCharacterAs(long number, const CCharString& name)
{
    GetPlayer(number).InitCharacterAs(name);
}