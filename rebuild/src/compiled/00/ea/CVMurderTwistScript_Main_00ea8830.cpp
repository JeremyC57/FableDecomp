// CV_MurderTwistScript::Main() -- the Murder Twist quest main: bind the leader / brothers / friends / buried-gem entity scripts and spawn the MagicRuneProcess and GemsFoundProcess threads
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA8830, 0x3da bytes, plain `ret`.
// Callees: operator new 0x00BFEA1A, CCharString 0x0099EBF0 / 0x0099EC30 / ~ 0x0099EAE0, AddEntityScriptBinding 0x00CB8230
//   (binding vtable 0x012EE73C), CScriptBase::PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase 0x00CDD450 (spawn vtable
//   0x012EE648), AddSpawnedFunction 0x00CB7E50; threads MagicRuneProcess 0x00EA8C20, GemsFoundProcess 0x00EAD9F0;
//   CGameScriptInterface slot 64.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
#include "engine/CCountedPointer.h"

class __declspec(novtable) CEntityScriptBindingBase { public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

class CGameScriptInterface;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CV_MurderTwistScript : CScriptBase {
    struct CMurderTwistLeader {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EA8E50
    };
    struct CMurderTwistBrotherYounger {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EAAE40
    };
    struct CMurderTwistBrother {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EAAF10
    };
    struct CMurderTwistBrotherOlder {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EABA10
    };
    struct CMurderTwistBrotherFriend {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EAC4E0
    };
    struct CMurderTwistBrotherOlderFriend {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EACDE0
    };
    struct CMurderTwistBuriedGem {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_MurderTwistScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EAD730
    };
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    void Main();
    void MagicRuneProcess();                             // 0x00EA8C20
    void GemsFoundProcess();                             // 0x00EAD9F0
};

void CV_MurderTwistScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MurderTwistScript>("MurderTwistLeader", this, &CMurderTwistLeader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MurderTwistScript>("MurderTwistBrotherYounger", this, &CMurderTwistBrotherYounger::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MurderTwistScript>("MurderTwistBrother", this, &CMurderTwistBrother::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MurderTwistScript>("MurderTwistBrotherOlder", this, &CMurderTwistBrotherOlder::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MurderTwistScript>("MurderTwistBrotherFriend", this, &CMurderTwistBrotherFriend::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MurderTwistScript>("MurderTwistBrotherOlderFriend", this, &CMurderTwistBrotherOlderFriend::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_MurderTwistScript>("MurderTwistBuriedGem", this, &CMurderTwistBuriedGem::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_MurderTwistScript>("MagicRuneProcess", 0, this, &CV_MurderTwistScript::MagicRuneProcess), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_MurderTwistScript>("GemsFoundProcess", 0, this, &CV_MurderTwistScript::GemsFoundProcess), CCharString(""));
}