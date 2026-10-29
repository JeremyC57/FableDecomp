// CAIStateGroup_MazeRangedCombat::IsRunnable() -- runnable while the boss is in a live phase it has not completed yet and its target (the targeting AI's enemy, else the hero) is alive between 8 and 50 units away in XY
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00954470, length 441 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined lookup 0xAD = CTCBoss) and the const LowerBound @ 0x004365B0
// (8 = CTCTargetingAI), CTCBoss::GetCurrentPhase @ 0x007AED10 (three times; manifest "CWorld::DrawGetEnvironment"
// propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
// CIntelligentPointer<CThing>(CThing*) @ 0x004A96F0 (twice), CIntelligentPointer<CThing>::operator= @ 0x004A9760 (out of
// line the first time, inlined the second), CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90,
// CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, CThing::GetPos @ 0x004C73D0 (twice).
// Globals: 50.0f @ 0x012944BC, 8.0f @ 0x012944C4.
// Shape: the boss lookup is the out-pointer GetInterface (its miss path passes the never-written word); operator= is a
// plain in-class inline, which VC calls out of line for the first assignment and inlines for the second; the creature
// and target are named before the two GetPos calls.
// Layout: Ego_r.pdb CAIStateGroup_MazeRangedCombat (LastCompletedPhase +0x20); the target pointer +0x14 is the inherited
// ranged-combat member (local model, the engine header flattens the base); CTCTargetingAI's enemy pointer sits at the
// unaligned +0x1d (pack(1), as the landed rows read it).
#include <math.h>

class C3DVector { public: float X, Y, Z; };
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
    }
};
class CThingPlayerCreature : public CThing { };

class CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointee link
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(const CThing* item);          // 0x00A01B90
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
template <class T> class CIntelligentPointer;
template <> class CIntelligentPointer<CThing> : public CBaseIntelligentPointer { public:   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* thing);         // 0x004A96F0
    CIntelligentPointer& operator=(const CIntelligentPointer& other)   // 0x004A9760
    {
        SetPItem(((CThing*)other.GetPItem()));
        return *this;
    }
};

#pragma pack(push, 1)
class CTCTargetingAI { public:                         // interface 8
    unsigned char _pad0[0x1d];
    CBaseIntelligentPointer PEnemy;             // +0x1d
};
#pragma pack(pop)
class CTCBoss { public:                                // interface 0xAD
    long GetCurrentPhase();                     // 0x007AED10
};

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                   // 0x00449970
};

class CAIBrain { public:
    virtual void v00();
    unsigned char _pad[0x14];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated, Priority, Enabled, _pad13;
    protected:
    CPlayerManager& GetPlayerManager() const;        // 0x008FCF90
    public:
};

struct CAIStateGroup_MazeRangedCombat : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTarget;                // +0x14
    long _unk1c;                                // +0x1c
    long LastCompletedPhase;                    // +0x20
    bool IsRunnable();
};

bool CAIStateGroup_MazeRangedCombat::IsRunnable()
{
    CTCBoss* boss;
    PBrain->PCreature->GetInterface(&boss, 0xad);
    if (boss->GetCurrentPhase() != 0 && boss->GetCurrentPhase() != 5 && LastCompletedPhase < boss->GetCurrentPhase()) {
        const CThing* creature = PBrain->PCreature;
        if (creature->HasTC(8)) {
            CTCTargetingAI* targeting = creature->GetTC<CTCTargetingAI>(8);
            if (((CThing*)targeting->PEnemy.GetPItem()) != 0)
                PTarget = ((CThing*)targeting->PEnemy.GetPItem());
        }
        if (((CThing*)PTarget.GetPItem()) == 0 || (((CThing*)PTarget.GetPItem())->m_flags91 & 1))
            PTarget = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (((CThing*)PTarget.GetPItem()) != 0 && !(((CThing*)PTarget.GetPItem())->m_flags91 & 1)) {
            const CThing* self = PBrain->PCreature;
            const CThing* target = ((CThing*)PTarget.GetPItem());
            const C3DVector& me = self->GetPos();
            const C3DVector& him = target->GetPos();
            float dx = him.X - me.X;
            float dy = him.Y - me.Y;
            float dist = (float)sqrt(dx * dx + dy * dy);
            if (dist < 50.0f && dist > 8.0f)
                return true;
        }
    }
    return false;
}