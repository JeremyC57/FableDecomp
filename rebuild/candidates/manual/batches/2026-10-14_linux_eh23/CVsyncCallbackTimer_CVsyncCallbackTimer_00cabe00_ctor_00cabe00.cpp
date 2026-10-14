// CVsyncCallbackTimer::CVsyncCallbackTimer(CSource*, const CVsyncCallbackTimer*) -- EH-framed constructor (vtable 0x012BF69C): words +4/+8 zeroed, the source stored at +0xc, +0x14 set to 1, the info member at +0x18 built with 0 (@ 0x00CB12E0, state 0); the source's command interface at +0xc referenced (`__stdcall` slot 1 with `this` pushed); without a template the source's virtual slots 5 and 6 fill +0x10 and +8, else +4/+8/+0x10 are copied from the template and its info list copied (@ 0x00CB1540)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CABE00, oracle length 198 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[esp+14h] ; push esi ; mov esi,ecx ; push edi ; lea edi,[esi+18h] ; push 0 ; mov ecx,edi ; mov [esp+0Ch],esi ; mov dword ptr [esi],012BF69Ch ; mov dword ptr [esi+4],0 ; mov dword ptr [esi+8],0 ; mov [esi+0Ch],eax ; mov dword ptr [esi+14h],1 ; call CVsyncCallbackTimerInfo::CVsyncCallbackTimerInfo ; mov eax,[esi+0Ch] ; mov ecx,[eax+0Ch] ; add eax,0Ch ; push eax ; mov dword ptr [esp+18h],0 ; call [ecx+4] ; mov eax,[esp+20h] ; test eax,eax ; jne copy ; mov ecx,[esi+0Ch] ; mov edx,[ecx] ; call [edx+14h] ; mov ecx,[esi+0Ch] ; mov [esi+10h],eax ; mov eax,[ecx] ; call [eax+18h] ; mov [esi+8],eax ; ... ret 8 ; copy: mov ecx,[eax+4] ; mov [esi+4],ecx ; mov edx,[eax+8] ; mov [esi+8],edx ; mov ecx,[eax+10h] ; add eax,18h ; mov [esi+10h],ecx ; push eax ; mov ecx,edi ; call CVsyncCallbackTimerInfo::CopyFrom ; ... ret 8
// The source has the same head/interface layout as the audio streams (0x00CA7120); the
// interface call through the derived pointer adjusts unchecked. Named from the manifest.
struct CSourceHead_00cabe00 {              // the 12 bytes in front of the command interface
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual long GetPeriod();              // slot 5
    virtual long GetPhase();               // slot 6
    long m_a, m_b;
};

struct ICommandTarget_00cabe00 {           // +0xc of the source: COM-style, `this` pushed
    virtual long __stdcall QueryInterface();
    virtual unsigned long __stdcall AddRef();        // slot 1
    virtual unsigned long __stdcall Release();       // slot 2
};

struct CSource_00cabe00 : CSourceHead_00cabe00, ICommandTarget_00cabe00 { };

struct CVsyncCallbackTimerInfo_00cabe00 {  // +0x18, 0x18 bytes
    unsigned char _body[0x18];
    CVsyncCallbackTimerInfo_00cabe00(long mode);   // 0x00CB12E0
    ~CVsyncCallbackTimerInfo_00cabe00();   // 0x00CA7A10
    void CopyFrom(const CVsyncCallbackTimerInfo_00cabe00* other);   // 0x00CB1540
};

struct CVsyncCallbackTimer_00cabe00 {      // vtable 0x012BF69C
    virtual void v00();
    long m_4;
    long m_phase;                          // +8
    CSource_00cabe00* m_pSource;           // +0xc
    long m_period;                         // +0x10
    long m_14;
    CVsyncCallbackTimerInfo_00cabe00 m_info;   // +0x18
    CVsyncCallbackTimer_00cabe00(CSource_00cabe00* source, const CVsyncCallbackTimer_00cabe00* other);
};

CVsyncCallbackTimer_00cabe00::CVsyncCallbackTimer_00cabe00(CSource_00cabe00* source, const CVsyncCallbackTimer_00cabe00* other)
    : m_4(0), m_phase(0), m_pSource(source), m_14(1), m_info(0)
{
    m_pSource->AddRef();
    if (!other) {
        m_period = m_pSource->GetPeriod();
        m_phase = m_pSource->GetPhase();
        return;
    }
    m_4 = other->m_4;
    m_phase = other->m_phase;
    m_period = other->m_period;
    m_info.CopyFrom(&other->m_info);
}
