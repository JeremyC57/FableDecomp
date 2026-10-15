// CSlot_00c3cb30::~CSlot_00c3cb30() -- EH-framed destructor of the 0x14-byte slot: own vtable (0x012B586C), clear the slot out of line when it still holds entries or is open, then the base's vtable (0x012B5868) from its inlined empty destructor
// Leaf: ??1CSlot_00c3cb30
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3CAA0, length 86 bytes. Built under /EHsc.
// Callees: CSlot_00c3cb30::Clear() @ 0x00C3C930 (labelled "LinkedList_ClearAndDestructor": walks the list from +4 and
// resets +4/+8/+0x10 to 0 and +0xc to -1). Callers: the landed owner destructor 0x00C3CB30 (four slot members at
// +0x1c/+0x30/+0x44/+0x58, which names this class) and its EH funclets; the manifest's "CTCFishingSpot::
// Destructor_UnwindHelper" label is propagated (no-RTTI audio-library module).
// Layout: vptr, a list head +4, a tail +8, an index +0xc (>= 0 while open: 0x00C3C880 is `return m_index >= 0;`) and
// an entry count +0x10; the slot constructor 0x00C3C8A0 stores the 0x012B586C vtable and zeroes the words.
// Shape: `mov [esp+4],esi` saves `this` for the unwind; EH state 0 (the base sub-object) is stored between the count
// load and its test; `m_count != 0 || IsOpen()` as two branches to the call; the base destructor is inline and empty
// but its vtable store survives (polymorphic base); a non-virtual destructor keeps `??_G` out of the TU.
struct CSlotBase_00c3cb30 {                      // vtable 0x012B5868
    virtual void v00();
    ~CSlotBase_00c3cb30() { }
};

struct CSlot_00c3cb30 : CSlotBase_00c3cb30 {     // vtable 0x012B586C, 0x14 bytes
    void* m_pHead;                               // +0x04
    void* m_pTail;                               // +0x08
    long  m_index;                               // +0x0c
    long  m_count;                               // +0x10
    virtual void v00();
    bool IsOpen() const { return m_index >= 0; }
    void Clear();                                // 0x00C3C930
    ~CSlot_00c3cb30();
};

CSlot_00c3cb30::~CSlot_00c3cb30()
{
    if (m_count != 0 || IsOpen())
        Clear();
}
