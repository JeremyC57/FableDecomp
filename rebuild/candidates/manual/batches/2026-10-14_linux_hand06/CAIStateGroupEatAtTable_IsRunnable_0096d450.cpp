// CAIStateGroup_EatAtTable::IsRunnable()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096D450, length 38 bytes (+ int3 padding).
// Callees: virtual slot 15 (+0x3c) IsRunnableLeaf (the hook the WorkBase/EatAtTable family adds
// below CAIStateGroupBase), CAIStateGroup_EatAtTable::GetSeat @ 0x0096CB40 (bool), virtual
// slot 10 (+0x28) OnReplace.
// Runnable only when the leaf test passes and a seat could be claimed; claiming one re-runs
// OnReplace so the seat position is picked up. `mov al,1` / `xor al,al`: a bool result.
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void OnReplace();               // slot 10 (+0x28)
    virtual bool IsRunnable();              // slot 11 (+0x2c)
    virtual void Update();
    virtual void v13();
    virtual void v14();
    virtual bool IsRunnableLeaf();          // slot 15 (+0x3c)
    unsigned char _pad[0x10];               // +0x4..+0x13
};

struct CAIStateGroup_EatAtTable : CAIStateGroupBase {
    bool GetSeat();                         // 0x0096CB40
    virtual bool IsRunnable();
};

bool CAIStateGroup_EatAtTable::IsRunnable()
{
    if (IsRunnableLeaf() && GetSeat()) {
        OnReplace();
        return true;
    }
    return false;
}
