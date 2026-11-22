// Standalone C++03 behavior test for the selected FindObstacles body.
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <vector>
#define CHECK(expression) do { if (!(expression)) { fprintf(stderr, "line %d: %s\n", __LINE__, #expression); exit(1); } } while (0)

struct C2DVector {
    C2DVector(float x = 0, float y = 0) : X(x), Y(y) {}
    float X, Y;
};

struct C3DVector {
    C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
    void operator-=(const C3DVector& other) { X -= other.X; Y -= other.Y; Z -= other.Z; }
    const float Dot(const C3DVector& other) const { return X * other.X + Y * other.Y + Z * other.Z; }
    float X, Y, Z;
};

struct CRightHandedSet {
    CRightHandedSet() : Up(0, 2, 0), Forward(4, 0, 3) {}
    const C3DVector& GetForward() const { return Forward; }
    C3DVector GetRight() const { return C3DVector(Forward.Y * Up.Z - Forward.Z * Up.Y,
        Forward.Z * Up.X - Forward.X * Up.Z, Forward.X * Up.Y - Forward.Y * Up.X); }
    C3DVector Up, Forward;
};

struct CCoinGameObstacleDef {
    explicit CCoinGameObstacleDef(float radius) : Radius(radius), References(0) {}
    float Radius;
    mutable unsigned int References;
};

enum ENullDef { NULL_DEF };
template<class T> class CDefPointer {
public:
    explicit CDefPointer(ENullDef) : Pointer(0) {}
    ~CDefPointer() { if (Pointer) --Pointer->References; }
    T* operator->() const { return Pointer; }
    void Set(T* value) { CHECK(Pointer == 0); Pointer = value; ++Pointer->References; }
private:
    CDefPointer(const CDefPointer&);
    CDefPointer& operator=(const CDefPointer&);
    T* Pointer;
};

class CTCThingOwner;
class CThingBaseDef {
public:
    explicit CThingBaseDef(const CCoinGameObstacleDef* def) : Def(def), Queries(0) {}
    bool GetSubDef(CDefPointer<const CCoinGameObstacleDef>* result) const {
        ++Queries;
        if (Def) { result->Set(Def); return true; }
        return false;
    }
    const CCoinGameObstacleDef* Def;
    mutable unsigned int Queries;
};

class CThing {
public:
    CThing(const CCoinGameObstacleDef* def = 0, const C3DVector& pos = C3DVector(), bool dead = false)
        : Owner(0), Definition(def), Position(pos), Dead(dead), PositionReads(0) {}
    bool GetTC(CTCThingOwner** result) { if (Owner) { *result = Owner; return true; } return false; }
    const CThingBaseDef& PeekBaseDef() const { return Definition; }
    const C3DVector& GetPos() const { ++PositionReads; return Position; }
    bool IsDead() const { return Dead; }
    CTCThingOwner* Owner;
    CThingBaseDef Definition;
    C3DVector Position;
    bool Dead;
    mutable unsigned int PositionReads;
};

template<class T> class CIntelligentPointer {
public:
    explicit CIntelligentPointer(T* value) : Value(value) {}
    T* operator->() const { CHECK(Value != 0); return Value; }
    bool operator!=(int zero) const { CHECK(zero == 0); return Value != 0; }
    T* Get() const { return Value; }
private:
    T* Value;
};
template<class T> bool IS_THING_ALIVE(const T& thing) { return thing != 0 && !thing->IsDead(); }

struct Coin {
    Coin(float radius, float density, CThing* thing, bool owned)
        : Thing(thing), Owner(-1), Radius(radius), Density(density), Owned(owned) {}
    C2DVector Pos;
    CIntelligentPointer<CThing> Thing;
    int Owner;
    float Radius, Density;
    bool Owned;
};

class CTCThingOwner {
public:
    std::list<CIntelligentPointer<CThing> >& GetObjectList() { return Children; }
private:
    std::list<CIntelligentPointer<CThing> > Children;
};

class CTCTavernGameCoinBase {
public:
    CTCTavernGameCoinBase() : BoardPos(3, 4, 5) {}
    void FindObstacles();
    CThing& GetParentThing() { return Parent; }
    CThing Parent;
    std::vector<Coin> Coins;
    C3DVector BoardPos;
    CRightHandedSet BoardRHSet;
};

void CTCTavernGameCoinBase::FindObstacles()
{
    CTCThingOwner* tc_owner;
    if (GetParentThing().GetTC(&tc_owner)) {
        const std::list<CIntelligentPointer<CThing> >& children = tc_owner->GetObjectList();
        for (std::list<CIntelligentPointer<CThing> >::const_iterator i = children.begin(); i != children.end(); ++i) {
            CDefPointer<const CCoinGameObstacleDef> obstacle(NULL_DEF);
            if (IS_THING_ALIVE(*i) && (*i)->PeekBaseDef().GetSubDef(&obstacle)) {
                C3DVector pos((*i)->GetPos());
                pos -= BoardPos;
                float y = pos.Dot(BoardRHSet.GetForward());
                float x = pos.Dot(BoardRHSet.GetRight());
                C2DVector local_pos(x, y);
                Coin c(obstacle->Radius, 0.0f, 0, false);
                c.Pos = local_pos;
                c.Thing = CIntelligentPointer<CThing>((*i).Get());
                c.Owner = 0;
                Coins.push_back(c);
            }
        }
    }
}

static void CheckCoin(const Coin& coin, CThing* thing, float radius, float x, float y)
{
    CHECK(coin.Radius == radius && coin.Density == 0.0f && !coin.Owned && coin.Owner == 0);
    CHECK(coin.Thing.Get() == thing && coin.Pos.X == x && coin.Pos.Y == y);
}

static void CheckObstacles(bool reserve_space)
{
    CTCTavernGameCoinBase game;
    if (reserve_space)
        game.Coins.reserve(8);
    game.FindObstacles();
    CHECK(game.Coins.empty());
    CTCThingOwner owner;
    game.Parent.Owner = &owner;
    game.FindObstacles();
    CHECK(game.Coins.empty());
    CCoinGameObstacleDef first_def(1.5f), second_def(2.5f);
    CThing dead(&first_def, C3DVector(), true), no_def;
    CThing first(&first_def, C3DVector(5, 9, 8)), second(&second_def, C3DVector(1, 4, 1));
    owner.GetObjectList().push_back(CIntelligentPointer<CThing>(0));
    owner.GetObjectList().push_back(CIntelligentPointer<CThing>(&dead));
    owner.GetObjectList().push_back(CIntelligentPointer<CThing>(&no_def));
    game.FindObstacles();
    CHECK(game.Coins.empty());
    CHECK(dead.Definition.Queries == 0 && no_def.Definition.Queries == 1);
    CHECK(dead.PositionReads == 0 && no_def.PositionReads == 0);
    owner.GetObjectList().push_back(CIntelligentPointer<CThing>(&first));
    owner.GetObjectList().push_back(CIntelligentPointer<CThing>(&second));
    game.FindObstacles();
    CHECK(game.Coins.size() == 2);
    CheckCoin(game.Coins[0], &first, 1.5f, 12.0f, 17.0f);
    CheckCoin(game.Coins[1], &second, 2.5f, -20.0f, -20.0f);
    CHECK(first.Definition.Queries == 1 && second.Definition.Queries == 1);
    CHECK(first.PositionReads == 1 && second.PositionReads == 1);
    CHECK(!first_def.References && !second_def.References);
    game.FindObstacles();
    CHECK(game.Coins.size() == 4);
    CheckCoin(game.Coins[2], &first, 1.5f, 12.0f, 17.0f);
    CheckCoin(game.Coins[3], &second, 2.5f, -20.0f, -20.0f);
    CHECK(!first_def.References && !second_def.References);
}

int main()
{
    CheckObstacles(false);
    CheckObstacles(true);
    puts("008efb00_TEST PASS");
    return 0;
}