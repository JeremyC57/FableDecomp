// CSingleton_00c3d090::Activate(void*) -- claim the global current-instance slot at 0x0143BB48 when it is free, then tail-call the activation callback at +0x7c with the same argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3D090, length 35 bytes (the 64-byte chunk row also holds the sibling setter @ 0x00C3D0C0, `m_pfnActivate = arg`, after int3 padding; only this body is authored).
// Callees: the `__stdcall` callback pointer at +0x7c (one argument; the sibling-call rewrites the argument slot and jumps). Shape: the global is read once,
// the argument is stored into it and re-pushed through edx; both early exits share `ret 4`. The chunk label "CDrawAutoPickups::ClearPrimitivesIfNecessary"
// is propagated; the class is named by address.
typedef void (__stdcall* ActivateCallback)(void* instance);

extern void* g_CurrentInstance_0143bb48;           // 0x0143BB48

struct CSingleton_00c3d090 {
    unsigned char    _pad[0x7c];
    ActivateCallback m_pfnActivate;                // +0x7c
    void Activate(void* instance);
};

void CSingleton_00c3d090::Activate(void* instance)
{
    if (g_CurrentInstance_0143bb48 == 0) {
        g_CurrentInstance_0143bb48 = instance;
        if (m_pfnActivate)
            m_pfnActivate(instance);
    }
}
