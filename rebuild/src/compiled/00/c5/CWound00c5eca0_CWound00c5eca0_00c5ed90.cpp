// CWound_00c5eca0::CWound_00c5eca0() -- this class's vtable, then the five embedded Win32 holders built inline (two -1 handles, two null handles, a critical section initialised through the import), then four body words zeroed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5ED90, length 89 bytes (96 with padding).
// Leaf: ??0CWound_00c5eca0
// The chunk label (CSpecialAbilitiesBattleChargeDef) is propagated garbage. This is the constructor matching
// the landed destructor 0x00C5ECA0 (CWound_00c5eca0::~CWound, the no-RTTI 0x00C5xxxx module), whose model is
// reused: primary vtable 0x012B914C over a root whose own vtable store (0x012B5868) is dead here; members
// m_4 (vtable 0x012B90D4, handle -1), m_c (0x012B9148 over the same holder, handle -1), m_2c (0x012B90E8,
// handle 0), m_34 (0x012B90EC, handle 0) and m_40 (0x012B8FC8, the critical section at +0x44; its
// constructor is the landed 0x00C5D550 inlined).
// Callees: KERNEL32 InitializeCriticalSection (IAT 0x0143FEE4).
// Shape: the derived vtable first (after the empty root), each member's vtable and handle stored by its
// inline constructor (the shared -1 in eax, the shared 0 in edi; the derived holder's base vtable store is
// dead and dropped), the section address pushed before its vtable store, then the four words of the body in
// source order +0x1c, +0x14, +0x18, +0x20 (all scheduled after the import call).
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);   // IAT 0x0143FEE4

struct CHandle_00c5eca0 {                  // vtable 0x012B90D4: -1 sentinel
    virtual void v00();
    void* m_hHandle;                       // +4
    CHandle_00c5eca0() : m_hHandle((void*)-1) { }
};

struct CHandleEx_00c5eca0 : CHandle_00c5eca0 {   // vtable 0x012B9148
    virtual void v04();
    CHandleEx_00c5eca0() { }
};

struct CEvent_00c5eca0 {                   // vtable 0x012B90E8: null sentinel
    virtual void v00();
    void* m_hHandle;                       // +4
    CEvent_00c5eca0() : m_hHandle(0) { }
};

struct CMutex_00c5eca0 {                   // vtable 0x012B90EC: null sentinel
    virtual void v00();
    void* m_hHandle;                       // +4
    CMutex_00c5eca0() : m_hHandle(0) { }
};

struct CSection_00c5eca0 {                 // vtable 0x012B8FC8
    virtual void v00();
    unsigned char m_section[0x18];         // +4: CRITICAL_SECTION
    CSection_00c5eca0() { InitializeCriticalSection(m_section); }
};

struct CWoundBase_00c5eca0 {               // vtable 0x012B5868
    virtual void v00();
};

struct CWound_00c5eca0 : CWoundBase_00c5eca0 {
    CHandle_00c5eca0   m_4;                // +0x04
    CHandleEx_00c5eca0 m_c;                // +0x0c
    long               m_14;               // +0x14
    long               m_18;               // +0x18
    long               m_1c;               // +0x1c
    long               m_20;               // +0x20
    unsigned char      _24[8];             // +0x24..+0x2b
    CEvent_00c5eca0    m_2c;               // +0x2c
    CMutex_00c5eca0    m_34;               // +0x34
    unsigned char      _3c[4];             // +0x3c
    CSection_00c5eca0  m_40;               // +0x40
    virtual void v00();
    CWound_00c5eca0();
};

CWound_00c5eca0::CWound_00c5eca0()
{
    m_1c = 0;
    m_14 = 0;
    m_18 = 0;
    m_20 = 0;
}