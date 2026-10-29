// CAIStateGroup_RangedCombatUsePreferredFirePoint::AttackEnemy() -- when the creature may attack and stands within
// 0.05 (squared 0.0025) of its fire point in the XY plane, pump the combat actions; false when they report 1.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095D070, length 108 bytes (+ int3 padding).
// Callees: the creature's attack-readiness test @ 0x00662410 (bool; manifest "C3DAnimationBlendState::
// AddCombinationBlend" is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on PFirePoint at +0x1c,
// CThing::GetPosition @ 0x004C73D0 (fire point, then creature), CAIStateGroupBase::AIAction_PumpCombatActions
// @ 0x0097D980. Global: the squared fire-point tolerance float @ 0x0129531C (0.0025f).
// Shape (retry 10b): three plain early returns (`if (!CanAttackNow()) return false; if (IsAwayFromFirePoint()) return
// false; if (Pump() == 1) return false; return true;`); the creature is read through an inline GetCreature() accessor
// (PBrain->PCreature) at both uses -- the accessor keeps the first `xor al,al` exit after the first test and gives the
// second PBrain load ecx as retail; the 2D delta (creature - fire point) is built on the x87 stack by a by-value
// `operator-` and `X*X + Y*Y` compared with `fcomp [g] ; test ah,41h` (greater-than).
// PFirePoint is the engine header name (the header flattens the base so the class is modelled locally).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
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

class C3DVector;
class CThing { public:
    const C3DVector& GetPos() const;            // 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ (Ego_r.pdb)
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    CThing* GetCreature() const { return PBrain->PCreature; }
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    float MagnitudeSquared() const { return X * X + Y * Y; }
};

class C3DVector : public C2DVector { public:    // the X/Y head is the C2DVector base, so the 2D arithmetic below binds the position directly
    float Z;
};
struct CThingPos : CThing {
    bool CanAttackNow();                        // 0x00662410
};

extern const float g_firePointToleranceSquared;    // 0x0129531C (0.0025f)

struct CAIStateGroupBaseC : CAIStateGroupBase {
    long AIAction_PumpCombatActions();          // 0x0097D980
};

struct CAIStateGroup_RangedCombatUsePreferredFirePoint : CAIStateGroupBaseC {
    CIntelligentPointer<CThing> PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    CIntelligentPointer<CThing> PFirePoint;         // +0x1c CIntelligentPointer<CThing>
    long TimeOutCounter;                        // +0x24
    bool AttackEnemy();
    // the creature is a named local read before the fire-point position; the 2D delta is built by the by-value
    // `operator-` and `X*X + Y*Y` compared with the tolerance
    __forceinline bool IsAwayFromFirePoint()
    {
        CThingPos* creature = (CThingPos*)GetCreature();
        const C2DVector& firePos = ((CThingPos*)PFirePoint.GetPItem())->GetPos();
        return (creature->GetPos() - firePos).MagnitudeSquared() > g_firePointToleranceSquared;
    }
};

bool CAIStateGroup_RangedCombatUsePreferredFirePoint::AttackEnemy()
{
    if (!((CThingPos*)GetCreature())->CanAttackNow())
        return false;
    if (IsAwayFromFirePoint())
        return false;
    if (AIAction_PumpCombatActions() == 1)
        return false;
    return true;
}