// CXMVPlayer::CXMVPlayer(bool, unsigned long) -- construct the XMV movie player: a null threaded-file pointer and an empty subtitle list (STLport list sentinel allocated and self-linked); the two arguments are not used here
// Leaf: ??0CXMVPlayer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BDB00, length 49 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (landed), malloc @ 0x00BFEA0E.
// Vtable 0x0129B3BC = CXMVPlayer (RTTI); Ego_r.pdb ??0CXMVPlayer@@QAE@_NK@Z gives the (bool, unsigned long)
// signature (`ret 8`).
// Identity: the manifest label CPreallocTexturePool::CPreallocTexturePool is propagated.
// Layout (Ego_r.pdb = retail up to +0x58): PThreadedFile (CCountedPointer) +0x44, SubTitles +0x58 (retail
// list = the sentinel pointer only; node = links + 28-byte CSubTitle = 0x24).
// Shape: vtable store, then the counted pointer's inline zeroing constructor, then the STLport list
// constructor (`node = 0 ; n = allocate ; n->next = n->prev = n ; node = n`).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

struct CBaseClassNonCopyable {                              // vtable 0x0129A7C4
    CBaseClassNonCopyable();                                // 0x0099A2F0
    virtual void v00();
};

class CThreadedFile;
class CCPPointerInfo;
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};

struct SSubTitleNode {                                      // list node: links + the 28-byte CSubTitle
    SSubTitleNode* Next;
    SSubTitleNode* Prev;
    unsigned char  SubTitle[0x1c];
};

struct CSubTitleList {                                      // retail std::list<CSubTitle>: the sentinel pointer
    SSubTitleNode* m_pNode;
    CSubTitleList() : m_pNode(0)
    {
        SSubTitleNode* node = (SSubTitleNode*)malloc(sizeof(SSubTitleNode));
        node->Next = node;
        node->Prev = node;
        m_pNode = node;
    }
};

struct CXMVPlayer : CBaseClassNonCopyable {                 // vtable 0x0129B3BC
    unsigned char                  _pad04[0x40];            // +0x04 .. +0x44 (critical section, buffers, sizes)
    CCountedPointer<CThreadedFile> PThreadedFile;           // +0x44
    long                           FilePosition;            // +0x4c
    bool                           AllowSkipping;           // +0x50
    long                           LocalXMVPlayerRefCount;  // +0x54
    CSubTitleList                  SubTitles;               // +0x58
    CXMVPlayer(bool allowSkipping, unsigned long fps);
    virtual void v00();
};

CXMVPlayer::CXMVPlayer(bool allowSkipping, unsigned long fps)
{
}