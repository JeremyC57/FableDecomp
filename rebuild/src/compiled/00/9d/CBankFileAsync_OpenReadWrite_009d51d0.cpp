// CBankFileAsync::OpenReadWrite(const CCharString&, unsigned long, unsigned long) -- the base open, then the async flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D51D0, oracle length 36 bytes.
// Callees: CBankFile::OpenReadWrite(const CCharString&, ulong, ulong) @ 0x009CB960.
//
// Retail bytes:  mov eax,[esp+0Ch] ; mov edx,[esp+4] ; push esi ; mov esi,ecx ; mov ecx,[esp+0Ch] ; push eax ; push ecx ; push edx ;
//                mov ecx,esi ; call CBankFile::OpenReadWrite ; mov byte ptr [esi+149h],1 ; pop esi ; ret 0Ch
// Virtual override forwarding all three arguments to the base class (the arguments are gathered
// into registers before `this` is saved), then the byte at +0x149 of the async bank is set
// (the base class ends at +0x148: the derived flags start there, so +0x149 is the second one).
class CCharString;

struct CBankFile_Retail {
    virtual void v00();
    unsigned char _pad[0x148 - 4];      // the retail base ends at +0x148
    void OpenReadWrite(const CCharString& name, unsigned long flags, unsigned long alignment);   // 0x009CB960
};

class CBankFileAsync : public CBankFile_Retail { public:
    bool m_asyncReading;                                                                         // +0x148 (untouched here)
    bool m_asyncOpen;                                                                            // +0x149
    virtual void OpenReadWrite(const CCharString& name, unsigned long flags, unsigned long alignment);   // 0x009D51D0
};

void CBankFileAsync::OpenReadWrite(const CCharString& name, unsigned long flags, unsigned long alignment)
{
    CBankFile_Retail::OpenReadWrite(name, flags, alignment);
    m_asyncOpen = true;
}