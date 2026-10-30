// CGameScriptInterface::CreateExplosion(const CCharString& defName, const C3DVector& pos, const C3DVector* dir, CCharString scriptName) const -- create the named explosion definition at a position (optionally aimed), give it the script name and return it as a script thing (the null script thing when the definition is unknown or nothing live was made)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089FC80, length 147 bytes.
// Callees: CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
//          the explosion factories (__fastcall) @ 0x0080C500 (with a direction) and @ 0x0080C460, CCharString::operator= @ 0x0099EFB0,
//          CThing::GetScriptThing @ 0x004C7E50, CBaseClass::CBaseClass @ 0x0099A2D0, CCharString::~CCharString @ 0x0099EAE0 (the by-value name).
// Shape: CScriptThing returned by hidden pointer; the null script thing is built inline (CBase ctor out of line, vtable, two zeroes from the shared zero register).
// Leaf: CreateExplosion
class CCharString { public:                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class C3DVector { public: float x, y, z; };
struct SCountBlock;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct CScriptThingImp;
struct CScriptThingPImp {                          // the counted pointer member: an inline constructor zeroes both words after the vtable store
    CScriptThingImp* Data;                         // +4
    SCountBlock*     Info;                         // +8
    CScriptThingPImp() : Data(0), Info(0) {}
};
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C (rtti-vtables.tsv)
    CScriptThingPImp PImp;
    CScriptThing() {}                              // inline: CBaseClass::CBaseClass, the vtable, two zero words
};
struct CThing_Retail {
    unsigned char  _pad00[0x74];
    CCharString    ScriptName;                     // +0x74
    unsigned char  _pad78[0x91 - 0x78];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    CScriptThing GetScriptThing();                 // 0x004C7E50 (manifest "GFPredicateAnd<...>" is propagated)
};
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager_Retail { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0 ?Get@CGameDefinitionManager@@SIAAV1@XZ
class CWorld; class CMainGameComponent; class CDisplayEngine; struct CPlayerManager_Retail; class CThingSearchTools;
CThing_Retail* __fastcall CreateExplosionThing(long defIndex, const C3DVector& pos, const C3DVector& dir);   // 0x0080C500
CThing_Retail* __fastcall CreateExplosionThing(long defIndex, const C3DVector& pos);                         // 0x0080C460
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                     World;             // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent*         Component;         // +0x08
    CDisplayEngine*             DisplayEngine;     // +0x0c
    CDefinitionManager_Retail*  DefinitionManager; // +0x10
    CPlayerManager_Retail*      PlayerManager;     // +0x14
    CThingSearchTools*          ThingSearchTools;  // +0x18
    virtual CScriptThing CreateExplosion(const CCharString& defName, const C3DVector& pos, const C3DVector* dir, CCharString scriptName) const;
};

CScriptThing CGameScriptInterface::CreateExplosion(const CCharString& defName, const C3DVector& pos, const C3DVector* dir, CCharString scriptName) const
{
    long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(defName);
    if (defIndex > 0) {
        CThing_Retail* t = dir ? CreateExplosionThing(defIndex, pos, *dir) : CreateExplosionThing(defIndex, pos);
        if (t && !t->m_dead) {
            t->ScriptName = scriptName;
            return t->GetScriptThing();
        }
    }
    return CScriptThing();
}