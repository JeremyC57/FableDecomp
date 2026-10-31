// Grade: DIFFER(538v529) 19 difflines (was 34; frame now 0xd8 as retail): Flourish read into a bool local before the event allocation; left: retail reads the flourish after operator new (ours before), input device in eax then ecx (retail ecx and vtable after the rumble temp) (retry wave 2 sec2b)
// Was: DIFFER(523v529) -- the Flourish byte is stored to a frame byte and pushed as a dword after operator new (an extra frame word), and the input device goes to eax where retail loads it into ecx and its vtable after the rumble temp
// CThingPlayerCreature::OnHit(CHitParameters& hit) -- creature-base hit handling, a joystick rumble, a hit-thing event to a live attacker's entity events (enemies / melee / ranged / flourish), the attacker remembered; a carried thing of carry type 0x22 dropped from the primary slot unless it is a physical switch; the spirit hit response action when flagged; the hero GUI told of the effective damage
// Leaf: ?OnHit@CThingPlayerCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006AB0E0, length 529 bytes (`ret 4`), CThingPlayerCreature vtable 0x012457FC
//   slot 65 (Ego_r.pdb ?OnHit@CThingPlayerCreature@@UAEXAAVCHitParameters@@@Z; manifest "vfunc_65").
// Callees: CThingCreatureBase::OnHit @ 0x00667D30, CThing::GetJoystickDeviceNumber @ 0x004C7CA0, the input device's
//   virtual +0x44 (rumble), CBaseIntelligentPointer::GetPItem @ 0x00A01B50, LowerBound @ 0x0040F020 / const @ 0x004365B0,
//   AreThingsEnemies @ 0x0076D2F0 (__fastcall), IsThingCarryingMeleeWeapon @ 0x006A15A0 / ranged twin @ 0x006A1620
//   (__fastcall), operator new @ 0x00BFEA1A (0x18), NEntityEvents::CEventHitThing ctor @ 0x008110A0,
//   CTCEntityEvents::AddEvent @ 0x008113C0, CThingCreatureBase::SetAttackedByCreature @ 0x006624F0, the carrying TC's
//   carried-thing getter @ 0x0073A6E0 (+ its virtual slot 1 type), the 0x46 TC's physical-switch test @ 0x006A1270 /
//   RemoveThingFromPrimarySlot @ 0x006A1E00, CCreatureAction_SpiritHitResponse ctor @ 0x00854610 / dtor @ 0x00693EF0,
//   CThingCreatureBase::SetCurrentAction @ 0x006644F0, Health_ComputeWithEffects @ 0x00868690, CPlayerGui::OnHeroHit
//   @ 0x00437950.
// Globals: input manager @ 0x013B8388 (device table +0x2c, 8-byte entries), player GUI @ 0x013B8790.
// Layout (retail): CHitParameters Direction +0x0e, Flourish +0x1d, PThingHitting +0x50 [PDB +0x58], PWeapon +0x58 [+0x60];
//   CThing presence bitset +0x20, map +0x44, flag word +0x6c (bit 3), Dead +0x91 bit 0.
struct C3DVector { float X, Y, Z; };
class CThing;
struct CBaseIntelligentPointer {
    void* m_vtable;
    CThing* PItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
#pragma pack(push, 1)
struct CHitParameters {
    unsigned char _pad00[0x0e];
    C3DVector Direction;                                // +0x0e (packed)
    unsigned char _pad1a[0x1d - 0x1a];
    bool Flourish;                                      // +0x1d
    unsigned char _pad1e[0x50 - 0x1e];
    CBaseIntelligentPointer PThingHitting;              // +0x50
    CBaseIntelligentPointer PWeapon;                    // +0x58
    float Health_ComputeWithEffects(CThing* thing);     // 0x00868690
};
#pragma pack(pop)
struct SRumble {
    float Strength;
    float Duration;
    float Delay;
    float Fade;
    bool Looping;
    SRumble(float strength, float duration, float delay, float fade, bool looping)
        : Strength(strength), Duration(duration), Delay(delay), Fade(fade), Looping(looping) { }
};
struct CInputDevice {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void Rumble(long motor, const SRumble& rumble, long a);     // slot 17 (+0x44)
};
struct SInputDeviceEntry { CInputDevice* Device; long Data; };
struct CInputManager {
    unsigned char _pad00[0x2c];
    SInputDeviceEntry* Devices;                         // +0x2c
};
extern CInputManager* g_pInputManager;                  // 0x013B8388
struct CPlayerGui {
    void OnHeroHit(float damage);                       // 0x00437950
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CEventBase { };
namespace NEntityEvents {
struct CEventHitThing : CEventBase {
    unsigned char _body[0x18];
    CEventHitThing(void* hitter, void* thing, bool enemies, bool melee, bool ranged, void* weapon, bool flourish, bool e);   // 0x008110A0
};
}
void* operator new(unsigned int size);                  // 0x00BFEA1A
struct CTCEntityEvents {
    void AddEvent(CEventBase* e);                       // 0x008113C0
};
struct CCarriedThing {
    virtual void v00();
    virtual long GetType();                             // slot 1
};
struct CTCCarryingConst {
    CCarriedThing* GetCarried() const;                  // 0x0073A6E0
};
struct CTCCarrying {
    bool HasPhysicalSwitch();                           // 0x006A1270
    void RemoveThingFromPrimarySlot(bool a);            // 0x006A1E00
};
bool __fastcall AreThingsEnemies(const CThing* a, const CThing* b);     // 0x0076D2F0
bool __fastcall IsThingCarryingMeleeWeapon(const CThing* thing);        // 0x006A15A0
bool __fastcall IsThingCarryingRangedWeapon(const CThing* thing);       // 0x006A1620
class CThingCreatureBase;
struct CCreatureActionBase { };
struct CCreatureAction_SpiritHitResponse : CCreatureActionBase {
    unsigned char _body[0xb4];
    CCreatureAction_SpiritHitResponse(CThingCreatureBase* creature, const C3DVector* direction);   // 0x00854610
    ~CCreatureAction_SpiritHitResponse();               // 0x00693EF0
};

class CThing {
public:
    void* m_vtable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long Flags6c;                              // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;                             // +0x91 bit 0
    long GetJoystickDeviceNumber() const;               // 0x004C7CA0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    __forceinline const CTCBase* GetConstTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
class CThingCreatureBase : public CThing {
public:
    void OnHit(CHitParameters& hit);                    // 0x00667D30
    void SetAttackedByCreature(CThing* creature);       // 0x006624F0
    void SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CThingPlayerCreature : public CThingCreatureBase {
public:
    void OnHit(CHitParameters& hit);
};

void CThingPlayerCreature::OnHit(CHitParameters& hit)
{
    CThingCreatureBase::OnHit(hit);
    g_pInputManager->Devices[GetJoystickDeviceNumber()].Device->Rumble(2, SRumble(1.0f, 0.5f, 0.0f, 0.4f, false), 0);
    CThing* hitter = hit.PThingHitting.GetPItem();
    if (hitter && !hitter->Dead && (hitter->Flags6c & 8)) {
        if (hitter->HasTC(0x42)) {
            CTCEntityEvents* events = (CTCEntityEvents*)hitter->GetTC(0x42);
            bool enemies = AreThingsEnemies(this, hitter);
            bool melee = IsThingCarryingMeleeWeapon(hitter);
            bool ranged = IsThingCarryingRangedWeapon(hitter);
            bool flourish = hit.Flourish;
            events->AddEvent(new NEntityEvents::CEventHitThing(hitter, this, enemies, melee, ranged, hit.PWeapon.GetPItem(), flourish, false));
        }
        SetAttackedByCreature(hitter);
    }
    if (HasTC(0x31)) {
        if (((const CTCCarryingConst*)GetConstTC(0x31))->GetCarried()->GetType() == 0x22) {
            CTCCarrying* carrying;
            if (HasTC(0x46))
                carrying = (CTCCarrying*)GetTC(0x46);
            if (!carrying->HasPhysicalSwitch())
                carrying->RemoveThingFromPrimarySlot(false);
        }
    }
    if (HasTC(0xc7))
        SetCurrentAction(CCreatureAction_SpiritHitResponse(this, &hit.Direction));
    CPlayerGui* gui = g_pPlayerGui;
    gui->OnHeroHit(hit.Health_ComputeWithEffects(this));
}
