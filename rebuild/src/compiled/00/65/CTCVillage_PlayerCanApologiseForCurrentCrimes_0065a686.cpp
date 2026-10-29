// CTCVillage::PlayerCanApologiseForCurrentCrimes(long seconds) -- false while the last apology is under 450 frames old; otherwise true unless one of the hero's crimes against this village is of a non-apologisable severity or older than the given number of seconds
// Leaf: PlayerCanApologiseForCurrentCrimes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065A686, 291 bytes (`ret 4`, `leave`; Ego_r.pdb
// `?PlayerCanApologiseForCurrentCrimes@CTCVillage@@QAE_NJ@Z`).
// Callees: CTCBase::GetWorld() 0x00686A70 (evaluated for the static frame getter); GFGetCurrentFrame 0x0049D870 (x2,
//          `__fastcall`, the static CWorld frame getter of the landed callers); CWorld::GetCurrentPlayer() const 0x0049D9D0
//          (on g_pMainGameComponent 0x013B86A0 +0x24); CPlayer::GetPControlledCreature() 0x00487DC0; std::bitset<>::test(size_t)
//          const 0x00410DE0 (interface 0x51, the deed log); CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020;
//          CTCHeroOpinionDeedLog::GetDeedFromHandle(long) 0x006C58F0; the deed log's effects-def getter 0x006C5630
//          (CDefPointer by hidden pointer); the STLport tree increment Successor 0x004292C0 (`__cdecl`); g_framesPerSecond
//          0x01375550; the def's slot 1 on the last release.
// Layout: as the landed GetRegionLeavingDurationForCurrentCrimes 0x0065A5BB (whose model this follows): retail CTCVillage
//         PlayersCrimes +0x64, LastFrameApologised +0xd8 (PDB +0x164); COpinionDeed FrameHappened +0x14; retail
//         COpinionDeedEffectsDef CrimeSeverity +0x7c (PDB +0x84; bit 3 = apologisable).
// Shape: frame-pointer size TU (`push 51h ; pop edi` key, `and [ebp-8],0` construction flags); the refusal test is a named
//        bool over `!(def->CrimeSeverity & 8) || age > limit` -- the effects-def temporary gets the construction flag
//        bit and its conditional destructor, the bool its frame byte; the deed log is assigned only when present.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return it->second;
    }
};
class CThingPlayerCreature : public CThing { };

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CWorld { public:
    CPlayer* GetCurrentPlayer() const;          // 0x0049D9D0
    static long __fastcall GetFrame();          // 0x0049D870 (GFGetCurrentFrame)
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern long g_framesPerSecond;                  // 0x01375550

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
class COpinionDeedEffectsDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad_0x08[0x34];
    long KnowledgeLifeInSeconds;                // +0x3c
    unsigned char _pad_0x40[0x3c];
    long CrimeSeverity;                         // +0x7c (ECrimeSeverity)
    __forceinline void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
};
class COpinionDeed { public:
    long DeedType;                              // +0x00 EOpinionDeedType
    long DeedHandle;                            // +0x04
    float Position[3];                          // +0x08
    long FrameHappened;                         // +0x14
};
class CTCHeroOpinionDeedLog { public:
    COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType);   // 0x006C5630
};

struct SNode {                                  // STLport _Rb_tree_node<long>
    long   _M_color;
    SNode* _M_parent;
    SNode* _M_left;
    SNode* _M_right;
    long   _M_value_field;                      // +0x10
};
SNode* __cdecl Successor(SNode* node);          // 0x004292C0

class CTCBase
{
public:
    virtual void v00();
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCVillage : public CTCBase { public:
    unsigned char _pad_0x04[0x60];
    SNode*        PlayersCrimes;                // +0x64 set<long> header node
    unsigned char _pad_0x68[0x70];
    long          LastFrameApologised;          // +0xd8
    bool PlayerCanApologiseForCurrentCrimes(long seconds);
};

bool CTCVillage::PlayerCanApologiseForCurrentCrimes(long seconds)
{
    if (LastFrameApologised != 0 && GetWorld().GetFrame() - LastFrameApologised < 450)
        return false;
    CThing* hero = g_pMainGameComponent->World->GetCurrentPlayer()->GetPControlledCreature();
    if (hero) {
        CTCHeroOpinionDeedLog* log;
        if (hero->HasTC(0x51))
            log = (CTCHeroOpinionDeedLog*)hero->GetTC(0x51);
        long now = CWorld::GetFrame();
        SNode* end = PlayersCrimes;
        for (SNode* it = end->_M_left; it != end; it = Successor(it)) {
            COpinionDeed* deed = log->GetDeedFromHandle(it->_M_value_field);
            if (deed) {
                bool refused = !(log->GetDeedEffectsDef(deed->DeedType).Object->CrimeSeverity & 8)
                               || now - deed->FrameHappened > g_framesPerSecond * seconds;
                if (refused)
                    return false;
            }
        }
    }
    return true;
}