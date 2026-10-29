// CThingSwitch::SwitchTriggerPlayerOnlyMultipleTimesInArea() -- for each of the four players whose player object is initialised, if its living hero stands inside the switch's trigger square, count the trigger and queue a CEventTriggered for the hero on the switch's entity-events component (0x42)
// Leaf: SwitchTriggerPlayerOnlyMultipleTimesInArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083C690, length 361 bytes (plain `ret`).
// Callees: CThing::PeekPos() const @ 0x006A4D00 (this TU's out-of-line copy; declared on CThingPhysical so it does
//   not share a symbol with the 0x004C73D0 copy), the const/non-const player-manager accessors @ 0x004C79F0 / 0x004C7A00
//   (byte twins, `[[0x013B8A1C]+0x30]`), CPlayerManager::GetPlayer(long) const @ 0x00449910 and non-const @ 0x004498C0
//   (byte twins), CInitBaseClass::IsInitialised() const @ 0x0099A330, CPlayer::GetPControlledCreature @ 0x00487DC0,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, operator new @ 0x00BFEA1A,
//   NEntityEvents::CEventTriggered(CThing*) @ 0x0083CF20, CTCEntityEvents::AddEvent @ 0x008113C0.
// Layout (retail, as the landed CThingSwitch rows 0x0083BE90 / 0x0083C230): interface bitset +0x20, interface map
//   +0x44, Dead bit +0x91, TriggerRadius +0xd0, TimesTriggered +0xe4 (Ego_r.pdb +0xfc, CThingPhysical 0x18 lower).
// Shape: the trigger square is a C2DBoxF filled member by member (one PeekPos() call per component); the hero's
//   point is a C2DVector built from two PeekPos() calls (Y first: arguments are evaluated right to left) and tested
//   with the half-open ContainsPoint inlined. The const query chain is reached through a `const CThing*`.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    __forceinline bool ContainsPoint(const C2DVector& p) const { return p.X >= TLX && p.X < BRX && p.Y >= TLY && p.Y < BRY; }
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
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
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
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  Dead : 1;                            // +0x91 bit 0
    const CPlayerManager* GetPlayerManager() const;     // 0x004C79F0
    CPlayerManager* GetPlayerManager();                 // 0x004C7A00
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
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
    unsigned char _padd4[0xe4 - 0xd4];
    long          TimesTriggered;                       // +0xe4
    void SwitchTriggerPlayerOnlyMultipleTimesInArea();
};

void CThingSwitch::SwitchTriggerPlayerOnlyMultipleTimesInArea()
{
    C2DBoxF area;
    area.TLX = PeekPos().X - TriggerRadius;
    area.TLY = PeekPos().Y - TriggerRadius;
    area.BRX = PeekPos().X + TriggerRadius;
    area.BRY = PeekPos().Y + TriggerRadius;
    for (long i = 0; i < 4; ++i) {
        const CThing* self = this;
        if (self->GetPlayerManager()->GetPlayer(i)->IsInitialised()) {
            CThingPhysical* hero = static_cast<CThingPhysical*>(GetPlayerManager()->GetPlayer(i)->GetPControlledCreature());
            if (hero && !hero->Dead && area.ContainsPoint(C2DVector(hero->PeekPos().X, hero->PeekPos().Y))) {
                ++TimesTriggered;
                if (HasTC(0x42)) {
                    CTCEntityEvents* events = (CTCEntityEvents*)GetTC(0x42);
                    events->AddEvent(new NEntityEvents::CEventTriggered(hero));
                }
            }
        }
    }
}