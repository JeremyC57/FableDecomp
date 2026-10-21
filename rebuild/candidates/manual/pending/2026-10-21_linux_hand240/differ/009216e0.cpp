// CAIStateGroup_MinionFollowThing::IsRunnable() -- runnable when the live hero has been shown the follow expression (or already uses it / already leads this creature as an AI follower) and accepts the creature as a follower; otherwise when the creature's follow state (6 or 7) names a thing that exists
// Grade: DIFFER(ours v retail) -- the null-preset followed component is promoted to a register (xor edi,edi) where retail keeps it in the shared key slot (zero store at entry, miss path reloads it); this esi vs edi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009216E0, length 400 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970,
//          CPlayer::GetPCharacter @ 0x00487DC0, CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface 0xBC =
//          CTCFollowed on the hero, bit 28 of +0x34, preset to null) / non-const @ 0x0040F020 (0xAA, bit 10 of +0x34),
//          CCharString(const char*, long) @ 0x0099EBF0 ("EXPRESSION_FOLLOW" @ 0x012757EC) / copy ctor @ 0x0099EC30 /
//          ~CCharString @ 0x0099EAE0, IsExpressionPerfomedTo @ 0x00921440, CTCFollowed::IsAIFollower @ 0x006D4D70 /
//          IsCreatureValidFollower @ 0x006D4E60, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
//          CThingSearchTools::GetPFirstThing<CThingFilter_HasScriptName, CThing> @ 0x0056D050.
// Shape: the followed component is preset to null in the key's home (read on the lookup miss path); the expression test
//          is an `||` chain materialised into bl inside the name's block; the fallback copies the state's name and
//          builds the filter from the copy, the search result in bl across both destructors.
// Layout: PDB names from engine/CAIStateGroup_MinionFollowThing.h (flattened base: modelled locally like the landed
//          Cleanup 0x00921870); the follow state component's +0x38 state and +0x50 CCharString name are descriptive.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CTCFollowed {                            // interface 0xBC
    bool IsAIFollower(CThing* creature);        // 0x006D4D70
    bool IsCreatureValidFollower(CThing* creature, bool* addToFront);   // 0x006D4E60
};
struct CTCFollowerState {                       // interface 0xAA
    unsigned char _pad[0x38];
    long State;                                 // +0x38 (descriptive)
    unsigned char _pad3c[0x50 - 0x3c];
    CCharString FolloweeName;                   // +0x50 (descriptive)
};
struct CThingFilter_HasScriptName {
    CCharString Name;
    CThingFilter_HasScriptName(const CCharString& name) : Name(name) { }
};
struct CThingSearchTools {
    bool GetPFirstThing(const CThingFilter_HasScriptName& filter, CThing** ppThing);   // 0x0056D050
};
struct CPlayer {
    CThing* GetPCharacter();                    // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};

struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
};
struct CAIStateGroup_MinionFollowThing : CAIStateGroupBase {
    CBaseIntelligentPointer PFollowee;          // +0x14 CIntelligentPointer<CThingCreatureBase>
    float FollowDistance;                       // +0x1c
    bool AvoidDynamicObstacles;                 // +0x20
    bool PreviousPushableByHero;                // +0x21
    bool ExpressionUsed;                        // +0x22
    bool FollowerAdded;                         // +0x23
    long CurrentMovementType;                   // +0x24
    bool IsExpressionPerfomedTo(const CCharString& expression);   // 0x00921440
    bool IsRunnable();
};

bool CAIStateGroup_MinionFollowThing::IsRunnable()
{
    CTCFollowed* followed = 0;
    CurrentMovementType = 3;
    CThing* hero = GetPPlayerManager()->GetMainPlayer()->GetPCharacter();
    if (hero && !(hero->m_flags91 & 1)) {
        const CThing* self = hero;
        if (self->HasTC(0xbc))
            followed = self->GetTC<CTCFollowed>(0xbc);
        bool ok;
        {
            CCharString expression("EXPRESSION_FOLLOW");
            ok = IsExpressionPerfomedTo(expression) || ExpressionUsed || followed->IsAIFollower(GetCreature());
        }
        if (ok && followed->IsCreatureValidFollower(GetCreature(), 0)) {
            ExpressionUsed = true;
            return true;
        }
    }
    CTCFollowerState* state;
    GetCreature()->GetInterface(&state, 0xaa);
    if (state->State == 6 || state->State == 7) {
        bool found;
        {
            CCharString name(state->FolloweeName);
            CThing* thing;
            found = PeekThingSearchTools()->GetPFirstThing(CThingFilter_HasScriptName(name), &thing);
        }
        if (found)
            return true;
    }
    return false;
}
