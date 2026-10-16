// CTCWife::IsCourtableAndNotBlockingIt() const -- true when courting is not blocked, the wife's attitude allows it, and the living hero is free
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CF810, 124 bytes (+4 int3). __fastcall, returns bool in al.
// Callees: CTCBase::GetPlayerManager() const 0x00686D50 (catalog "CViewBase::GetDisplayEngine" twin of 0x00686D40);
//          CPlayerManager::GetMainPlayer() const 0x00449960; CPlayer::GetHeroThing() 0x00487DC0;
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0.
// Shape: early-out tests on CourtingBlocked (+0x15), OpinionAttitude (+0x10) == 4 and bit 3 of the parent thing's +0x1C9
//        byte; the hero (not dead) must carry interface 4 (bitset byte +0x20 bit 4) whose pointer at +0x114 is null.
// Layout: CTCWife (engine header): ParentThing +0x04, OpinionAttitude +0x10, CourtingBlocked +0x15; retail CThing bitset
//         +0x20, interface map +0x44, Dead +0x91.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0 (the other TU copy is 0x0040F020)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    unsigned char _pad92[0x1c9 - 0x92];
    unsigned char Flags1C9;                     // +0x1c9 (CThingCreatureBase flag byte; bit 3 tested, descriptive)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CPlayer {
    CThing* GetHeroThing() const;               // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;             // 0x00449960 (const)
    CPlayer* GetMainPlayer();                   // 0x00449970
};
struct CWorld {
    static long GetWorldFrame();                // 0x0049D870
};
struct CHeroInterface4 {                         // interface 4 = CTCHeroStats (CTCWife::Died calls its SetMarriageState 0x0057BC6E)
    unsigned char _pad_0x00[0x114];
    void* BlockingCourtship;                     // +0x114 (descriptive)
};
struct CTCWife {
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[8];
    long OpinionAttitude;                        // +0x10 EOpinionAttitudeType
    unsigned char AttitudeUpdateCounter;         // +0x14
    bool CourtingBlocked;                        // +0x15
    const CPlayerManager* GetPlayerManager() const;   // 0x00686D50
    bool IsCourtableAndNotBlockingIt() const;
};
bool CTCWife::IsCourtableAndNotBlockingIt() const
{
    if (!CourtingBlocked && OpinionAttitude != 4 && (ParentThing->Flags1C9 & 8)) {
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
        if (hero && !(hero->Flags91 & 1)) {
            CHeroInterface4* component;
            if (hero->GetInterface(&component, 4)) {
                if (!component->BlockingCourtship)
                    return true;
            }
        }
    }
    return false;
}