// CTCPhotojournalCapture::`scalar deleting destructor'(unsigned int flags) -- its vtable, the global-TC removal, ~Components (free the vector storage), ~FileName, ~Description, ~CTCBase, then operator delete for flags bit 0
// Leaf: ??_GCTCPhotojournalCapture
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055DE40, 75 bytes, `ret 4`. Vtable slot 0 of CTCPhotojournalCapture 0x0124C4B4
//   (rtti-vtables.tsv; the landed constructor 0x0055D520 stores it).
// Callees: CTCBase::RemoveFromGlobalTCs 0x00686DF0 (the destructor body), free 0x00BFEA14 (the STLport malloc allocator's
//   deallocate of Components' storage), CWideString::~CWideString 0x0099B510 (+0x28 then +0x0c), CTCBase::~CTCBase
//   0x00686830, operator delete 0x00BFE9BC.
// Label correction: catalogued as "CTCBase_Destructor"; it is CTCPhotojournalCapture's `??_G`.
// Layout: the landed constructor's model (PDB CTCPhotojournalCapture with the 12-byte retail STLport vector): Description
//   +0x0c, DiscardScreenShot +0x10, FrameCounter +0x14, FirstRenderFrame +0x18, ScreenOpen +0x19, LastRenderTime +0x20,
//   FileName +0x28, Components +0x2c (vector<NUISystem::CComponent*>), PMannequin +0x38, WasPlayerGuiOpen +0x3c,
//   TakeScreenshotAfterFrames +0x40. engine/CTCBase.h models the vptr as a data word, so CTCBase is restated (as the
//   landed CTCRegionDisplay 0x0063A230).
// Shape: the landed CTCRegionDisplay deleting-destructor form: a `__forceinline` user virtual destructor calling
//   RemoveFromGlobalTCs right after the vtable store; the members die in reverse order (the vector's
//   `if (_M_start != 0) free`, two out-of-line string destructors) before the out-of-line CTCBase destructor; a `new`
//   factory pulls the `??_G` into the TU.
extern "C" void __cdecl free(void* block);               // 0x00BFEA14

class CThing;
namespace NUISystem { class CComponent; }

class CTCBase { public:
    CTCBase();
    virtual ~CTCBase();                                  // 0x00686830
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
    void RemoveFromGlobalTCs();                          // 0x00686DF0
};

class CWideString { public:
    void* m_pData;
    CWideString();                                       // 0x0099AED0
    ~CWideString();                                      // 0x0099B510
};

struct CComponentVector {                                // STLport vector<NUISystem::CComponent*> (12 bytes)
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    CComponentVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CComponentVector()
    {
        if (_M_start != 0)
            free(_M_start);
    }
};

class CTCPhotojournalCapture : public CTCBase { public:
    CWideString      Description;                        // +0x0c
    bool             DiscardScreenShot;                  // +0x10
    long             FrameCounter;                       // +0x14
    bool             FirstRenderFrame;                   // +0x18
    bool             ScreenOpen;                         // +0x19
    double           LastRenderTime;                     // +0x20
    CWideString      FileName;                           // +0x28
    CComponentVector Components;                         // +0x2c
    CThing*          PMannequin;                         // +0x38
    bool             WasPlayerGuiOpen;                   // +0x3c
    long             TakeScreenshotAfterFrames;          // +0x40
    __forceinline virtual ~CTCPhotojournalCapture() { RemoveFromGlobalTCs(); }
};

CTCPhotojournalCapture* NewCTCPhotojournalCapture()
{
    return new CTCPhotojournalCapture;
}