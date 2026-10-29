// CTCWife::IsCourtableAndNotBlockingIt() const -- true when courting is not blocked, the wife's attitude allows it, and the living hero is free
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CF810, 124 bytes (+4 int3). __fastcall, returns bool in al.
// Callees: CTCBase::PeekPlayerManager() const 0x00686D50 (catalog "CViewBase::GetDisplayEngine" twin of 0x00686D40);
//          CPlayerManager::GetMainPlayer() const 0x00449960; CPlayer::GetPControlledCreature() 0x00487DC0;
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0.
// Shape: early-out tests on CourtingBlocked (+0x15), OpinionAttitude (+0x10) == 4 and bit 3 of the parent thing's +0x1C9
//        byte; the hero (not dead) must carry interface 4 (bitset byte +0x20 bit 4) whose pointer at +0x114 is null.
// Layout: CTCWife (engine header): ParentThing +0x04, OpinionAttitude +0x10, CourtingBlocked +0x15; retail CThing bitset
//         +0x20, interface map +0x44, Dead +0x91.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0 (the other TU copy is 0x0040F020)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
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
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;               // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;             // 0x00449960 (const)
    CPlayer* GetMainPlayer();                   // 0x00449970
};
class CWorld { public:
    static long GetWorldFrame();                // 0x0049D870
};
struct CHeroInterface4 {                         // interface 4 = CTCHeroStats (CTCWife::Died calls its SetMarriageState 0x0057BC6E)
    unsigned char _pad_0x00[0x114];
    void* BlockingCourtship;                     // +0x114 (descriptive)
};
class CTCBase
{
public:
    virtual void v00();
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};

class CTCWife : public CTCBase { public:
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[8];
    long OpinionAttitude;                        // +0x10 EOpinionAttitudeType
    unsigned char AttitudeUpdateCounter;         // +0x14
    bool CourtingBlocked;                        // +0x15
    bool IsCourtableAndNotBlockingIt() const;
};
bool CTCWife::IsCourtableAndNotBlockingIt() const
{
    if (!CourtingBlocked && OpinionAttitude != 4 && (ParentThing->Flags1C9 & 8)) {
        CThing* hero = PeekPlayerManager().GetMainPlayer()->GetPControlledCreature();
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