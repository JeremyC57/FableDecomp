// CVolume_00ca8940::~CVolume_00ca8940() -- EH-framed destructor: the COM interface at +0x9c released (`__stdcall` slot 2) and nulled when present, then the out-of-line base destructor (@ 0x00CA7C80)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA8940, oracle length 94 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov eax,[esi+9Ch] ; test eax,eax ; mov dword ptr [esp+10h],0 ; je skip ; mov ecx,[eax] ; push eax ; call [ecx+8] ; mov dword ptr [esi+9Ch],0 ; skip: mov ecx,esi ; mov dword ptr [esp+10h],-1 ; call ~CBase ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// Neither class stores a vtable. The manifest label (`ReleaseVolume`) is propagated garbage for a
// destructor; the class is named by address and derives from the one destroyed at 0x00CA7C80.
struct IUnknown_00ca8940 {                 // COM-style: `this` is pushed as the first stack argument
    virtual long __stdcall QueryInterface();
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();   // slot 2
};

struct CVolumeData_00ca8940 {
    unsigned char _body[0x38];
    ~CVolumeData_00ca8940();               // 0x00CA7C80
};

struct CVolume_00ca8940 : CVolumeData_00ca8940 {
    unsigned char _pad[0x64];
    IUnknown_00ca8940* m_pTexture;         // +0x9c
    ~CVolume_00ca8940();
};

CVolume_00ca8940::~CVolume_00ca8940()
{
    if (m_pTexture) {
        m_pTexture->Release();
        m_pTexture = 0;
    }
}