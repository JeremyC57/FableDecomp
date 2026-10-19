// CGameScriptInterface::GetThingWithScriptName(const CCharString& mapName, const CCharString& scriptName) const -- the
// Grade: DIFFER(ours v retail) -- 186v186: register rotation in the filter setup (World->tools through edx where retail uses ecx), push edi before the zero store, and the two result blocks in the opposite order (retail: null CScriptThing fall-through, GetScriptThing last)
// script thing of the first live game object with that script name on the named map, else the null script thing
// Leaf: ?GetThingWithScriptName@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008930F0, length 192 bytes, `ret 0Ch` (hidden result, two strings).
// Callees: CWorld virtual slots 13 PeekWorldMap (+0x34) and 12 GetWorldMap (+0x30), CWorldMap::
// GetMapNumberFromScriptName @ 0x004FB880, CCharString copy ctor @ 0x0099EC30, CThingSearchTools::GetPFirstThing<
// CFilter_Bind2<CThingFilter_HasScriptName, CThingFilter_InMap>, CThingGameObject> @ 0x008ABDF0, ~CCharString
// @ 0x0099EAE0, CScriptThing::CScriptThing() @ 0x008CFF40, CThing::GetScriptThing() @ 0x004C7E50.
// Layout: CGameScriptInterface World +0x04; CWorld thing search tools +0x20; CThing dead bit +0x91 bit 0.
// Shape: the found-thing out pointer is zeroed first; the map number is a statement before the search; the filters
// are argument temporaries of the bound `&&` (evaluated right to left, so GetWorldMap runs before the name copy, which
// lands in the dead `mapName` slot); the not-found flag `f() ? false : true` rides in bl across the filter's string
// dtor; both results pass the hidden pointer straight through.

struct CThing;
struct CWorldMap;

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};

struct CBaseClass {
    virtual ~CBaseClass();                          // 0x0099A2E0
};
struct CScriptThing : CBaseClass {
    void* PImp_Data;
    void* PImp_Info;
    CScriptThing();                                 // 0x008CFF40
};

struct CThing {
    unsigned char _pad[0x91];
    unsigned char m_dead : 1;                       // +0x91 bit 0
    unsigned char m_91_1 : 7;
    CScriptThing GetScriptThing();                  // 0x004C7E50
};
struct CThingGameObject : CThing {};

struct CThingFilter_HasScriptName {
    CCharString Name;
    CThingFilter_HasScriptName(const CCharString& name) : Name(name) {}
};
struct CThingFilter_InMap {
    CWorldMap* WorldMap;
    long MapNo;
    CThingFilter_InMap(CWorldMap* worldMap, long mapNo) : WorldMap(worldMap), MapNo(mapNo) {}
};
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) {}
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }

struct CThingSearchTools {
    // the GetPFirstThing<CFilter_Bind2<CThingFilter_HasScriptName, CThingFilter_InMap>, CThingGameObject> instantiation
    bool GetPFirstThing(const CFilter_Bind2<CThingFilter_HasScriptName, CThingFilter_InMap>& filter, CThingGameObject** out);   // 0x008ABDF0
};

struct CWorldMap {
    long GetMapNumberFromScriptName(const CCharString& name);   // 0x004FB880
};

struct CWorld;
// the search call with the tools fetched before the bound filter is formed
inline bool GetPFirstGameObject(CWorld* world, const CThingFilter_HasScriptName& name, const CThingFilter_InMap& map, CThingGameObject** out);

struct CWorld {
    virtual void w000(); virtual void w001(); virtual void w002(); virtual void w003(); virtual void w004(); virtual void w005();
    virtual void w006(); virtual void w007(); virtual void w008(); virtual void w009(); virtual void w010(); virtual void w011();
    virtual CWorldMap* GetWorldMap();               // slot 12 (+0x30)
    virtual CWorldMap* PeekWorldMap();              // slot 13 (+0x34)
    unsigned char _pad04[0x1c];
    CThingSearchTools* ThingSearchTools;            // +0x20
};

inline bool GetPFirstGameObject(CWorld* world, const CThingFilter_HasScriptName& name, const CThingFilter_InMap& map, CThingGameObject** out)
{
    CThingSearchTools* tools = world->ThingSearchTools;
    return tools->GetPFirstThing(name && map, out);
}

struct CGameScriptInterface {
    virtual void v000();
    CWorld* World;                                  // +0x04
    virtual CScriptThing GetThingWithScriptName(const CCharString& mapName, const CCharString& scriptName) const;
};

CScriptThing CGameScriptInterface::GetThingWithScriptName(const CCharString& mapName, const CCharString& scriptName) const
{
    CThingGameObject* thing = 0;
    long mapNo = World->PeekWorldMap()->GetMapNumberFromScriptName(mapName);
    bool notFound = GetPFirstGameObject(World, CThingFilter_HasScriptName(scriptName), CThingFilter_InMap(World->GetWorldMap(), mapNo), &thing) ? false : true;
    if (!notFound && thing != 0 && !thing->m_dead)
        return thing->GetScriptThing();
    return CScriptThing();
}
