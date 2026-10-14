// CPixelShaderData::~CPixelShaderData() -- EH-framed destructor: own vtable (0x012B2DEC), the shader at +4 released through its virtual slot 14 with 1 and nulled when present, then the inline base destructor restores the base vtable (0x012B2DB0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C17B10, oracle length 86 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov ecx,[esi+4] ; test ecx,ecx ; mov dword ptr [esp+10h],0 ; je done ; mov eax,[ecx] ; push 1 ; call [eax+38h] ; mov dword ptr [esi+4],0 ; done: mov ecx,[esp+8] ; mov dword ptr [esi],offset base vtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// EH state 0 covers the body (the base is alive; its inline empty destructor only restores its
// vtable). The class comes from the manifest label; the shader interface is descriptive.
struct IShader_00c17b10 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void Release(long flags);      // slot 14
};

struct CPixelShaderDataBase {              // vtable 0x012B2DB0
    virtual void v00();
    ~CPixelShaderDataBase() { }
};

struct CPixelShaderData : CPixelShaderDataBase {   // vtable 0x012B2DEC
    IShader_00c17b10* m_pShader;           // +4
    ~CPixelShaderData();
};

CPixelShaderData::~CPixelShaderData()
{
    if (m_pShader) {
        m_pShader->Release(1);
        m_pShader = 0;
    }
}