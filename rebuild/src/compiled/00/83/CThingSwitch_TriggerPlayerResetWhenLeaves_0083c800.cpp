// CThingSwitch::TriggerPlayerResetWhenLeaves() -- re-arming area switch: if an initialised player's living hero is inside the trigger square and the switch is not already triggered, count the trigger and queue a CEventTriggered (component 0x42); the triggered flag ends up as "some hero is inside", so the switch re-arms once every hero has left
// Leaf: TriggerPlayerResetWhenLeaves
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083C800, length 388 bytes (plain `ret`).
// Callees: as the sibling SwitchTriggerPlayerOnlyMultipleTimesInArea 0x0083C690 (same TU): CThing::PeekPos() const
//   @ 0x006A4D00, player-manager accessors @ 0x004C79F0 (const) / 0x004C7A00, CPlayerManager::GetPlayer(long) const
//   @ 0x00449910 / non-const @ 0x004498C0, CInitBaseClass::IsInitialised() const @ 0x0099A330, CPlayer::GetPControlledCreature
//   @ 0x00487DC0, LowerBound @ 0x0040F020, operator new @ 0x00BFEA1A, NEntityEvents::CEventTriggered(CThing*)
//   @ 0x0083CF20, CTCEntityEvents::AddEvent @ 0x008113C0.
// Layout (retail): as the sibling, plus Triggered +0xdc (Ego_r.pdb +0xf4, CThingPhysical 0x18 lower).
// Shape: `inArea` lives in bl and is written back to Triggered on both exits (the loop end and the break after the
//   event), the loop index in ebp.
#include "fable_compiler.h"
class CTCBase;
#include "engine/C3DVector.h"
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    FABLE_FORCEINLINE bool ContainsPoint(const C2DVector& p) const { return p.X >= TLX && p.X < BRX && p.Y >= TLY && p.Y < BRY; }
};

struct CEventBase { virtual void v00(); };
class CThing;
namespace NEntityEvents {
class CEventTriggered : public CEventBase { public:
    unsigned char _body[0x10 - 4];
    CEventTriggered(CThing* triggerer);                 // 0x0083CF20
};
}
class CTCEntityEvents { public:
    void AddEvent(CEventBase* event);                   // 0x008113C0
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature;
class CPlayer { public:
    bool IsInitialised() const;                         // 0x0099A330 (CInitBaseClass)
    CThingPlayerCreature* GetPControlledCreature() const;                          // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetPlayer(long number);                    // 0x004498C0
    const CPlayer* GetPlayer(long number) const;        // 0x00449910
};

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad54[0x3d];
    unsigned char  Dead : 1;                            // +0x91 bit 0
    const CPlayerManager* GetPlayerManager() const;     // 0x004C79F0
    CPlayerManager* GetPlayerManager();                 // 0x004C7A00
    FABLE_FORCEINLINE bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    FABLE_FORCEINLINE CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(key);
        CInterfaceMap::value_type* end = map->end();
        return (it == end || type < it->first ? end : it)->second;
    }
};

class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;               // 0x006A4D00
};
class CThingPlayerCreature : public CThingPhysical { };

class CThingSwitch : public CThingPhysical { public:
    unsigned char _pad94[0xd0 - 0x94];
    float         TriggerRadius;                        // +0xd0
    unsigned char _padd4[0xdc - 0xd4];
    bool          Triggered;                            // +0xdc
    unsigned char _paddd[0xe4 - 0xdd];
    long          TimesTriggered;                       // +0xe4
    void TriggerPlayerResetWhenLeaves();
};

void CThingSwitch::TriggerPlayerResetWhenLeaves()
{
    C2DBoxF area;
    area.TLX = PeekPos().X - TriggerRadius;
    area.TLY = PeekPos().Y - TriggerRadius;
    area.BRX = PeekPos().X + TriggerRadius;
    area.BRY = PeekPos().Y + TriggerRadius;
    bool inArea = false;
    for (long i = 0; i < 4; ++i) {
        const CThing* self = this;
        if (self->GetPlayerManager()->GetPlayer(i)->IsInitialised()) {
            CThingPhysical* hero = static_cast<CThingPhysical*>(GetPlayerManager()->GetPlayer(i)->GetPControlledCreature());
            if (hero && !hero->Dead && area.ContainsPoint(C2DVector(hero->PeekPos().X, hero->PeekPos().Y))) {
                inArea = true;
                if (!Triggered) {
                    ++TimesTriggered;
                    if (HasTC(0x42)) {
                        CTCEntityEvents* events = (CTCEntityEvents*)GetTC(0x42);
                        events->AddEvent(new NEntityEvents::CEventTriggered(hero));
                    }
                    break;
                }
            }
        }
    }
    Triggered = inArea;
}