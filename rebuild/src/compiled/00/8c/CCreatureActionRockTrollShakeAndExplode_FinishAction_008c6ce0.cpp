// CCreatureAction_RockTrollShakeAndExplode::FinishAction() -- when the troll's CTrollBattleDef has a death explosion, create it at the troll and make the troll immune to it; finish the combat action and kill the troll
// Leaf: ?FinishAction@CCreatureAction_RockTrollShakeAndExplode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C6CE0, 194 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): RTTI vtable 0x0128154C (.?AVCCreatureAction_RockTrollShakeAndExplode@@) slot 4 = FinishAction (slot 4
//   of every CCreatureActionBase vtable is FinishAction, e.g. landed 0x008C2980 / 0x008D5960); Ego_r.pdb
//   ?FinishAction@CCreatureAction_RockTrollShakeAndExplode@@UAEXXZ (donor 0x008FD910, after Initialise 0x008FD7C0 = retail
//   0x008C6070 landed). Same-TU twin: landed CCreatureAction_RockTrollFallToDeath::FinishAction 0x008C74F0 (the
//   FallDownExplosionIndex +0x5C, a follow-up action instead of Kill).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CDefClassBase::GetSubDef<CTrollBattleDef> 0x008C78F0 (landed),
//   CThing::GetPos 0x004C73D0, CTCDExplosion::Create(long, const C3DVector&) 0x0080C460 (static `__fastcall`, plain ret =
//   the two-argument overload), CTCDExplosion::SetThingAsImmune(CThing&) 0x0080CBF0 (builds a CIntelligentPointer<CThing>
//   and appends it: the only "add a thing" member of CTCDExplosion in Ego_r, donor order Create, Create, SetThingAsImmune),
//   CCombatActionBase::FinishAction 0x00859A20, CThing::Kill(bool) 0x004C9B80.
// Shape: CDefClassBase::GetSubDef<CTrollBattleDef> is compiled here as a same-TU noinline body (the landed 0x008C78F0
//   reconstruction, verbatim): only with its body visible does VC7.1 keep the address-taken def pointer in ebp after
//   the call and `this` in a frame slot, as retail (declared-only it reloads the local and keeps `this` in ebp).
//   Then the FallToDeath model -- a by-value copy of the creature's PBaseDef (count +1) queried for the sub-def and
//   released after the whole `&&` condition, so the condition's bool lives across the release (bl here).
// Layout: retail CThing (PDB - 0x10 after the STLport containers): UsingComponents +0x20, PTCDriver +0x5C, PBaseDef
//   +0x70; CTrollBattleDef DeathExplosionIndex +0x58 (PDB +0x5C; retail CDefClassBase is 4 bytes shorter).
class C3DVector;

class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    void* PStringData;
};

struct CRefCounted {                                         // the def's reference count at +4 (behind the vptr)
    mutable long m_count;
    void DecRef() const { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Delete() const;                             // slot 1
};

class CDefPointeeBase : public CDefVirtual, public CRefCounted {
public:
    void Release() const
    {
        if (--m_count == 0)
            Delete();
    }
    void ReleaseTemporary() const
    {
        DecRef();
        if (GetRefCount() == 0)
            Delete();
    }
};

class CDefClassBase;
template <class T> class CDefPointer;

template <> class CDefPointer<CDefClassBase> {               // CDefinitionManager::GetPDefFromGlobalIndex's result
public:
    CDefPointeeBase* m_p;
    CDefPointer() { }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

template <class T> class CDefPointer {
public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_count; }
    CDefPointer(const CDefPointer<CDefClassBase>& other) : m_p((T*)other.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
    T* operator->() const { return m_p; }
};

class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10
};

class CDefClassBase : public CDefPointeeBase {
public:
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual const long* GetSubDefIndex(const CCharString& name) const;   // slot 14 (+0x38), descriptive
    unsigned char _pad_0x08[0x1c - 8];
    CDefinitionManager* PDefManager;                         // +0x1C
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};

class CThingBaseDef : public CDefClassBase { };

class CTrollBattleDef : public CDefClassBase {
public:
    unsigned char _pad_0x20[0x58 - 0x20];
    long DeathExplosionIndex;                                // +0x58
    long FallDownExplosionIndex;                             // +0x5C
};

#pragma auto_inline(off)
template <>
bool CDefClassBase::GetSubDef<CTrollBattleDef>(CDefPointer<const CTrollBattleDef>* out) const    // 0x008C78F0 (landed body)
{
    const long* pIndex;
    {
        CCharString name("CTrollBattleDef", -1);
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<const CTrollBattleDef> result(PDefManager->GetPDefFromGlobalIndex(*pIndex));
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
enum ETCInterfaceType { };                                   // 0x41: the explosion driver TC
class CBaseObject;
class CTCDriverBase;

class CThing {
public:
    virtual ~CThing();
    unsigned char _pad_0x04[0x20 - 0x04];
    unsigned long UsingComponents[9];                        // +0x20 bitset<274>
    unsigned char _pad_0x44[0x5c - 0x44];
    CTCDriverBase* PTCDriver;                                // +0x5C
    unsigned char _pad_0x60[0x70 - 0x60];
    CDefPointer<const CThingBaseDef> PBaseDef;               // +0x70
    const C3DVector& GetPos() const;                         // 0x004C73D0
    void Kill(bool now);                                     // 0x004C9B80
    bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return PBaseDef; }
};

class CThingCreatureBase : public CThing { };

class CTCDExplosion {
public:
    static CThing* __fastcall Create(long explosion_index, const C3DVector& pos);   // 0x0080C460
    void SetThingAsImmune(CThing& thing);                    // 0x0080CBF0
};

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void* PData;
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer();
    T* operator->() const { return (T*)GetPItem(); }
    T& operator*() const { return *(T*)GetPItem(); }
};

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    virtual void v01();
    virtual void v02();
    virtual void PerformAction();
    virtual void FinishAction();
    unsigned char _pad_0x04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
};

class CCombatActionBase : public CCreatureActionBase {
public:
    protected: virtual void FinishAction(); public:                             // 0x00859A20
};

class CCreatureAction_RockTrollShakeAndExplode : public CCombatActionBase {
public:
    virtual void FinishAction();
};

void CCreatureAction_RockTrollShakeAndExplode::FinishAction()
{
    CDefPointer<const CTrollBattleDef> troll_battle_def;
    if (PCreature->GetPBaseDef()->GetSubDef(&troll_battle_def) && troll_battle_def->DeathExplosionIndex > 0) {
        CThing* explosion = CTCDExplosion::Create(troll_battle_def->DeathExplosionIndex, PCreature->GetPos());
        if (explosion && explosion->HasTC((ETCInterfaceType)0x41)) {
            CTCDExplosion* explosion_tc = (CTCDExplosion*)explosion->PTCDriver;
            explosion_tc->SetThingAsImmune(*PCreature);
        }
    }
    CCombatActionBase::FinishAction();
    PCreature->Kill(true);
}