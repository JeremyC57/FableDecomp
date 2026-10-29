// Behaviour test for 00487de0: the model below is a copy of src/00487de0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#define __int64 long long
#endif
template <class T> T* AddressOfTemporary(const T& t) { return const_cast<T*>(&t); }
class CBaseObject;

class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char m_stateFlags;                         // +0x91 (bit 0: dead / being deleted)
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action); // 0x006644F0
};
class CThingPlayerCreature : public CThingCreatureBase { public: };

bool __fastcall IsThingCarryingWeapon(const CThing* thing);   // 0x006A1540

inline bool IsThingAlive(const CThing* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

class CCreatureActionBase { public:                            // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
};
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public:   // retail sizeof 0xb0
    unsigned char PNextAction[8];                       // +0xa8 CCountedPointer<CCreatureActionBase>
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, CCreatureActionBase* next, long kind);   // 0x0069FFD0
    ~CCreatureAction_SheatheItemToInventory();          // 0x0048C950 (out-of-line copy)
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                                 // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CGlobalDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x3c - 0x08];
    long ControlSchemePC;                               // +0x3c
    long ControlSchemePCAggressive;                     // +0x40
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) {}                        // test-only: lets the fixture build the holder
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CDefPointer() { Object->Release(); }
    T* operator->() const { return Object; }
};

class CGameDefinitionManager { public:
    unsigned char _pad00[0xd0];
    CDefPointer<CGlobalDef> GlobalDef;                  // +0xd0
};

class CGamePlayerInterface { public:
    void SetControlScheme(long scheme);                 // 0x00446EF0
};

class CPlayer { public:
    void*                   __vftable;
    unsigned char           _pad04[0x0c - 0x04];
    CGamePlayerInterface*   PlayerInterface;            // +0x0c
    unsigned char           _pad10[0x18 - 0x10];
    CGameDefinitionManager* DefinitionManager;          // +0x18
    unsigned char           _pad1c[0x2c - 0x1c];
    CIntelligentPointer<CThingPlayerCreature> ControlledCreature;   // +0x2c
    unsigned char           _pad34[0x217 - 0x34];
    bool                    AggressiveMode;             // +0x217
    void SetUpControlsBasedOnAggressiveMode();
};

void CPlayer::SetUpControlsBasedOnAggressiveMode()
{
    CDefPointer<CGlobalDef> globalDef = DefinitionManager->GlobalDef;
    PlayerInterface->SetControlScheme(AggressiveMode ? globalDef->ControlSchemePCAggressive : globalDef->ControlSchemePC);
    if (!AggressiveMode && IsThingAlive(ControlledCreature.GetPItem()) && IsThingCarryingWeapon(ControlledCreature.GetPItem()))
        ControlledCreature.GetPItem()->SetCurrentAction(*(AddressOfTemporary(CCreatureAction_SheatheItemToInventory(ControlledCreature.GetPItem(), 0, 100))));
}

// ---- test doubles and driver ----
static long g_scheme = -1; static int g_destroyed = 0; static int g_actions = 0; static int g_dtors = 0;
static CThingCreatureBase* g_actionOwner = 0; static long g_kind = 0; static CThingCreatureBase* g_setOn = 0;
static bool g_carrying = false;
void CGamePlayerInterface::SetControlScheme(long scheme) { g_scheme = scheme; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::v00() {}
void CDefPointeeBase::v00() {}
void CDefPointeeBase::Destroy() { ++g_destroyed; }
void CCreatureActionBase::v00() {}
bool __fastcall IsThingCarryingWeapon(const CThing*) { return g_carrying; }
CCreatureAction_SheatheItemToInventory::CCreatureAction_SheatheItemToInventory(CThingCreatureBase* owner, CCreatureActionBase*, long kind) { g_actionOwner = owner; g_kind = kind; }
CCreatureAction_SheatheItemToInventory::~CCreatureAction_SheatheItemToInventory() { ++g_dtors; }
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase& a_ref) { CCreatureActionBase* a = const_cast<CCreatureActionBase*>(&a_ref); g_setOn = this; ++g_actions; return a != 0; }
int main()
{
    static CGlobalDef def; def.RefCount = 1; def.ControlSchemePC = 7; def.ControlSchemePCAggressive = 9;
    static CGameDefinitionManager dm; dm.GlobalDef.Object = &def;
    static CGamePlayerInterface pi;
    static CThingPlayerCreature hero;
    static CPlayer p; p.PlayerInterface = &pi; p.DefinitionManager = &dm; p.ControlledCreature.PItem = (CBaseObject*)&hero;
    bool ok = true;
    p.AggressiveMode = true; g_carrying = true;
    p.SetUpControlsBasedOnAggressiveMode();
    if (g_scheme != 9 || g_actions != 0 || def.RefCount != 1 || g_destroyed) ok = false;
    p.AggressiveMode = false;
    p.SetUpControlsBasedOnAggressiveMode();
    if (g_scheme != 7 || g_actions != 1 || g_setOn != &hero || g_actionOwner != &hero || g_kind != 100 || g_dtors != 1 || def.RefCount != 1) ok = false;
    g_carrying = false;
    p.SetUpControlsBasedOnAggressiveMode();
    if (g_actions != 1) ok = false;
    g_carrying = true; hero.m_stateFlags = 1;        // dead creature: no action
    p.SetUpControlsBasedOnAggressiveMode();
    if (g_actions != 1) ok = false;
    hero.m_stateFlags = 0; p.ControlledCreature.PItem = 0;
    p.SetUpControlsBasedOnAggressiveMode();
    if (g_actions != 1) ok = false;
    def.RefCount = 0;                                // copy add-refs to 1, release drops to 0 -> Destroy
    p.SetUpControlsBasedOnAggressiveMode();
    if (g_destroyed != 1 || def.RefCount != 0) ok = false;
    if (!ok) { std::printf("00487de0_TEST FAIL\n"); return 1; }
    std::printf("00487de0_TEST PASS\n");
    return 0;
}