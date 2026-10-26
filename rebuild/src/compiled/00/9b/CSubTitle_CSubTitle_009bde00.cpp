// CSubTitle::CSubTitle(const CSubTitle&) -- copy constructor of a movie subtitle: share the text entry (bump its count), copy the formatted text and sound name, copy the frame/time range
// Leaf: ??0CSubTitle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009bde00, 74 bytes (`ret 4`, returns this).
// Callees: CWideString copy ctor 0x0099B720, CCharString copy ctor 0x0099EC30.
// Identity: the catalog label `_Cons_val<allocator<CSubTitle>,CSubTitle,const CSubTitle&>` names the
// construct helper of the CMovieSubtitles list that calls it; this body is a thiscall copy
// constructor (this in ecx, source on the stack, `mov eax,esi`).
// Layout: Ego_r.pdb CSubTitle (0x1C) = CIVCountedPointer<const NGameText::CTextEntry> TextEntry +0,
// CWideString FormattedText +4, CCharString Sound +8, long FrameStart +0x0C, long FrameEnd +0x10,
// float StartTime +0x14, float EndTime +0x18 -- the model of the landed ??_GCSubTitle 0x009BDF10.
// Shape: memberwise copy in declaration order; the intrusive counted pointer's copy is
// `Object(other.Object) { if (Object) ++Object->RefCount; }` (count at +4); the floats move as dwords.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                   // 0x0099EC30
};
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);                   // 0x0099B720
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
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
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
    CSubTitle(const CSubTitle& other);
};

CSubTitle::CSubTitle(const CSubTitle& other)
    : TextEntry(other.TextEntry), FormattedText(other.FormattedText), Sound(other.Sound),
      FrameStart(other.FrameStart), FrameEnd(other.FrameEnd), StartTime(other.StartTime), EndTime(other.EndTime)
{
}