// CIsHornTaskWithThisHorn::operator()(CCountedPointer<CTask> task) -- this functor is an intelligent pointer to a horn: true when the task's horn pointer (+8 of the task) is the same thing; the by-value counted task is released at the end (0x0065D5A0, `__fastcall`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065F116, length 42 bytes.
// Shape: GetPItem (0x00A01B50) on `this` then on the task, `sete bl`, the parameter destroyed last, `mov al,bl`, `ret 8`.
#pragma optimize("s", on)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    void* GetPItem() const;                        // 0x00A01B50
};
struct CTask_0065f116 {
    unsigned char           _pad[8];
    CBaseIntelligentPointer m_horn;                // +0x08
};
struct CCountedPointer_0065f116 {                  // 8 bytes: the task and its count block
    CTask_0065f116* m_pObject;
    void*           m_pCount;
    ~CCountedPointer_0065f116();
};
void __fastcall DestroyCountedPointer_0065d5a0(CCountedPointer_0065f116* pointer);   // 0x0065D5A0 (labelled "_Dest_val<...>")
__forceinline CCountedPointer_0065f116::~CCountedPointer_0065f116() { DestroyCountedPointer_0065d5a0(this); }
struct CIsHornTaskWithThisHorn : CBaseIntelligentPointer {
    bool operator()(CCountedPointer_0065f116 task);
};

bool CIsHornTaskWithThisHorn::operator()(CCountedPointer_0065f116 task)
{
    void* horn = GetPItem();
    bool same = task.m_pObject->m_horn.GetPItem() == horn;
    return same;
}