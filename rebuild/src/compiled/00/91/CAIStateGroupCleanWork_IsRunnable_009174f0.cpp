// CAIStateGroup_CleanWork::IsRunnable()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009174F0, length 37 bytes (+ int3 padding).
// Callees: virtual slot 14 (+0x38) GetBuilding (rtti-vtables: 0x01289AEC slot 14 ->
// 0x00917DB0 "CAIStateGroup_CleanWork::GetBuilding"), GFGetFrameCount @ 0x0049D870 (reads the
// frame counter global; __fastcall, no arguments).
// Runnable when the work building exists and the current frame is past the stored frame at
// +0x18. The result is materialised as `mov eax,1` / `xor eax,eax` (a long, not a bool) --
// the PDB prototype says bool, but a bool would be `mov al,1`; an int-typed `&&` expression
// returned through a bool conversion reproduces it.
struct CBuilding;
long __fastcall GFGetFrameCount();          // 0x0049D870

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();
    virtual void Update();
    virtual void v13();
    unsigned char _pad[0x10];               // +0x4..+0x13
};

struct CAIStateGroup_CleanWork : CAIStateGroupBase {
    long m_nextFrame;                       // +0x14 (WorkBase: frame the work was last tried)
    long m_frameToStart;                    // +0x18
    virtual bool IsRunnable();
    virtual CBuilding* GetBuilding();       // slot 14 (+0x38)
};

bool CAIStateGroup_CleanWork::IsRunnable()
{
    return GetBuilding() != 0 && GFGetFrameCount() > m_frameToStart;
}