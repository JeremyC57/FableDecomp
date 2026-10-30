// CEntityScriptBinding<NScript::CBenTestScriptsScript>::CreateActiveEntityScript(const CScriptThing& thing, unsigned __int64 uid) -- start an entity script for a newly bound entity: with a class allocator, allocate the entity script object (parent, parent's master data, thing), Init() it and wrap it in a new CActiveEntityScript; otherwise wrap the parent's member function
// Leaf: ?CreateActiveEntityScript@?$CEntityScriptBinding@VCBenTestScriptsScript@NScript@@@@MAE?AV?$CCountedPointer@VCActiveEntityScriptBase@@@@ABVCScriptThing@@_K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EFEE50, 272 bytes, `ret 10h` (hidden result + thing + 64-bit uid). Slot 1 of the
//   CEntityScriptBinding<CBenTestScriptsScript> vtable @ 0x012F6F04 (slot 0 GetEntityScriptName).
// Identity: that binding vtable is stored by 0x00EFE4B0 (the script's Main building `new CEntityScriptBinding<T>(name, this,
//   alloc, flags)`), which sits in the vtable of the script whose Alloc CScriptInfoManager::RegisterScripts (0x00CD52D0)
//   registers as "BenTestScripts" -> NScript::CBenTestScriptsScript; the PDB name sets hold ?CreateActiveEntityScript@?$CEntityScriptBinding@VCBenTestScriptsScript@NScript@@@@MAE?AV?$CCountedPointer@VCActiveEntityScriptBase@@@@ABVCScriptThing@@_K@Z.
//   Family F002 (85 byte-identical bodies): the manifest label (CScriptGameResourceObjectMonsterBase_CreateFromEntity) is propagated.
// Callees: CCountedPointer<CEntityScriptBase>::~CCountedPointer @ 0x00CE1000 (release + zero both words), scalar operator new
//   @ 0x00BFEA1A (0x60 bytes), CActiveEntityScriptBase(const CScriptThing&, unsigned __int64, unsigned long) @ 0x00CE1110,
//   CCountedPointer<CEntityScriptBase>::ShareData @ 0x00CE1040, CCountedPointer<CActiveEntityScriptBase>(CActiveEntityScriptBase*)
//   @ 0x00CE10B0; the class allocator (+0x10, `__fastcall`, hidden result in ecx, parent in edx) and the entity script's Init
//   (CEntityScriptBase slot 2, +0x08). Data: CActiveEntityScript<CBenTestScriptsScript> vtable @ 0x012F6F74.
// Layout (retail = PDB with 4-byte STLport lists / 0x40 CScriptBase): CEntityScriptBinding<T> {vptr, EntityScriptName +4,
//   PParentScript +8, Func +0xc, ClassAllocFunc +0x10, ClassAllocValid +0x14, Flags +0x18}; CActiveEntityScriptBase 0x58
//   (CSpawnedFuncBase 0x34, PEntityScript +0x34, Flags +0x3c, EntityUID +0x40, Entity +0x48); CActiveEntityScript<T> adds
//   Func +0x58 and PParentScript +0x5c (0x60). The parent's +0x44 is CLevelScriptBase<TData>::MasterScriptData (viewed here
//   through its CScriptDataBase base; the data class does not enter any symbol of this function).
// Shape: the counted-pointer destructor must have its body in the TU (`noinline`): with only a declaration VC7.1 swaps the two
//   frame slots of the allocator's result temporary and the named copy and does not keep the copy's Data/Info in ebp/ebx for
//   the later ShareData pushes. Both CActiveEntityScript constructors are `__forceinline` (a plain inline one stays a call).
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
class CCharString {
public:
    void* PStringData;
};
class CCPPointerInfo {                                  // PDB ??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data);                           // 0x00CE10B0 (T = CActiveEntityScriptBase)
    __forceinline CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __declspec(noinline) ~CCountedPointer()            // 0x00CE1000
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
    void ShareData(T* data, CCPPointerInfo* info);      // 0x00CE1040 (T = CEntityScriptBase)
    T* operator->() const { return Data; }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
class CActiveEntityScriptBase : public CSpawnedFuncBase {
public:
    CCountedPointer<CEntityScriptBase> PEntityScript;   // +0x34
    unsigned long Flags;                                // +0x3c
    unsigned __int64 EntityUID;                         // +0x40
    CScriptThing Entity;                                // +0x48 (0x58 with the 8-byte tail alignment)
    CActiveEntityScriptBase(const CScriptThing& entity, unsigned __int64 uid, unsigned long flags);   // 0x00CE1110
};
template <class T> class CActiveEntityScript : public CActiveEntityScriptBase {
public:
    void (T::*Func)(CScriptThing);                      // +0x58
    T* PParentScript;                                   // +0x5c
    __forceinline CActiveEntityScript(const CScriptThing& entity, unsigned __int64 uid, T* parent, void (T::*func)(CScriptThing), unsigned long flags)
        : CActiveEntityScriptBase(entity, uid, flags), Func(func), PParentScript(parent) {}
    __forceinline CActiveEntityScript(const CScriptThing& entity, unsigned __int64 uid, T* parent, CCountedPointer<CEntityScriptBase>& script, unsigned long flags)
        : CActiveEntityScriptBase(entity, uid, flags), PParentScript(parent) { PEntityScript = script; }
    virtual void SuspendableProcess();
};
class CScriptDataBase {
public:
    virtual ~CScriptDataBase();
};
namespace NScript { class CQ_SunnyvaleMasterData : public CScriptDataBase { }; }   // PDB base only (Ego_r.pdb: the master data class derives from CScriptDataBase); its members are not used here
class CGameScriptInterfaceBase;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
class __declspec(novtable) CEntityScriptBindingBase {
public:
    virtual const CCharString& GetEntityScriptName() const = 0;
protected:
    virtual CCountedPointer<CActiveEntityScriptBase> CreateActiveEntityScript(const CScriptThing& thing, unsigned __int64 uid) = 0;
};
template <class T> class CEntityScriptBinding : public CEntityScriptBindingBase {
public:
    typedef CCountedPointer<CEntityScriptBase> (__fastcall* TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString EntityScriptName;                       // +0x04
    T* PParentScript;                                   // +0x08
    void (T::*Func)(CScriptThing);                      // +0x0c
    TAllocFunc ClassAllocFunc;                          // +0x10
    bool ClassAllocValid;                               // +0x14
    unsigned long Flags;                                // +0x18
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
protected:
    virtual CCountedPointer<CActiveEntityScriptBase> CreateActiveEntityScript(const CScriptThing& thing, unsigned __int64 uid);
};
template <class T>
CCountedPointer<CActiveEntityScriptBase> CEntityScriptBinding<T>::CreateActiveEntityScript(const CScriptThing& thing, unsigned __int64 uid)
{
    if (ClassAllocValid) {
        CCountedPointer<CEntityScriptBase> script(ClassAllocFunc(*PParentScript, PParentScript->MasterScriptData, thing));
        script->Init();
        return CCountedPointer<CActiveEntityScriptBase>(new CActiveEntityScript<T>(thing, uid, PParentScript, script, Flags));
    }
    return CCountedPointer<CActiveEntityScriptBase>(new CActiveEntityScript<T>(thing, uid, PParentScript, Func, Flags));
}
namespace NScript {
class CBenTestScriptsScript : public CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {
};
}
template class CEntityScriptBinding<NScript::CBenTestScriptsScript>;