// NScript::CV_BanditTollScript::CM_BanditLeader::Main() -- bandit-leader spawner: when the "BanditLeader" thing is dead (or gone), spawn a new one here (creature def from this marker's data string, scripted "BanditLeader") and attach it to the "BanditTollVillage"
// Leaf: Main
// Identity (naming wave 11): ?Main@CM_BanditLeader@CV_BanditTollScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012E4F94 slot 1 is stored by the factory 0x00E4A660 registered as "M_BanditLeader"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E4A740, length 335 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("BanditLeader" @ 0x012E4C14,
//   "BanditTollVillage" @ 0x012E4FB0), IsDone @ 0x00F35B30, ~CScriptThing @ 0x004AA840 (inlined at the first temporary,
//   out of line at the other two), CBaseClass::~CBaseClass @ 0x0099A2E0, operator delete @ 0x00BFE9BC (the count block);
//   CScriptThing slots 3 GetDataString, 6 GetPos, 76 IsDead (virtual on the returned temporary); CGameScriptInterface
//   (vtable 0x01260F0C) slots 72 GetThingWithScriptName, 91 CreateCreature, 492 EntityAttachToVillage.
// Layout: entity-script base {vptr, Game +4, CScriptThing Me +8}; CScriptThing {vptr, CCountedPointer PImp {object,
//   count block}} over CBase as the landed CScriptThing scalar deleting destructor 0x004AB170.
// Shape: the dead test is the `if` condition on the returned temporary (destroyed before the name string; VC7.1 inlines
//   the destructor at this first site only, given an inline `virtual ~CScriptThing() { }`); the new leader is scoped
//   inside the branch so it shares the frame words of the first temporary (function scope moves it, DIFFER 335v335).
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public: float x, y, z; };
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> class CCountedPointer { public:
    void*        m_pObject;                        // +0
    SCountBlock* m_pCount;                         // +4
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::~CScriptThing() { }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CV_BanditTollScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CM_BanditLeader;                        // the entity class below (naming wave 11)
};
}
using NScript::CV_BanditTollScript;
namespace NScript {
class CV_BanditTollScript::CM_BanditLeader : public CEntityScriptBase {
public:
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CV_BanditTollScript::CM_BanditLeader::Main()
{
    if (Game->GetThingWithScriptName("BanditLeader").IsDead()) {
        if (IsActiveThreadTerminating())
            return;
        CScriptThing leader = Game->CreateCreature(Me.GetDataString(), Me.GetPos(), "BanditLeader", false);
        Game->EntityAttachToVillage(leader, Game->GetThingWithScriptName("BanditTollVillage"));
    }
}