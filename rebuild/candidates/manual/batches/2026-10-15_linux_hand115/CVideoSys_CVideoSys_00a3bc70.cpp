// CVideoSys::CVideoSys() -- clear the DirectShow interface pointers and the texture renderer, construct the subtitle set, default PlayMode 1, RenderVideo and SubtitlesEnabled true, CoInitialize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3BC70, length 96 bytes.
// Callees: CMovieSubtitles::CMovieSubtitles @ 0x009BDBA0 (manifest "CTCCreatureGeneratorCreator" is propagated;
// it builds the subtitle set at +0x30), CoInitialize(0) (ole32 import 0x01440644).
// Shape: init-list members in declaration order around the member-object constructor: PAsfReader and
// PFileSource (+0x18/+0x1c) are cleared BEFORE the Subtitles call, SubtitlesEnabled/pTextBank/RenderEvent/
// Height/Width (+0x5c..+0x88) after it; the interface pointers +0..+0x14, TexRenderer, PlayMode = 1, State and
// RenderVideo are body assignments (scheduled after the init list, with `push edi` of CoInitialize's 0
// hoisted above them). Layout: engine/CVideoSys.h agrees up to +0x30; retail CMovieSubtitles is 0x2c
// bytes (the PDB's 0x34 holds a 16-byte vector where retail has 12), so the members after it sit 8 bytes
// lower than the header (SubtitlesEnabled +0x5c, pTextBank +0x60, RenderEvent +0x7c, Height/Width +0x84/+0x88).
extern "C" __declspec(dllimport) long __stdcall CoInitialize(void* reserved);

struct CMovieSubtitles {                      // 0x2c bytes in retail
    unsigned char _body[0x2c];
    CMovieSubtitles();                        // 0x009BDBA0
};

struct CVideoSys {                            // engine/CVideoSys.h layout (no vptr) up to +0x30, see above
    void*           GBuilder;                 // +0x00
    void*           MControl;                 // +0x04
    void*           MPosition;                // +0x08
    void*           MSeeking;                 // +0x0c
    void*           MEvent;                   // +0x10
    void*           BasicAudio;               // +0x14
    void*           PAsfReader;               // +0x18
    void*           PFileSource;              // +0x1c
    void*           TexRenderer;              // +0x20
    long            PlayMode;                 // +0x24
    long            State;                    // +0x28
    unsigned char   RenderVideo;              // +0x2c
    unsigned char   _pad2d[3];
    CMovieSubtitles Subtitles;                // +0x30
    unsigned char   SubtitlesEnabled;         // +0x5c
    unsigned char   _pad5d[3];
    void*           pTextBank;                // +0x60
    unsigned char   CriticalSection[0x18];    // +0x64 (left to InitializeCriticalSection elsewhere)
    void*           RenderEvent;              // +0x7c
    void*           PRenderTexture;           // +0x80
    long            Height;                   // +0x84
    long            Width;                    // +0x88
    CVideoSys();
};

CVideoSys::CVideoSys()
    : PAsfReader(0), PFileSource(0), Subtitles(), SubtitlesEnabled(1), pTextBank(0), RenderEvent(0), Height(0), Width(0)
{
    GBuilder = 0;
    MControl = 0;
    MPosition = 0;
    MSeeking = 0;
    MEvent = 0;
    BasicAudio = 0;
    TexRenderer = 0;
    PlayMode = 1;
    State = 0;
    RenderVideo = 1;
    CoInitialize(0);
}
