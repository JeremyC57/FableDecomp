// NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Init() -- start the melee opponent: the parent's tutorial state goes to 1, BadHit = 0 and RepeatMelee = true
// Leaf: ?Init@CMeleeOpponent@CQ_GuildTrainingMeleeScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D56750, 22 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Init) of the RTTI-less vtable 0x012D0328, which the entity Alloc 0x00D566A0 (`NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Alloc`) stores; ?Init@CMeleeOpponent@CQ_GuildTrainingMeleeScript@NScript@@EAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d56750" is a gap-scan placeholder.
// Layout: Ego_r.pdb CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}; the entity's own members keep their Ego_r offsets. CMeleeOpponent: BadHit +0x1c, RepeatMelee +0x20; parent TutorialState +0x4c (Ego_r.pdb +0x60).
// Shape: the constant 1 is loaded once into eax and reused as the byte `true` (mov byte [ecx+0x20], al).
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
class CGameScriptInterfaceBase;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_GuildTrainingMeleeScript {
public:
    class CMeleeOpponent;
    unsigned char _pad000[0x4c];
    long TutorialState;                                 // +0x4c (retail = Ego_r.pdb - 0x14)
};
class CQ_GuildTrainingMeleeScript::CMeleeOpponent : public CEntityScriptBase {
public:
    CQ_GuildTrainingMeleeScript* ParentClass;                // +0x14
    CQ_SunnyvaleMasterData*   PMasterData;                // +0x18
    long                      BadHit;                     // +0x1c
    bool                      RepeatMelee;                // +0x20
private:
    virtual void Init();
};
}

void NScript::CQ_GuildTrainingMeleeScript::CMeleeOpponent::Init()
{
    ParentClass->TutorialState = 1;
    BadHit = 0;
    RepeatMelee = true;
}