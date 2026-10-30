// CGameScriptInterface::CreateCreature(const CCharString& defName, const C3DVector& position, const CCharString& scriptName, bool appear) const -- create an AI creature from the named def at the position (the def's player, a CThingAICreatureInit carrying the script name), return its script thing; optionally start the creature's appear action, then wait CreateCreatureDelayFrames script frames
// Leaf: CreateCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A9100, length 532 bytes (virtual, `ret 14h`: hidden result + 4 args).
// Callees: CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CDefinitionManager::GetDef<CThingCreatureDef>
//   0x00513160 (same-TU auto_inline(off) stand-in; it only fills the out slot), CCharString ctor/copy/dtor/operator=
//   0x0099EBF0 / 0x0099EC30 / 0x0099EAE0 / 0x0099EFB0, CThingAICreature::Create 0x00833800 (static __fastcall),
//   CThing::GetScriptThing 0x004C7E50, CScriptThing::operator= 0x008AB980, CBaseClass ctor/dtor 0x0099A2D0 /
//   0x0099A2E0, operator delete 0x00BFE9BC, the appear action's ctor 0x0074DBB0 (manifest label
//   "CCombatAction_Guildmaster_HealFollowersInto" is propagated) and dtor 0x0062E900, CThingCreatureBase::SetCurrentAction
//   0x006644F0, GSI slot 7 NewScriptFrame. CScriptThing vtable 0x01238C8C; "" 0x0122D70E.
// Layout: CThingAICreatureInit is the Ego_r.pdb record (28 bytes). CGameScriptInterface is modelled locally: retail
//   STLport maps are 12 bytes where the PDB-generated engine/CGameScriptInterface.h has 16, so the retail
//   CreateCreatureDelayFrames is at +0x4C (the header's +0x50); the loop reads [this+4Ch].
// Shape: the init's two CCharString default arguments are `""` literals (converted temporaries built before the
//   copies, passed by `lea`); `result` is declared before the def pointer; the delay loop counts down from the member.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class C3DVector { public: float X, Y, Z; };

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CCPPointerInfo { public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CCountedScriptImp {                              // CCountedPointer<CScriptThingImp>
    void* Data;
    CCPPointerInfo* Info;
    CCountedScriptImp() : Data(0), Info(0) {}
    CCountedScriptImp(const CCountedScriptImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedScriptImp()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CScriptThing : public CBaseClass { public:                      // vtable 0x01238C8C
    CCountedScriptImp PImp;                             // +0x04
    CScriptThing() {}
    virtual ~CScriptThing() {}
    CScriptThing& operator=(const CScriptThing& other); // 0x008AB980
};

class CThingCreatureDef { public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    unsigned char _pad08[0x11c - 0x08];
    long Player;                                        // +0x11C
};
struct CDefPointerThingCreatureDef {
    CThingCreatureDef* Object;
    CDefPointerThingCreatureDef() : Object(0) {}
    ~CDefPointerThingCreatureDef() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
};
class CGameDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);                     // 0x009AD410
    bool GetDef(long globalIndex, CDefPointerThingCreatureDef* out);             // 0x00513160
};
void* FindDef_CGameDefinitionManager_GetDef_0();
#pragma auto_inline(off)
__declspec(noinline) bool CGameDefinitionManager::GetDef(long globalIndex, CDefPointerThingCreatureDef* out)
{
    void* def = FindDef_CGameDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThingPhysical;
class CThingBuilding;
class CThingAICreatureInit { public:                           // Ego_r.pdb, 28 bytes
    CThingPhysical* OriginalThing;                      // +0x00
    CThingPhysical* PreviousThing;                      // +0x04
    CCharString ScriptName;                             // +0x08
    CCharString OverridingBrainName;                    // +0x0C
    bool PlayerCharacter;                               // +0x10
    bool Drawable;                                      // +0x11
    bool PlaceAtEndOfList;                              // +0x12
    bool EnableCreatureAutoPlacing;                     // +0x13
    CThingBuilding* HomeBuilding;                       // +0x14
    CThingBuilding* WorkBuilding;                       // +0x18
    CThingAICreatureInit(const CCharString& scriptName = "", const CCharString& brainName = "")
        : OriginalThing(0), PreviousThing(0), ScriptName(scriptName), OverridingBrainName(brainName),
          PlayerCharacter(false), Drawable(true), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) {}
};
class CCreatureActionBase;
class CThingAICreature { public:
    CScriptThing GetScriptThing();                      // 0x004C7E50 (CThing)
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0 (CThingCreatureBase)
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                               CThingAICreatureInit* init);   // 0x00833800
};
struct CCreatureActionAppear {                          // 0x114-byte creature action built on the new creature
    unsigned char _data[0x114];
    CCreatureActionAppear(CThingAICreature* creature);  // 0x0074DBB0
    ~CCreatureActionAppear();                           // 0x0062E900
    operator const CCreatureActionBase&() const { return *(const CCreatureActionBase*)this; }
};

class CWorld; class CMainGameComponent; class CDisplayEngine;
class CGameScriptInterface { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void NewScriptFrame();                      // slot 7 (+0x1C)
    CWorld*                 World;                      // +0x04
    CMainGameComponent*     Component;                  // +0x08
    CDisplayEngine*         DisplayEngine;              // +0x0c
    CGameDefinitionManager* DefinitionManager;          // +0x10
    unsigned char           _pad14[0x4c - 0x14];
    long                    CreateCreatureDelayFrames;  // +0x4C
    virtual CScriptThing CreateCreature(const CCharString& defName, const C3DVector& position, const CCharString& scriptName, bool appear) const;
};

CScriptThing CGameScriptInterface::CreateCreature(const CCharString& defName, const C3DVector& position,
                                                  const CCharString& scriptName, bool appear) const
{
    long index = DefinitionManager->GetDefGlobalIndexFromName(defName);
    CScriptThing result;
    CDefPointerThingCreatureDef def;
    if (DefinitionManager->GetDef(index, &def)) {
        CThingAICreatureInit init;
        init.ScriptName = scriptName;
        CThingAICreature* creature = CThingAICreature::Create(index, position, def.Object->Player, &init);
        result = creature->GetScriptThing();
        if (appear)
            creature->SetCurrentAction(CCreatureActionAppear(creature));
        for (long i = CreateCreatureDelayFrames; i > 0; --i)
            const_cast<CGameScriptInterface*>(this)->NewScriptFrame();
    }
    return result;
}