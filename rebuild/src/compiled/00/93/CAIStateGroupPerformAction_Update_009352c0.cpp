// CAIStateGroup_PerformAction::Update() -- scripted "perform action": while the perceived thing lives and the scratchpad says FaceHeroForAction, keep looking at it; then loop on the scratchpad's scripting state group (1: play the scratchpad's Action as a combat animation at the hero, 2: speak to the hero) while the action is still running; mark the group fired and terminate (state 0 terminates at once)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009352C0, length 572 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: the player-manager accessor @ 0x008FCF80, GetPMainPlayer @ 0x00449960, CPlayer::GetPControlledCreature @ 0x00487DC0,
// CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface 0x34 = CTCPerceiveThing, bit 20 of +0x24) and
// non-const @ 0x0040F020 (GetInterface 0xAA = CTCAIScratchpad, bit 10 of +0x34), CTCPerceiveThing::GetPerceivedThing
// @ 0x007446A0, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 (by-value CIntelligentPointer<const
// CThing>, vtable 0x01237F1C, and CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8), AIAction_LookAtThing
// @ 0x0097D6F0, Fired @ 0x009350C0, AIAction_SpeakToTarget @ 0x00980BD0, CCharString copy ctor @ 0x0099EC30 /
// ~CCharString @ 0x0099EAE0, CCreatureAction_PlayCombatAnimation ctor @ 0x008B7F80 / ~CCombatActionBase @ 0x0062E900,
// AIAction_PerformAction @ 0x0097D8B0, Terminate @ 0x008FCF40.
// Data: 1/72 (0.013888889f) look speed (immediate).
// Layout: engine/CAIStateGroup_PerformAction.h flattens the base, so the model is local (PBrain +4); CTCAIScratchpad from
// Ego_r.pdb (ScriptingStateGroup +0x38, Action +0x48, FaceHeroForAction +0x4c); the combat action is 0x118 bytes in
// retail (PDB 0x130).
// Shape: both lookups use the out-parameter GetInterface with the HasTC byte bound to a `const unsigned char&`; the
// perception lookup goes through a const creature (const LowerBound); the misses read the never-written slots.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
long TmplSink(const void* self, long key);      // stand-in: the callee's own work (outside the graded bytes)
#pragma auto_inline(off)
// Same-TU stand-ins for the two CVectorMap::LowerBound copies: they read the key through its pointer and let no argument
// escape, so the inlined lookup keys share one frame word (as retail's LTCG saw).
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    return (SEntry*)TmplSink(this, *type);
}
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    return (const SEntry*)TmplSink(this, *type + 0);
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char HasTC(long interfaceType) const { return (unsigned char)((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const unsigned char& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        const unsigned char& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present != 0;
    }
};
class CThingCreatureBase : public CThing { public: };
class CThingPlayerCreature : public CThingCreatureBase { };
__forceinline bool IsAlive(const CThing* thing)
{
    if (thing != 0 && !(thing->m_flags91 & 1))
        return true;
    return false;
}
class CTCPerceiveThing { public:                       // interface 0x34
    CThing* GetPerceivedThing();                // 0x007446A0
};
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};
class CTCAIScratchpad { public:                        // interface 0xAA
    unsigned char _pad[0x38];
    long ScriptingStateGroup;                   // +0x38
    unsigned char _pad3c[0x48 - 0x3c];
    CCharString Action;                         // +0x48
    bool FaceHeroForAction;                     // +0x4c
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {    // vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(const CThing* item) { SetPItem(item); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {      // vtable 0x0123E7D8
    virtual void v00();
    __forceinline CIntelligentPointerCreature(CThingCreatureBase* item) { SetPItem(item); }
};
class CCreatureActionBase { public:
    virtual void v00();
};
class CCombatActionBase : public CCreatureActionBase { public:    // retail sizeof 0x118
    unsigned char _body[0x114];
    virtual ~CCombatActionBase();                       // 0x0062E900
};
class CCreatureAction_PlayCombatAnimation : public CCombatActionBase { public:
    CCreatureAction_PlayCombatAnimation(CThingCreatureBase* owner, CThingCreatureBase* target, const CCharString& animation,
        bool flag);                             // 0x008B7F80
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;      // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetPMainPlayer();                  // 0x00449960
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CPlayerManager* GetPPlayerManager_008fcf80();        // 0x008FCF80
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtThing(long repeat, CIntelligentPointerConstThing thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
    long AIAction_SpeakToTarget(long repeat, CIntelligentPointerCreature speaker, CIntelligentPointerConstThing target,
        unsigned long speech, bool interrupt, long priority, bool wait);   // 0x00980BD0
};
struct CAIStateGroup_ScriptingBase : CAIStateGroupBase {
    void Fired();                               // 0x009350C0
};
struct CAIStateGroup_PerformAction : CAIStateGroup_ScriptingBase {
    virtual void Update();
};

void CAIStateGroup_PerformAction::Update()
{
    CThingCreatureBase* hero = GetPPlayerManager_008fcf80()->GetPMainPlayer()->GetPControlledCreature();
    CThingCreatureBase* creature = GetCreature();
    CTCPerceiveThing* perceive;
    static_cast<const CThing*>(creature)->GetInterface(&perceive, 0x34);
    CTCAIScratchpad* scratchpad;
    creature->GetInterface(&scratchpad, 0xaa);
    if (IsAlive(perceive->GetPerceivedThing()) && scratchpad->FaceHeroForAction) {
        do {
            if (scratchpad->ScriptingStateGroup == 0) {
                Terminate();
                return;
            }
        } while (AIAction_LookAtThing(2, CIntelligentPointerConstThing(perceive->GetPerceivedThing()), 11, 0.013888889f) == 4
                && IsAlive(perceive->GetPerceivedThing()));
        Fired();
    }
    if (IsAlive(perceive->GetPerceivedThing()) && scratchpad->ScriptingStateGroup != 0) {
        for (;;) {
            long result;
            if (scratchpad->ScriptingStateGroup == 0) {
                Terminate();
                return;
            }
            switch (scratchpad->ScriptingStateGroup) {
            case 1: {
                bool face = scratchpad->FaceHeroForAction;
                CCharString action(scratchpad->Action);
                result = AIAction_PerformAction(&CCreatureAction_PlayCombatAnimation(GetCreature(), hero, action, face));
                break;
            }
            case 2:
                result = AIAction_SpeakToTarget(2, CIntelligentPointerCreature(GetCreature()),
                    CIntelligentPointerConstThing(hero), 0, true, 0x93, false);
                break;
            default:
                continue;
            }
            if (result == 4)
                continue;
            break;
        }
    }
    Fired();
    Terminate();
}