// CScriptThing::func_0088b800() -- return the word at +0x3c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088B800, length 4 bytes (16 with padding). Its address is taken at
// 0x00477697 (a vtable or callback table entry built in code). The catalogue row 0088b7e0 held
// GetCurrentScriptPriority and this body (skipped ledger, chunk 901); the manifest class is propagated (see
// 0x0088B7E0), so `this` is modelled locally.
// Shape: `mov eax,[ecx+3Ch] ; ret`.
struct CScriptThing_Host_Retail {
    unsigned char _pad_0x00[0x38];
    void*         m_pCurrentScript;            // +0x38
    long          m_scriptWord3C;              // +0x3c
    long func_0088b800();
};

long CScriptThing_Host_Retail::func_0088b800()
{
    return m_scriptWord3C;
}
