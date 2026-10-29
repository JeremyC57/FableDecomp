// CAIStateGroup_MinionReactToCriminal::IsRunnable() -- refresh the hero and home pointers (an open, non-stall shopkeeper's home is its work building), stay out while the home holds a living thing with interface 0xAB, then defer to IsValid when the creature is aware of the hero or left the group less than ten seconds ago
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00931010, length 485 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPPlayerManager @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970,
// CPlayer::GetPControlledCreature @ 0x00487DC0, CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @
// 0x00A01B90 / dtor @ 0x00A01C10, CVectorMap::LowerBound @ 0x0040F020 (inlined lookup 0x90 = CTCShopKeeper),
// CTCShopKeeper::GetShopTC @ 0x00798F50 (three times), CIntelligentPointer<CThingBuilding>(CThingBuilding*) @ 0x0050F380
// (out of line the second time), IsAwareOfThingImplementation @ 0x00864700 (anonymous-namespace register call; retail
// passes the creature in ecx, the hero in edx and one pushed false), CWorld::GetWorldFrame @ 0x0049D870 (static),
// IsValid @ 0x00930EE0 (this class).
// Globals: g_framesPerSecond @ 0x01375550; 10.0f @ 0x01291DF0; vtables CIntelligentPointer<CThingPlayerCreature>
// 0x01231C4C, CIntelligentPointer<CThingBuilding> 0x01244344.
// Shape: each building read goes through a named creature local (`lea ecx,[eax+168h]` rather than `add ecx`); the
// awareness test names the creature before the call so it is loaded first (edi); fps is a named long local copied to
// the frame for the fild; the CThingBuilding pointer temporary is inlined once and called out of line the second time.
// Layout: Ego_r.pdb CAIStateGroup_MinionReactToCriminal (PHero +0x14, PHome +0x1c, FrameLastLeftStategroup +0x24);
// CThingAICreature HomeBuilding +0x168 / WorkBuilding +0x170 as in the landed 0x00831F80; CTCShop StockState +0x0c,
// StallLike +0x50 (12-byte retail containers). The engine header flattens the base, so the model is local.
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
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
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
struct CIntelligentPointerPlayer : CBaseIntelligentPointer {    // CIntelligentPointer<CThingPlayerCreature>, vtable 0x01231C4C
    virtual void v00();
    CIntelligentPointerPlayer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerPlayer& operator=(const CIntelligentPointerPlayer& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem(item); return *this; }
};
struct CIntelligentPointerBuilding : CBaseIntelligentPointer {  // CIntelligentPointer<CThingBuilding>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerBuilding(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }   // 0x0050F380 out of line
    CIntelligentPointerBuilding& operator=(const CIntelligentPointerBuilding& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem(item); return *this; }
};

class CThingAICreature : public CThing { public:
    unsigned char _pad94[0x168 - 0x94];
    CBaseIntelligentPointer HomeBuilding;       // +0x168
    CBaseIntelligentPointer WorkBuilding;       // +0x170
};
bool __fastcall IsAwareOfThingImplementation(const CThingAICreature* creature, const CThing* thing, bool flag);  // 0x00864700

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                     // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                   // 0x00449970
};
class CWorld { public:
    static long __fastcall GetFrame();     // 0x0049D870
};
extern long g_framesPerSecond;                  // 0x01375550

class CTCShop { public:
    unsigned char _pad0[0x0c];
    long StockState;                            // +0x0c ESTOCK_STATE
    unsigned char _pad10[0x40];
    bool StallLike;                             // +0x50
};
class CTCShopKeeper { public:                          // interface 0x90
    CTCShop* GetShopTC() const;                 // 0x00798F50
};

class CAIBrain { public:
    virtual void v00();
    unsigned char _pad[0x14];
    CThingAICreature* PCreature;                // +0x18
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

struct CAIStateGroup_MinionReactToCriminal : CAIStateGroupBase {
    CIntelligentPointerPlayer PHero;            // +0x14
    CIntelligentPointerBuilding PHome;          // +0x1c
    long FrameLastLeftStategroup;               // +0x24
    bool IsValid() const;                       // 0x00930EE0
    bool IsRunnable();
};

bool CAIStateGroup_MinionReactToCriminal::IsRunnable()
{
    PHero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    const CThingAICreature* me = PBrain->PCreature;
    PHome = ((CThing*)me->HomeBuilding.GetPItem());
    CThingAICreature* creature = PBrain->PCreature;
    if (creature->HasTC(0x90)) {
        CTCShopKeeper* keeper = creature->GetTC<CTCShopKeeper>(0x90);
        if (keeper->GetShopTC() != 0 && keeper->GetShopTC()->StockState != 1) {
            if (keeper->GetShopTC()->StallLike)
                return false;
            const CThingAICreature* worker = PBrain->PCreature;
            PHome = ((CThing*)worker->WorkBuilding.GetPItem());
        }
    }
    if (((CThing*)PHome.GetPItem()) != 0 && !(((CThing*)PHome.GetPItem())->m_flags91 & 1) && ((CThing*)PHome.GetPItem())->HasTC(0xab))
        return false;
    if (((CThing*)PHero.GetPItem()) != 0 && !(((CThing*)PHero.GetPItem())->m_flags91 & 1)) {
        me = PBrain->PCreature;
        if (IsAwareOfThingImplementation(me, ((CThing*)PHero.GetPItem()), false))
            return IsValid();
    }
    long fps = g_framesPerSecond;
    if ((float)(CWorld::GetFrame() - FrameLastLeftStategroup) < fps * 10.0f)
        return IsValid();
    return false;
}