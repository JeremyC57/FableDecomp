// NScript::CProximityPodiumTestScript::Main() -- bind the "ProximityPodium" entity script (CProximityPodiumTestScript::CProximityPodium::Alloc), finish adding the scripted entities here and in the game interface
// Leaf: ?Main@CProximityPodiumTestScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EFB1E0, 137 bytes, thiscall, plain `ret`.
// Identity (probable): slot 2 (Main) of the script vtable 0x012F6AD8 (stored by the landed CProximityPodiumTestScript constructor); no PDB entry (test script); the
//   binding's allocator 0x00EFB280 is the landed CProximityPodiumTestScript::CProximityPodium::Alloc and its name literal 0x012F6B10 is "ProximityPodium".
// Callees: scalar operator new 0x00BFEA1A (0x1C = sizeof(CEntityScriptBinding<CProximityPodiumTestScript>)), CCharString(const char*, long)
//   0x0099EBF0 / CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding
//   0x00CB8230, CScriptBase::PostAddScriptedEntities 0x00CB8930, CGameScriptInterface::PostAddScriptedEntities (slot 64,
//   +0x100); the binding vtable 0x012F6B08.
// Shape (family K071, representative 0x00E96F10): the landed CQ_GuildTrainingWoodsDepartureScript::Main 0x00D61D20 binding
//   idiom with flags 0: the conditional name temporary of the new-expression is tracked in ebx and destroyed after the call.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
#include "engine/CCountedPointer.h"
class __declspec(novtable) CEntityScriptBindingBase {
public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> class CEntityScriptBinding : public CEntityScriptBindingBase {
public:
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
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript { class CDanyalTestScriptsData; }
namespace NScript {
class CProximityPodiumTestScript : public CLevelScriptBase<NScript::CDanyalTestScriptsData> {
public:
    class CProximityPodium { public: static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CProximityPodiumTestScript& parent, CScriptDataBase* data, const CScriptThing& me); };   // 0x00EFB280
    virtual void Main();
};
void CProximityPodiumTestScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CProximityPodiumTestScript>("ProximityPodium", this, &CProximityPodium::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
}
}