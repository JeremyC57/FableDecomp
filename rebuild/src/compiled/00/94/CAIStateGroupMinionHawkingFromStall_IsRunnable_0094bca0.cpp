// CAIStateGroup_MinionHawkingFromStall::IsRunnable() -- once the next hawk frame is reached, a silent shopkeeper in work hours with a shop hawks from the shop thing; with no shop thing (and a stall-less shop) it hawks on the spot unless the creature dislikes the player (attitude 3, 4 or 8)
// Retry 10b (getsw sweep, FIXES B-15): the opinion reaction manager is read through an inline getter `GetOpinionReactionManager()`; the plain member read gives the parked register/scheduling difference.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094BCA0, length 336 bytes (+ int3 padding).
// Callees: CAIStateGroupBase::GetPWorld @ 0x008FCFC0, CWorld::GetWorldFrame @ 0x0049D870 (static), CVectorMap::LowerBound
//          @ 0x0040F020 (inlined GetInterface 0x90 = CTCShopKeeper, bit 16 of +0x30 tested in memory), IsSpeaking
//          @ 0x008644E0 (__fastcall), CAIStateGroup_WorkBase::IsInWorkHours @ 0x0094BEF0, CTCShopKeeper::GetShopTC
//          @ 0x00798F50 / GetShopThing @ 0x00798D80, CIntelligentPointer<const CThing>(CThing*) @ 0x00668B60 and
//          operator= @ 0x00668B80 (both out of line), ~CBaseIntelligentPointer @ 0x00A01C10, GetPItem @ 0x00A01B50,
//          CAIStateGroupBase::GetPThingWorld @ 0x008FCFA0, COpinionReactionManager::GetThingAttitudeToPlayer @ 0x00701840.
// Shape: HasTC + entry pick (keeper in edi); each PStall assignment is an implicit conversion (the frame temporary's
//          address is re-taken for the out-of-line operator=); the result is
//          `PStall.GetPItem() != 0` (neg/sbb/neg).
// Layout: no engine header for this class; modelled locally like the landed OnReplace / IsStillValid rows (next hawk
//          frame +0x14, PStall +0x18); the shop's +0x50 flag is descriptive.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
bool __fastcall IsSpeaking(CThing* creature);   // 0x008644E0
class CTCShop { public:
    unsigned char _pad[0x50];
    bool OnTheSpot;                             // +0x50 (descriptive)
};
class CTCShopKeeper { public:                          // interface 0x90
    CTCShop* GetShopTC();                       // 0x00798F50
    CThing* GetShopThing();                     // 0x00798D80
};
class COpinionReactionManager { public:
    long GetThingAttitudeToPlayer(CThing* thing);   // 0x00701840
};
class CWorld { public:
    unsigned char _pad[0x74];
    COpinionReactionManager* POpinionReactionManager;   // +0x74
    COpinionReactionManager* GetOpinionReactionManager() const { return POpinionReactionManager; }
    static long GetWorldFrame();                // 0x0049D870
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;                            // +0x04
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
namespace N_00668b60 { struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    CIntelligentPointerConstThing(CThing* item);    // 0x00668B60
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other);   // 0x00668B80
}; } using N_00668b60::CIntelligentPointerConstThing;
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
protected:
    CWorld& GetWorld() const;    // 0x008FCFC0
public:
};

struct CAIStateGroup_MinionHawkingFromStall : CAIStateGroupBase {
    long NextHawkFrame;                         // +0x14 (descriptive)
    CIntelligentPointerConstThing PStall;       // +0x18
    bool IsInWorkHours();                       // 0x0094BEF0 (CAIStateGroup_WorkBase)
    bool IsRunnable();
};

bool CAIStateGroup_MinionHawkingFromStall::IsRunnable()
{
    if (GetWorld().GetWorldFrame() < NextHawkFrame)
        return false;
    CThing* creature = GetCreature();
    if (creature->HasTC(0x90)) {
        CTCShopKeeper* keeper = creature->GetTC<CTCShopKeeper>(0x90);
        if (!IsSpeaking(creature) && IsInWorkHours()) {
            CTCShop* shop = keeper->GetShopTC();
            if (shop) {
                PStall = keeper->GetShopThing();
                if (!PStall.GetPItem() && shop->OnTheSpot) {
                    PStall = GetCreature();
                    CThing* self = GetCreature();
                    long attitude = PeekWorld().GetOpinionReactionManager()->GetThingAttitudeToPlayer(self);
                    if (attitude == 3 || attitude == 4 || attitude == 8)
                        PStall = (CThing*)0;
                }
                return PStall.GetPItem() != 0;
            }
        }
    }
    return false;
}