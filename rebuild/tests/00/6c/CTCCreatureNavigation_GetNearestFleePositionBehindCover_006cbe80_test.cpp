#include <assert.h>
#ifdef _MSC_VER
#define FABLE_RETAIL_STLP_CONST_MEM_FUN_REF
#include "stlport/fable_algorithm.h"
#else
#include <functional>
#endif
#include <math.h>
#include <stdio.h>

namespace FleeFixture {

struct Fixture2DCoordI {
    long X, Y;
    Fixture2DCoordI(long x, long y) : X(x), Y(y) {}
};

static const float DELTA_NOTIONAL_ZERO = 0.0001f;
static unsigned long RandomCalls;
static unsigned long HeightCalls;

struct Fixture3DVector {
    float X, Y, Z;
    Fixture3DVector() : X(0), Y(0), Z(0) {}
    Fixture3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    Fixture3DVector operator-(const Fixture3DVector& other) const
    {
        return Fixture3DVector(X-other.X, Y-other.Y, Z-other.Z);
    }
    Fixture3DVector operator+(const Fixture3DVector& other) const
    {
        return Fixture3DVector(X+other.X, Y+other.Y, Z+other.Z);
    }
    Fixture2DCoordI ToC2DCoordI() const { return Fixture2DCoordI(static_cast<long>(X), static_cast<long>(Y)); }
    float GetMagnitude2D() const { return static_cast<float>(sqrt(X*X+Y*Y)); }
    float GetSquaredMagnitude() const { return X*X+Y*Y+Z*Z; }
    void InitialiseFromAngles(float heading, float inclination, float magnitude)
    {
        const double two_pi = 6.2831854820251465;
        const double flat = cos(inclination*two_pi)*magnitude;
        X = static_cast<float>(sin(heading*two_pi)*flat);
        Y = static_cast<float>(cos(heading*two_pi)*flat);
        Z = static_cast<float>(sin(inclination*two_pi)*magnitude);
    }
    void Normalise()
    {
        *this *= static_cast<float>(1.0/sqrt(GetSquaredMagnitude()));
    }
    void operator*=(float scale) { X*=scale; Y*=scale; Z*=scale; }
};

struct FixturePhysics {
    float Radius;
    float GetRadius() const { return Radius; }
};

struct FixtureThingGameObject {
    Fixture3DVector Position;
    FixturePhysics Physics;
    bool Cover;
    FixtureThingGameObject(float x, float y, float z, float radius, bool cover)
        : Position(x,y,z), Cover(cover) { Physics.Radius=radius; }
    virtual bool IsCover() const { return Cover; }
    const Fixture3DVector& PeekPos() const { return Position; }
    FixturePhysics* GetPhysicsTC() { return &Physics; }
};

struct FixtureThingCreatureBase : FixtureThingGameObject {
    mutable Fixture3DVector GroundProbe;
    FixtureThingCreatureBase() : FixtureThingGameObject(7.8f,-1.2f,4.0f,1.5f,false) {}
    float GetMapSizeZBelowAt(const Fixture3DVector& pos) const
    {
        ++HeightCalls;
        GroundProbe=pos;
        return 39.0f;
    }
};

struct FixtureMap {
    Fixture2DCoordI WorldPosition;
    FixtureMap() : WorldPosition(-5,-4) {}
    const Fixture2DCoordI& GetWorldPosition() const { return WorldPosition; }
    long GetWorldSizeX() const { return 19; }
    long GetWorldSizeY() const { return 17; }
};

struct FixtureWorldMap {
    FixtureMap Map;
    const FixtureMap& PeekMapAt(const Fixture2DCoordI& pos) const
    {
        assert(pos.X==7 && pos.Y==-1);
        return Map;
    }
};

struct FixtureThingFilter_In2DRange {
    Fixture3DVector Origin;
    float Range;
    FixtureThingFilter_In2DRange(const Fixture3DVector& origin,float range) : Origin(origin), Range(range) {}
    bool operator()(const FixtureThingGameObject& thing) const
    {
        return (thing.PeekPos()-Origin).GetMagnitude2D()<Range;
    }
};

struct FixtureThingCompare_Nearest {
    Fixture3DVector Origin;
    FixtureThingCompare_Nearest(const Fixture3DVector& origin) : Origin(origin) {}
    FixtureThingGameObject* operator()(FixtureThingGameObject* a,FixtureThingGameObject* b) const
    {
        if (!a) return b;
        return (a->PeekPos()-Origin).GetSquaredMagnitude()<(b->PeekPos()-Origin).GetSquaredMagnitude()?a:b;
    }
};

template<class Filter> struct FixtureFilterNot {
    const Filter& F;
    FixtureFilterNot(const Filter& filter) : F(filter) {}
    bool operator()(const FixtureThingGameObject& thing) const { return !F(thing); }
};

template<class Filter> static FixtureFilterNot<Filter> GFFilter_NOT(const Filter& filter)
{
    return FixtureFilterNot<Filter>(filter);
}

template<class First,class Second> struct FixtureFilterAnd {
    const First& A;
    const Second& B;
    FixtureFilterAnd(const First& first,const Second& second) : A(first),B(second) {}
    bool operator()(const FixtureThingGameObject& thing) const { return A(thing) && B(thing); }
};

template<class First,class Second> static FixtureFilterAnd<First,Second> GFFilter_Bind2(const First& first,const Second& second)
{
    return FixtureFilterAnd<First,Second>(first,second);
}

struct FixtureSearch {
    FixtureThingGameObject* Things[4];
    int Count;
    const Fixture3DVector* MoveResultTo;
    FixtureSearch() : Count(0), MoveResultTo(0) {}
    void Add(FixtureThingGameObject& object) { assert(Count<4); Things[Count++]=&object; }
    template<class Filter,class Compare> bool GetPBestThing(const Filter& filter,const Compare& compare,FixtureThingGameObject** out) const
    {
        FixtureThingGameObject* best=0;
        for (int i=0;i<Count;++i)
            if (filter(*Things[i]))
                best=compare(best,Things[i]);
        *out=best;
        if (best && MoveResultTo) best->Position=*MoveResultTo;
        return best!=0;
    }
};

template<class T,class L,class U> static void GFLimit(T& value,const L& lower,const U& upper)
{
    if (value<lower) value=static_cast<T>(lower);
    else if (value>upper) value=static_cast<T>(upper);
}

static float GFFloatRandom(float upper,unsigned long* seed)
{
    ++RandomCalls;
    assert(upper==1.0f);
    ++*seed;
    return 0.25f;
}

struct FixtureCreatureNavigation {
    FixtureThingCreatureBase& Creature;
    FixtureWorldMap Map;
    FixtureSearch Search;
    mutable unsigned long Seed;
    FixtureCreatureNavigation(FixtureThingCreatureBase& creature) : Creature(creature), Seed(0) {}
    FixtureThingCreatureBase& GetParentCreature() const { return Creature; }
    const FixtureWorldMap& PeekWorldMap() const { return Map; }
    const FixtureSearch& PeekThingSearchTools() const { return Search; }
    unsigned long* GetPWorldSeed() const { return &Seed; }
    bool GetNearestFleePositionBehindCover(const Fixture3DVector&,Fixture3DVector&) const;
};

bool FixtureCreatureNavigation::GetNearestFleePositionBehindCover(const Fixture3DVector& threat_pos, Fixture3DVector& pos) const
{
    FixtureThingCreatureBase& creature = GetParentCreature();
    const FixtureMap& map = PeekWorldMap().PeekMapAt(creature.PeekPos().ToC2DCoordI());
    const Fixture2DCoordI& map_world_pos = map.GetWorldPosition();
    FixtureThingGameObject* object = 0;
    float chicken_sacrifice = 0.5f;
    const float threat_too_close_distance = 4.0f;
    if (PeekThingSearchTools().GetPBestThing(
            GFFilter_Bind2(std::mem_fun_ref(&FixtureThingGameObject::IsCover),
                           GFFilter_NOT(FixtureThingFilter_In2DRange(threat_pos, threat_too_close_distance))),
            FixtureThingCompare_Nearest(creature.PeekPos()), &object)) {
        Fixture3DVector vector = object->PeekPos() - threat_pos;
        vector.Z = 0.0f;
        float distance = vector.GetMagnitude2D();
        if (distance < DELTA_NOTIONAL_ZERO)
            vector.InitialiseFromAngles(GFFloatRandom(1.0f, GetPWorldSeed()), 0.0f, 1.0f);
        else
            vector.Normalise();
        float radius = object->GetPhysicsTC()->GetRadius() + creature.GetPhysicsTC()->GetRadius() + chicken_sacrifice;
        vector *= radius;
        Fixture3DVector hide_pos = object->PeekPos() + vector;
        GFLimit(hide_pos.X, map_world_pos.X, map_world_pos.X + map.GetWorldSizeX());
        GFLimit(hide_pos.Y, map_world_pos.Y, map_world_pos.Y + map.GetWorldSizeY());
        hide_pos.Z = creature.GetMapSizeZBelowAt(hide_pos);
        pos = hide_pos;
        return true;
    }
    return false;
}

static void CheckPosition(const Fixture3DVector& value,float x,float y,float z)
{
    assert(fabs(value.X-x)<0.001 && fabs(value.Y-y)<0.001 && fabs(value.Z-z)<0.001);
}

static void Check()
{
    FixtureThingCreatureBase creature;
    Fixture3DVector threat(0,0,1);
    Fixture3DVector result(20,21,22);
    FixtureCreatureNavigation navigation(creature);
    FixtureThingGameObject near_threat(1,1,4,1,true);
    FixtureThingGameObject invalid(7,-1,4,10,false);
    FixtureThingGameObject closest(9,0,8,2,true);
    FixtureThingGameObject other(-10,7,5,4,true);
    navigation.Search.Add(near_threat);
    navigation.Search.Add(invalid);
    assert(!navigation.GetNearestFleePositionBehindCover(threat,result));
    CheckPosition(result,20,21,22);
    assert(HeightCalls==0 && RandomCalls==0);

    navigation.Search.Add(other);
    assert(navigation.GetNearestFleePositionBehindCover(threat,result));
    CheckPosition(result,-5,10.441f,39);
    assert(creature.GroundProbe.Z==5);
    assert(HeightCalls==1 && RandomCalls==0);

    navigation.Search.Add(closest);
    assert(navigation.GetNearestFleePositionBehindCover(threat,result));
    CheckPosition(result,13,0,39);
    CheckPosition(creature.GroundProbe,13,0,8);
    assert(HeightCalls==2 && RandomCalls==0);

    FixtureCreatureNavigation high(creature);
    FixtureThingGameObject upper(20,20,5,2,true);
    high.Search.Add(upper);
    assert(high.GetNearestFleePositionBehindCover(threat,result));
    CheckPosition(result,14,13,39);

    FixtureCreatureNavigation low(creature);
    FixtureThingGameObject lower(-8,-10,0,1,true);
    low.Search.Add(lower);
    assert(low.GetNearestFleePositionBehindCover(threat,result));
    CheckPosition(result,-5,-4,39);

    FixtureCreatureNavigation coincident(creature);
    coincident.Search.Add(closest);
    coincident.Search.MoveResultTo=&threat;
    assert(coincident.GetNearestFleePositionBehindCover(threat,result));
    CheckPosition(result,4,0,39);
    assert(RandomCalls==1 && HeightCalls==5 && coincident.Seed==1);
}

}

int main()
{
    FleeFixture::Check();
    puts("006cbe80_TEST PASS");
    return 0;
}