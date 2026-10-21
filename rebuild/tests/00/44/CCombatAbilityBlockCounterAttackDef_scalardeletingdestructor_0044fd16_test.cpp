// test for 0044fd16 CCombatAbilityBlockCounterAttackDef scalar deleting destructor
#include <stdio.h>
#include <string.h>
#include <new>
class CResourceList;
class CResource {
public:
    unsigned long  IVRefCount;                           // +0x04
    CResourceList* ResourceList;                         // +0x08
    CResource*     PrevResource;                         // +0x0c
    CResource*     NextResource;                         // +0x10
    unsigned long  ResourceSize;                         // +0x14
    unsigned long  LastUsedFrame;                        // +0x18
    virtual ~CResource();                                // 0x009FC550
};
class CDefPointeeBase : public CResource {
public:
    virtual ~CDefPointeeBase() {}
};
class CDefinitionManager;
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {
public:
    CDefinitionManager* PDefManager;                     // +0x1c
    unsigned long       GlobalIndex;                     // +0x20
    unsigned char       Setup : 1;                       // +0x24
    unsigned char       Template : 1;
    unsigned char       DefaultValsApplied : 1;
};
#pragma pack(pop)
class CCombatAbilityData {
public:
    virtual ~CCombatAbilityData() {}
    long  PercentageChance;                              // +0x04
    float SecondsDuration;                               // +0x08
    float SecondsToWaitBeforeRepeat;                     // +0x0c
    long  Anim;                                          // +0x10 CDefString
    long  InterfaceType;                                 // +0x14 ECombatAbilityInterfaceType
};
class CCombatAbilityMeleeAttackData : public CCombatAbilityData {
public:
    unsigned char _rest[0x8];                        // +0x18
};
class CCombatAbilityAttackBase : public CDefClassBase {
public:
    CCombatAbilityMeleeAttackData CombatAbility;          // +0x28
};
class CCombatAbilityBlockCounterAttackDef : public CCombatAbilityAttackBase {
};
void ConstructCCombatAbilityBlockCounterAttackDef(void* p)          // emission helper, not a retail function: the vtable (and its ??_G) is emitted
{
    new (p) CCombatAbilityBlockCounterAttackDef;
}

static int g_res; static void* g_resthis;
CResource::~CResource() { ++g_res; g_resthis = this; }
int main()
{
    static double buf[0x20];
    ConstructCCombatAbilityBlockCounterAttackDef(buf);
    CCombatAbilityBlockCounterAttackDef* p = (CCombatAbilityBlockCounterAttackDef*)buf;
    p->~CCombatAbilityBlockCounterAttackDef();                                       // virtual: ??_G with flags 0, no free
    int ok = g_res == 1 && g_resthis == (CResource*)p && (char*)&p->CombatAbility - (char*)p == 0x28;
    CCombatAbilityBlockCounterAttackDef* h = new CCombatAbilityBlockCounterAttackDef;
    CDefPointeeBase* b = h;
    delete b;                                            // virtual: ??_G with flags 1
    ok &= g_res == 2 && g_resthis == (CResource*)h;
    printf(ok ? "0044fd16_TEST PASS\n" : "0044fd16_TEST FAIL\n");
    return ok ? 0 : 1;
}