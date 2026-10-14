// CVolume_00ca6370::~CVolume() -- EH-framed destructor over three polymorphic bases (vtables 0x012BF498 / 0x012BF458 at +0xc / 0x012BF444 at +0x10; state 6): shuts the volume down (@ 0x00CA4E30) and finishes it (@ 0x00CA4BF0), releases the entry at +0x50 through the interface embedded at its +8 (slot 3, flag 1) and the interface at +0x78 (slot 3, flag 1), then destroys the members in reverse: the three critical-section holders at +0xc4 / +0x94 / +0x7c (inline, DeleteCriticalSection cached in edi), the three one-word holders at +0x5c / +0x58 / +0x54 (@ 0x00CB01B0, states 2-0) and the first base (@ 0x00CAAAC0, state -1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA6370, oracle length 216 bytes. Built under /EHsc.
// Retail bytes:  ... mov [esp+8],esi ; mov dword ptr [esi],12BF498h ; mov dword ptr [esi+0Ch],12BF458h ; mov dword ptr [esi+10h],12BF444h ; mov dword ptr [esp+14h],6 ; call Shutdown ; mov ecx,esi ; call Finish ; mov eax,[esi+50h] ; test eax,eax ; je a ; lea ecx,[eax+8] ; mov eax,[ecx] ; push 1 ; call [eax+0Ch] ; mov dword ptr [esi+50h],0 ; a: mov ecx,[esi+78h] ; test ecx,ecx ; je b ; mov edx,[ecx] ; push 1 ; call [edx+0Ch] ; mov dword ptr [esi+78h],0 ; b: mov edi,[__imp_DeleteCriticalSection] ; lea eax,[esi+0C4h] ; push eax ; call edi ; lea ecx,[esi+94h] ; push ecx ; call edi ; lea edx,[esi+7Ch] ; push edx ; call edi ; lea ecx,[esi+5Ch] ; mov byte ptr [esp+14h],2 ; call CHolder::~CHolder ; ... (+58h, 1 ; +54h, 0) ; mov ecx,esi ; mov dword ptr [esp+14h],-1 ; call CVolumeBase::~CVolumeBase ; ... ret
// The unwind funclets call the out-of-line copy of the section holder destructor (0x00CA4870). The classes are named by address.
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);

struct IRef_00ca6370 {                     // released through slot 3
    virtual void Slot0();
    virtual void Slot4();
    virtual void Slot8();
    virtual void Release(long flags);
};

struct CEntry_00ca6370 {                   // the released interface is embedded at +8
    unsigned char _pad[8];
    IRef_00ca6370 m_ref;
};

struct CHolder_00ca6370 {                  // one word
    void* m_p;
    ~CHolder_00ca6370();                   // 0x00CB01B0
};

struct CSection_00ca6370 {                 // 0x18 bytes: a critical section
    unsigned char m_section[0x18];
    ~CSection_00ca6370() { DeleteCriticalSection(m_section); }
};

struct CVolumeBase_00ca6370 {              // +0: vtable 0x012BF498 in the volume
    virtual ~CVolumeBase_00ca6370();       // 0x00CAAAC0
    long m_4;
    long m_8;
};
struct CVolumeSide1_00ca6370 { virtual void v00(); };   // +0xc
struct CVolumeSide2_00ca6370 { virtual void v00(); };   // +0x10

struct CVolume_00ca6370 : CVolumeBase_00ca6370, CVolumeSide1_00ca6370, CVolumeSide2_00ca6370 {
    unsigned char _pad[0x50 - 0x14];       // +0x14
    CEntry_00ca6370* m_pEntry;             // +0x50
    CHolder_00ca6370 m_54;
    CHolder_00ca6370 m_58;
    CHolder_00ca6370 m_5c;
    unsigned char _pad2[0x78 - 0x60];      // +0x60
    IRef_00ca6370* m_pRef;                 // +0x78
    CSection_00ca6370 m_7c;                // +0x7c
    CSection_00ca6370 m_94;                // +0x94
    unsigned char _pad3[0xc4 - 0xac];      // +0xac
    CSection_00ca6370 m_c4;                // +0xc4
    void Shutdown();                       // 0x00CA4E30
    void Finish();                         // 0x00CA4BF0
    virtual ~CVolume_00ca6370();
};

CVolume_00ca6370::~CVolume_00ca6370()
{
    Shutdown();
    Finish();
    if (m_pEntry) {
        m_pEntry->m_ref.Release(1);
        m_pEntry = 0;
    }
    if (m_pRef) {
        m_pRef->Release(1);
        m_pRef = 0;
    }
}
