// CTCPhotojournalCapture::CTCPhotojournalCapture(CThing* parent) -- CTCBase(parent), own vtable, empty description and file name, 30 frames to wait, first render frame pending, screen closed, zero render time, no components or mannequin, no pending screenshot (-1), then register for the global TC update
// Leaf: ??0CTCPhotojournalCapture
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055D520, 97 bytes. Vtable 0x0124C4B4 (CTCPhotojournalCapture; RTTI slots 4/11/26).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CWideString::CWideString() @ 0x0099AED0 (twice; catalog label "CCharString"
//   is a byte twin), CTCBase::AddToGlobalTCs() @ 0x00686ED0 (catalog "std::list::push_back").
// Layout: PDB CTCPhotojournalCapture (0x48) with a 12-byte retail STLport vector (retail 0x44): Description +0x0c,
//   DiscardScreenShot +0x10 (not initialised here), FrameCounter +0x14, FirstRenderFrame +0x18, ScreenOpen +0x19,
//   LastRenderTime +0x20 (double; the CTCBase base carries the vptr so the double does not pad it), FileName +0x28,
//   Components +0x2c, PMannequin +0x38 (PDB +0x3c), WasPlayerGuiOpen +0x3c, TakeScreenshotAfterFrames +0x40.
// Shape: init list in declaration order (the double is loaded from the 0.0 pool constant 0x0122ED70 and scheduled first), one
//   shared zero register; the registration call is the body.
class CThing;
class CWideString { public:
    void* m_data;
    CWideString();                              // 0x0099AED0
};
namespace NUISystem { class CComponent; }
struct CComponentVector {                       // STLport vector<NUISystem::CComponent*> (12 bytes)
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    CComponentVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
class CTCBase { public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCBase(CThing* parent);                    // 0x00686800
public:
    void AddToGlobalTCs();   // 0x00686ED0
};
class CTCPhotojournalCapture : public CTCBase { public:
    CWideString Description;                    // +0x0c
    bool        DiscardScreenShot;              // +0x10
    long        FrameCounter;                   // +0x14
    bool        FirstRenderFrame;               // +0x18
    bool        ScreenOpen;                     // +0x19
    double      LastRenderTime;                 // +0x20
    CWideString FileName;                       // +0x28
    CComponentVector Components;                // +0x2c
    CThing*     PMannequin;                     // +0x38
    bool        WasPlayerGuiOpen;               // +0x3c
    long        TakeScreenshotAfterFrames;      // +0x40
    virtual ~CTCPhotojournalCapture();
    CTCPhotojournalCapture(CThing* parent);
};

CTCPhotojournalCapture::CTCPhotojournalCapture(CThing* parent)
    : CTCBase(parent)
    , FrameCounter(30)
    , FirstRenderFrame(true)
    , ScreenOpen(false)
    , LastRenderTime(0.0)
    , PMannequin(0)
    , WasPlayerGuiOpen(false)
    , TakeScreenshotAfterFrames(-1)
{
    AddToGlobalTCs();
}