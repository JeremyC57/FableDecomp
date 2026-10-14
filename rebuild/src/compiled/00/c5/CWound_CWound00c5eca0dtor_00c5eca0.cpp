// CWound_00c5eca0::~CWound() -- EH-framed destructor (the manifest's "Kill" is the name of the slot-5 virtual it calls first through the class's own vtable, 0x012B914C; state 5): the +4 handle holder is closed explicitly (CloseHandle import cached in edi, -1 sentinel), then the members are destroyed in reverse with their destructors inlined: the critical-section holder at +0x40 (vtable 0x012B8FC8, DeleteCriticalSection on +0x44), the two null-sentinel handle holders at +0x34 / +0x2c (vtables 0x012B90EC / 0x012B90E8), the derived handle holder at +0xc (its own vtable store is dead and dropped) and the holder at +4 (both 0x012B90D4, -1 sentinel), then the base vtable 0x012B5868 is restored
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5ECA0, oracle length 212 bytes. Built under /EHsc.
// Retail bytes:  ... mov [esp+8],esi ; mov dword ptr [esi],12B914Ch ; mov dword ptr [esp+14h],5 ; call [12B9160h] ; mov eax,[esi+8] ; cmp eax,-1 ; mov edi,[__imp_CloseHandle] ; je a ; push eax ; call edi ; mov dword ptr [esi+8],-1 ; a: lea eax,[esi+44h] ; push eax ; mov dword ptr [esi+40h],12B8FC8h ; call [__imp_DeleteCriticalSection] ; mov eax,[esi+38h] ; test eax,eax ; mov dword ptr [esi+34h],12B90ECh ; je b ; push eax ; call edi ; mov dword ptr [esi+38h],0 ; b: ... (+0x2c, +0xc, +4 likewise) ; mov ecx,[esp+0Ch] ; mov dword ptr [esi],12B5868h ; ... ret
// The unwind funclets call out-of-line copies of the inline member destructors (0x00C5EBD0 / 0x00C5EC50 / 0x00C5E420 / 0x00C5E480 / 0x00C5D570) and the base destructor 0x00C56E60. The classes are named by address.
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void* handle);
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);

struct CHandle_00c5eca0 {                  // vtable 0x012B90D4: -1 sentinel
    virtual void v00();
    void* m_hHandle;                       // +4
    void Close() { if (m_hHandle != (void*)-1) { CloseHandle(m_hHandle); m_hHandle = (void*)-1; } }
    ~CHandle_00c5eca0() { Close(); }
};

struct CHandleEx_00c5eca0 : CHandle_00c5eca0 {   // vtable 0x012B9148 (dead store in the destructor)
    virtual void v04();
    ~CHandleEx_00c5eca0() { }
};

struct CEvent_00c5eca0 {                   // vtable 0x012B90E8: null sentinel
    virtual void v00();
    void* m_hHandle;                       // +4
    ~CEvent_00c5eca0() { if (m_hHandle) { CloseHandle(m_hHandle); m_hHandle = 0; } }
};

struct CMutex_00c5eca0 {                   // vtable 0x012B90EC: null sentinel
    virtual void v00();
    void* m_hHandle;                       // +4
    ~CMutex_00c5eca0() { if (m_hHandle) { CloseHandle(m_hHandle); m_hHandle = 0; } }
};

struct CSection_00c5eca0 {                 // vtable 0x012B8FC8
    virtual void v00();
    unsigned char m_section[0x18];         // +4
    ~CSection_00c5eca0() { DeleteCriticalSection(m_section); }
};

struct CWoundBase_00c5eca0 {               // vtable 0x012B5868
    virtual ~CWoundBase_00c5eca0() { }
};

struct CWound_00c5eca0 : CWoundBase_00c5eca0 {
    CHandle_00c5eca0 m_4;                  // +4
    CHandleEx_00c5eca0 m_c;                // +0xc
    unsigned char _pad[0x18];              // +0x14
    CEvent_00c5eca0 m_2c;                  // +0x2c
    CMutex_00c5eca0 m_34;                  // +0x34
    unsigned char _pad2[4];                // +0x3c
    CSection_00c5eca0 m_40;                // +0x40
    virtual void v04();
    virtual void v08();
    virtual void v0C();
    virtual void v10();
    virtual void Kill();                   // slot 5
    void NotifyKilled() { Kill(); }                 // the virtual call written through a non-virtual helper: VC7.1 folds the just-stored vtable into `call [vtable+14h]`
    virtual ~CWound_00c5eca0();
};

CWound_00c5eca0::~CWound_00c5eca0()
{
    NotifyKilled();
    m_4.Close();
}