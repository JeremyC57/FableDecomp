// CWound_00c5eca0::Kill() -- stop the kernel-streaming pin (connection state PAUSE then STOP), signal and join the worker thread, close every handle and free the buffers
// Leaf: Kill
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C5EAD0, 241 bytes (+ int3 padding), plain `ret`.
// Identity: slot 5 of the RTTI-less vtable 0x012B914C (word at 0x012B9160 = 0x00C5EAD0); the landed destructor
//   0x00C5ECA0 (CWound_00c5eca0, "CWound" is a propagated class label) calls it first through that slot. The class is the
//   audio module's kernel-streaming output device; names are descriptive.
// Callees: CKsObject::SetProperty 0x00C5E050 (landed: KSPROPERTY set via DeviceIoControl) on the pin object at +0x0C with
//   KSPROPSETID_Connection @ 0x012B90D8, KSPROPERTY_CONNECTION_STATE (0) and a 4-byte KSSTATE; KERNEL32 SetEvent
//   [0x0143FDE0], WaitForSingleObject [0x0143FEAC], CloseHandle [0x0143FE30] (cached in ebx for the whole body); the
//   module's free wrapper 0x00C3C370 (__cdecl).
// Layout (as the landed destructor 0x00C5ECA0 reads it): pin CKsObject +0x0C (handle +0x10, -1 sentinel), block buffer
//   +0x14, event-handle array +0x18, sample buffer +0x1C, event count +0x20, queued count +0x24, stop event +0x2C
//   (handle +0x30), worker thread +0x34 (handle +0x38); null-sentinel handle holders close with an inline Close().
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);                                  // IAT 0x0143FE30
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                                      // IAT 0x0143FDE0
extern "C" __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* handle, unsigned long ms); // IAT 0x0143FEAC
void __cdecl FreeAudioBlock(void* block);                                                                  // 0x00C3C370

struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
extern const GUID KSPROPSETID_Connection;                                                                  // 0x012B90D8

enum KSSTATE { KSSTATE_STOP = 0, KSSTATE_ACQUIRE = 1, KSSTATE_PAUSE = 2, KSSTATE_RUN = 3 };

struct CKsObject {                                  // vtable 0x012B90D4 / 0x012B9148
    virtual void v00();
    void* m_handle;                                 // +4 (-1 when closed)
    bool SetProperty(const GUID& propertySet, unsigned long id, void* value, unsigned long size);   // 0x00C5E050
    void Close() { if (m_handle != (void*)-1) { CloseHandle(m_handle); m_handle = (void*)-1; } }
};

struct CWin32Handle {                               // vtables 0x012B90E8 / 0x012B90EC: null sentinel
    virtual void v00();
    void* m_handle;                                 // +4
    void Close() { if (m_handle != 0) { CloseHandle(m_handle); m_handle = 0; } }
};

struct CWound_00c5eca0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Kill();                            // slot 5
    CKsObject m_unused04;                           // +0x04
    CKsObject m_pin;                                // +0x0c
    void* m_pBlocks;                                // +0x14
    void** m_pEvents;                               // +0x18
    void* m_pSamples;                               // +0x1c
    unsigned long m_eventCount;                     // +0x20
    long m_queued;                                  // +0x24
    long _28;                                       // +0x28
    CWin32Handle m_stopEvent;                       // +0x2c
    CWin32Handle m_thread;                          // +0x34
};

void CWound_00c5eca0::Kill()
{
    if (m_pin.m_handle != (void*)-1) {
        long state = KSSTATE_PAUSE;
        m_pin.SetProperty(KSPROPSETID_Connection, 0, &state, sizeof(state));
        state = KSSTATE_STOP;
        m_pin.SetProperty(KSPROPSETID_Connection, 0, &state, sizeof(state));
        SetEvent(m_stopEvent.m_handle);
        if (m_thread.m_handle != 0)
            WaitForSingleObject(m_thread.m_handle, 0xFFFFFFFF);
        m_thread.Close();
        m_stopEvent.Close();
        m_pin.Close();
    }
    m_queued = 0;
    if (m_pEvents != 0) {
        for (unsigned long i = 0; i < m_eventCount; ++i)
            CloseHandle(m_pEvents[i]);
        FreeAudioBlock(m_pEvents);
        m_pEvents = 0;
    }
    if (m_pBlocks != 0) {
        FreeAudioBlock(m_pBlocks);
        m_pBlocks = 0;
    }
    if (m_pSamples != 0) {
        FreeAudioBlock(m_pSamples);
        m_pSamples = 0;
    }
}