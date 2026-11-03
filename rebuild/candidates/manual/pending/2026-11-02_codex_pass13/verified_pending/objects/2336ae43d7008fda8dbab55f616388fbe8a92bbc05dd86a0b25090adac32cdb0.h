#ifndef FABLE_ENGINE_CGAMEFLOWSCRIPT_H
#define FABLE_ENGINE_CGAMEFLOWSCRIPT_H
// Retail opt-in (Codex pass 13): Ego_r TPI 0x10446; constructor 00ce6cb0 stores
// vtable 012c3fa4. Base 0x48; native vectors +0x4c/+0x58 are each 12 bytes.
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CGameflowScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    unsigned long CoreQuestWaiting;
    std::vector<CCharString, std::allocator<CCharString> > SavedScriptNames;
    std::vector<CCharString, std::allocator<CCharString> > SavedCardDefNames;
    virtual void OnPersist(CPersistContext&);
    virtual void Init();
    void DeclareGossipCategories();
    virtual void Main();
    void CoreQuestReminder();
    void CheckBarrowFieldsGuards();
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CGameflowScript) == 0x64);
#endif
