// CBankFileAsync::OpenFileReadWrite(const CWideString&, unsigned long, unsigned long) -- the base open, then the async flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D5200, oracle length 36 bytes.
// Callees: CBankFile::OpenFileReadWrite(const CWideString&, ulong, ulong) @ 0x009D1160.
//
// Retail bytes:  mov eax,[esp+0Ch] ; mov edx,[esp+4] ; push esi ; mov esi,ecx ; mov ecx,[esp+0Ch] ; push eax ; push ecx ; push edx ;
//                mov ecx,esi ; call CBankFile::OpenFileReadWrite ; mov byte ptr [esi+149h],1 ; pop esi ; ret 0Ch
// The wide-string twin of OpenReadWrite @ 0x009D51D0 (RTTI name ?OpenFileReadWrite@CBankFileAsync@@UAEXABVCWideString@@KK@Z).
class CWideString;

struct CBankFile_Retail {
    virtual void v00();
    unsigned char _pad[0x148 - 4];      // the retail base ends at +0x148
    void OpenFileReadWrite(const CWideString& filename, unsigned long flags, unsigned long alignment);   // 0x009D1160
};

class CBankFileAsync : public CBankFile_Retail { public:
    bool m_asyncReading;                                                                                 // +0x148 (untouched here)
    bool m_asyncOpen;                                                                                    // +0x149
    virtual void OpenFileReadWrite(const CWideString& filename, unsigned long flags, unsigned long alignment);   // 0x009D5200
};

void CBankFileAsync::OpenFileReadWrite(const CWideString& filename, unsigned long flags, unsigned long alignment)
{
    CBankFile_Retail::OpenFileReadWrite(filename, flags, alignment);
    m_asyncOpen = true;
}