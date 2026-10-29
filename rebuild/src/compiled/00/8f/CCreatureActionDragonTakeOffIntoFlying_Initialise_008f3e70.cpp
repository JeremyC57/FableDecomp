// CCreatureAction_DragonTakeOffIntoFlying::Initialise() -- names the "TAKE_OFF" animation, runs the base Initialise, makes the dragon unliftable, clears the interface-9 byte, makes the dragon not solid to the hero (adding a CTCSolidToThing to the hero when missing) and records the take-off destination 35.148 units above the dragon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F3E70, length 338 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetCombatAnimation @ 0x008574D0,
// ~CCharString @ 0x0099EAE0, CCombatActionBase::Initialise @ 0x00857520, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// SetThingIsAbleToBeLifted(CThing*, bool) @ 0x007E4330 (`__fastcall`: ecx thing, dl flag), CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface for types 9 and 0x4D), CPlayerManager::GetMainPlayer @ 0x00449970 (g_pMainGameComponent +0x1c),
// CPlayer::GetPControlledCreature @ 0x00487DC0, CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60,
// CTCSolidToThing::AddThingNotSolidTo(CThing*) @ 0x00770FA0, CThing::GetPos @ 0x004C73D0.
// Shape: the interface-9 lookup is untested (the absent arm reads the dead out word `mov eax,[esp+0Ch]`); the hero's
// CTCSolidToThing is fetched with HasTC/GetTC else added by name (string temporary dies before the null test); the
// destination is a C3DVector temporary (X, Y, Z + 35.148f) built in the frame and copied memberwise. Retail
// CCombatActionBase is 0x114 bytes (the PDB header places Dest at +0x12c), so `this` is modelled locally: Dest at +0x120.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCParamsBase;
class CCharString;
class CTCSolidToThing;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    const C3DVector& GetPos() const;   // 0x004C73D0
    CTCSolidToThing* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (T*)it->second;
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
class CThingPlayerCreature : public CThing { };
class CCharString { public:                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
class CTCSolidToThing { public:
    void AddThingNotSolidTo(CThing* thing); // 0x00770FA0
};
struct CTCCreatureFlags9 {                  // the interface-9 component: one byte at +0x10 cleared by this action
    unsigned char _pad[0x10];
    unsigned char m_canBeTargeted;          // +0x10
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;              // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();               // 0x00449970
};
class CMainGameComponent { public:
    unsigned char _pad[0x1c];
    CPlayerManager* PlayerManager;          // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
void __fastcall SetThingIsAbleToBeLifted(CThing* thing, bool able);   // 0x007E4330

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};
class CCreatureAction_DragonTakeOffIntoFlying : public CCombatActionBase { public:
    unsigned char _pad114[0x120 - 0x114];   // +0x114
    C3DVector Dest;                         // +0x120 (take-off destination)
    virtual void Initialise();
};

void CCreatureAction_DragonTakeOffIntoFlying::Initialise()
{
    SetCombatAnimation("TAKE_OFF");
    CCombatActionBase::Initialise();
    SetThingIsAbleToBeLifted(PCreature.GetPItem(), false);
    CTCCreatureFlags9* pFlags;
    PCreature.GetPItem()->GetInterface(&pFlags, 9);
    pFlags->m_canBeTargeted = 0;
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCSolidToThing* pSolid;
    if (hero->HasTC(0x4d))
        pSolid = hero->GetTC<CTCSolidToThing>(0x4d);
    else
        pSolid = hero->AddTC("CTCSolidToThing", false, 0);
    if (pSolid)
        pSolid->AddThingNotSolidTo(PCreature.GetPItem());
    const C3DVector& pos = PCreature.GetPItem()->GetPos();
    Dest = C3DVector(pos.X, pos.Y, pos.Z + 35.14837646484375f);
}