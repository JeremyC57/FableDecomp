// CEnginePrimitive::AddChildPrimitiveWithSettings(void* child, long settings) -- add the child to the internal primitive at +4; on success apply the quality settings to this primitive and set the dirty flags 0x8e02 at +0xa4 (true); on failure record the error once in the global (reporting it through the hook) and return false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3DA00, length 86 bytes.
// Callees: CEngineInternalPrimitiveBase::AddChildPrimitive @ 0x00C3EA90 (on the object at +4: (child, settings), a long error code), the settings apply @ 0x00C3D710
// (on `this`, the child; the manifest label "Render_MapQualityLevelToSettings" is propagated), the `__stdcall` report hook @ 0x0143BBD4 when the last-error global
// @ 0x0143BB48 is still clear. Shape: the success arm ORs the immediate 0x8e02 into +0xa4 and returns `mov al,1`; the error arm is the 0x00C3xxxx last-error idiom
// (`test ecx,ecx ; jne ; mov ecx,[hook] ; test ; mov [g_error],eax ; je ; push eax ; call ecx`) and returns `xor al,al`.
struct CEngineInternalPrimitiveBase_00c3da00 {
    long AddChildPrimitive(void* child, long settings);   // 0x00C3EA90
};

typedef void (__stdcall* PFNREPORT_00c3da00)(long error);
extern long g_lastError_0143bb48;                  // 0x0143BB48
extern PFNREPORT_00c3da00 g_pfnReport_0143bbd4;    // 0x0143BBD4

struct CEnginePrimitive {
    long                                  m_0;
    CEngineInternalPrimitiveBase_00c3da00* m_pInternal;   // +0x04
    unsigned char                         _pad[0x9c];
    unsigned long                         m_dirtyFlags;   // +0xa4
    void ApplySettings(void* child);               // 0x00C3D710
    bool AddChildPrimitiveWithSettings(void* child, long settings);
};

bool CEnginePrimitive::AddChildPrimitiveWithSettings(void* child, long settings)
{
    long error = m_pInternal->AddChildPrimitive(child, settings);
    if (!error) {
        ApplySettings(child);
        m_dirtyFlags |= 0x8e02;
        return true;
    }
    if (!g_lastError_0143bb48) {
        g_lastError_0143bb48 = error;
        if (g_pfnReport_0143bbd4)
            g_pfnReport_0143bbd4(error);
    }
    return false;
}
