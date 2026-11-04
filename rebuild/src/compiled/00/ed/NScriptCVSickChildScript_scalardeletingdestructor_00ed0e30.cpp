// NScript::CV_SickChildScript::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set
// Leaf: ??_GCV_SickChildScript@NScript@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ED0E30, 48 bytes, thiscall, `ret 4`; calls 007e74d0=CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase ; 007e74d0=CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase ; 00cbd510=CScriptBase::~CScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete.
// Identity (naming wave 35, family sdd): outer script vtable 012f17dc slot 0 (slot 1 RegisterMain pushes "Main" 0x012C2F94); enclosing 00ec54e0 (slot 2) registers 11 entity scripts whose PDB Alloc outer is CV_SickChildScript; PDB ??_G/??1CV_SickChildScript@NScript@@; refs=1; was CVtable_012f17dc::`scalar deleting destructor'.
//   ??_GCV_SickChildScript@NScript@@UAEPAXI@Z (rebuild/candidates/manual/naming/2026-11-04_anniversary/wave3/labels_alloc.tsv); PDB: Ego_r.pdb / FableWin.pdb ??_GCV_SickChildScript@NScript@@UAEPAXI@Z; Ego_r.pdb / FableWin.pdb ??1CV_SickChildScript@NScript@@UAE@XZ.
//   Was: CThingSwitch::`vector_deleting_destructor' @ 0x00ED0E30 (retail, 48 bytes) -- a scalar deleting destructor body (`??_G`, flags in the stack slot, __thiscall shape modelled as __fastcall with a dead edx): destroys the ... (catalog stand-in CVtable_012f17dc, Leaf `vector_deleting_destructor').
// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the object (wave 33 ctor): the class model and
//   the PDB constructor ??0CV_SickChildScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z of the landed row 0x00EC53C0 (rebuild/src/compiled/00/ec/NScriptCVSickChildScript_CV_SickChildScript_00ec53c0.cpp), its own source unchanged; its user-declared destructor dropped (retail expands the destructor into the ??_G). The constructor body is not part of this graded body.
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

namespace NScript {
class CV_SickChildScript : public CScriptObject {
public:          // vtable 0x012F17DC
    CScriptGameResourceObjectScriptedThingBase m_first;    // +0x48
    CScriptGameResourceObjectScriptedThingBase m_second;   // +0x58
    virtual void v00();
    CV_SickChildScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
};
}
using NScript::CV_SickChildScript;

NScript::CV_SickChildScript::CV_SickChildScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptObject((void*)data, (void*)&game), m_first(), m_second()
{
}