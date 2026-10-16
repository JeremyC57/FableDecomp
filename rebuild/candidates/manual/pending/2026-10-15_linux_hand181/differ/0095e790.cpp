// CAIStateGroup_BoastingCrowdMember::IsRunnable() -- the live hero runs a boasting crowd (interface 0x73) whose current group is active (state not 0 or 10) and accepts this creature: target the group's boaster
// Grade: DIFFER(209v209) -- register swap only: hero in edi and the interface map in esi (retail hero esi, map edi pushed late); the frame order is right with the group out-pointer declared first
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095E790, length 209 bytes (+ int3 padding).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 on the singleton 0x013B86A0's +0x1c, CPlayer::GetHeroThing @ 0x00487DC0,
// CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (type 0x73 = word 3 bit 19),
// CTCBoastingCrowdControl::GetCrowdGroup(CThing*, CCrowdGroup**) @ 0x00783730, the group's virtual slot 28 (+0x70),
// CIntelligentPointer<..>::CIntelligentPointer(CThing*) @ 0x004A96F0 and ::operator= @ 0x004A9760 (out of line in this TU),
// CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: out-parameter GetInterface (component in eax), the group out-pointer is the first frame word; the pointer
// assignment goes through a named local (retail re-forms its address with `lea`).
struct CThing;
struct CTCBase;
struct CInterfaceMap                                // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
{
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

struct CThing
{
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad_0x4c[0x91 - 0x4c];
    unsigned char m_flags91;                        // +0x91 bit 0: dead

    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
        {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CPlayer { CThing* GetHeroThing(); };                     // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };            // 0x00449970
struct CEngine
{
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                  // +0x1c
};
extern CEngine* g_pEngine;                          // 0x013B86A0

struct CAIBrain
{
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    ~CBaseIntelligentPointer();                     // 0x00A01C10
};

struct CIntelligentPointerThing : CBaseIntelligentPointer
{
    CIntelligentPointerThing(CThing* item);                         // 0x004A96F0
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other);   // 0x004A9760
};

struct CCrowdGroup
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27();
    virtual bool AcceptsMember(CThing* creature);   // slot 28 (+0x70)
    CThing* Boaster;                                // +0x04
    unsigned char _pad08[0x2c - 0x08];
    long State;                                     // +0x2c
};

struct CTCBoastingCrowdControl                      // interface type 0x73
{
    bool GetCrowdGroup(CThing* hero, CCrowdGroup** group);   // 0x00783730
};

struct CAIStateGroup_BoastingCrowdMember
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CIntelligentPointerThing PTargetThing;          // +0x14
    bool IsRunnable();
};

bool CAIStateGroup_BoastingCrowdMember::IsRunnable()
{
    CThing* hero = g_pEngine->PlayerManager->GetMainPlayer()->GetHeroThing();
    CCrowdGroup* group;
    CTCBoastingCrowdControl* crowd;
    if (hero && !(hero->m_flags91 & 1) && hero->GetInterface(0x73, &crowd))
    {
        if (crowd->GetCrowdGroup(hero, &group) && group->State != 0 && group->State != 10
            && group->AcceptsMember(PBrain->Creature))
        {
            CIntelligentPointerThing boaster(group->Boaster);
            PTargetThing = boaster;
            return true;
        }
    }
    return false;
}
