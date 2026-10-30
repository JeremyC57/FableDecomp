class CCharString;

class CThingPlayerCreature
{
public:
    bool IsExpressionAvailable(const CCharString& expression) const;   // Ego_r.pdb: ?IsExpressionAvailable@CThingPlayerCreature@@QBE_NABVCCharString@@@Z
};

class CPlayer
{
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};

class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
class HeroResolverStage1
{
public:

};

class CGameScriptInterface
{
public:
    bool HeroHasExpression(const CCharString& expression) const;
};

struct InterfaceOverlay
{
    unsigned char pad00[0x14];
    HeroResolverStage1* field14;
};

struct HeroOverlay
{
    unsigned char pad00[0x91];
    unsigned char flags91;
};

bool CGameScriptInterface::HeroHasExpression(
    const CCharString& expression) const
{
    const InterfaceOverlay* self = (const InterfaceOverlay*)this;
    CThingPlayerCreature* hero = ((CPlayerManager*)self->field14)->GetMainPlayer().GetPControlledCreature();
    if (hero != 0 && (((const HeroOverlay*)hero)->flags91 & 1) == 0)
    {
        return hero->IsExpressionAvailable(expression);
    }
    return false;
}