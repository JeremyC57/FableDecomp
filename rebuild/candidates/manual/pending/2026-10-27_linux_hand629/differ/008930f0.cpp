// CGameScriptInterface::GetThingWithScriptName(const CCharString& mapName, const CCharString& scriptName) const -- the script thing of the first live game object with that script name on the named map, else the null script thing
// Grade: DIFFER(186v186) -- the two result blocks in the opposite order (retail: CreateNullThing fall-through, GetScriptThing block last, `test [ecx+91h],1 ; je found`), the thing=0 store before `push eax` (retail after it) and `mov [esp+1Ch],eax` scheduled before `mov ecx,[ecx+20h]`
// Leaf: ?GetThingWithScriptName@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008930F0, 186 bytes, `ret 0Ch` (hidden result, two strings).
// Identity: vtable slot 73 of CGameScriptInterface (engine/CGameScriptInterface.h), FableWin.pdb mangled name above (the
//   two-string overload; slot 72 0x008A7D60 is the one-string overload, landed).
// Callees: CWorld virtual slots 13 PeekWorldMap (+0x34) and 12 GetWorldMap (+0x30) (FableWin.pdb
//   ?PeekWorldMap@CWorld@@UBEABVCWorldMap@@XZ / ?GetWorldMap@CWorld@@UAEAAVCWorldMap@@XZ), CWorldMap::
//   GetMapNumberFromScriptName 0x004FB880 (Ego_r.pdb, const), CCharString copy ctor 0x0099EC30,
//   CThingSearchTools::GetPFirstThing<CFilter_Bind2<CThingFilter_HasScriptName, CThingFilter_InMap>, CThingGameObject>
//   0x008ABDF0 (landed), ~CCharString 0x0099EAE0, CGameScriptThing::CreateNullThing 0x008CFF40 (static __fastcall, the
//   hidden result in ecx; bound), CThing::GetScriptThing() const 0x004C7E50.
// Layout: CGameScriptInterface World +0x04; CWorld thing search tools +0x20; CThing dead bit +0x91 bit 0.
// Shape: the found-thing out pointer is zeroed first; the map number is a statement before the search; the filters
//   are argument temporaries of the bound `&&` (evaluated right to left, so GetWorldMap runs before the name copy, which
//   lands in the dead `mapName` slot); the not-found flag `f() ? false : true` rides in bl across the filter's string
//   dtor; both results pass the hidden pointer straight through.
// Tried (sweep): failure-first return, IsDead accessor, IS_THING_ALIVE helper, nested alive if. Chunk 629: the null result is
//   CGameScriptThing::CreateNullThing() (bound, not a CScriptThing ctor) -- with it, success-first and failure-first returns,
//   a `found` bool, separate not-found return, IS_THING_ALIVE(const CThing*) and thing declared after the map number (store
//   moves below the call) all keep our found-first layout; a ternary return builds a temporary (236 bytes).
//   The inline GetThingSearchTools() accessor (landed RemoveDeadCreature 0x00894490) fixed the edx/ecx tools rotation.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
};
#include "engine/CBaseClass.h"
class CScriptThing : public CBaseClass { public:    // vtable 0x01238C8C, 0xc bytes
    void* PImp_Data;
    void* PImp_Info;
    CScriptThing(const CScriptThing& other);
    virtual ~CScriptThing();
};
class CGameScriptThing : public CScriptThing { public:   // Ego_r.pdb: CGameScriptThing : CScriptThing
    static CScriptThing __fastcall CreateNullThing();     // 0x008CFF40
};
class CThing { public:
    unsigned char _pad[0x91];
    unsigned char Dead : 1;                         // +0x91 bit 0
    unsigned char _bits91 : 7;
    CScriptThing GetScriptThing() const;            // 0x004C7E50
};
class CThingGameObject : public CThing { };
class CWorldMap { public:
    long GetMapNumberFromScriptName(const CCharString& name) const;   // 0x004FB880
};
class CThingFilter_HasScriptName { public:
    CCharString Name;
    CThingFilter_HasScriptName(const CCharString& name) : Name(name) { }
};
class CThingFilter_InMap { public:
    const CWorldMap* WorldMap;
    long MapNumber;
    CThingFilter_InMap(const CWorldMap& worldMap, long mapNumber) : WorldMap(&worldMap), MapNumber(mapNumber) { }
};
template <class A, class B> class CFilter_Bind2 { public:     // both filters, held by reference (landed 0x008ABDF0)
    const A& First;
    const B& Second;
    CFilter_Bind2(const A& a, const B& b) : First(a), Second(b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
class CThingSearchTools { public:
    template <class F, class T> bool GetPFirstThing(const F& filter, T** out) const;   // <CFilter_Bind2<..>,CThingGameObject> 0x008ABDF0
};
class CWorld { public:
    virtual void w000(); virtual void w001(); virtual void w002(); virtual void w003(); virtual void w004(); virtual void w005();
    virtual void w006(); virtual void w007(); virtual void w008(); virtual void w009(); virtual void w010(); virtual void w011();
    virtual CWorldMap& GetWorldMap();               // slot 12 (+0x30)
    virtual const CWorldMap& PeekWorldMap() const;  // slot 13 (+0x34)
    unsigned char _pad04[0x1c];
    CThingSearchTools* PThingSearchTools;           // +0x20 scoped_ptr<CThingSearchTools>
    CThingSearchTools& GetThingSearchTools() { return *PThingSearchTools; }
};
class CGameScriptInterface { public:
    virtual void v000();
    CWorld* World;                                  // +0x04
    virtual CScriptThing GetThingWithScriptName(const CCharString& mapName, const CCharString& scriptName) const;
};

CScriptThing CGameScriptInterface::GetThingWithScriptName(const CCharString& mapName, const CCharString& scriptName) const
{
    CThingGameObject* thing = 0;
    long mapNumber = World->PeekWorldMap().GetMapNumberFromScriptName(mapName);
    bool notFound = World->GetThingSearchTools().GetPFirstThing(CThingFilter_HasScriptName(scriptName) && CThingFilter_InMap(World->GetWorldMap(), mapNumber), &thing) ? false : true;
    if (notFound || thing == 0 || thing->Dead)
        return CGameScriptThing::CreateNullThing();
    return thing->GetScriptThing();
}
