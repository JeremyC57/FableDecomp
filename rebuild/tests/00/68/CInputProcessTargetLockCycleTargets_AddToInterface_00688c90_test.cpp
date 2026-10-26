#include <stdio.h>
// Behaviour test for CInputProcessTargetLockCycleTargets::AddToInterface (0x00688C90).
static int g_destroyed = 0, g_base = 0; static bool g_baseResult = true;
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
};
struct CDefObject : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(T* p) : Object(p) { }
    ~CDefPointer()
    {
        if (Object) {
            Object->ReleaseTemporary();
            Object = 0;
        }
    }
    CDefPointer& operator=(const CDefPointer& o)
    {
        if (Object != o.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = o.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
    T* operator->() const { return Object; }
};
class CHeroCombatDef : public CDefObject { public:
    unsigned char _pad08[0xe4 - 0x08];
    float         ClickPressureThreshold;          // +0xe4
};
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                        // +0x91 bit 0
    unsigned char _bits91 : 7;
    CHeroCombatDef* Def;
    CDefPointer<CHeroCombatDef> GetHeroCombatDef() const { ++Def->m_count; return CDefPointer<CHeroCombatDef>(Def); }
};
class CPlayer {
public:
    CThing* Creature;
    CThing* GetControlledCreature() { return Creature; }
};
bool g_targetLockAllowPulsesOnly = true;
class CInputProcessManager;
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00() {}
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    virtual bool AddToInterface(CInputProcessManager&) { ++g_base; return g_baseResult; }
};
class CInputProcessTargetLockCycleTargets : public CInputProcessGameBase {
public:
    float LastButtonPressure;                      // +0x28
    float ButtonPressureThresholdForClick;         // +0x2c
    long  ClickAcceptancePulseTimer;               // +0x30
    bool  AllowPulsesOnly;                         // +0x34
    virtual bool AddToInterface(CInputProcessManager& manager);
};

struct CHeroCombatDefHolder {
    CHeroCombatDef* Object;
    CHeroCombatDefHolder() : Object(0) { }
    ~CHeroCombatDefHolder() { if (Object) { if (--Object->m_count == 0) Object->Destroy(); } }
    CHeroCombatDefHolder& operator=(const CDefPointer<CHeroCombatDef>& o)
    {
        if (Object != o.Object) {
            CHeroCombatDef* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = o.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
    CHeroCombatDef* operator->() const { return Object; }
};
bool CInputProcessTargetLockCycleTargets::AddToInterface(CInputProcessManager& manager)
{
    LastButtonPressure = 0.0f;
    ClickAcceptancePulseTimer = -1;
    AllowPulsesOnly = g_targetLockAllowPulsesOnly;
    CHeroCombatDefHolder def;
    CThing* creature = MyPlayer->GetControlledCreature();
    if (creature && !creature->Dead) {
        def = creature->GetHeroCombatDef();
        ButtonPressureThresholdForClick = def->ClickPressureThreshold;
    } else {
        ButtonPressureThresholdForClick = 0.75f;
    }
    bool added = CInputProcessGameBase::AddToInterface(manager);
    return added;
}

int main()
{
    int ok = 1;
    CHeroCombatDef def; def.m_count = 1; def.ClickPressureThreshold = 0.25f;
    CThing c; c.Dead = 0; c.Def = &def; CPlayer pl; pl.Creature = &c;
    CInputProcessTargetLockCycleTargets p; p.MyPlayer = &pl; p.LastButtonPressure = 5.0f; p.AllowPulsesOnly = false;
    CInputProcessManager* m = 0;
    bool r = p.AddToInterface(*m);
    if (!r || g_base != 1 || p.ButtonPressureThresholdForClick != 0.25f || p.LastButtonPressure != 0.0f || p.ClickAcceptancePulseTimer != -1 || !p.AllowPulsesOnly) ok = 0;
    if (def.m_count != 1 || g_destroyed != 0) ok = 0;
    c.Dead = 1; g_baseResult = false;
    r = p.AddToInterface(*m);
    if (r || p.ButtonPressureThresholdForClick != 0.75f) ok = 0;
    pl.Creature = 0; p.ButtonPressureThresholdForClick = 0;
    p.AddToInterface(*m);
    if (p.ButtonPressureThresholdForClick != 0.75f || g_base != 3) ok = 0;
    if (ok) { printf("00688c90_TEST PASS\n"); return 0; }
    printf("00688c90_TEST FAIL\n"); return 1;
}