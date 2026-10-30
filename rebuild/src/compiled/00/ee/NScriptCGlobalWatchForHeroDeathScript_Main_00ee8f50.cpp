// NScript::CGlobal_WatchForHeroDeathScript::Main() -- spawn the script's "WatchForHeroDeath" thread: AddSpawnedFunction(new CSpawnedFunc<CGlobal_WatchForHeroDeathScript>("WatchForHeroDeath", 0, this, &CGlobal_WatchForHeroDeathScript::WatchForHeroDeath), "")
// Leaf: ?Main@CGlobal_WatchForHeroDeathScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE8F50, 144 bytes, plain `ret`. Slot 2 (Main) of the script vtable @ 0x012F5470.
// Identity: the vtable is stored by the script's Alloc, which CScriptInfoManager::RegisterScripts (0x00CD52D0) registers under
//   the name "Global_WatchForHeroDeath" (site 0x00CD9A4E); script classes are C<registered name>Script in NScript (the PDB name sets hold
//   ?Main@CGlobal_WatchForHeroDeathScript@NScript@@UAEXXZ). Family F001 (163 byte-identical bodies) -- the manifest label of every member was
//   propagated from one body; this one is CGlobal_WatchForHeroDeathScript's.
// Callees: scalar operator new @ 0x00BFEA1A (0x3c bytes), CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0,
//   CSpawnedFuncBase::CSpawnedFuncBase(const CCharString&, unsigned long) @ 0x00CDD450, CScriptBase::AddSpawnedFunction @ 0x00CB7E50.
// Data: CSpawnedFunc<CGlobal_WatchForHeroDeathScript> vtable @ 0x012F5484; the thread name literal "WatchForHeroDeath" @ 0x012F54A0; the empty region name @ 0x0122D70E;
//   the member pointer is CGlobal_WatchForHeroDeathScript::WatchForHeroDeath @ 0x00EE8FE0 (a plain member, PDB ?WatchForHeroDeath@CGlobal_WatchForHeroDeathScript@NScript@@QAEXXZ).
// Layout: CSpawnedFuncBase is 0x34 bytes (vptr + 0x30), CSpawnedFunc<T> adds the member pointer Func +0x34 and PClass +0x38
//   (0x3c); CScriptBase is retail 0x40 bytes with the virtuals ~CScriptBase, RegisterMain, Main, Init, OnPersist (PDB order;
//   the retail vtable has exactly these five slots).
// Shape: the new-expression's CCharString temporary is conditional (built only when new succeeded), so VC7.1 keeps a flag
//   in ebx and destroys the temporary at the end only if bit 0 is set; the region name is the CCharString built by the inline
//   SpawnFunction helper after the new-expression (arguments of AddSpawnedFunction would otherwise build it first).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CPersistContext;
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
namespace NScript {
class CGlobal_WatchForHeroDeathScript : public CScriptBase {
public:
    virtual void RegisterMain();
    virtual void Main();
    void WatchForHeroDeath();                       // 0x00EE8FE0
};
void CGlobal_WatchForHeroDeathScript::Main()
{
    AddSpawnedFunction(new CSpawnedFunc<CGlobal_WatchForHeroDeathScript>("WatchForHeroDeath", 0, this, &CGlobal_WatchForHeroDeathScript::WatchForHeroDeath), CCharString(""));
}
}