// NScript::CQ_HeroSoulsArenaScript::`scalar deleting destructor'(unsigned int flags) -- the implicit destructor inline (Rounds,
// GuardChatTimer, then ~CScriptBase out of line), then free this when bit 0 of flags is set; returns this
// Leaf: ??_GCQ_HeroSoulsArenaScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D80C00, 48 bytes, thiscall, `ret 4`.
// Identity: slot 0 of vtable 0x012D3994, the vtable the landed NScript::CQ_HeroSoulsArenaScript::CQ_HeroSoulsArenaScript 0x00D7C700
//   and ::Alloc 0x00D80BA0 store (slots 1..4 the landed RegisterMain 0x00D7C740 / Main 0x00D7C890 / Init 0x00F26B90 / OnPersist
//   0x00D7C800, PDB names); the members it destroys are engine/CQ_HeroSoulsArenaScript.h's (Rounds +0x5C: the vector destructor
//   0x00CDB960 steps 0x38-byte polymorphic elements = CHeroSoulsRoundDef; GuardChatTimer +0x58: ~CTimer 0x00CD4470).
// Shape: `new` of the class with its constructor defined inline is this TU's emitter of the vtable and so of ??_G (descriptive
//   emitter name, not graded).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CHEROSOULSROUNDDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CHEROSOULSWAVEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CHEROSOULSWAVEDEF_DEF_VIRTUALS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CHeroSoulsRoundDef.h"
#include "engine/CHeroSoulsWaveDef.h"
#include "engine/CQ_HeroSoulsArenaScript.h"

inline NScript::CQ_HeroSoulsArenaScript::CQ_HeroSoulsArenaScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
CScriptBase* NewHeroSoulsArenaScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new NScript::CQ_HeroSoulsArenaScript(data, game);
}
