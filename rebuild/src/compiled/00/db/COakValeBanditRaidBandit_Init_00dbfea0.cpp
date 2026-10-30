// COakValeBanditRaidBandit::Init() -- set the raid bandit's sight and hearing radii from the script def, make him non-turncoatable and open the "OakValeBanditRaidGate" door
// Leaf: ?Init@COakValeBanditRaidBandit@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DBFEA0, 210 bytes (+ int3 padding). __fastcall, `ret`.
// Callees: CGameScriptInterface virtuals (ghidra_out/cgsi_slots_aligned.tsv): EntitySetSightRadius 640 (+0xa00) 0x0089D220,
//   EntitySetSoundRadius 638 (+0x9f8) 0x0089D160, SetIsThingTurncoatable 744 (+0xba0) 0x008A1950, GetThingWithScriptName 72
//   (+0x120) 0x008A7D60, OpenDoor 425 (+0x6a4) 0x00895DB0; CCharString::CCharString(const char*, long) 0x0099EBF0 and ~CCharString
//   0x0099EAE0; the CScriptThing temporary's inline destructor (vtable 0x01238C8C, counted pimpl release: destroy + operator
//   delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0).
// Layout: entity-script base {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14, PMasterData +0x18 (Ego_r.pdb, 28 bytes);
//   pGlobalScriptDef (0x0143E90C) OVBRBanditSightRadius +0xd5c / OVBRBanditSoundRadius +0xd60 (PDB +0xe0c / +0xe10, the retail def
//   is 0xb0 smaller before them).
// Shape: as the landed CM_HiddenBooty::Init 0x00E93080: the gate name is a scoped CCharString local and the door lookup is
//   nested in the OpenDoor call (the outer vtable fetched first).
void __cdecl operator delete(void* p);             // 0x00BFE9BC
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
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
class CScriptThing : public CBaseClass { public:                 // 12 bytes, vtable 0x01238C8C
    CScriptThingPImp PImp;
    __forceinline ~CScriptThing() {}
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad[0xd5c];
    float         OVBRBanditSightRadius;           // +0xd5c
    float         OVBRBanditSoundRadius;           // +0xd60
};
extern CScriptDef* pGlobalScriptDef;                   // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
struct CQ_OakValeBanditRaidScript;
struct CQ_SunnyvaleMasterData;
struct COakValeBanditRaidBandit {
    virtual void v00();
    CGameScriptInterface*       Game;              // +0x04
    CScriptThing                Me;                // +0x08
    CQ_OakValeBanditRaidScript* ParentClass;       // +0x14
    CQ_SunnyvaleMasterData*     PMasterData;       // +0x18
    void Init();
};

void COakValeBanditRaidBandit::Init()
{
    Game->EntitySetSightRadius(Me, pGlobalScriptDef->OVBRBanditSightRadius);
    Game->EntitySetSoundRadius(Me, pGlobalScriptDef->OVBRBanditSoundRadius);
    Game->SetIsThingTurncoatable(Me, false);
    {
        CCharString gateName("OakValeBanditRaidGate", -1);
        Game->OpenDoor(Game->GetThingWithScriptName(gateName));
    }
}