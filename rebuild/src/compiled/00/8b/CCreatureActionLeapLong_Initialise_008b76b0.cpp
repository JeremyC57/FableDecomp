// CCreatureAction_LeapLong::Initialise() -- leap 3 units back along the facing when that point is in navigable line of
// sight (MOVEMENT_LONG_BACK), else 3 units forward (MOVEMENT_LONG_FORWARD, made non-solid to the combat target), else
// SPECIAL_IDLE with the leap flag set; then the base Initialise
// Leaf: ?Initialise@CCreatureAction_LeapLong
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B76B0, length 624 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound (const copy) 0x004365B0, interface 2's
//   GetFacing (slot 70, +0x118, C3DVector by hidden pointer), CThing::GetPos 0x004C73D0,
//   CTCCreatureNavigation::IsPosNavLineOfSightTo 0x006CBA20 (interface 7), CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   CCombatActionBase::SetCombatAnimation 0x008574D0 / Initialise 0x00857520, CThing::AddTC 0x004C9D60,
//   CTCSolidToThing::AddThingNotSolidTo 0x00770FA0.
// Layout: retail CCombatActionBase is 0x114 bytes (landed WhisperSommersaultForward::Initialise 0x008C0820); the flag
//   at +0x114 is this class's first member; PTarget +0xa8; CThing interface bitset +0x20, map +0x44.
class CThing;
class CTCBase;
class CTCParamsBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { C3DVector r; r.X = X + o.X; r.Y = Y + o.Y; r.Z = Z + o.Z; return r; }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                   // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
struct CTCFacing {                                    // interface 2
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing() const;              // slot 70 (+0x118)
};
class CTCCreatureNavigation { public:                        // interface 7
    bool IsPosNavLineOfSightTo(const C3DVector& pos); // 0x006CBA20
};
class CTCSolidToThing { public:
    void AddThingNotSolidTo(CThing* thing);           // 0x00770FA0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    const C3DVector& GetPos() const;             // 0x004C73D0
    CTCSolidToThing* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
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
class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                        // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;                  // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_LeapLong : public CCombatActionBase { public:
    bool NoRoom;                                      // +0x114
    virtual void Initialise();
};

void CCreatureAction_LeapLong::Initialise()
{
    CTCFacing* facingTC;
    GetCreature()->GetInterface(&facingTC, 2);
    C3DVector facing = facingTC->GetFacing();
    C3DVector target = GetCreature()->GetPos() - facing * 3.0f;
    CTCCreatureNavigation* nav;
    GetCreature()->GetInterface(&nav, 7);
    if (nav->IsPosNavLineOfSightTo(target)) {
        SetCombatAnimation("MOVEMENT_LONG_BACK");
        NoRoom = false;
        CCombatActionBase::Initialise();
        return;
    }
    target = facing * 3.0f + GetCreature()->GetPos();
    if (nav->IsPosNavLineOfSightTo(target)) {
        SetCombatAnimation("MOVEMENT_LONG_FORWARD");
        NoRoom = false;
        CTCSolidToThing* solid = GetCreature()->AddTC("CTCSolidToThing", false, 0);
        solid->AddThingNotSolidTo(PTarget.GetPItem());
        CCombatActionBase::Initialise();
        return;
    }
    SetCombatAnimation("SPECIAL_IDLE");
    NoRoom = true;
    CCombatActionBase::Initialise();
}