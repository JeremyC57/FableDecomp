// NScript::CQ_EndGameBossBattleScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCQ_EndGameBossBattleScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D34440, 48 bytes, thiscall, `ret 4`; calls 004aa840=CScriptThing::~CScriptThing ; 007e74d0=CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase ; 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): outer script vtable 012cc60c slot 0 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00d2f7f0 (slot 2) registers 4 entity scripts whose PDB Alloc outer is CQ_EndGameBossBattleScript; PDB ??_G/??1CQ_EndGameBossBattleScript@NScript@@; refs=1; was CVtable_012cc60c::`scalar deleting destructor'.
//   ??_GCQ_EndGameBossBattleScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: Ego_r.pdb / FableWin.pdb ??_ECQ_EndGameBossBattleScript@NScript@@UAEPAXI@Z; Ego_r.pdb / FableWin.pdb ??1CQ_EndGameBossBattleScript@NScript@@UAE@XZ.
//   Was:  (catalog stand-in CVtable_012cc60c, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CQ_EndGameBossBattleScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00D2F6B0 (rebuild/src/compiled/00/d2/NScriptCQEndGameBossBattleScript_CQ_EndGameBossBattleScript_00d2f6b0.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
class CScriptDataBase;
class CGameScriptInterfaceBase;
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"

struct __declspec(novtable) CScriptObject : CScriptBase {   // the argument-carrying intermediate base: both words stored before the derived vtable
    void* m_pInterface;                            // +0x40 (second argument, CGameScriptInterfaceBase*)
    void* m_pData;                                 // +0x44 (first argument, CScriptDataBase*)
    CScriptObject(void* data, void* iface) : CScriptBase(), m_pInterface(iface), m_pData(data) { }
};

#include "engine/CCountedPointer.h"
void operator delete(void* p);                         // 0x00BFE9BC (MSVCR71)

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() : CBaseClass(), PImp() { }

namespace NScript {
class CQ_EndGameBossBattleScript : public CScriptObject {
public:          // vtable 0x012CC60C
    unsigned char _pad48[8];                       // +0x48..+0x4f
    CScriptGameResourceObjectScriptedThingBase m_scriptedThing;   // +0x50
    CScriptThing  m_thing;                         // +0x60
    virtual void v00();
    CQ_EndGameBossBattleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
}
using NScript::CQ_EndGameBossBattleScript;

NScript::CQ_EndGameBossBattleScript::CQ_EndGameBossBattleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptObject((void*)data, (void*)&game), m_scriptedThing(), m_thing()
{
}