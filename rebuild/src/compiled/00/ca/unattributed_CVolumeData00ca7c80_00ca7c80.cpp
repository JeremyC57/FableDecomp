// CVolumeData_00ca7c80::~CVolumeData_00ca7c80() -- EH-framed destructor: the block at +0x14 freed through operator delete (state 1), the member at +0x34 destroyed (@ 0x00CB1CC0, state 0), then the member at +4 (@ 0x00CACAF0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA7C80, oracle length 94 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov eax,[esi+14h] ; push eax ; mov dword ptr [esp+14h],1 ; call operator delete ; add esp,4 ; lea ecx,[esi+34h] ; mov byte ptr [esp+10h],0 ; call ~CSecond ; lea ecx,[esi+4] ; mov dword ptr [esp+10h],-1 ; call ~CFirst ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The body frees the raw block (no null test: `::operator delete`, not a delete-expression) under
// EH state 1 (both members alive); the members then go last to first. The class stores no vtable.
// The manifest label is propagated garbage; the class is named by address (it is the base of the
// destructor at 0x00CA8940).
struct CFirst_00ca7c80 {                   // +4
    unsigned char _body[0x10];
    ~CFirst_00ca7c80();                    // 0x00CACAF0
};

struct CSecond_00ca7c80 {                  // +0x34
    unsigned char _body[4];
    ~CSecond_00ca7c80();                   // 0x00CB1CC0
};

struct CVolumeData_00ca7c80 {
    unsigned char _pad[4];
    CFirst_00ca7c80 m_first;               // +4
    void* m_pBlock;                        // +0x14
    unsigned char _pad2[0x1c];
    CSecond_00ca7c80 m_second;             // +0x34
    ~CVolumeData_00ca7c80();
};

CVolumeData_00ca7c80::~CVolumeData_00ca7c80()
{
    ::operator delete(m_pBlock);
}