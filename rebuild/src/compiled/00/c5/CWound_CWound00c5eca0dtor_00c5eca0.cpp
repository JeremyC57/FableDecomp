// CSalOutputKS::~CSalOutputKS() -- the GameCODA kernel-streaming output's destructor: ReleaseBuffer() through the class's own vtable, the filter
// file closed, then the members destroyed in reverse with their destructors expanded (m_mutexFrameCount +0x40, m_threadNotify +0x34,
// m_eventTerminate +0x2c, m_pin +0x0c, m_filter +0x04) and the CSalOutput / CCodaObjectBase base vtable 0x012B5868 restored
// Leaf: ??1CSalOutputKS@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5ECA0, oracle length 212 bytes. Built under /EHsc (catalog CompilerFlags) (state 5 = the five members; the unwind
//   funclets 0x01216360..0x01216394 are separate rows that include this file).
// Retail bytes:  ... mov [esp+8],esi ; mov dword ptr [esi],12B914Ch ; mov dword ptr [esp+14h],5 ; call [12B9160h] ; mov eax,[esi+8] ;
//   cmp eax,-1 ; mov edi,[__imp_CloseHandle] ; je a ; push eax ; call edi ; mov dword ptr [esi+8],-1 ; a: lea eax,[esi+44h] ; push eax ;
//   mov dword ptr [esi+40h],12B8FC8h ; call [__imp_DeleteCriticalSection] ; mov eax,[esi+38h] ; test eax,eax ; mov dword ptr [esi+34h],12B90ECh ;
//   je b ; push eax ; call edi ; mov dword ptr [esi+38h],0 ; b: ... (+0x2c, +0xc, +4 likewise) ; mov dword ptr [esi],12B5868h ; ... ret
// Name (NF404, T3 nf401 0x00C5ECA0; was the stand-in ??1CWound_00c5eca0@@UAE@XZ, earlier mis-listed as ?NotifyKilled, catalog CVtable_012b5868):
//   (1) the original GameCODA 2.10 SDK CODAApi.lib Release/SalOutputKS.obj defines ??1CSalOutputKS@@UAE@XZ with the same sequence over the
//       same members (vptr store, ReleaseBuffer through the vtable, CloseHandle on +8 with the -1 sentinel, the +0x40 mutex, +0x38 / +0x30
//       null-sentinel handles under the CSalThread / CSalEvent vtables, +0x10 / +8 under the CSalFile vtable, then ??_7CCodaObjectBase);
//       that SDK build has no EH frame, calls [vtbl+10h] (no CreateReverbBuffer slot) and calls ~CSalMutex out of line, so it is not
//       byte-identical; its member destructors ??1CSalFile / ??1CSalKsPin / ??1CSalEvent / ??1CSalThread / ??1CSalOutput are byte-identical
//       (relocations masked) to the retail copies the funclets jump to (0x00C5EBD0 / 0x00C5EC50 / 0x00C5E420 / 0x00C5E480 / 0x00C56E60).
//   (2) retail vtable 0x012B914C (no RTTI in this module) slot 3 = 0x00C5E650, the landed ?CreateBuffer@CSalOutputKS@@UAE_NII@Z (vendor proof);
//       slot 0 = 0x00C5EDF0 (scalar deleting destructor, the sole caller of 0x00C5ECA0 at 0x00C5EDF3); and reloc_bindings binds
//       ??0CSalOutputKS@@QAE@XZ to the class's constructor 0x00C5ED90 (CSalPlatform::CreateSw 0x00C3F9D0, `new CSalOutputKS`).
// Layout: the shared engine/CSalOutputKS.h (FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23: the retail slot order, CreateReverbBuffer at slot 4,
//   ReleaseBuffer at slot 5 = [0x012B9160]) with the new opt-in FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS (the member destructors as the
//   retail TU expands them) and FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR (engine/CCodaObjectBase.h: the root destructor, also expanded).
// Shape: retail (and the SDK build) calls ReleaseBuffer through the just-stored vtable's slot (`call [0x012B9160]`, FF 15), not directly:
//   the call is written through the CSalOutput base view, where it is the base's pure virtual; called on CSalOutputKS itself VC7.1 binds
//   the override directly (`call ?ReleaseBuffer@CSalOutputKS`, E8, one byte shorter: DIFFER(211v212), NF404).
#define FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23
#define FABLE_RETAIL_CSALOUTPUTKS_INLINE_MEMBERS
#define FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR
#include "engine/CSalOutputKS.h"

CSalOutputKS::~CSalOutputKS()
{
    static_cast<CSalOutput*>(this)->ReleaseBuffer();
    m_filter.Close();
}
