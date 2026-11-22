#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <utility>
static void require(bool condition) { if (!condition) abort(); }

struct CContainmentVolume { int id; };
template<class T> struct CCountedPointer {
    T* value;
    explicit CCountedPointer(T* p = 0) : value(p) {}
};
template<class K, class V> struct CVectorMap {
    typedef std::pair<K,V> Entry;
    std::vector<Entry> entries;
    static bool Less(const Entry& x,const Entry& y) { return x.first < y.first; }
    void Sort() { std::sort(entries.begin(), entries.end(), Less); }
    unsigned size() const { return (unsigned)entries.size(); }
};
typedef CVectorMap<long,CCountedPointer<CContainmentVolume> > Volumes;
struct CPhysicsMeshObject {
    Volumes result;
    int* reads;
    void GetNavigationLevelVolumes(Volumes& output) const {
        ++*reads;
        output.entries.insert(output.entries.end(),result.entries.begin(),result.entries.end());
    }
};
struct CTCPhysicsBase {
    bool meshPresent;
    CPhysicsMeshObject mesh;
    mutable int checks, meshReads;
    bool HasPhysicsMeshObject() const { ++checks; return meshPresent; }
    CPhysicsMeshObject GetPhysicsMeshObject() const { require(meshPresent); ++meshReads; return mesh; }
};
struct CThingPhysical {
    int area;
    bool navigation;
    CTCPhysicsBase physics;
    const CTCPhysicsBase* GetPhysicsTC() const { return &physics; }
};
struct C2DBoxI {};
struct CWorldMap {
    long requested;
    mutable int boxReads;
    C2DBoxI box;
    const C2DBoxI& GetMapBox(long n) const { require(n == requested); ++boxReads; return box; }
};
struct CThingFilter_BoundingBoxOverlapsMap {
    const CWorldMap& map;
    long number;
    CThingFilter_BoundingBoxOverlapsMap(const CWorldMap& m,long n) : map(m),number(n) {}
    bool operator()(const CThingPhysical& t) const { require(number == map.requested); return number == t.area; }
};
struct CThingFilter_HasNavigationInformation {
    bool operator()(const CThingPhysical& t) const { return t.navigation; }
};
template<class A, class B> struct CFilter_Bind2 {
    const A& first;
    const B& second;
    CFilter_Bind2(const A& a,const B& b) : first(a),second(b) {}
    bool operator()(const CThingPhysical& t) const { return first(t) && second(t); }
};
template<class A, class B> CFilter_Bind2<A,B> GFFilter_Bind2(const A& a,const B& b) { return CFilter_Bind2<A,B>(a,b); }
struct CThingSearchTools {
    std::vector<const CThingPhysical*> population;
    mutable int queries;
    template<class T, class F> void GetAllThings(const F& filter,std::vector<T*>& out) const {
        ++queries;
        for (unsigned i=0; i<population.size(); ++i)
            if (filter(*population[i])) out.push_back(population[i]);
    }
};
struct CWorld {
    CWorldMap map;
    CThingSearchTools tools;
    CWorldMap& GetWorldMap() { return map; }
    const CThingSearchTools& PeekThingSearchTools() const { return tools; }
};
struct CCachedVolumes {
    static int destroyed;
    int writes;
    Volumes values;
    CCachedVolumes() : writes(0) {}
    ~CCachedVolumes() { ++destroyed; }
    void InitialiseNavigationContainmentVolumes(const Volumes& input) { ++writes; values=input; }
};
int CCachedVolumes::destroyed = 0;
struct CacheOwner {
    CCachedVolumes* current;
    CacheOwner() : current(0) {}
    ~CacheOwner() { delete current; }
    CCachedVolumes& operator*() const { require(current != 0); return *current; }
    void reset(CCachedVolumes* fresh) { delete current; current=fresh; }
private:
    CacheOwner(const CacheOwner&);
    CacheOwner& operator=(const CacheOwner&);
};
struct CMap {
    CWorld world;
    CacheOwner CachedVolumes;
    CWorld* GetWorld() { return &world; }
    long GetWorldMapNumber() const { return world.map.requested; }
    void InitCachedVolumes();
};

void CMap::InitCachedVolumes()
{
    CachedVolumes.reset(new CCachedVolumes);
    const C2DBoxI& area=GetWorld()->GetWorldMap().GetMapBox(GetWorldMapNumber());
    CVectorMap<long,CCountedPointer<CContainmentVolume> > volumes;
    std::vector<const CThingPhysical*> things;
    GetWorld()->PeekThingSearchTools().GetAllThings<const CThingPhysical>(
        GFFilter_Bind2(CThingFilter_BoundingBoxOverlapsMap(GetWorld()->GetWorldMap(),GetWorldMapNumber()),CThingFilter_HasNavigationInformation()),
        things);
    long thingssize=things.size();
    for(long c1=0;c1<thingssize;++c1) {
        const CThingPhysical* thing=things[c1];
        if(thing->GetPhysicsTC()->HasPhysicsMeshObject()) {
            CPhysicsMeshObject physics_object=thing->GetPhysicsTC()->GetPhysicsMeshObject();
            long old_size=volumes.size();
            physics_object.GetNavigationLevelVolumes(volumes);
        }
    }
    volumes.Sort();
    CCachedVolumes& containment_volumes=*CachedVolumes;
    containment_volumes.InitialiseNavigationContainmentVolumes(volumes);
}

static void exercise(unsigned pattern, unsigned count, bool existing)
{
    CMap map;
    map.world.map.requested=7;
    map.world.map.boxReads=0;
    map.world.tools.queries=0;
    CContainmentVolume volumes[6];
    CThingPhysical things[3];
    int reads[3]={0,0,0};
    std::vector<int> expected;
    for (unsigned i=0; i<3; ++i) {
        CThingPhysical& t=things[i];
        unsigned state=(pattern >> (i*4)) & 15;
        t.area=(state & 1) ? 7 : 8;
        t.navigation=(state & 2)!=0;
        t.physics.meshPresent=(state & 4)!=0;
        t.physics.checks=t.physics.meshReads=0;
        t.physics.mesh.reads=&reads[i];
        if (state & 8)
            for (unsigned j=0; j<2; ++j) {
                unsigned slot=2*i+j;
                volumes[slot].id=8-int(slot);
                Volumes::Entry entry(volumes[slot].id,CCountedPointer<CContainmentVolume>(&volumes[slot]));
                t.physics.mesh.result.entries.push_back(entry);
                if (i<count && (state & 7)==7) expected.push_back(volumes[slot].id);
            }
        if (i<count) map.world.tools.population.push_back(&t);
    }
    std::sort(expected.begin(),expected.end());
    if (existing) map.CachedVolumes.reset(new CCachedVolumes);
    int destroyed=CCachedVolumes::destroyed;
    map.InitCachedVolumes();
    require(CCachedVolumes::destroyed == destroyed+int(existing));
    require(map.world.map.boxReads==1 && map.world.tools.queries==1);
    CCachedVolumes& result=*map.CachedVolumes;
    require(result.writes==1 && result.values.size()==expected.size());
    for (unsigned k=0; k<expected.size(); ++k) {
        const Volumes::Entry& e=result.values.entries[k];
        require(e.first==expected[k] && e.second.value==&volumes[8-expected[k]]);
    }
    for (unsigned i=0; i<3; ++i) {
        bool included=i<count && things[i].area==7 && things[i].navigation;
        bool hasMesh=included && things[i].physics.meshPresent;
        require(things[i].physics.checks==int(included));
        require(things[i].physics.meshReads==int(hasMesh) && reads[i]==int(hasMesh));
    }
}
int main()
{
    for (unsigned n=0; n<=3; ++n)
        for (unsigned mask=0; mask<(1u << (4*n)); ++mask) {
            exercise(mask,n,false);
            exercise(mask,n,true);
        }
    puts("00821850_TEST PASS");
    return 0;
}