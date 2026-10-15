// CScriptThing::GetCurrentScriptPriority() -- the current script's (+0x38) virtual slot 6, or 0 with no script
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088B7E0, length 17 bytes (32 with padding). Caller 0x00833E0C.
// The catalogue row 0088b7e0 (48 bytes) held this body and the +0x3c getter at 0x0088B800 (skipped ledger, chunk 901).
// The manifest label's class is propagated: engine/CScriptThing.h is the 0xc-byte counted pointer, while this
// object keeps a polymorphic script pointer at +0x38 (and a second word at +0x3c), so `this` is modelled locally.
// Shape: `mov eax,[ecx+38h] ; test eax,eax ; je Z ; mov ecx,eax ; mov eax,[ecx] ; jmp [eax+18h] ; Z: xor eax,eax ; ret`.
struct CScript_Retail {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual long GetPriority();                // slot 6 (+0x18)
};

struct CScriptThing_Host_Retail {
    unsigned char   _pad_0x00[0x38];
    CScript_Retail* m_pCurrentScript;          // +0x38
    long            m_scriptWord3C;            // +0x3c
    long GetCurrentScriptPriority();
};

long CScriptThing_Host_Retail::GetCurrentScriptPriority()
{
    if (m_pCurrentScript)
        return m_pCurrentScript->GetPriority();
    return 0;
}
