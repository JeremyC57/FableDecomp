// CAIStateGroup_EnemyCreatureBase::IsRunnable() -- runnable when an enemy is found (remembered in PEnemy) and the slot-15 validity check agrees
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091DD30, length 99 bytes (+ int3 padding).
// Callees: CAIStateGroup_EnemyCreatureBase::GetEnemy @ 0x0091DB50, CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor
// @ 0x00A01B10/90/50/0x00A01C10, `this`'s virtual slot 15 (+0x3c).
// Shape: the enemy is a named local (edi) tested once; `PEnemy = CIntelligentPointer<CThingCreatureBase>(enemy)` --
// a frame temporary through the inline converting constructor (vtable 0x0123E7D8, SetPItem(enemy)), the inline
// operator= and the base destructor; nested ifs with `mov al,1` / shared `xor al,al`.
// Model: local (engine/CAIStateGroup_EnemyCreatureBase.h flattens the base).
class CThingCreatureBase;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThingCreatureBase* GetPItem() const;           // 0x00A01B50
    void SetPItem(CThingCreatureBase* item);        // 0x00A01B90
};

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                      // slot 11 (+0x2c)
    virtual void v12(); virtual void v13(); virtual void v14();
    virtual bool IsStateGroupStillValid();          // slot 15 (+0x3c)
    void* PBrain;                                   // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CIntelligentPointerCreature PEnemy;             // +0x14
    CThingCreatureBase* GetEnemy();                 // 0x0091DB50
    virtual bool IsRunnable();
};

bool CAIStateGroup_EnemyCreatureBase::IsRunnable()
{
    CThingCreatureBase* enemy = GetEnemy();
    if (enemy != 0) {
        PEnemy = CIntelligentPointerCreature(enemy);
        if (IsStateGroupStillValid())
            return true;
    }
    return false;
}