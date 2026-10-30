// CMK_HS2_SOUL::Main() -- Hero Souls 2 trapped-soul marker (first soul): wait until the quest has freed a soul, then keep a "DG_SOUL_TRAPPED" effect alive at Me (oriented by Me's XY angle), re-creating it whenever it dies, until the script is done
// Levers: ~CScriptThing __forceinline; effect bound as `CScriptThing& effect = Game->CreateEffect(...)` (VC binds the temporary: IsAlive stays virtual); recreate as `effect.PImp = Game->CreateEffect(...).PImp` with depth 0 just before the `;` (temp dtor out of line, counted operator= still direct); for(;;){ if (IsActiveThreadTerminating()) return; ...}
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE0920, length 470 bytes (+ int3 padding). Three CMK_HS2_SOUL Mains (0x00CE0920 /
//   0x00CE0B00 / 0x00CE0CE0) differ only in the soul threshold (0 / 1 / 2).
// Callees: IsDone @ 0x00F35B30; CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("" 0x0122D70E,
//   "DG_SOUL_TRAPPED" 0x012C3568); CScriptThing slots 6 GetPos (+0x18), 10 GetAngleXY (+0x28), 75 IsAlive (+0x12c), ~CScriptThing
//   @ 0x004AA840, the implicit CScriptThing::operator= forwarding to CCountedPointer::operator= @ 0x00704580, ~CBaseClass @ 0x0099A2E0,
//   operator delete @ 0x00BFE9BC; CGameScriptInterface slots 7 NewScriptFrame, 100 CreateEffect (+0x190).
// Layout: the entity-script base {vptr, Game +4, Me +8}; the quest object at +0x18 with the freed-soul count at +0x6c. CScriptThing /
//   CCountedPointer as the landed CLadyGreyWifeManagerScript::Init 0x00CF8970.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580
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
struct CScriptThingImp;
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
__forceinline CScriptThing::~CScriptThing() {}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_HeroSouls2Script {
    unsigned char _pad[0x6c];
    long          SoulsFreed;                      // +0x6c (descriptive)
};

// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

class CMK_HS2_SOUL : public CEntityScriptBase {
public:
        unsigned char         _pad14[4];
    CQ_HeroSouls2Script*  ParentClass;             // +0x18
    void Main();
};

void CMK_HS2_SOUL::Main()
{
    while (ParentClass->SoulsFreed <= 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing& effect = Game->CreateEffect("DG_SOUL_TRAPPED", Me.GetPos(), "", Me.GetAngleXY(), false, false);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (!effect.IsAlive()) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            effect.PImp = Game->CreateEffect("DG_SOUL_TRAPPED", Me.GetPos(), "", Me.GetAngleXY(), false, false).PImp
#pragma inline_depth(0)
            ;
#pragma inline_depth()
        }
        Game->NewScriptFrame();
    }
}