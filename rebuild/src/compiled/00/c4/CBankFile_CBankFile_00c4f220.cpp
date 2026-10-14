// CBankFile::~CBankFile() -- EH-framed destructor: own vtable (0x012B87F4), the progress object at +0xe4 deleted through its virtual destructor (slot 0 with the deleting flag) and nulled when present, then the inline base destructor restores the base vtable (0x012B5868)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C4F220, oracle length 91 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov dword ptr [esi],offset vtable ; mov ecx,[esi+0E4h] ; test ecx,ecx ; mov dword ptr [esp+10h],0 ; je skip ; mov eax,[ecx] ; push 1 ; call [eax] ; mov dword ptr [esi+0E4h],0 ; skip: mov ecx,[esp+8] ; mov dword ptr [esi],offset base vtable ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// `delete` of a polymorphic object is its virtual deleting destructor (slot 0) with flag 1. EH
// state 0 covers the body. The class comes from the manifest label (the body is the destructor
// of the class whose member is created at 0x00C4E2F0); the rest is descriptive.
struct CProgress_00c4f220 {
    virtual ~CProgress_00c4f220();
};

struct CBankFileBase_00c4f220 {            // vtable 0x012B5868
    virtual void v00();
    ~CBankFileBase_00c4f220() { }
};

struct CBankFile : CBankFileBase_00c4f220 {   // vtable 0x012B87F4
    unsigned char _pad[0xe0];
    CProgress_00c4f220* m_pProgress;       // +0xe4
    ~CBankFile();
};

CBankFile::~CBankFile()
{
    if (m_pProgress) {
        delete m_pProgress;
        m_pProgress = 0;
    }
}