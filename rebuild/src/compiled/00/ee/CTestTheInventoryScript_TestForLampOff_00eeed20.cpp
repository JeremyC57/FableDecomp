// CTestTheInventoryScript::TestForLampOff() -- test script thread: wait (frame by frame) while the hero's hand lamp is lit; then show the lamp-off message for 3 seconds, pause 3 seconds and spawn the TestForLampOn thread
// Leaf: ?TestForLampOff@CTestTheInventoryScript
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EEED20, 274 bytes (+14 int3), `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940, operator new @ 0x00BFEA1A, CSpawnedFuncBase::
//   CSpawnedFuncBase(const CCharString&, unsigned long) @ 0x00CDD450, CScriptBase::AddSpawnedFunction(CSpawnedFuncBase*,
//   const CCharString&) @ 0x00CB7E50, CTestTheInventoryScript::TestForLampOn @ 0x00EEEC00 (stored as the thread's
//   member-function pointer), CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0; CGameScriptInterface
//   slots 7 NewScriptFrame (+0x1c), 185 IsHeroHandLampLit (+0x2e4), 111 ShowOnScreenMessage (+0x1bc), 376 Pause (+0x5e0)
//   (vtable 0x01260F0C).
// Strings: "You've turned the lamp off. I can't describe how happy that makes me." @ 0x012F5A30, "TestForLampOn" @ 0x012F59DC,
//   "" @ 0x0122D70E. CSpawnedFunc0<CTestTheInventoryScript> vtable 0x012F59AC.
// Layout: retail CScriptBase 0x40 bytes, GSI +0x40 (the landed CQ_FireHeartScript::CheckCompletion 0x00D3AD60 model).
// Shape: the top-tested lamp wait loop with the termination test inside; the message is an implicit conversion temporary
//   (built before the 3.0f push); the spawned thread is the FireHeart new-expression (its name a conditional temporary
//   guarded by bit 0 of ebx, the empty region name an explicit temporary).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};

void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A

// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"

template <class T> struct CSpawnedFunc0 : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc0(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CTestTheInventoryScript : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    void TestForLampOff();
    void TestForLampOn();                               // 0x00EEEC00
};

void CTestTheInventoryScript::TestForLampOff()
{
    while (GSI->IsHeroHandLampLit()) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->ShowOnScreenMessage("You've turned the lamp off. I can't describe how happy that makes me.", 3.0f);
    GSI->Pause(3.0f);
    AddSpawnedFunction(new CSpawnedFunc0<CTestTheInventoryScript>("TestForLampOn", 0, this,
                                                                 &CTestTheInventoryScript::TestForLampOn),
                       CCharString("", -1));
}