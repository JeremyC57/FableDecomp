// CCreatureAction_TrollProjectileAttack::PerformAction() -- whack the ground, then create a SHOT_TROLL_CRACK_IN_GROUND shot from the troll at its target with the troll's shot accuracy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C6430, length 187 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CCreatureAction_TrollWhackGroundBase::WhackGround @ 0x008C61C0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CThingCreatureBase::GetShotAccuracyPercentage @ 0x00663B10, CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// CCreatureActionBase::GetPWorld @ 0x00692940 (manifest label is STL noise), CDefinitionManager::GetDefGlobalIndexFromName
// @ 0x009AD410 (the world's definition manager at +0x10), CCharString::~CCharString @ 0x0099EAE0,
// CreateShot(long, CThing*, CThing*, CThing*, float, float, float, CDefPointer<CMeleeCombatAbilityDef>*, C3DVector*,
// CRightHandedSet*) @ 0x007C0AD0 (`__fastcall`, `ret 20h`).
// Shape: the accuracy is a named long converted with `fild` at the call; the definition manager is read into a local after
// the name string is built (fetched before the argument push); the zeroed CDefPointer passed by address lives in a SIBLING
// block of the string's, so both share one frame word, and is released at the block end through the def's inline
// ReleaseTemporary on its ref-count base at +4 (null test kept, null store elided); the troll is a local fetched before the
// stack arguments.
// Layout: owner CIntelligentPointer +8, target +0xa8.
struct C3DVector;
struct CRightHandedSet;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CWorld {
    unsigned char       _pad[0x10];
    CDefinitionManager* PDefinitionManager;            // +0x10
};

struct CRefCounted {                                   // second base of the def: the count at +4
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                            // slot 1
};
struct CMeleeCombatAbilityDef : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

struct CThing { };
struct CThingCreatureBase : CThing {
    long GetShotAccuracyPercentage();                  // 0x00663B10
};

CThing* __fastcall CreateShot(long defIndex, CThing* shooter, CThing* origin, CThing* target, float accuracy, float a, float b,
                              CDefPointer<CMeleeCombatAbilityDef>* ability, C3DVector* position, CRightHandedSet* frame);   // 0x007C0AD0

struct CBaseIntelligentPointer {
    void*               __vftable;
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const;              // 0x00A01B50
};

class CCreatureAction_TrollWhackGroundBase {
public:
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;                   // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    CWorld* GetPWorld() const;                         // 0x00692940
    void WhackGround();                                // 0x008C61C0
};

class CCreatureAction_TrollProjectileAttack : public CCreatureAction_TrollWhackGroundBase {
public:
    virtual void PerformAction();
};

void CCreatureAction_TrollProjectileAttack::PerformAction()
{
    WhackGround();
    long accuracy = PCreature.GetPItem()->GetShotAccuracyPercentage();
    long shotIndex;
    {
        CCharString name("SHOT_TROLL_CRACK_IN_GROUND", -1);
        CDefinitionManager* manager = GetPWorld()->PDefinitionManager;
        shotIndex = manager->GetDefGlobalIndexFromName(name);
    }
    {
        CDefPointer<CMeleeCombatAbilityDef> ability;
        CThing* troll = PCreature.GetPItem();
        CreateShot(shotIndex, troll, 0, PTarget.GetPItem(), (float)accuracy, 1.0f, 1.0f, &ability, 0, 0);
    }
}