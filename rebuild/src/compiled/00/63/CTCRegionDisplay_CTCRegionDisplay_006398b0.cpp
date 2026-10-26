// CTCRegionDisplay::CTCRegionDisplay(CThing* thing) -- CTCBase(thing), own vtable, two empty area-name strings, counters/alpha/time zeroed, time-out and update-while-paused on, info display off, then join the global update list
// Leaf: ??0CTCRegionDisplay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006398B0, length 72 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CWideString::CWideString() @ 0x0099AED0 (twice), CTCBase::AddToGlobalUpdateTCs()
// @ 0x00686ED0. Vtable 0x0125A0EC = CTCRegionDisplay (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCRegionDisplay (0x28): CurrentAreaName +0x0c, CurrentAreaNameExtra +0x10 (CWideString, one pointer each),
// FrameNameChanged +0x14, CurrentAlpha +0x18, LastAlpha +0x1c, DisplayTime +0x20, TimeOut +0x24, InfoDisplay +0x25,
// UpdateWhilePaused +0x26.
class CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
    void AddToGlobalUpdateTCs();                            // 0x00686ED0
};

class CWideString { public:
    void* m_pData;
    CWideString();                                          // 0x0099AED0
};

class CTCRegionDisplay : public CTCBase_Calls { public:                   // vtable 0x0125A0EC
    CWideString CurrentAreaName;                            // +0x0c
    CWideString CurrentAreaNameExtra;                       // +0x10
    long        FrameNameChanged;                           // +0x14
    long        CurrentAlpha;                               // +0x18
    long        LastAlpha;                                  // +0x1c
    float       DisplayTime;                                // +0x20
    bool        TimeOut;                                    // +0x24
    bool        InfoDisplay;                                // +0x25
    bool        UpdateWhilePaused;                          // +0x26
    virtual void v00();
    CTCRegionDisplay(CThing* thing);
};

CTCRegionDisplay::CTCRegionDisplay(CThing* thing)
    : CTCBase_Calls(thing)
    , FrameNameChanged(0)
    , CurrentAlpha(0)
    , LastAlpha(0)
    , DisplayTime(0)
    , TimeOut(true)
    , InfoDisplay(false)
    , UpdateWhilePaused(true)
{
    AddToGlobalUpdateTCs();
}