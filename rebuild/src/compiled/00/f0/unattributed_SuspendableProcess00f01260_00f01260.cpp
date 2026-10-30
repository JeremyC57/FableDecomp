// CActiveEntityScript_00f01260::SuspendableProcess() -- suspends the running process or fires the stored callback with a CScriptThing copy, then flags itself
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F01260, oracle length 68 bytes.
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
class CThing;
struct SCount { long m_references; };

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class CScriptThing : public CBaseClass { public:
    CThing* m_pThing;                      // +4
    SCount* m_pCount;                      // +8
    CScriptThing(const CScriptThing& other) : CBaseClass(other), m_pThing(other.m_pThing), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
};

struct CProcess {
    virtual void v00();
    virtual void Suspend();                // slot 1
};

struct CActiveEntityScript_00f01260 {
    virtual void v00();
    unsigned char _pad0[0x1];
    bool m_bSuspended;                     // +0x5
    unsigned char _pad1[0x2e];
    CProcess* m_pProcess;                  // +0x34
    unsigned char _pad2[0x10];
    CScriptThing m_thing;                  // +0x48
    unsigned char _pad3[0x4];
    void (__fastcall* m_pfnCallback)(void* object, CScriptThing thing);   // +0x58
    void* m_pCallbackObject;               // +0x5c
    void SuspendableProcess();
};

void CActiveEntityScript_00f01260::SuspendableProcess()
{
    if (m_pProcess)
        m_pProcess->Suspend();
    else
        m_pfnCallback(m_pCallbackObject, m_thing);
    m_bSuspended = true;
}