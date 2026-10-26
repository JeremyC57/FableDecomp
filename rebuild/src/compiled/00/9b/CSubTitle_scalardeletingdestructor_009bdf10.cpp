// CSubTitle::`scalar deleting destructor'(unsigned int flags) -- destroy a movie subtitle (sound name, formatted text, release the text entry) and free it when bit 0 of the flags is set
// Leaf: ??_GCSubTitle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BDF10, length 63 bytes, `ret 4`; no call or address references (an
// unreferenced COMDAT of the CMovieSubtitles TU: the landed CMovieSubtitles ctor 0x009BDBA0 and list<CSubTitle> helpers sit
// next to it). The catalog's CDefPointer<CHeroAbilityDef const> label is propagated over the byte-identical body; the
// member order {intrusive counted pointer, CWideString, CCharString} is Ego_r.pdb CSubTitle {TextEntry +0, FormattedText
// +4, Sound +8, FrameStart, FrameEnd, StartTime, EndTime} (0x1c bytes, the list node payload of 0x009BDBA0).
// Callees: CCharString::~CCharString @ 0x0099EAE0, CWideString::~CWideString @ 0x0099B510 (landed), the text entry's
// virtual slot 1 (+4, its self-delete), operator delete @ 0x00BFE9BC.
// Shape: VC7.1 emits `??_G` for a NON-polymorphic class deleted with `delete` (GOTCHAS 24m; a `delete` helper pulls it in);
// members are destroyed in reverse order; the CIVCountedPointer release reads the object into a local (ecx), decrements its
// count at +4, calls the virtual delete on zero and then zeroes the member (inside the non-null arm).
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                          // 0x0099B510
};

namespace NGameText {
class CTextEntry { public:                                          // intrusive virtual counted pointee
    virtual void v00();
    virtual void DeleteThis();                               // slot 1 (+4)
    long RefCount;                                           // +0x04
};
}

template<class T> class CIVCountedPointer {
public:
    T* Object;
    ~CIVCountedPointer()
    {
        T* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->DeleteThis();
            Object = 0;
        }
    }
};

struct CSubTitle {                                           // 0x1c bytes
    CIVCountedPointer<NGameText::CTextEntry> TextEntry;      // +0x00 CIVCountedPointer<const NGameText::CTextEntry>
    CWideString FormattedText;                               // +0x04
    CCharString Sound;                                       // +0x08
    long  FrameStart;                                        // +0x0c
    long  FrameEnd;                                          // +0x10
    float StartTime;                                         // +0x14
    float EndTime;                                           // +0x18
};

void DeleteSubTitle(CSubTitle* subTitle)
{
    delete subTitle;
}