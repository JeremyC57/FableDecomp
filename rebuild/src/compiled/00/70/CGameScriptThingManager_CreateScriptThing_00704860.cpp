// CGameScriptThingManager::CreateScriptThing(const CThing&) -- return the script thing for a game thing: when the list already holds an entry with the thing's unique id, refresh that entry's script thing (UpdateThingAttachment, slot 71) and wrap it; otherwise create a CGameScriptThing for the current world and the thing, append {uid, counted pointer} to the list and wrap that
// Leaf: ?CreateScriptThing@CGameScriptThingManager@@QAE?AVCScriptThing@@ABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00704860, length 531 bytes (`ret 8`: hidden result + thing).
// Shape: the protected FindScriptThing (Ego_r 0x00724B40, no retail body) is expanded: the uid halves preloaded, the node's
//   ThingUID compared. Found: slot-71 call, then a CScriptThing built + SetImplementation with a by-value copy of the entry's
//   counted pointer, copied into the hidden result (VC7.1 has no NRVO). Not found (the `else` branch: both branch locals share
//   the frame slot +14h): CScriptThing, then a named CScriptThingInfo (its ThingUID word at +20h is never stored -- the uid
//   goes straight into the list node), Reset (same-TU COMDAT, called), STLport push_back expanded (malloc node), then
//   SetImplementation. Reset and SetImplementation are called out of line (their landed rows 0x007045F0 / 0x007046D0);
//   the shared headers' plain declarations are enough, no body-visibility steering is needed.
// Callees: CBaseClass ctor/dtor 0x0099A2D0 / 0x0099A2E0 (inline CScriptThing ctor/dtor), operator new 0x00BFEA1A,
//   CGameScriptThing::CGameScriptThing(CWorld&, const CThing&) 0x008D0250, CCountedPointer<CScriptThing>::Reset 0x007045F0,
//   malloc 0x00BFEA0E (STLport list node), CScriptThing::SetImplementation 0x007046D0, operator delete 0x00BFE9BC,
//   CScriptThing::UpdateThingAttachment (virtual slot 71). Global CMainGameComponent::PMainGameComponent 0x013B86A0.
#include "fable_compiler.h"
#include <stdlib.h>
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_CGAMESCRIPTTHING
#define FABLE_RETAIL_CGAMESCRIPTTHING_MANAGER_FRIEND
#include "engine/CGameScriptThing.h"
class CWorld;
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#include "engine/CMainGameComponent.h"

inline CScriptThing::CScriptThing() { }                                  // 0x006E7B40 (inline here)
inline CScriptThing::~CScriptThing() { }

class CGameScriptThingManager {
public:
    class CScriptThingInfo {
    public:
        unsigned __int64              ThingUID;                         // +0x00
        CCountedPointer<CScriptThing> PScriptThing;                     // +0x08
        CScriptThingInfo() { }
    };
    CScriptThing CreateScriptThing(const CThing& thing);
protected:
    bool FindScriptThing(const CThing& thing, CScriptThingInfo** info)
    {
        for (std::list<CScriptThingInfo>::iterator it = ScriptThings.begin(); it != ScriptThings.end(); ++it) {
            if ((*it).ThingUID == thing.GetUniqueID()) {
                *info = &*it;
                return true;
            }
        }
        return false;
    }
    std::list<CScriptThingInfo> ScriptThings;                           // +0x00
    CScriptThing                NullThing;                              // +0x04
};

CScriptThing CGameScriptThingManager::CreateScriptThing(const CThing& thing)
{
    CScriptThingInfo* found;
    if (FindScriptThing(thing, &found)) {
        found->PScriptThing->UpdateThingAttachment();
        CScriptThing scriptThing;
        scriptThing.SetImplementation(found->PScriptThing);
        return scriptThing;
    } else {
        CScriptThing scriptThing;
        CScriptThingInfo info;
        info.PScriptThing.Reset(new CGameScriptThing(CMainGameComponent::Get()->GetWorld(), thing));
        info.ThingUID = thing.GetUniqueID();
        ScriptThings.push_back(info);
        scriptThing.SetImplementation(info.PScriptThing);
        return scriptThing;
    }
}