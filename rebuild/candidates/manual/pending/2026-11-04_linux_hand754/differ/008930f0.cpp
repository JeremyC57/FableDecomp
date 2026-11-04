// CGameScriptInterface::GetThingWithScriptName(const CCharString& mapName, const CCharString& scriptName) const -- the script thing of the first live game object with that script name on the named map, else the null script thing
// Grade: DIFFER(186v186) -- result blocks in the opposite order (ours GetScriptThing fall-through, retail CreateNullThing fall-through + je to GetScriptThing), thing=0 store and tools load scheduled one slot early (dl 27, 186v186, try e)
// Leaf: ?GetThingWithScriptName@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008930F0, length 192 bytes, `ret 0Ch` (hidden result, two strings).
// Flags: none
// Callees: CWorld virtual slots 13 PeekWorldMap (+0x34) and 12 GetWorldMap (+0x30), CWorldMap::
// GetMapNumberFromScriptName @ 0x004FB880, CCharString copy ctor @ 0x0099EC30, CThingSearchTools::GetPFirstThing<
// CFilter_Bind2<CThingFilter_HasScriptName, CThingFilter_InMap>, CThingGameObject> @ 0x008ABDF0, ~CCharString
// @ 0x0099EAE0, CScriptThing::CScriptThing() @ 0x008CFF40, CThing::GetScriptThing() @ 0x004C7E50.
// Layout: CGameScriptInterface World +0x04; CWorld thing search tools +0x20; CThing dead bit +0x91 bit 0.
// Shape: the found-thing out pointer is zeroed first; the map number is a statement before the search; the filters
// are argument temporaries of the bound `&&` (evaluated right to left, so GetWorldMap runs before the name copy, which
// lands in the dead `mapName` slot); the not-found flag `f() ? false : true` rides in bl across the filter's string
// dtor; both results pass the hidden pointer straight through.

class CThing;
class CWorldMap;

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};

class CBaseClass { public:
    virtual ~CBaseClass();                          // 0x0099A2E0
};
class CScriptThing : public CBaseClass { public:
    void* PImp_Data;
    void* PImp_Info;
};
class CGameScriptThing : public CScriptThing { public:
    static CScriptThing __fastcall CreateNullThing();   // 0x008CFF40
};

class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_dead : 1;                       // +0x91 bit 0
    unsigned char m_91_1 : 7;
    bool IsDead() const { return m_dead; }
    CScriptThing GetScriptThing() const;            // 0x004C7E50
};
class CThingGameObject : public CThing { public:};
inline bool IS_THING_ALIVE(const CThing* thing) { return thing != 0 && !thing->IsDead(); }

class CThingFilter_HasScriptName { public:
    CCharString Name;
    CThingFilter_HasScriptName(const CCharString& name) : Name(name) {}
};
class CThingFilter_InMap { public:
    CWorldMap* WorldMap;
    long MapNo;
    CThingFilter_InMap(CWorldMap* worldMap, long mapNo) : WorldMap(worldMap), MapNo(mapNo) {}
};
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) {}
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }

class CThingSearchTools { public:
    // the GetPFirstThing<CFilter_Bind2<CThingFilter_HasScriptName, CThingFilter_InMap>, CThingGameObject> instantiation
    bool GetPFirstThing(const CFilter_Bind2<CThingFilter_HasScriptName, CThingFilter_InMap>& filter, CThingGameObject** out);   // 0x008ABDF0
};

class CWorldMap { public:
    long GetMapNumberFromScriptName(const CCharString& name);   // 0x004FB880
};

class CWorld;
// the search call with the tools fetched before the bound filter is formed
inline bool GetPFirstGameObject(CWorld* world, const CThingFilter_HasScriptName& name, const CThingFilter_InMap& map, CThingGameObject** out);

class CWorld { public:
    virtual void w000(); virtual void w001(); virtual void w002(); virtual void w003(); virtual void w004(); virtual void w005();
    virtual void w006(); virtual void w007(); virtual void w008(); virtual void w009(); virtual void w010(); virtual void w011();
    virtual CWorldMap* GetWorldMap();               // slot 12 (+0x30)
    virtual CWorldMap* PeekWorldMap();              // slot 13 (+0x34)
    unsigned char _pad04[0x1c];
    CThingSearchTools* ThingSearchTools;            // +0x20
    CThingSearchTools& GetThingSearchTools() { return *ThingSearchTools; }
};

inline bool GetPFirstGameObject(CWorld* world, const CThingFilter_HasScriptName& name, const CThingFilter_InMap& map, CThingGameObject** out)
{
    CThingSearchTools* tools = world->ThingSearchTools;
    return tools->GetPFirstThing(name && map, out);
}

class CGameScriptInterface { public:
    virtual void v000();
    CWorld* World;                                  // +0x04
    virtual CScriptThing GetThingWithScriptName(const CCharString& mapName, const CCharString& scriptName) const;
};

CScriptThing CGameScriptInterface::GetThingWithScriptName(const CCharString& mapName, const CCharString& scriptName) const
{
    CThingGameObject* thing = 0;
    long mapNo = World->PeekWorldMap()->GetMapNumberFromScriptName(mapName);
    bool notFound = !World->GetThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName(scriptName) && CThingFilter_InMap(World->GetWorldMap(), mapNo), &thing);
    if (notFound || !IS_THING_ALIVE(thing))
        return CGameScriptThing::CreateNullThing();
    return thing->GetScriptThing();
}
