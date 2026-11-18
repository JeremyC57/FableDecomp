#include <math.h>
#include <stdio.h>
#include <string>

struct C3DVector {
    float X,Y,Z;
    C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X+o.X,Y+o.Y,Z+o.Z); }
    C3DVector operator*(float k) const { return C3DVector(X*k,Y*k,Z*k); }
};
struct CRightHandedSet { C3DVector Forward; const C3DVector& GetForward() const { return Forward; } };
typedef std::string CCharString;
struct CThing;
struct CTCOwnedEntity {
    CThing* Owner;
    CTCOwnedEntity():Owner(0) {}
    void SetPOwner(CThing* owner) { Owner=owner; }
};
struct CThingPhysics {
    C3DVector Position;
    float Radius;
    unsigned Moves;
    CThingPhysics():Radius(1),Moves(0) {}
    void SetPosition(const C3DVector& position) { Position=position; ++Moves; }
};
struct CThing {
    CTCOwnedEntity Owned;
    bool HasOwned;
    unsigned Added;
    signed char Player;
    CThingPhysics Physical;
    CThingPhysics* Physics;
    CThing():HasOwned(false),Added(0),Player(0),Physics(&Physical) {}
    unsigned long GetPlayerNumber() const { return Player; }
    CThingPhysics* GetPhysicsTC() { return Physics; }
    const C3DVector& GetPos() const { return Physics->Position; }
    bool GetTC(long type, CTCOwnedEntity** out)
    {
        if(!HasOwned || type!=0x33) return false;
        *out=&Owned;
        return true;
    }
    CTCOwnedEntity* AddTC(const CCharString& name,bool active,void* params)
    {
        if(name=="CTCOwnedEntity" && !active && !params) ++Added;
        return &Owned;
    }
};
struct CGameDefinitionManager {
    static CGameDefinitionManager& Get() { static CGameDefinitionManager instance; return instance; }
    long GetDefGlobalIndexFromName(const CCharString& name) { return name=="Sign" ? 19 : -1; }
};
enum {TCI_OWNED_ENTITY=0x33};
struct CThingObject : CThing {
    static CThingObject Sign;
    static bool BadCreate;
    static CThingObject* Create(long id,const C3DVector& pos,long player,const CRightHandedSet* orientation,
        unsigned long uid,const CCharString& script)
    {
        if(id!=19 || player!=4 || !orientation || uid || !script.empty()) BadCreate=true;
        Sign.Physics->Position=pos;
        return &Sign;
    }
};
CThingObject CThingObject::Sign;
typedef CThing CThingPlayerCreature;
bool CThingObject::BadCreate=false;
struct CPlayer {
    CThing* Hero;
    CPlayer():Hero(0) {}
    CThing* GetPControlledCreature() const { return Hero; }
};
struct CPlayerManager {
    CPlayer Main;
    CPlayer& GetMainPlayer() { return Main; }
};
struct CTCBuyableHouse {
    CThing* Thing;
    CPlayerManager Players;
    bool HavePosition;
    explicit CTCBuyableHouse(CThing& thing):Thing(&thing),HavePosition(false) {}
    CThing& GetParentThing() { return *Thing; }
    CPlayerManager& GetPlayerManager() { return Players; }
    bool GetSignPostPos(bool dressing,C3DVector& pos,CRightHandedSet* orientation)
    {
        if(!HavePosition || !dressing) return false;
        pos=C3DVector(4,6,2);
        orientation->Forward=C3DVector(0,1,0);
        return true;
    }
    CThing* CreateSignPost(const CCharString&,bool);
};

CThing* CTCBuyableHouse::CreateSignPost(const CCharString& sign_post_def, bool main)
{
    C3DVector post_pos;
    CRightHandedSet rhs;
    CThingObject* new_post = 0;
    if (GetSignPostPos(main, post_pos, &rhs)) {
        long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(sign_post_def);
        new_post = CThingObject::Create(def_index, post_pos, GetParentThing().GetPlayerNumber(), &rhs, 0, "");
        CTCOwnedEntity* ptc_owned_entity;
        if (!new_post->GetTC(TCI_OWNED_ENTITY, &ptc_owned_entity))
            ptc_owned_entity = (CTCOwnedEntity*)new_post->AddTC("CTCOwnedEntity", false, 0);
        ptc_owned_entity->SetPOwner(&GetParentThing());
        CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (player) {
            float total_rad = new_post->GetPhysicsTC()->Radius + player->GetPhysicsTC()->Radius;
            const C3DVector& heroPos = player->GetPos();
            const C3DVector& postPos = new_post->GetPos();
            float dx = postPos.X - heroPos.X;
            float dy = postPos.Y - heroPos.Y;
            if ((float)sqrt(dy * dy + dx * dx) < total_rad)
                player->GetPhysicsTC()->SetPosition(post_pos + rhs.GetForward() * total_rad);
        }
    }
    return new_post;
}

int main()
{
    CThing host,hero;
    host.Player=4;
    CTCBuyableHouse tc(host);
    if(tc.CreateSignPost("Sign",true) || CThingObject::Sign.Added)
        return 1;
    tc.HavePosition=true;
    if(tc.CreateSignPost("Sign",false) || CThingObject::Sign.Added)
        return 2;
    if(tc.CreateSignPost("Sign",true)!=&CThingObject::Sign || CThingObject::Sign.Owned.Owner!=&host ||
        CThingObject::Sign.Added!=1 || CThingObject::BadCreate)
        return 3;
    CThingObject::Sign.HasOwned=true;
    tc.Players.Main.Hero=&hero;
    hero.Physical.Position=C3DVector(4,7,20);
    tc.CreateSignPost("Sign",true);
    if(hero.Physical.Moves!=1 || hero.Physical.Position.X!=4 || hero.Physical.Position.Y!=8 ||
        hero.Physical.Position.Z!=2 || CThingObject::Sign.Added!=1)
        return 4;
    tc.CreateSignPost("Sign",true);
    if(hero.Physical.Moves!=1)
        return 5;
    puts("006c0970_TEST PASS");
    return 0;
}
