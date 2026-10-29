// CAIStateGroup_BanditKingThrowWeapons::IsRunnable() -- with a live hero, a boss component (0xAD) that is not on its final phase but is ready for a phase change, and the hero outside the throw radius (squared XY distance > 25.0f @ 0x0123003C): target the hero
// Leaf: IsRunnable@CAIStateGroup_BanditKingThrowWeapons
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00961550, length 240 bytes.
// Callees: CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroThing
// @ 0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (type 0xAD = word 5 bit 13),
// CTCBoss::IsOnFinalPhase @ 0x007AEDE0, CTCBoss::IsReadyForPhaseChange @ 0x007AEE90, CThing::GetPos @ 0x004C73D0,
// CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer(CThingCreatureBase*) @ 0x0048BD00 and ::operator= @ 0x0048BD20
// (both out of line in this TU), CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: the boss component kept in esi (named local from the lookup); `d = dx*dx; d += dy*dy;` gives retail's X-term-first x87
// order (a single sum puts the Y term first); the pointer is a named local (a temporary reuses the ctor's eax); `> R` gives `test ah,41h ; jne`.
class C3DVector { public: float X, Y, Z; };

class CThing { public:
    unsigned char _pad[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    const C3DVector& GetPos() const;           // 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is propagated)
};
class CThingPlayerCreature : public CThing { };

class CPlayer
{ public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};

class CPlayerManager
{ public:
    CPlayer& GetMainPlayer();                       // 0x00449970
};

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};


class CAIBrain
{ public:
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};


struct CIntelligentPointerCreature : CBaseIntelligentPointer   // CIntelligentPointer<CThingCreatureBase>
{
    CIntelligentPointerCreature(CThing* item);                  // 0x0048BD00
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other);   // 0x0048BD20
};

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);           // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CThingWithTCs
{
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CInterfaceMap::SEntry* FindEntry(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        CInterfaceMap::SEntry* sel;
        if (it == end)
            sel = end;
        else if (it->first <= type)
            sel = it;
        else
            sel = end;
        return sel;
    }
};

class CTCBoss
{ public:
    bool IsOnFinalPhase();                          // 0x007AEDE0
    bool IsReadyForPhaseChange();                   // 0x007AEE90
};

struct CAIStateGroup_BanditKingThrowWeapons
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CIntelligentPointerCreature PTargetThing;       // +0x14
    CPlayerManager* GetPPlayerManager();            // 0x008FCF90
    virtual bool IsRunnable();
};

bool CAIStateGroup_BanditKingThrowWeapons::IsRunnable()
{
    CThing* hero = GetPPlayerManager()->GetMainPlayer().GetPControlledCreature();
    if (hero && !(hero->m_flags91 & 1))
    {
        CThingWithTCs* creature = (CThingWithTCs*)PBrain->Creature;
        if (creature->HasTC(0xad))
        {
            CTCBoss* boss = (CTCBoss*)creature->FindEntry(0xad)->second;
            if (!boss->IsOnFinalPhase() && boss->IsReadyForPhaseChange())
            {
                const C3DVector& from = PBrain->Creature->GetPos();
                const C3DVector& to = hero->GetPos();
                float dx = to.X - from.X;
                float dy = to.Y - from.Y;
                float dist2 = dx * dx;
                dist2 += dy * dy;
                if (dist2 > 25.0f)
                {
                    CIntelligentPointerCreature target(hero);
                    PTargetThing = target;
                    return true;
                }
            }
        }
    }
    return false;
}