// Grade: DIFFER(734v724) -- the first 0x4F lookup's miss reads the frame slot (retail jumps to the store with eax still the creature: the out variable has no frame home); argument/temporary register picks (ecx/edx vs eax) at the GetSomeoneElse push, AddMessage pushes and the perceived position copy; the 16-byte loop-head pad before the FindAGuard retry follows from the size gap
// CAIStateGroup_MinionReactToCrime::Update() -- run for help after a crime: hold the villager's home open (ForceOpenBuilding, bumping its force-open count); unless duplicates are allowed, give up when someone else is already getting help; with a perceived criminal, post the GOING_FOR_HELP message event from the villager, flag the opinion component as yelling for help, drop out of the hero's AI followers when the criminal is the hero, then find a guard and yell (retrying while it runs) and mark the guard told on success; terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00958840, length 724 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10 (the villager's
// PHome +0x168; a CIntelligentPointer<CThingBuilding> temporary, vtable 0x01244344), GetSomeoneElseGettingHelpMessage @
// 0x00958770, CVectorMap::LowerBound const @ 0x004365B0 (0x34 CTCPerceiveThing) / non-const @ 0x0040F020 (0x4F
// CTCCreatureOpinionOfHero twice, 0xBC CTCFollowed on the hero), CTCPerceiveThing::GetPerceivedThing @ 0x007446A0,
// CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, GetPDefinitionManager @ 0x008FCFB0,
// CDefinitionManager::GetDef<CMessageEventDef> @ 0x007923C0, GetPWorld @ 0x008FCFC0, CThing::GetPosition @ 0x004C73D0,
// CMessageEventManager::AddMessage @ 0x00687730, GetPPlayerManager @ 0x008FCF90 / GetMainPlayer @ 0x00449970 /
// GetPCharacter @ 0x00487DC0, CTCFollowed::IsAIFollower @ 0x006D4DB0 / QuitAIFollower @ 0x006D56E0,
// AIAction_FindAGuardAndYellForHelp @ 0x0097FEA0, Terminate @ 0x008FCF40.
// Data: "STANDARD_MESSAGE_EVENT_GOING_FOR_HELP" @ 0x01294E74.
// Layout: engine/CAIStateGroup_MinionReactToCrime.h (OpinionDeedHandle +0x14, GuardTold +0x18, AllowDuplicateReactToCrime
// +0x19, ForceOpenBuilding +0x1b unaligned); PBrain is the first dword at +0x04. Retail CThingBuilding ForceOpenCount +0xd0
// (PDB +0xe8) and CTCCreatureOpinionOfHero InFindAGuardAndYellForHelp +0x343 (PDB +0x64f) from the retail accesses.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU stand-ins for the two CVectorMap::LowerBound copies (STLport lower_bound over the 8-byte entries).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct C3DVector {
    float x, y, z;
    C3DVector(const C3DVector& other) : x(other.x), y(other.y), z(other.z) { }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
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
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* component;
        unsigned char present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            component = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return component;
    }
    template<class T> __forceinline bool GetInterfaceChecked(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
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
struct CThingBuilding : CThing {
    unsigned char _pad94[0xd0 - 0x94];
    long ForceOpenCount;                                // +0xd0
    void AddForceOpen() { ++ForceOpenCount; }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +4
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointerBuilding : CBaseIntelligentPointer {  // vtable 0x01244344
    virtual void v00();
    __forceinline CIntelligentPointerBuilding(CThing* item) { SetPItem(item); }
    __forceinline CIntelligentPointerBuilding& operator=(const CIntelligentPointerBuilding& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CThingCreatureBase : CThing {
    unsigned char _pad94[0x168 - 0x94];
    CBaseIntelligentPointer PHome;                      // +0x168 CIntelligentPointer<CThingBuilding>
};
struct CTCPerceiveThing {                               // interface 0x34
    CThing* GetPerceivedThing();                        // 0x007446A0
};
struct CTCCreatureOpinionOfHero {                       // interface 0x4f
    unsigned char _pad[0x343];
    bool InFindAGuardAndYellForHelp;                    // +0x343
};
struct CTCFollowed {                                    // interface 0xbc
    bool IsAIFollower(const CThing* thing) const;       // 0x006D4DB0
    void QuitAIFollower(CThing* thing);                 // 0x006D56E0
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CMessageEventDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long m_count;                                       // +4
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointer {                                    // CDefPointer<CMessageEventDef>
    CMessageEventDef* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) { if (DecRefToZero(m_p->m_count)) m_p->Destroy(); } }
};
struct CDefinitionManager {
    bool GetDef(const CCharString& name, CDefPointer& def);   // 0x007923C0
};
CMessageEventDef* FindDefStandIn(const CDefinitionManager* manager, const void* name);
#pragma auto_inline(off)
// Same-TU stand-in for CDefinitionManager::GetDef<CMessageEventDef> 0x007923C0: it fills only the def pointer, so VC7.1
// knows the def's address does not escape (retail's LTCG view) and keeps the released pointer in ecx.
bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer& def)
{
    CMessageEventDef* found = FindDefStandIn(this, name.m_pData);
    def.m_p = found;
    return found != 0;
}
#pragma auto_inline(on)
struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* AddMessage(CMessageEventDef* def, const C3DVector* position, CThing* thing1, CThing* thing2, long priority);   // 0x00687730
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageEventManager;       // +0x60
};
struct CPlayer {
    CThing* GetPCharacter();                            // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;                      // +0x18
};

#include "engine/CAIStateGroup_MinionReactToCrime.h"

struct CAIStateGroupMinionReactToCrimeMethods : CAIStateGroup_MinionReactToCrime {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThingCreatureBase* GetCreature() const { return Brain()->PCreature; }
    CIntelligentPointerBuilding& PForceOpenBuilding() { return *(CIntelligentPointerBuilding*)ForceOpenBuilding; }
    void Terminate();                                   // 0x008FCF40
    CPlayerManager* GetPPlayerManager();                // 0x008FCF90
    CDefinitionManager* GetPDefinitionManager();        // 0x008FCFB0
    CWorld* GetPWorld();                                // 0x008FCFC0
    long AIAction_FindAGuardAndYellForHelp(C3DVector position, long deedHandle, bool flag);   // 0x0097FEA0
    CMessageEvent* GetSomeoneElseGettingHelpMessage(long handle);   // 0x00958770
    void Update();
};

void CAIStateGroupMinionReactToCrimeMethods::Update()
{
    PForceOpenBuilding() = CIntelligentPointerBuilding(GetCreature()->PHome.GetPItem());
    if (PForceOpenBuilding().GetPItem() && !(PForceOpenBuilding().GetPItem()->m_flags91 & 1))
        ((CThingBuilding*)PForceOpenBuilding().GetPItem())->AddForceOpen();
    if (AllowDuplicateReactToCrime || !GetSomeoneElseGettingHelpMessage(OpinionDeedHandle)) {
        CTCPerceiveThing* perceive;
        if (static_cast<const CThing*>(GetCreature())->GetInterface(&perceive, 0x34) && perceive->GetPerceivedThing()) {
            CDefPointer def;
            bool found;
            {
                CCharString name("STANDARD_MESSAGE_EVENT_GOING_FOR_HELP", -1);
                found = GetPDefinitionManager()->GetDef(name, def);
            }
            if (found) {
                CThing* creature = GetCreature();
                CMessageEventManager* manager = GetPWorld()->m_pMessageEventManager;
                manager->AddMessage(def.m_p, &creature->GetPosition(), creature, 0, OpinionDeedHandle);
            }
            C3DVector position = perceive->GetPerceivedThing()->GetPosition();
            GetCreature()->GetTC<CTCCreatureOpinionOfHero>(0x4f)->InFindAGuardAndYellForHelp = true;
            CThing* hero = GetPPlayerManager()->GetMainPlayer()->GetPCharacter();
            if (hero && !(hero->m_flags91 & 1) && perceive->GetPerceivedThing() == hero) {
                CTCFollowed* followed;
                if (hero->GetInterface(&followed, 0xbc) && followed->IsAIFollower(GetCreature()))
                    followed->QuitAIFollower(GetCreature());
            }
            long result;
            for (;;) {
                result = AIAction_FindAGuardAndYellForHelp(position, OpinionDeedHandle, true);
                if (result != 4)
                    break;
            }
            CTCCreatureOpinionOfHero* opinionAfter;
            if (GetCreature()->GetInterfaceChecked(&opinionAfter, 0x4f))
                opinionAfter->InFindAGuardAndYellForHelp = false;
            if (result == 2)
                GuardTold = true;
        }
    }
    Terminate();
}
