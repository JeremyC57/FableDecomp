// CV_DemonDoorsScript::FadeOutFaceAndOpenDoor(const CScriptThing& face, const CCharString& doorName) -- fade the demon-door face out over three seconds, open the named door and make it persistent, then remove the face
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6F040, length 169 bytes (`ret 8`; 176 with padding).
// Callees: FadeOutAndKillEntity @ 0x00CBE716 (`__fastcall`: thing in ecx, the first bool in dl, the float and the
// second bool on the stack), CBaseClass::~CBaseClass @ 0x0099A2E0, operator delete @ 0x00BFE9BC, and through the interface
// (+0x40): GetThingWithScriptName (slot 72, +0x120, CScriptThing by hidden pointer), OpenDoor (slot 425, +0x6a4),
// SetThingPersistent (slot 477, +0x774), RemoveThing(const CScriptThing&, bool, bool) (slot 108, +0x1b0).
// Identity: the catalog label stands (no member is touched beyond the interface pointer).
// Shape: the door handle is a named local whose in-class inline destructor (vtable store, counted release, zeroed
// words, base destructor) runs at the end; the zero register of RemoveThing's `false` is reused by that destructor.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

struct SScriptThingInfo {                                   // the counted block behind CScriptThing::PImp_Info
    long m_references;                                      // +0
    void (__fastcall* m_pfnDestroy)(void*);                 // +4
    void* m_pObject;                                        // +8
};

class CScriptThing : public CBaseClass { public:                               // vtable 0x01238C8C, 12 bytes
    void*             PImp_Data;                            // +0x04
    SScriptThingInfo* PImp_Info;                            // +0x08
    CScriptThing(const CScriptThing& other);
    ~CScriptThing()
    {
        if (PImp_Info) {
            --PImp_Info->m_references;
            if (PImp_Info->m_references == 0) {
                PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
};
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, float seconds, bool kill, bool fadeIn);   // 0x00CBE716
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

namespace NScript {
class CV_DemonDoorsScript : public CQuestScriptBase {
public:
    void FadeOutFaceAndOpenDoor(const CScriptThing& face, const CCharString& doorName);
};
} // namespace NScript
using NScript::CV_DemonDoorsScript;

void NScript::CV_DemonDoorsScript::FadeOutFaceAndOpenDoor(const CScriptThing& face, const CCharString& doorName)
{
    FadeOutAndKillEntity(face, 3.0f, false, true);
    CScriptThing door = GSI->GetThingWithScriptName(doorName);
    GSI->OpenDoor(door);
    GSI->SetThingPersistent(door, true);
    GSI->RemoveThing(face, false, true);
}