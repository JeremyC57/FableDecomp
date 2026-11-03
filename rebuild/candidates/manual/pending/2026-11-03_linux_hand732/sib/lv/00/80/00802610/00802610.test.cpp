// Behaviour test for 0x00802610 CTCTargetingPlayer::GetPTargetedThing() const: first-person targeting returns the
// (virtual) projectile-targeted thing, otherwise the base class's PTargetedThing item.
#include <cstdio>

class CThing;
class CPlayer {
public:
    bool FirstPerson;
    bool IsFirstPersonTargeting() const { return FirstPerson; }
};
class CThing {
public:
    CPlayer Player;
    const CPlayer& PeekPlayer() const { return Player; }
};

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer() : Item(0) { }
    virtual ~CBaseIntelligentPointer() { }
    void* GetPItem() const { return Item; }
    void SetPItem(void* item) { Item = item; }
private:
    void* Item;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CTCBase {
public:
    virtual ~CTCBase() { }
    CThing* ParentThing;
};

class CTCTargetingBase : public CTCBase {
public:
    virtual CThing* GetPTargetedThing() const { return PTargetedThing.GetPItem(); }
    virtual CThing* GetPProjectileTargetedThing() const { return 0; }
    CIntelligentPointer<CThing> PTargetedThing;
};

class CTCTargetingPlayer : public CTCTargetingBase {
public:
    virtual CThing* GetPTargetedThing() const;
    virtual CThing* GetPProjectileTargetedThing() const { return PProjectileTargetedThing.GetPItem(); }
    CIntelligentPointer<CThing> PProjectileTargetedThing;
};

CThing* CTCTargetingPlayer::GetPTargetedThing() const
{
    if (ParentThing->PeekPlayer().IsFirstPersonTargeting())
        return GetPProjectileTargetedThing();
    return CTCTargetingBase::GetPTargetedThing();
}

int main()
{
    CThing hero, targeted, projectile;
    CTCTargetingPlayer t;
    t.ParentThing = &hero;
    t.PTargetedThing.SetPItem(&targeted);
    t.PProjectileTargetedThing.SetPItem(&projectile);
    hero.Player.FirstPerson = false;
    const CTCTargetingBase& b = t;
    if (b.GetPTargetedThing() != &targeted) { std::printf("00802610_TEST FAIL\n"); return 1; }
    hero.Player.FirstPerson = true;
    if (b.GetPTargetedThing() != &projectile) { std::printf("00802610_TEST FAIL\n"); return 1; }
    t.PProjectileTargetedThing.SetPItem(0);
    if (b.GetPTargetedThing() != 0) { std::printf("00802610_TEST FAIL\n"); return 1; }
    std::printf("00802610_TEST PASS\n");
    return 0;
}