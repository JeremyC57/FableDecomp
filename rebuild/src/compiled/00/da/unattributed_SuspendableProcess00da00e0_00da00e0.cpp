// CActiveEntityScript<NScript::CQ_MinionCampScript>::SuspendableProcess() -- runs the entity script's Main, or calls the bound member function with a CScriptThing copy, then stops the process
// Leaf: ?SuspendableProcess@?$CActiveEntityScript@VCQ_MinionCampScript@NScript@@@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA00E0, oracle length 68 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+0x34] ; test ecx,ecx ; je E ; mov eax,[ecx] ; call [eax+4] ; mov byte ptr [esi+0x5],1 ; pop esi ; ret ; E: sub esp,0Ch ; mov eax,esp ; mov dword ptr [eax],offset CBaseClass::`vftable' ; mov dword ptr [eax],offset CScriptThing::`vftable' ; mov ecx,[esi+0x4c] ; mov [eax+4],ecx ; mov ecx,[esi+0x50] ; test ecx,ecx ; mov [eax+8],ecx ; je I ; inc dword ptr [ecx] ; I: mov ecx,[esi+0x5c] ; call [esi+0x58] ; mov byte ptr [esi+0x5],1 ; pop esi ; ret
// Template-instantiated script-entity method (manifest: `CActiveEntityScript<...>::SuspendableProcess`,
// one copy per script class; the class is not identifiable from the body). When a process object
// is attached (+0x34) its virtual slot 1 is called; otherwise the stored callback (+0x58,
// called with the object at +0x5c in ecx) receives a by-value copy of the CScriptThing member
// at +0x48: the copy is built in the argument slot with the CBaseClass and CScriptThing
// vtables (0x01231710, 0x01238C8C; both stores are kept), the thing pointer and the shared count
// block whose counter is incremented when present. Both paths then set the byte flag at +0x5.
// One of 120 bodies of this shape.
// Identity (wave 8): slot 4 of the RTTI-less vtable 0x012D6E5C, which only CEntityScriptBinding<NScript::CQ_MinionCampScript>::CreateActiveEntityScript
//   stores (scripts/aes_T.py); FableWin.pdb ?SuspendableProcess@?$CActiveEntityScript@VCQ_MinionCampScript@NScript@@@@UAEXXZ. Layout and
//   members: engine/CActiveEntityScript.h (PEntityScript +0x34, Entity +0x48, Func +0x58, PParentScript +0x5c; StopRunning +0x05).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
// CCountedPointer<T>: the Ego_r.pdb members (Data, Info; CCPPointerInfo RefCount +0); the copy is the inline addref retail
// expands inside the CScriptThing copy below.
#include "engine/CCountedPointer.h"
// CScriptThing / CEntityScriptBase / CActiveEntityScript<T>: the retail models from the shared headers.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CScriptThing.h"
// Retail expands the CScriptThing copy for the by-value argument (CBaseClass vtable 0x01231710 then 0x01238C8C, PImp addref).
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
#include "engine/CEntityScriptBase.h"
#include "engine/CActiveEntityScript.h"
namespace NScript { class __single_inheritance CQ_MinionCampScript; }   // T: Func is a 4-byte member pointer (single inheritance)

template <class T> void CActiveEntityScript<T>::SuspendableProcess()
{
    if (PEntityScript.Data)
        PEntityScript.Data->Main();
    else
        (PParentScript->*Func)(Entity);
    StopRunning = true;
}
template void CActiveEntityScript<NScript::CQ_MinionCampScript>::SuspendableProcess();
