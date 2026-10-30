// NScript::CV_BanditTollScript::CM_TollBandit::Main() -- toll-bandit spawner entity script: when the "TollBandit" thing is dead and the script is not done, create the script def's toll-bandit creature at Me's position under the script name "TollBandit"
// Leaf: ?Main@CM_TollBandit@CV_BanditTollScript@NScript@@EAEXXZ
// Identity (naming wave 11): ?Main@CM_TollBandit@CV_BanditTollScript@NScript@@EAEXXZ (Ego_r.pdb / FableWin.pdb name lists); retail vtable 0x012E4D6C slot 1 is stored by the factory 0x00E48950 registered as "M_TollBandit"
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E48A30, 263 bytes (+ int3 padding). Entity-script slot 1 (Main).
// Callees: CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0, CDefString::operator CCharString() 0x00415D70 (the script def
//   0x0143E90C, creature def name at +0x438), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840,
//   IsDone 0x00F35B30; CScriptThing slots 6 GetPos (+0x18) and 76 IsDead (+0x130); CGameScriptInterface slots 72
//   GetThingWithScriptName (+0x120) and 91 CreateCreature (+0x16c).
// Shape: the landed CScriptThing model (inline `~CScriptThing() {}` over the inline counted-pointer release): VC7.1 itself
//   inlines the destructor of the GetThingWithScriptName temporary in the bool initialiser and calls the out-of-line copy
//   for the discarded CreateCreature result; the "TollBandit" implicit conversion is built before the def-string
//   conversion (right-to-left argument order, implicit temporaries first).
namespace NScript { class CPredicateBase; }
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CDefString { public:
    long Index;
    operator CCharString() const;                        // 0x00415D70
};
class CScriptDef { public:
    unsigned char _pad[0x438];
    CDefString TollBanditCreature;                       // +0x438
};
extern CScriptDef* pGlobalScriptDef;                         // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class C3DVector { public: float X, Y, Z; };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CV_BanditTollScript {                // the PDB owner script (naming wave 11); only its nested entity class is modelled
public:
    class CM_TollBandit;                        // the entity class below (naming wave 11)
};
}
using NScript::CV_BanditTollScript;
namespace NScript {
class CV_BanditTollScript::CM_TollBandit : public CEntityScriptBase {
public:
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};
}

void NScript::CV_BanditTollScript::CM_TollBandit::Main()
{
    bool dead = Game->GetThingWithScriptName("TollBandit").IsDead();
    if (dead && !IsActiveThreadTerminating())
        Game->CreateCreature(pGlobalScriptDef->TollBanditCreature, Me.GetPos(), "TollBandit", false);
}