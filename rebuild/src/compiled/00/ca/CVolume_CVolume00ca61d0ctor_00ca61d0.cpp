// CVolume_00ca61d0::CVolume(a, b, c, d) -- EH-framed constructor matching the landed destructor 0x00CA6370 (the manifest's "CHeroExperienceDef" is propagated): the base is built from (b, c, &m_7c, a) (@ 0x00CAC830; state -1), the three derived vtables stored (0x012BF498 / 0x012BF458 at +0xc / 0x012BF444 at +0x10), the entry at +0x50 cleared, the three event holders at +0x54 / +0x58 / +0x5c created with 0 / 1 / 1 (@ 0x00CB0190, states 0-2), the words at +0x60..+0x78 zeroed, the three critical sections at +0x7c / +0x94 / +0xc4 initialised (import cached in ebp) around +0xac cleared, +0xb0 set to 1 and +0xb4..+0xc0 cleared, and the +0x5c holder's event signalled (SetEvent)
// Leaf: ??0CHeroExperienceDef@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA61D0, oracle length 248 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esp+14h] ; mov edx,[esp+18h] ; push ebx ; push ebp ; push esi ; push edi ; mov esi,ecx ; mov ecx,[esp+2Ch] ; push eax ; lea ebp,[esi+7Ch] ; push ebp ; push ecx ; push edx ; mov ecx,esi ; mov [esp+20h],esi ; call CVolumeBase::CVolumeBase ; xor edi,edi ; push edi ; lea ecx,[esi+54h] ; mov [esp+20h],edi ; mov dword ptr [esi],12BF498h ; mov dword ptr [esi+0Ch],12BF458h ; mov dword ptr [esi+10h],12BF444h ; mov [esi+50h],edi ; call CEventHolder::CEventHolder ; push 1 ; lea ecx,[esi+58h] ; mov byte ptr [esp+20h],1 ; call ... ; lea ebx,[esi+5Ch] ; push 1 ; mov ecx,ebx ; mov byte ptr [esp+20h],2 ; call ... ; push ebp ; mov ebp,[__imp_InitializeCriticalSection] ; mov [esi+60h],edi ; ... mov [esi+78h],edi ; call ebp ; lea eax,[esi+94h] ; push eax ; call ebp ; lea ecx,[esi+0C4h] ; push ecx ; mov [esi+0ACh],edi ; mov dword ptr [esi+0B0h],1 ; mov [esi+0B4h],edi ; ... mov [esi+0C0h],edi ; call ebp ; mov edx,[ebx] ; push edx ; call [__imp_SetEvent] ; ... mov eax,esi ; ... ret 10h
// The classes are named by address, matching the destructor's model.
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(void* section);
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);

struct IRef_00ca61d0 { virtual void Slot0(); };
struct CEntry_00ca61d0 { unsigned char _pad[8]; };

struct CEventHolder_00ca61d0 {             // one word: an event handle
    void* m_hEvent;
    CEventHolder_00ca61d0(long manualReset);   // 0x00CB0190
    ~CEventHolder_00ca61d0();              // 0x00CB01B0
};

struct CSection_00ca61d0 {                 // 0x18 bytes: a critical section
    unsigned char m_section[0x18];
    CSection_00ca61d0() { InitializeCriticalSection(m_section); }
    ~CSection_00ca61d0() { }
};

struct CVolumeBase_00ca61d0 {              // +0: vtable 0x012BF498 in the volume
    virtual ~CVolumeBase_00ca61d0();       // 0x00CAAAC0
    long m_4;
    long m_8;
    CVolumeBase_00ca61d0(void* b, void* c, void* section, void* a);   // 0x00CAC830
};
struct __declspec(novtable) CVolumeSide1_00ca61d0 { virtual void v00(); };   // +0xc (its own vtable is never stored)
struct __declspec(novtable) CVolumeSide2_00ca61d0 { virtual void v00(); };   // +0x10

struct CVolume_00ca61d0 : CVolumeBase_00ca61d0, CVolumeSide1_00ca61d0, CVolumeSide2_00ca61d0 {
    unsigned char _pad[0x50 - 0x14];       // +0x14
    CEntry_00ca61d0* m_pEntry;             // +0x50
    CEventHolder_00ca61d0 m_54;
    CEventHolder_00ca61d0 m_58;
    CEventHolder_00ca61d0 m_5c;
    long m_60;
    long m_64;
    long m_68;
    long m_6c;
    long m_70;
    long m_74;
    IRef_00ca61d0* m_pRef;                 // +0x78
    CSection_00ca61d0 m_7c;                // +0x7c
    CSection_00ca61d0 m_94;                // +0x94
    long m_ac;
    long m_b0;
    long m_b4;
    long m_b8;
    long m_bc;
    long m_c0;
    CSection_00ca61d0 m_c4;                // +0xc4
    CVolume_00ca61d0(void* a, void* b, void* c, void* d);
    virtual ~CVolume_00ca61d0();
};

CVolume_00ca61d0::CVolume_00ca61d0(void* a, void* b, void* c, void* d)
    : CVolumeBase_00ca61d0(b, c, m_7c.m_section, a), m_pEntry(0), m_54(0), m_58(1), m_5c(1),
      m_60(0), m_64(0), m_68(0), m_6c(0), m_70(0), m_74(0), m_pRef(0), m_7c(), m_94(),
      m_ac(0), m_b0(1), m_b4(0), m_b8(0), m_bc(0), m_c0(0), m_c4()
{
    SetEvent(m_5c.m_hEvent);
}