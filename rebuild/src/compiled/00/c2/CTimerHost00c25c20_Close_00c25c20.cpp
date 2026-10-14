// CTimerHost_00c25c20::Close() -- run the out-of-line close @ 0x00C25A50, destroy and free the owned object at +0x14 (destructor @ 0x00C354A0, operator delete @ 0x00BFE9BC), null the pointer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C25C20, length 48 bytes.
// Callees: the close @ 0x00C25A50 (on `this`; manifest "PKCTimerManager_Close"), the owned object's destructor @ 0x00C354A0 (the landed CScriptModule_00c354a0, non-virtual),
// operator delete @ 0x00BFE9BC. Shape: the pointer in edi, the destroy chain written out (`~T(); operator delete`) so no `??_G` is used, the null store after the join (`pop edi` first).
// The chunk label "CGuiControlListBoxPane::Close" is propagated (0x00C2xxxx module); the class is named by address.
void __cdecl operator delete(void* block);         // 0x00BFE9BC

struct CScriptModule_00c354a0 {                    // the owned object (landed destructor @ 0x00C354A0)
    unsigned char _body[0xc];
    ~CScriptModule_00c354a0();                     // 0x00C354A0
};

struct CTimerHost_00c25c20 {
    unsigned char           _pad[0x14];
    CScriptModule_00c354a0* m_pModule;             // +0x14
    void CloseTimers();                            // 0x00C25A50
    void Close();
};

void CTimerHost_00c25c20::Close()
{
    CloseTimers();
    CScriptModule_00c354a0* module = m_pModule;   // the pointer held in edi across the destroy chain
    if (module) {
        module->~CScriptModule_00c354a0();
        operator delete(module);
    }
    m_pModule = 0;
}