// CTextSlider::OnDeactivate() -- EH-framed: an out-of-line helper (@ 0x00C37AD0), then a 4-byte stack visitor (vtable 0x012B5284) handed to virtual slot 10 of the member at +0x28, then the active flag at +0x24 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C37BF0, oracle length 80 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; call Helper ; mov dword ptr [esp+4],offset visitor vtable ; mov eax,[esi+28h] ; lea ecx,[esi+28h] ; lea edx,[esp+4] ; push edx ; mov dword ptr [esp+14h],0 ; call [eax+28h] ; mov ecx,[esp+8] ; mov byte ptr [esi+24h],0 ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The stack visitor has an inline constructor (vtable only) and an inline empty destructor, which
// is what registers EH state 0 around the virtual call. The class comes from the manifest label;
// the visitor, the helper and the member are descriptive.
struct CSliderVisitor_00c37bf0 {           // vtable 0x012B5284
    virtual void v00();
    CSliderVisitor_00c37bf0() { }
    ~CSliderVisitor_00c37bf0() { }
};

struct CSliderTarget_00c37bf0 {            // polymorphic member at +0x28
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Visit(CSliderVisitor_00c37bf0* visitor);   // slot 10
};

struct CTextSlider {
    unsigned char _pad[0x24];
    bool m_bActive;                        // +0x24
    unsigned char _pad2[3];
    CSliderTarget_00c37bf0 m_target;       // +0x28
    void Stop();                           // 0x00C37AD0
    void OnDeactivate();
};

void CTextSlider::OnDeactivate()
{
    Stop();
    {
        CSliderVisitor_00c37bf0 visitor;
        m_target.Visit(&visitor);
    }
    m_bActive = false;
}