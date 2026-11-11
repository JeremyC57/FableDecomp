// CGameScriptInterface::CreateCreature(const CCharString& defName, const C3DVector& position, const CCharString& scriptName, bool appear) const -- create an AI creature from the named def at the position (the def's player, a CThingAICreatureInit carrying the script name), return its script thing; optionally start the creature's appear action, then wait CreateCreatureDelayFrames script frames
// Leaf: CreateCreature
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
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
#include "engine/C3DVector.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }

class CThingCreatureDef { public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    unsigned char _pad08[0x11c - 0x08];
    long Player;                                        // +0x11C
};
template <class T> class CDefPointer;
template <> class CDefPointer<const CThingCreatureDef> { public:   // Ego_r.pdb CDefPointer<const CThingCreatureDef> (was the row-local CDefPointerThingCreatureDef)
    CThingCreatureDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
};
class CDefinitionManager { public:   // the PDB base the accessor belongs to (empty here: offset 0)
    template <class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x00513160 GetDef<CThingCreatureDef> (Ego_r.pdb member template)
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410 (Ego_r.pdb CDefinitionManager, const)
};
class CGameDefinitionManager : public CDefinitionManager { public:
};
void* FindDef_CGameDefinitionManager_GetDef_0();
#pragma auto_inline(off)
template <> __declspec(noinline) bool CDefinitionManager::GetDef<CThingCreatureDef>(long globalIndex, CDefPointer<const CThingCreatureDef>* out) const
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

CScriptThing CGameScriptInterface::CreateCreature(const CCharString& defName, const C3DVector& position,
                                                  const CCharString& scriptName, bool appear) const
{
    long index = DefinitionManager->GetDefGlobalIndexFromName(defName);
    CScriptThing result;
    CDefPointer<const CThingCreatureDef> def;
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