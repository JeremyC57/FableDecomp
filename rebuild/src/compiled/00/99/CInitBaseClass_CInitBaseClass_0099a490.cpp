// CInitBaseClass::~CInitBaseClass() (scalar deleting destructor) -- this class's vtable (0x0129A7CC); when the initialised flag (+4) is set, the global initialised counter (0x013BCA14) is decremented and the flag cleared; then the CBaseClass destructor (vtable 0x01231710) and, for a set bit 0 of the flags argument, `operator delete` (0x00BFE9BC)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099A490, length 64 bytes.
// Leaf: ??_GCInitBaseClass@@UAEPAXI@Z
// Shape: the constant 1 in al for both `cmp cl,al` and `test [esp+8],al`, `mov eax,esi`, `ret 4`.
extern long g_initialisedCount_013bca14;          // 0x013BCA14
void __cdecl operator delete(void* block);         // 0x00BFE9BC
class CBaseClass { public:                                // vtable 0x01231710
    virtual ~CBaseClass() { }
};
class CInitBaseClass : public CBaseClass { public:               // vtable 0x0129A7CC
    bool m_initialised;                            // +0x04
    virtual __forceinline ~CInitBaseClass();
};

__forceinline CInitBaseClass::~CInitBaseClass()
{
    if (m_initialised == true) {
        --g_initialisedCount_013bca14;
        m_initialised = false;
    }
}

void* MakeInitBaseClass_0099a490()                // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CInitBaseClass;
}