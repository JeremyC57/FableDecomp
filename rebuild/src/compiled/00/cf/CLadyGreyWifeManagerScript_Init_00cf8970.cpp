// CLadyGreyWifeManagerScript::Init() -- fetch the "LadyGrey" thing; when she is alive, put her in limbo (unless already), teleport her to the hero's position and make her persistent
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF8970, 248 bytes (+ int3 padding). Quest-script Init.
// Callees: CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0, CCountedPointer::operator= 0x00704580 (the inline
//   CScriptThing assignment), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC; CScriptThing slots 6 GetPos / 75 IsAlive;
//   CGameScriptInterface (+0x40) slots 70 GetHero, 72 GetThingWithScriptName, 453 EntitySetInLimbo, 454 IsEntityInLimbo,
//   473 EntityTeleportToPosition, 477 SetThingPersistent.
// Layout: CScriptBase 0x40 + GSI +0x40 / Owner +0x44; LadyGrey +0x48 (Ego_r.pdb CLadyGreyWifeManagerScript +0x5c less the
//   0x14 debug-container bytes).
// Shape: the landed CQ_GuildTrainingDepartureScript model (implicit string conversion first; the returned CScriptThing
//   temporary's inline destructor = vtable store + counted-pointer release + ~CBaseClass); the member assignment is the implicit
//   CScriptThing::operator= forwarding to the out-of-line CCountedPointer::operator=; IsAlive on the member dispatches through
//   the vtable (+0x12c).
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
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
inline CScriptThing::~CScriptThing() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
inline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};
struct CLadyGreyWifeManagerScript : CQuestScriptBase {
    CScriptThing LadyGrey;                               // +0x48 (Ego_r.pdb +0x5c less 0x14)
    void Init();
};

void CLadyGreyWifeManagerScript::Init()
{
    LadyGrey = GSI->GetThingWithScriptName("LadyGrey");
    if (LadyGrey.IsAlive()) {
        if (!GSI->IsEntityInLimbo(LadyGrey))
            GSI->EntitySetInLimbo(LadyGrey, true, false);
        GSI->EntityTeleportToPosition(LadyGrey, GSI->GetHero().GetPos(), 0.0f, false, false);
        GSI->SetThingPersistent(LadyGrey, true);
    }
}