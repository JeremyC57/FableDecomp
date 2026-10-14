// CAIStateGroupBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name) -- binds the state group to its brain, records priorities and name, then InitStateGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FCEC0, length 68 bytes.
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0 (landed-trusted), then the
// slot-13 virtual InitStateGroup (+0x34, rtti-vtables.tsv 0x008FD285) on `this`.
// Mangled name: ?Initialise@CAIStateGroupBase@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z.
// Layout from engine/CAIStateGroupBase.h: PBrain +0x04, ScriptAIPriority +0x08, Name +0x0c,
// Terminated +0x10, Priority +0x11 (the long argument truncated to a byte), Enabled +0x12.
// engine/CAIBrain.h: ThingManager +0x1c; the manager's +0x24 is the AI-side manager handed to
// InitStateGroup together with the creature (VC pushes right to left: creature first).
// Statement order matters for the store schedule: PBrain, Terminated, Priority, ScriptAIPriority
// is the only order of the four plain stores that reproduces retail (the byte rides in al from
// the first load to the last store of the group).
#include "fable_string.h"

struct CThingAICreature;
struct CThingAIManager;

struct CThingManager {
    unsigned char    _pad[0x24];
    CThingAIManager* m_pAIManager;          // +0x24
};

struct CAIBrain {
    void*          __vftable;               // +0x00
    unsigned char  _pad[0x18];              // +0x04..+0x1c
    CThingManager* ThingManager;            // +0x1c
};

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE = 0 };

struct CAIStateGroupBase {                  // vptr +0x00
    CAIBrain*         PBrain;               // +0x04
    EScriptAIPriority ScriptAIPriority;     // +0x08
    CCharString       Name;                 // +0x0c
    bool              Terminated;           // +0x10
    unsigned char     Priority;             // +0x11
    bool              Enabled;              // +0x12
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual void InitStateGroup(CThingAIManager* manager, CThingAICreature& creature);   // slot 13 (+0x34)
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);
};

void CAIStateGroupBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    PBrain = &brain;
    Terminated = false;
    Priority = (unsigned char)priority;
    ScriptAIPriority = scriptPriority;
    Name = name;
    Enabled = true;
    InitStateGroup(brain.ThingManager->m_pAIManager, creature);
}