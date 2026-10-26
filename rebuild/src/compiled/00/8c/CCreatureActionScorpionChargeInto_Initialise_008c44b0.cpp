// CCreatureAction_ScorpionChargeInto::Initialise() -- animation CHARGE_INTO, base Initialise; turn a live target's 0x93
// component toward the scorpion; then create the charge obstruction one willpower-indicator radius (+0.2) ahead of the
// scorpion and keep it in PObstruction
// Leaf: ?Initialise@CCreatureAction_ScorpionChargeInto
// Layout: retail CCombatActionBase is 0x114 (Ego_r.pdb +0x18), so ChargeDistance +0x114 and PObstruction +0x118 (PDB
//   +0x12c/+0x130); PTarget +0xa8; CThing def pointer +0x70, interface bitset +0x20, map +0x44, physics +0x60.
// Shape: GetSubDef<CAICreatureWillPowerIndicatorDef> (0x008C5A50) sits in this TU, so it has a same-TU noinline stand-in
//   (the def pointer's address does not escape: VC keeps it in edi and the release has no null test); operator* is the
//   constructor form with the scale on the left, operator+ the constructor form (Z, Y, X computed, then stored).
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector operator*(float s) const { return C3DVector(s * X, s * Y, s * Z); }
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CPhysicsBody {                                 // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing() const;                    // slot 70 (+0x118)
};
class CAICreatureWillPowerIndicatorDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long          RefCount;                           // +0x04
    unsigned char _pad08[0x5c - 0x08];
    float         Radius;                             // +0x5c
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (--Object->RefCount == 0)
            Object->Destroy();
    }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out);   // 0x008C5A50
};
void* FindSubDef(CDefClassBase* def);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CAICreatureWillPowerIndicatorDef>* out)
{
    void* found = FindSubDef(this);
    if (!found)
        return false;
    *(void**)out = found;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* PhysicsBody;                        // +0x60
    unsigned char _pad64[0x70 - 0x64];
    CDefClassBase* Def;                               // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
    CDefClassBase* GetDef() const { return Def; }
    const C3DVector& GetPosition() const;             // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTC93 {                                        // interface 0x93
    void SetDirection(CThing* towards, bool flag);    // 0x0079E580
};
CThing* __fastcall CreateObstruction(const C3DVector& a, const C3DVector& b, float radius);   // 0x007FD4B0

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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;               // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;                 // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
class CCreatureAction_ScorpionChargeInto : public CCombatActionBase { public:
    float                    ChargeDistance;          // +0x114
    CIntelligentPointer<CThing> PObstruction;            // +0x118
    void Initialise();
};

void CCreatureAction_ScorpionChargeInto::Initialise()
{
    SetAnimationName("CHARGE_INTO");
    CCombatActionBase::Initialise();
    CTC93* direction;
    if (IS_THING_ALIVE(PTarget) && PTarget.GetPItem()->GetInterface(&direction, 0x93))
        direction->SetDirection(GetCreature(), true);
    CDefPointer<CAICreatureWillPowerIndicatorDef> def;
    GetCreature()->GetDef()->GetSubDef(&def);
    C3DVector offset = GetCreature()->GetPhysics()->GetFacing() * (def.Object->Radius + 0.2f);
    CThing* obstruction = CreateObstruction(GetCreature()->GetPosition() + offset, GetCreature()->GetPosition() + offset,
                                            def.Object->Radius);
    if (obstruction)
        PObstruction = CIntelligentPointer<CThing>(obstruction);
}