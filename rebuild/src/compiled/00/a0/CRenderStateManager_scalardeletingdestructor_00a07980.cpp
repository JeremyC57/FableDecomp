// CRenderStateManager::`scalar deleting destructor'(unsigned int flags) -- this class's vtable (0x0129C978), the global instance pointer (0x013CB508) and the word at +4 cleared, then CBaseClassNonCopyable::~ (0x0099A300) and free for flag bit 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A07980, length 48 bytes.
// Leaf: ??_GCRenderStateManager@@UAEPAXI@Z
// The chunk label is propagated; the vtable (0x0129C978) names the class. Shape: the vtable store, the members, `test byte ptr [esp+8],1`, `ret 4`.
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CRenderStateManager;
extern CRenderStateManager* g_pRenderStateManager_013cb508;   // 0x013CB508
struct CBaseClassNonCopyable {
    virtual void v00();
    ~CBaseClassNonCopyable();                      // 0x0099A300
};
struct CRenderStateManager : CBaseClassNonCopyable {              // vtable 0x0129C978
    void* m_4;                                     // +0x04
    __forceinline virtual ~CRenderStateManager()
    {
        g_pRenderStateManager_013cb508 = 0;
        m_4 = 0;
    }
};

void* MakeOne_00a07980()                                 // forces the vtable (and so the scalar deleting destructor) into the object; not part of the retail body
{
    return new CRenderStateManager;
}