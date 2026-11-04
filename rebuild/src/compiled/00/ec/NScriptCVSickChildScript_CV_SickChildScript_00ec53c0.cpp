// NScript::CV_SickChildScript::CV_SickChildScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: CScriptBase(), the data/interface pair at +0x44/+0x40, own vtable, two empty scripted-thing handles at +0x48 and +0x58
// Leaf: ??0CV_SickChildScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EC53C0, length 84 bytes.
// Callees: CScriptBase::CScriptBase @ 0x00CB8110 (the manifest's "CHeroMorphDef" label is propagated),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x0099A380 twice (each handle's base: vtable 0x0129A7D8, +4 nulled).
// Vtable 0x012F17DC has no RTTI locator (quest-script module); the chunk label "CV_MurderTwistFinalScript" is
// carried by five different constructors in the manifest (propagated), so the class is named by its vtable.
// Shape (landed 0x00CE1A00 model): the base runs on `this`, the first argument is stored at +0x44 and the
// second at +0x40 BEFORE this class's vtable store (an inlined `__declspec(novtable)` intermediate base), then
// the two member objects: each base constructor is called on `lea edi,[esi+N]`, the member vtable stored, and
// the two counted-pointer words zeroed from ebx by the inline constructor of an embedded {Data, Info} member object
// (plain init-list zeros would hoist the `xor` above the vtable store); `ret 8`, `this` in eax.
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
// Naming wave 35: identity level-script ctor: called by the Alloc 00ed0e00 on the new block, stores script vtable 012f17dc; PDB ??0CV_SickChildScript@NScript@@QAE@PAVCScriptDataBase@@...; outer CV_SickChildScript proven by its Main 00ec54e0 entity registrations; was CVtable_012f17dc::CVtable_012f17dc (wave3/labels_alloc.tsv); PDB ??0CV_SickChildScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z in FableWin.pdb.
//   Was `??0CVtable_012f17dc@@QAE@PAX0@Z`; the class is renamed into NScript and takes the PDB parameter types, handed to the row's
//   argument-carrying base model unchanged (data = the first argument, stored at +0x44; the interface at +0x40); same bytes.
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
    virtual ~CV_SickChildScript();    // ??1CV_SickChildScript@NScript@@UAE@XZ
};
}
using NScript::CV_SickChildScript;

NScript::CV_SickChildScript::CV_SickChildScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CScriptObject((void*)data, (void*)&game), m_first(), m_second()
{
}