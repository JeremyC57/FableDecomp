// CKG_Sheriff::Init() -- the Knothole Glade sheriff script's set-up: clear ToldHeroToGatherGuards and attach its own thing (+8) to the "VILLAGE_KNOTHOLEGLADE" village through the script interface
// Leaf: ?Init@CKG_Sheriff@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D1DFF0, length 147 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("VILLAGE_KNOTHOLEGLADE" @ 0x012CAECC, -1), the script
// interface (+4) virtual slots 72 (+0x120, GetThingWithScriptName: CScriptThing by hidden pointer) and 492 (+0x7b0,
// EntityAttachToVillage), the CScriptThing temporary's inline destructor (vtable 0x01238C8C, counted pimpl release:
// destroy + operator delete @ 0x00BFE9BC, CBaseClass::~CBaseClass @ 0x0099A2E0), CCharString::~CCharString @ 0x0099EAE0.
// Model: exactly the landed sibling CGuardToHelpDefend::Init 0x00D1EAE0 (same village); Ego_r.pdb CKG_Sheriff has the
// interface at +4, its script thing at +8, ParentClass +0x14, PMasterData +0x18 and ToldHeroToGatherGuards +0x1c.
// Shape: the village name is a named local in its own block; the nested call fetches the interface vtable once (edi).
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
struct CScriptThingImp;
struct CScriptThingPImp {                          // CCountedPointer<CScriptThingImp>
    CScriptThingImp* Data;
    SCountBlock*     Info;
    __forceinline ~CScriptThingPImp()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CScriptThingPImp PImp;
    __forceinline ~CScriptThing() {}
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern const char g_knotholeGlade[];               // 0x012CAECC ("VILLAGE_KNOTHOLEGLADE")
struct CQ_BreakSiegeScript;
struct CQ_SunnyvaleMasterData;
struct CKG_Sheriff {
    virtual void v00();
    CGameScriptInterface*   Interface;             // +0x04
    CScriptThing            Me;                    // +0x08
    CQ_BreakSiegeScript*    ParentClass;           // +0x14
    CQ_SunnyvaleMasterData* PMasterData;           // +0x18
    bool                    ToldHeroToGatherGuards;   // +0x1c
    void Init();
};

void CKG_Sheriff::Init()
{
    ToldHeroToGatherGuards = false;
    {
        CCharString villageName(g_knotholeGlade, -1);
        Interface->EntityAttachToVillage(Me, Interface->GetThingWithScriptName(villageName));
    }
}