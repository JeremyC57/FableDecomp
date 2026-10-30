// CTestTheInventoryScript::Main() -- inventory test script: clear the +0x48 flag, wait (frame by frame) until the level "Empty" is loaded, spawn the TestForLampOn thread, yield two frames and activate the quest "QuestNotInScripts"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EEEA90, length 368 bytes (plain `ret`).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   @ 0x00CB7940, the implicit global operator new @ 0x00BFEA1A, CSpawnedFuncBase::CSpawnedFuncBase(const CCharString&, unsigned long)
//   @ 0x00CDD450, CScriptBase::AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&) @ 0x00CB7E50,
//   CTestTheInventoryScript::TestForLampOn @ 0x00EEEC00 (the thread's member-function pointer); CGameScriptInterface slots
//   (ghidra_out/cgsi_slots_aligned.tsv) 7 NewScriptFrame, 16 MsgIsLevelLoaded, 276 ActivateQuest.
// Strings: "Empty" @ 0x012F59EC, "TestForLampOn" @ 0x012F59DC, "" @ 0x0122D70E, "QuestNotInScripts" @ 0x012F59C8.
//   CSpawnedFunc0<CTestTheInventoryScript> vtable 0x012F59AC.
// Layout: retail CScriptBase 0x40 bytes, GSI +0x40, the cleared bool at +0x48 (descriptive name) -- the model of the
//   landed sibling CTestTheInventoryScript::TestForLampOff 0x00EEED20.
// Shape: `while (!GSI->MsgIsLevelLoaded("Empty"))` (implicit conversion temporary, neg/sbb/inc negation) with the
//   termination test inside; the spawned thread is the TestForLampOff new-expression (its name a conditional temporary
//   guarded by bit 0 of ebx, whose flag word is zeroed at entry).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};


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

class CScriptBase { public:                                    // retail 0x40 bytes (PDB 0x54: debug containers)
    unsigned char _body[0x40];                          // +0x00 .. +0x40 (vptr, PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
};
struct CTestTheInventoryScript : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    unsigned char         _pad44[4];
    bool                  TestFlag;                     // +0x48 (descriptive)
    void Main();
    void TestForLampOn();                               // 0x00EEEC00
};

void CTestTheInventoryScript::Main()
{
    TestFlag = false;
    while (!GSI->MsgIsLevelLoaded("Empty")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc0<CTestTheInventoryScript>("TestForLampOn", 0, this,
                                                                 &CTestTheInventoryScript::TestForLampOn),
                       CCharString("", -1));
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    GSI->ActivateQuest("QuestNotInScripts");
}