// CVideoSys::~CVideoSys() -- delete the texture renderer, CoUninitialize, destroy the subtitle set (its wide string and its subtitle list)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3BC20, length 80 bytes.
// Callees: the texture renderer's virtual deleting destructor (slot 3, `push 1 ; call [eax+0Ch]`),
// CoUninitialize (ole32 import 0x01440648), CWideString::~CWideString @ 0x0099B510 (the subtitle set's
// string at +0x44), the subtitle list's clear @ 0x009BE060 (manifest "list_node_destructor": walks the
// nodes destroying their strings and freeing them, then relinks the head to itself), free @ 0x00BFEA14.
// Shape: `if (TexRenderer) { delete TexRenderer; TexRenderer = 0; }` (explicit null test, the clear kept
// before the join), the import call, then the inlined ~CMovieSubtitles on `esi+30h` (`add esi,30h` because
// the member object is addressed once and used as `this` for both member destructors): the CWideString
// member (+0x14 of the set) first, then the list's inline destructor: out-of-line clear, free the head node.
// Layout: engine/CVideoSys.h puts TexRenderer at +0x20 and Subtitles (CMovieSubtitles, 0x34 bytes) at +0x30;
// the set is modelled here as {list head +0, pad, CWideString +0x14} because the header is a flat record.
extern "C" __declspec(dllimport) void __stdcall CoUninitialize(void);
extern "C" void __cdecl free(void*);          // 0x00BFEA14

struct CTextureRenderer {                     // COM-style object: slot 3 is the virtual destructor
    virtual long __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual ~CTextureRenderer();
};

struct CWideString {
    void* m_pData;
    ~CWideString();                           // 0x0099B510
};

struct CSubtitleList {                        // VC6-era list: {head node, count}
    void* m_pHead;                            // +0x00
    long  m_count;                            // +0x04
    void Clear();                             // 0x009BE060
    ~CSubtitleList()
    {
        Clear();
        if (m_pHead)
            free(m_pHead);
    }
};

struct CMovieSubtitles {                      // 0x2c bytes in retail
    CSubtitleList m_subtitles;                // +0x00
    unsigned char _pad08[0xc];                // +0x08
    CWideString   m_text;                     // +0x14
    ~CMovieSubtitles() { }
};

struct CVideoSys {                            // engine/CVideoSys.h layout (no vptr)
    void*             GBuilder;               // +0x00
    void*             MControl;               // +0x04
    void*             MPosition;              // +0x08
    void*             MSeeking;               // +0x0c
    void*             MEvent;                 // +0x10
    void*             BasicAudio;             // +0x14
    void*             PAsfReader;             // +0x18
    void*             PFileSource;            // +0x1c
    CTextureRenderer* TexRenderer;            // +0x20
    long              PlayMode;               // +0x24
    long              State;                  // +0x28
    unsigned char     RenderVideo;            // +0x2c
    unsigned char     _pad2d[3];
    CMovieSubtitles   Subtitles;              // +0x30
    ~CVideoSys();
};

CVideoSys::~CVideoSys()
{
    if (TexRenderer) {
        delete TexRenderer;
        TexRenderer = 0;
    }
    CoUninitialize();
}
