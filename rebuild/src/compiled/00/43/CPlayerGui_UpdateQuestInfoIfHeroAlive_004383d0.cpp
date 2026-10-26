// CPlayerGui::UpdateQuestInfoIfHeroAlive() -- update the quest-info display while the main player's character exists and is not dead
// Leaf: ?UpdateQuestInfoIfHeroAlive@CPlayerGui@@QAEXXZ
// Retail: Fable.exe @ 0x004383D0, 46 bytes, unreferenced. Catalog label CGameScriptInterface::GiveHeroExpression is
// wrong: `this` is the CPlayerGui (PlayerManager +0x08 -> CPlayerManager::GetMainPlayer 0x00449970, PDrawQuestInfo
// +0x1c8 as modelled by the landed CPlayerGui sources), it sits among the CPlayerGui methods
// (0x004382E0 SetButtonAlpha, 0x00438400 Close), and the tail-called 0x0064A32E is the NPlayerGui::CDrawQuestInfo method
// also called from 0x0043A151 (catalog: CDrawQuestInfo::Update). Body: the player's character through the inlined
// GetPlayerCharacter (CIntelligentPointer at CPlayer +0x34, CBaseIntelligentPointer::GetPItem 0x00A01B50); when it is
// non-null and its Dead bit (+0x91 bit 0) is clear, tail-call the quest-info update. The name is descriptive (no
// matching Ego_r.pdb / FableWin.pdb CPlayerGui symbol was found).
// Layout: retail CPlayerGui (engine/CPlayerGui.h carries the Ego_r.pdb offsets, PDrawQuestInfo +0x1d0 there, 8 higher).
// Shape: /O2.
class CBaseObject;
class CBaseIntelligentPointer {
public:
    void* PInfo;
    CBaseObject* GetPItem() const;                       // 0x00A01B50
};
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                              // +0x91 bit 0
};
class CPlayer {
public:
    unsigned char           _pad00[0x34];
    CBaseIntelligentPointer PlayerCharacter;             // +0x34 CIntelligentPointer<CThing>
    CThing* GetPlayerCharacter() { return (CThing*)PlayerCharacter.GetPItem(); }
};
class CPlayerManager {
public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
namespace NPlayerGui {
class CDrawQuestInfo {
public:
    void Update();                                       // 0x0064A32E
};
}
class CPlayerGui {
public:
    unsigned char               _pad000[8];
    CPlayerManager*             PlayerManager;           // +0x08
    unsigned char               _pad00c[0x1c8 - 0xc];
    NPlayerGui::CDrawQuestInfo* PDrawQuestInfo;          // +0x1c8 CCountedPointer data
    void UpdateQuestInfoIfHeroAlive();
};
void CPlayerGui::UpdateQuestInfoIfHeroAlive()
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPlayerCharacter();
    if (hero && !hero->Dead)
        PDrawQuestInfo->Update();
}