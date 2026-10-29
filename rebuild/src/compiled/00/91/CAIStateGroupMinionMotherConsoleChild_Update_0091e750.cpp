// CAIStateGroup_MinionMotherConsoleChild::Update() -- for each child-hurt-go-to-mother event (type 0x18) in the mother's entity events: look at the living child and, once it is within 3 units (or dead or gone), play the ST_BLOW_KISS_LOOP animation; stop when the look fails, the child is still too far away or the animation fails; then terminate
//   loop instead of `break` (VC7.1 tail-merges it into the shared Terminate). With the break, the function has one exit path
//   through the loop and VC7.1 shrink-wraps the esi/edi pushes after the presence test and swaps this/child (ebp/edi); the
//   duplicated exit keeps all four pushes at entry with this in edi and the 0x18 predicate / child in ebp, as retail.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091E750, length 432 bytes, plain `ret` (virtual).
// Callees: the const component-map LowerBound @ 0x004365B0 (inlined lookup 0x42 = CTCEntityEvents, bit 2 of +0x28),
// CTCEntityEvents::GetEvent<NEntityEvents::CEventChildHurtGoToMother, NEntityEvents::CIsEventOfType> @ 0x00915760,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the event's child at +8), ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90
// (CIntelligentPointer<const CThing> vtable 0x01237F1C, by value), AIAction_LookAtThing @ 0x0097D6F0,
// CThingCreatureBase::PeekPos @ 0x006A4D00, CThing::PeekPos @ 0x004C73D0, CCharString(const char*, long)
// @ 0x0099EBF0 ("ST_BLOW_KISS_LOOP" @ 0x0128FFB0) / ~CCharString @ 0x0099EAE0, CCreatureAction_PlayAnimation ctor
// @ 0x008424F0, AIAction_PerformAction @ 0x0097D8B0, ~CCreatureActionBase @ 0x00693EF0, Terminate @ 0x008FCF40.
// Data: 9.0f @ 0x0125EF98, 1/360 immediate.
// Shape: the PlayAnimation action is the landed 0x00958000 `&temporary` with the implicit CCharString conversion temporary
// built first; the event loop is `while (GetEvent(&event, CIsEventOfType(0x18)) && result != 1)`; GetEvent is a same-TU
// auto_inline(off) stand-in (the template sits next to this function in retail's TU) so the predicate's re-store is hoisted
// out of the loop; GetInterface is a plain (not forced) inline returning its `present` local, which leaves the present
// byte stored in the frame; the distance test sums `X*X + (Y*Y + Z*Z)`.
// Layout: retail CThing interface bitset +0x20 / map +0x44; the event's child pointer +8.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + (Y * Y + Z * Z); }
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
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;    // 0x004C73D0
    bool HasTC(long interfaceType) const { return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0; }
    template<class T> bool GetInterface(T** ppOut, long interfaceType) const
    {
        bool present = HasTC(interfaceType);
        if (present) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
        }
        return present;
    }
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;       // 0x006A4D00
};
class CThingCreatureBase : public CThingPhysical { public:
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // <const CThing>: vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointer(T* item) { SetPItem(item); }
};

namespace NEntityEvents {
class CEventChildHurtGoToMother { public:
    unsigned char _pad0[8];
    CBaseIntelligentPointer PChild;             // +0x08
};
class CIsEventOfType { public:
    long Type;
    CIsEventOfType(long type) : Type(type) { }
};
}
class CTCEntityEvents { public:                        // interface 0x42
    bool GetEvent(NEntityEvents::CEventChildHurtGoToMother** event, const NEntityEvents::CIsEventOfType& isType);   // 0x00915760
};
long EventSink_91e750(const void* self, long type);   // stand-in: the event-list walk GetEvent does outside the graded bytes
#pragma auto_inline(off)
// Same-TU stand-in for GetEvent<CEventChildHurtGoToMother, CIsEventOfType> @ 0x00915760 (a template instantiated in retail's
// TU, just before this function): it reads the predicate and writes the event out-pointer, and lets neither escape.
__declspec(noinline) bool CTCEntityEvents::GetEvent(NEntityEvents::CEventChildHurtGoToMother** event,
                                                    const NEntityEvents::CIsEventOfType& isType)
{
    *event = (NEntityEvents::CEventChildHurtGoToMother*)EventSink_91e750(this, isType.Type);
    return *event != 0;
}
#pragma auto_inline(on)

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair(long first, long last) : FirstSound(first), LastSound(last) { }
};
class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();             // 0x00693EF0
    unsigned char _pad04[0xb0];
};
class CCreatureAction_PlayAnimation : public CCreatureActionBase { public:
    CCreatureAction_PlayAnimation(CThingCreatureBase* creature, const CCharString& animation,
        const CSoundPair& sound, bool a, bool b, bool c, long d, long e);   // 0x008424F0
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
struct CAIStateGroup_MinionMotherConsoleChild : CAIStateGroupBase {
    virtual void Update();
};

void CAIStateGroup_MinionMotherConsoleChild::Update()
{
    CTCEntityEvents* events;
    if (GetCreature()->GetInterface(&events, 0x42)) {
        NEntityEvents::CEventChildHurtGoToMother* event = 0;
        long result = 4;
        while (events->GetEvent(&event, NEntityEvents::CIsEventOfType(0x18)) && result != 1) {
            CThing* child = ((CThing*)event->PChild.GetPItem());
            if (child != 0 && !(child->m_flags91 & 1)) {
                if (AIAction_LookAtThing(1, CIntelligentPointer<const CThing>(child), 1, 0.0027777f) == 1) {
                    Terminate();
                    return;
                }
                if (!(child->m_flags91 & 1) &&
                    (child->GetPos() - GetCreature()->PeekPos()).GetLengthSquared() > 9.0f)
                    break;
            }
            result = AIAction_PerformAction(&CCreatureAction_PlayAnimation(GetCreature(), "ST_BLOW_KISS_LOOP",
                CSoundPair(0, 0), false, true, false, 0, 0));
        }
    }
    Terminate();
}