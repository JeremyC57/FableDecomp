// CDefendingCombatantInfo::CDefendingCombatantInfo(const CDefendingCombatantInfo& other) -- copy constructor: attacker map and combat-wheel ring vector copied out of line, the scalars word by word, the last-attacker intelligent pointer re-pointed, the attack-pattern def pointer shared with an add-ref
// Leaf: ??0CDefendingCombatantInfo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F46D0, length 116 bytes (`ret 4`).
// Callees: map<unsigned __int64, CThingCreatureBase*> copy constructor @ 0x006F33A0, vector<CCombatRing> copy constructor
// @ 0x006F43C0 (CombatWheel's only member), CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10,
// ::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90 (CIntelligentPointer<CThing> vtable 0x01238C6C).
// Layout (Ego_r.pdb with retail 12-byte STLport containers): Attackers +0x00, CombatWheel +0x0c, LastVictimWheelUpdatePos
// +0x18, LastWheelUpdateWorldFrame +0x24, PLastAttacker +0x28, PAttackPattern +0x30, AttackPatternIndex +0x34,
// FrameForNextInstruction +0x38.
// Shape: a memberwise copy constructor in declaration order; the inline CDefPointer copy constructor loads, tests and
// stores the object word before `inc dword ptr [eax+4]`; C3DVector's memberwise user copy constructor moves its three
// words straight through ecx/edx/eax (an implicit struct copy goes through `lea` re-based pointers).
class CBaseObject;
class CThingCreatureBase;
class CAttackPatternDef;

struct CAttackerMap {                            // map<unsigned __int64, CThingCreatureBase*> (STLport, 12 bytes)
    unsigned char _tree[0xc];
    CAttackerMap(const CAttackerMap& other);     // 0x006F33A0
};
struct CCombatRingVector {                       // vector<CCombatRing> (STLport, 12 bytes)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CCombatRingVector(const CCombatRingVector& other);   // 0x006F43C0
};
struct CCombatWheel {
    CCombatRingVector CombatRings;               // +0x00
};
class C3DVector { public:                               // floats, moved as dwords (a float member copy would go through the FPU)
    unsigned long X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

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
class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CDefPointee {
    virtual void v00();
    long m_refCount;                             // +4
};
struct CDefPointer {                             // CDefPointer<const CAttackPatternDef>
    CDefPointee* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
};

class CDefendingCombatantInfo { public:
    CAttackerMap        Attackers;               // +0x00
    CCombatWheel        CombatWheel;             // +0x0c
    C3DVector           LastVictimWheelUpdatePos;   // +0x18
    long                LastWheelUpdateWorldFrame;  // +0x24
    CIntelligentPointer<CThing> PLastAttacker;           // +0x28
    CDefPointer         PAttackPattern;          // +0x30
    long                AttackPatternIndex;      // +0x34
    long                FrameForNextInstruction; // +0x38
    CDefendingCombatantInfo(const CDefendingCombatantInfo& other);
};

CDefendingCombatantInfo::CDefendingCombatantInfo(const CDefendingCombatantInfo& other)
    : Attackers(other.Attackers)
    , CombatWheel(other.CombatWheel)
    , LastVictimWheelUpdatePos(other.LastVictimWheelUpdatePos)
    , LastWheelUpdateWorldFrame(other.LastWheelUpdateWorldFrame)
    , PLastAttacker(other.PLastAttacker)
    , PAttackPattern(other.PAttackPattern)
    , AttackPatternIndex(other.AttackPatternIndex)
    , FrameForNextInstruction(other.FrameForNextInstruction)
{
}