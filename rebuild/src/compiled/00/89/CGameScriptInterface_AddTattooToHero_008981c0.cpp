// CGameScriptInterface::AddTattooToHero(const CCharString& tattooDefName) const -- create the named tattoo object just ahead of the live hero (position + 2 x physics vector), hand it to the hero morph (TC 3) and re-upload the hero's texture morphs, then kill the temporary object
// Leaf: AddTattooToHero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008981C0, length 382 bytes (`ret 4`).
// Callees: CEngine::PeekDefinitionManager @ 0x00415FBC (a jmp thunk; manifest "GFGetPreMainMemoryUsed") on the engine
//   singleton 0x013B86A0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CPlayerManager::GetMainPlayer
//   @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, the hero physics' slot 70 (+0x118, a C3DVector by value),
//   CThing::GetPos @ 0x004C73D0, CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0, CThingObject::Create @ 0x00703210
//   (static __fastcall), the component-map LowerBound @ 0x0040F020, CTCHeroMorph::AddTattoo(CThing*) @ 0x0071D7C0,
//   CTCAppearanceMorphBase::UploadTextureMorphsToEngine(CIEngine*, set<long>*, bool, float) @ 0x006CA7F0, CThing::Kill
//   @ 0x004C9B80.
// Layout: CThing player byte +0x90, dead flags +0x91 bit 0, UsingComponents +0x20 (TC 3 = word 0 bit 3), component map
//   +0x44, physics +0x60; the engine singleton's +0x28 object holds the CIEngine at +0x2C.
// Shape: the scratch script name is a named local in its own block around Create (destroyed right after it).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
class CRightHandedSet;
class CIEngine;
struct CEngineHolder_Retail {
    unsigned char _pad00[0x2c];
    CIEngine*     Engine;                               // +0x2C
};
struct CDefinitionManager_Retail { long GetDefGlobalIndexFromName(const CCharString& name) const; };   // 0x009AD410
struct CEngine_Retail {
    unsigned char         _pad00[0x28];
    CEngineHolder_Retail* Graphics;                     // +0x28
    const CDefinitionManager_Retail& PeekDefinitionManager() const;   // 0x00415FBC
};
extern CEngine_Retail* g_pEngine;                       // 0x013B86A0
struct CThingPhysics_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetFacingVector();                // slot 70 (+0x118)
};
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap_Retail;
class CThing { public:
    const C3DVector& GetPos() const;               // 0x004C73D0
};
struct CThing_Retail : public CThing {
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];         // +0x20
    CInterfaceMap_Retail  m_interfaces;             // +0x44
    unsigned char         _pad4c[0x14];
    CThingPhysics_Retail* m_pPhysics;               // +0x60
    unsigned char         _pad64[0x2c];
    signed char           m_player;                 // +0x90
    unsigned char         m_dead : 1;               // +0x91 bit 0
    unsigned char         m_91_1 : 7;
    __forceinline unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase_Retail* GetInterface(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return ((CTCBase_Retail*)it->second);
    }
    template<class T> __forceinline bool GetInterfaceOut(T** ppOut, long interfaceType)
    {
        if (HasInterface(interfaceType)) {
            *ppOut = (T*)GetInterface(interfaceType);
            return true;
        }
        return false;
    }
    void Kill(bool silent);                             // 0x004C9B80
};
class CThingPlayerCreature : public CThing_Retail { };
class CThingObject : public CThing_Retail { public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                           const CRightHandedSet* orientation, unsigned long uid,
                                           const CCharString& scriptName);   // 0x00703210
};
struct CLongSet;
class CTCHeroMorph { public:
    void AddTattoo(CThing_Retail* tattoo);              // 0x0071D7C0
    void UploadTextureMorphsToEngine(CIEngine* engine, CLongSet* morphs, bool b, float f);   // 0x006CA7F0
};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };      // 0x00487DC0
class CPlayerManager { public: CPlayer& GetMainPlayer(); };    // 0x00449970
class CGameScriptInterface { public:
    virtual void AddTattooToHero(const CCharString& tattooDefName) const;
    void*            World;                             // +0x04 (engine/CGameScriptInterface.h)
    void*            Component;                         // +0x08
    void*            DisplayEngine;                     // +0x0c
    void*            DefinitionManager;                 // +0x10
    CPlayerManager*  PlayerManager;                     // +0x14
};

void CGameScriptInterface::AddTattooToHero(const CCharString& tattooDefName) const
{
    long index = g_pEngine->PeekDefinitionManager().GetDefGlobalIndexFromName(tattooDefName);
    if (index <= 0)
        return;
    CThing_Retail* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->m_dead)
        return;
    C3DVector offset = hero->m_pPhysics->GetFacingVector() * 2.0f;
    C3DVector position = hero->GetPos() + offset;
    CThingObject* tattoo;
    {
        CCharString scriptName("");
        tattoo = CThingObject::Create(index, position, hero->m_player, 0, 0, scriptName);
    }
    if (!tattoo || tattoo->m_dead)
        return;
    if (hero->HasInterface(3)) {
        ((CTCHeroMorph*)hero->GetInterface(3))->AddTattoo(tattoo);
        CTCHeroMorph* morph;
        if (!hero->m_dead && hero->GetInterfaceOut(&morph, 3))
            morph->UploadTextureMorphsToEngine(g_pEngine->Graphics->Engine, 0, false, 0.0f);
    }
    tattoo->Kill(false);
}