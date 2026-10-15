// CMovieSubtitles::CMovieSubtitles() -- an empty subtitle list (STLport sentinel node malloc'd and self-linked), no text bank, 30 fps, not subtitle time, an empty formatted-text wide string, frame counter 0
// Leaf: ??0CMovieSubtitles
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BDBA0, length 63 bytes.
// Callees: malloc @ 0x00BFEA0E (the 0x24-byte list node: links + a 0x1c-byte CSubTitle),
// CWideString::CWideString() @ 0x0099AED0. No vtable: the chunk label
// "CTCCreatureGeneratorCreator::CTCCreatureGeneratorCreator" is propagated; the neighbours are the
// list<CSubTitle> helpers (0x009BE100 clear) and the single caller is the movie player (0x00A3BC7F).
// Layout = Ego_r.pdb CMovieSubtitles less 8 bytes (the retail STLport list is the 4-byte sentinel
// pointer, the PDB list 12 bytes): SubTitles +0, pTextBank +4, Fps +8, PSoundManager +0xc (left alone),
// IsSubtitleTime +0x10, FormattedText +0x14, VSubtitleWindow +0x18 (C2DBoxF, empty constructor),
// FrameCounter +0x28.
// Shape: the list's inline constructor zeroes the sentinel pointer, allocates and self-links the node
// and then stores it; plain initialisers in declaration order (immediate zero stores), the frame
// counter stored after the wide string's constructor.
#include <stdlib.h>
struct CDataBank;
struct CASoundManager;

class CWideString {
public:
    void* PStringData;
    CWideString();                                          // 0x0099AED0
};

struct C2DBoxF {
    float X0, Y0, X1, Y1;
    C2DBoxF() { }
};

struct SSubTitleNode {                                      // STLport _List_node<CSubTitle>: 0x24 bytes
    SSubTitleNode* Next;                                    // +0x00
    SSubTitleNode* Prev;                                    // +0x04
    unsigned char  SubTitle[0x1c];                          // +0x08 CSubTitle
};

struct CRetailList_SubTitle {                               // STLport list<CSubTitle>: the sentinel pointer only
    SSubTitleNode* Node;
    CRetailList_SubTitle() : Node(0)
    {
        SSubTitleNode* sentinel = static_cast<SSubTitleNode*>(malloc(sizeof(SSubTitleNode)));
        sentinel->Next = sentinel;
        sentinel->Prev = sentinel;
        Node = sentinel;
    }
};

struct CMovieSubtitles {
    CRetailList_SubTitle SubTitles;                         // +0x00
    CDataBank*           pTextBank;                         // +0x04
    long                 Fps;                               // +0x08
    CASoundManager*      PSoundManager;                     // +0x0c
    bool                 IsSubtitleTime;                    // +0x10
    CWideString          FormattedText;                     // +0x14
    C2DBoxF              VSubtitleWindow;                   // +0x18
    long                 FrameCounter;                      // +0x28
    CMovieSubtitles();
};

CMovieSubtitles::CMovieSubtitles()
    : SubTitles()
    , pTextBank(0)
    , Fps(30)
    , IsSubtitleTime(false)
    , FormattedText()
    , VSubtitleWindow()
    , FrameCounter(0)
{
}
