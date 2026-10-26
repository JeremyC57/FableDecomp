// CThingTrackNode::DrawIsDrawable() -- a track node is drawable exactly when the display engine says drawing is enabled
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00837040, length 12 bytes (vtable 0x01272CE4 slot 27, the CThing
// DrawIsDrawable slot; the catalogue row 00837040 also held the unrelated getter at 0x00837050, split via the
// skipped-row ledger, chunk 905). The manifest label "CThingTrackNode::vfunc_27" is the slot number only.
// Callees: CViewBase-style GetDisplayEngine() @ 0x004C7A70 (called with `this` in ecx, ignores it and reads
// [0x013B8A1C]->+0x1c), then CDisplayEngine virtual slot 9 (+0x24) on the engine.
// Shape: no frame; `mov edx,[eax] ; mov ecx,eax ; jmp [edx+24h]` is the tail jump of a no-argument
// virtual call whose result is returned unchanged.
class CDisplayEngine { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08();
    virtual bool IsDrawingEnabled();                 // slot 9 (+0x24)
};

class CThingTrackNode { public:
    CDisplayEngine* GetDisplayEngine();              // 0x004C7A70 (CViewBase-style, ignores `this`)
    bool DrawIsDrawable();
};

bool CThingTrackNode::DrawIsDrawable()
{
    return GetDisplayEngine()->IsDrawingEnabled();
}