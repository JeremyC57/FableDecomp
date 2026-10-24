// CSubTitle::~CSubTitle() -- destroy a movie subtitle: the sound name, the formatted text, then release the shared text entry
// Leaf: ??1CSubTitle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BDFA0, length 43 bytes, plain `ret`; no call or address references (an
// unreferenced COMDAT next to the landed CSubTitle copy ctor 0x009BDE00 / ??_GCSubTitle 0x009BDF10 and the list<CSubTitle>
// helpers of the CMovieSubtitles TU; 0x009BDE90 and 0x009BDFA0 are the same destructor emitted twice). The manifest label
// (_Dest_val<allocator<CIVCountedPointer<N3DPrimitive::CTriangleList>>>) is propagated and wrong.
// Callees: CCharString::~CCharString @ 0x0099EAE0, CWideString::~CWideString @ 0x0099B510, the text entry's virtual
// slot 1 (+4, its self-delete).
// Layout: Ego_r.pdb CSubTitle {TextEntry +0, FormattedText +4, Sound +8, FrameStart, FrameEnd, StartTime, EndTime}, the
// model of the landed ??_GCSubTitle 0x009BDF10.
// Shape: the compiler-generated destructor (CSubTitle declares none) emitted out of line through a qualified explicit
// destructor call; members in reverse order, the CIVCountedPointer zeroes its object inside the non-null arm.
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
struct CTextEntry {                                          // intrusive virtual counted pointee
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

void DestroySubTitle(CSubTitle* subTitle)                    // emits the implicit destructor; not part of the retail body
{
    subTitle->CSubTitle::~CSubTitle();
}