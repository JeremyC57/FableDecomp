// CClickable::OnRightUnclicked() -- EH-framed: a 4-byte visitor (vtable 0x012B262C) handed to this's virtual slot 10, then a second visitor (vtable 0x012B2634) handed to virtual slot 10 of the polymorphic member at +0x10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C102B0, oracle length 98 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; mov dword ptr [esp+4],offset first vtable ; mov eax,[esi] ; lea ecx,[esp+4] ; push ecx ; mov ecx,esi ; mov dword ptr [esp+18h],0 ; call [eax+28h] ; mov dword ptr [esp+8],offset second vtable ; mov edx,[esi+10h] ; lea ecx,[esi+10h] ; lea eax,[esp+8] ; push eax ; mov byte ptr [esp+18h],1 ; call [edx+28h] ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// Both visitors have inline constructors (vtable only) and inline empty destructors; they stay
// alive to the end, so the EH state rises from 0 to 1 (the second store narrowed to a byte).
// The class comes from the manifest label; the visitors and the member are descriptive.
struct CClickVisitor_00c102b0 {            // vtable 0x012B262C
    virtual void v00();
    CClickVisitor_00c102b0() { }
    ~CClickVisitor_00c102b0() { }
};

struct CReleaseVisitor_00c102b0 {          // vtable 0x012B2634
    virtual void v00();
    CReleaseVisitor_00c102b0() { }
    ~CReleaseVisitor_00c102b0() { }
};

struct CClickTarget_00c102b0 {             // polymorphic member at +0x10
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Visit(CReleaseVisitor_00c102b0* visitor);   // slot 10
};

struct CClickable {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void Visit(CClickVisitor_00c102b0* visitor);   // slot 10
    unsigned char _pad[0xc];
    CClickTarget_00c102b0 m_target;        // +0x10
    void OnRightUnclicked();
};

void CClickable::OnRightUnclicked()
{
    CClickVisitor_00c102b0 click;
    Visit(&click);
    CReleaseVisitor_00c102b0 release;
    m_target.Visit(&release);
}