// CActiveSoundInfo::CActiveSoundInfo(void* owner, void* sound, void* info) -- CBase, the sound at +4, own vtable, the owner at +8 and the info word at +0xc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAB810, length 41 bytes.
// Callees: CBaseClass::CBaseClass @ 0x0099A2D0. Vtable 0x0129E674 = CActiveSoundInfo (rtti-vtables.tsv); the chunk
// label "CVertexShaderData::CVertexShaderData" is propagated over this byte-identical constructor shell.
// Shape: the SECOND argument is loaded first and stored at +4 before the vtable store (an argument-carrying
// `__declspec(novtable)` intermediate base), then the first and third arguments at +8 / +0xc; `ret 0Ch`.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

struct __declspec(novtable) CActiveSoundInfoBase : CBaseClass {
    void* m_pSound;                           // +0x04 (second argument)
    CActiveSoundInfoBase(void* sound) : CBaseClass(), m_pSound(sound) { }
};

struct CActiveSoundInfo : CActiveSoundInfoBase {              // vtable 0x0129E674
    void* m_pOwner;                           // +0x08 (first argument)
    void* m_pInfo;                            // +0x0c (third argument)
    virtual void v00();
    CActiveSoundInfo(void* owner, void* sound, void* info);
};

CActiveSoundInfo::CActiveSoundInfo(void* owner, void* sound, void* info)
    : CActiveSoundInfoBase(sound), m_pOwner(owner), m_pInfo(info)
{
}