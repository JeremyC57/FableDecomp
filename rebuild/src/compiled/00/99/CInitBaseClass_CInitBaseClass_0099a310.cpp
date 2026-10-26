// CInitBaseClass::CInitBaseClass() -- this class's vtable (0x0129A7CC), the initialised flag (+4) cleared and the global instance counter (0x013BCA18) incremented
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A310, length 32 bytes.
// Shape: `mov eax,ecx` first, `inc dword ptr [g]`, `ret`.
extern long g_initBaseClassCount_013bca18;        // 0x013BCA18
class CBaseClass { public:
    virtual void v00();
};
class CInitBaseClass : public CBaseClass { public:               // vtable 0x0129A7CC
    bool m_initialised;                            // +0x04
    CInitBaseClass();
};

CInitBaseClass::CInitBaseClass()
    : m_initialised(false)
{
    ++g_initBaseClassCount_013bca18;
}