// CAIStateGroup_GoToPub::ThankBarmaid() -- say the creature's thanking phrase (waiting while the speech is busy), look at the barmaid who brought the tankard, play the acknowledge animation and clear the tankard's barmaid; false on any failure or when the group is stopping
// Grade: DIFFER(497v498) -- register permutation only: retail keeps the creature in esi (talk in edi, tankard in ebp, edi pushed after the creature load), ours keeps the creature in edi and talk in ebp; the unnamed-map GetInterface puts the creature in esi but drops retail's lea/add of the map register (494v498)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00949380, length 498 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: CTCTalk 0x0f = bit 15 of +0x20, CTCAIScratchpad 0xaa = bit
// 10 of +0x34, CTCTavernTankard 0xba = bit 26 of +0x34), CTCTalk::PlayAISpeech(unsigned long, bool, EDialogueLayer, bool, bool)
// @ 0x006E3B60, AIAction_DoNothing @ 0x0097E200, CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem
// @ 0x00A01B90 (CIntelligentPointer<const CThing> vtable 0x01237F1C), AIAction_LookAtThing @ 0x0097D6F0 (repeat 2, 0x16 frames,
// speed 0.0027777f), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("ACKNOWLEDGE_BARMAID" @ 0x0129304C),
// CCreatureAction_PlayAnimation ctor @ 0x008424F0, AIAction_PerformActionAndWait @ 0x0097DB70, the action base dtor @ 0x00693EF0,
// CTCTavernTankard::SetPBarmaid @ 0x0094B400 (manifest "CVillageTaskCarryCrate::AssignCreatureToTask" is one of 17 propagated
// byte-identical bodies), CAIStateGroupBase::IsStopping @ 0x008FD010.
// Layout: GoToPub from Ego_r.pdb (PTankard +0x34); CTCAIScratchpad ThankingPhrase +0x0c and CTCTavernTankard PBarmaid +0x34
// from Ego_r.pdb.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char HasTC(long interfaceType) const { return (unsigned char)((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const unsigned char& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return present != 0;
    }
};
struct CThingCreatureBase;
struct CThingAICreature;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    __forceinline CIntelligentPointer(T* item) { SetPItem(item); }
};

#define IS_THING_ALIVE(p) ((p) != 0 && !((p)->m_flags91 & 1))

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCreatureActionBase {                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
};
struct CCreatureAction_PlayAnimation : CCreatureActionBase {
    unsigned char _anim[0xb4 - 0xa8];
    CCreatureAction_PlayAnimation(CThingCreatureBase* owner, const CCharString& animationName, const CSoundPair& sound,
                                  bool useAnimationMovement, bool usePhysics, bool cameraUpdate, long c, long d);   // 0x008424F0
    ~CCreatureAction_PlayAnimation();           // 0x00693EF0 (the base dtor)
};

struct CTCTalk {                                // interface 0x0f
    long PlayAISpeech(unsigned long phrase, bool force, long layer, bool b1, bool b2);   // 0x006E3B60
};
struct CTCAIScratchpad {                        // interface 0xaa
    unsigned char _pad[0xc];
    unsigned long ThankingPhrase;               // +0x0c
};
struct CTCTavernTankard {                       // interface 0xba
    unsigned char _pad[0x34];
    CBaseIntelligentPointer PBarmaid;           // +0x34 CIntelligentPointer<CThingAICreature>
    void SetPBarmaid(CThingAICreature* barmaid);   // 0x0094B400
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
    CThingCreatureBase* GetPCreature() const { return (CThingCreatureBase*)PCreature; }
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    bool IsStopping();                          // 0x008FD010
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CBaseIntelligentPointer PPub;               // +0x14
    CBaseIntelligentPointer PBar;               // +0x1c
    CBaseIntelligentPointer PTable;             // +0x24
    CBaseIntelligentPointer PBarman;            // +0x2c
    CBaseIntelligentPointer PTankard;           // +0x34 CIntelligentPointer<CThing>
    bool ThankBarmaid();
};

bool CAIStateGroup_GoToPub::ThankBarmaid()
{
    CThing* creature = PBrain->PCreature;
    CTCTalk* talk;
    creature->GetInterface(&talk, 0x0f);
    CTCAIScratchpad* scratchpad;
    creature->GetInterface(&scratchpad, 0xaa);
    for (;;) {
        long result = talk->PlayAISpeech(scratchpad->ThankingPhrase, false, 2, false, true);
        if (result == 2) {
            if (AIAction_DoNothing() == 4)
                continue;
            return false;
        }
        if (result != 0)
            return false;
        break;
    }
    if (!IS_THING_ALIVE(PTankard.GetPItem()))
        return false;
    CTCTavernTankard* tankard;
    PTankard.GetPItem()->GetInterface(&tankard, 0xba);
    CThing* barmaid = tankard->PBarmaid.GetPItem();
    if (AIAction_LookAtThing(2, CIntelligentPointer<const CThing>(barmaid), 0x16, 0.0027777f) == 1)
        return false;
    bool failed;
    {
        CCharString anim("ACKNOWLEDGE_BARMAID");
        failed = AIAction_PerformActionAndWait(&CCreatureAction_PlayAnimation(PBrain->GetPCreature(), anim, CSoundPair(),
                                                                             false, true, false, 0, 0), false) == 1;
    }
    if (failed)
        return false;
    tankard->SetPBarmaid(0);
    return !IsStopping();
}
