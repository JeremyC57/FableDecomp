// NScript::CGameflowScript::OnPersist(CPersistContext& context) -- persist the master data's PostSavePosition, CoreQuestWaiting and the saved script / card-def name lists
// Leaf: ?OnPersist@CGameflowScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CEF8E0, 108 bytes, thiscall, `ret 4` (virtual, level-script vtable slot 4).
// Identity (proven): slot 4 of the RTTI-less level-script vtable 0x012C3FA4, which NScript::CGameflowScript::Alloc 0x00CEF950 (and the ctor 0x00CE6CB0) stores; level-script
//   slots are 0 dtor, 1 RegisterMain, 2 Main, 3 Init, 4 OnPersist (FableWin.pdb CScriptBase; slot 4 of 0x012CBA58 is the landed
//   CQ_EndGameScript::OnPersist 0x00D29830). Ego_r.pdb has ?OnPersist@CGameflowScript@NScript@@UAEXAAVCPersistContext@@@Z.
// Callees: CPersistContext::Transfer<EGameflowPosition> 0x00CDCA70 (Ego_r.pdb spelling; the landed row says EPostSavePosition),
//   Transfer<unsigned long> 0x004106F0, TransferVector<CCharString> 0x0049B8D0 (twice); keys 0x012C2EB0 / 0x012C5DCC / 0x012C5DB8 / 0x012C5DA4.
// Layout: Ego_r.pdb CGameflowScript (Ego_r - 0x14; STLport vectors 0xC): CoreQuestWaiting +0x48, SavedScriptNames +0x4c, SavedCardDefNames +0x58;
//   the master data (CLevelScriptBase<CQ_SunnyvaleMasterData>::MasterScriptData +0x44) holds PostSavePosition (EGameflowPosition) at +4.
//   The default position 0 has no proven enumerator name.
// Shape: the first default lives in the pushed-ecx slot, the second in the dead argument slot (VC7.1 reuses it).
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector { // STLport 4.6: start, finish, end of storage (0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
class CCharString {
public:
    void* PData;
};
class CPersistContext {
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
    template <class T> void TransferVector(const char* name, std::vector<T>& value);
};
enum EGameflowPosition { };                      // global enum (Ego_r.pdb W4EGameflowPosition@@)
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData {
public:
    long              _pad000;
    EGameflowPosition PostSavePosition;                 // +0x04
};
class CGameflowScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned long             CoreQuestWaiting;         // +0x48
    std::vector<CCharString>  SavedScriptNames;         // +0x4c
    std::vector<CCharString>  SavedCardDefNames;        // +0x58
    virtual void OnPersist(CPersistContext& context);
};

void CGameflowScript::OnPersist(CPersistContext& context)
{
    context.Transfer("PostSavePosition", MasterScriptData->PostSavePosition, (EGameflowPosition)0);
    context.Transfer("CoreQuestWaiting", CoreQuestWaiting, 0UL);
    context.TransferVector("SavedScriptNames", SavedScriptNames);
    context.TransferVector("SavedCardDefNames", SavedCardDefNames);
}
}