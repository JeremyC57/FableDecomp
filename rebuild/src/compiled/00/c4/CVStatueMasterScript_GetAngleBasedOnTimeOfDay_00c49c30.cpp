// CV_StatueMasterScript::GetAngleBasedOnTimeOfDay() -- EH-framed: guard object (ctor @ 0x00C0FFC0, dtor @ 0x00C0FFE0) around a virtual slot-2 call on the object at +0x18
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C49C30, oracle length 90 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  (EH prologue) ; sub esp,8 ; push esi ; mov esi,ecx ; push esi ; lea ecx,[esp+8] ; call CGuard::CGuard ; mov ecx,[esi+0x18] ; mov eax,[ecx] ; mov dword ptr [esp+14h],0 ; call [eax+0x8] ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call CGuard::~CGuard ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// Built with C++ exceptions enabled (/EHsc). An 8-byte guard is constructed from `this`
// @ 0x00C0FFC0, the result of virtual slot 2 of the object at +0x18 is kept in esi
// while the guard is destroyed out of line @ 0x00C0FFE0 (state -1), then returned. Classes other
// than the labelled one are placeholders. One of 5 bodies of this shape.
struct CGuard_00c0ffc0 {
    void* m_first;
    void* m_second;
    CGuard_00c0ffc0(void* owner);        // 0x00C0FFC0
    ~CGuard_00c0ffc0();                  // 0x00C0FFE0
};

struct CQueried {
    virtual void v00();
    virtual void slot01();
    virtual long Query();                  // slot 2
};

struct CV_StatueMasterScript {
    unsigned char _pad[0x18];
    CQueried* m_pObject;                   // +0x18
    long GetAngleBasedOnTimeOfDay();
};

long CV_StatueMasterScript::GetAngleBasedOnTimeOfDay()
{
    CGuard_00c0ffc0 guard(this);
    return m_pObject->Query();
}