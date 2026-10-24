// SNamedZeroRecord::SNamedZeroRecord() -- construct a 0x2c-byte record holding a CCharString at +0x10: the string, six zeroed words, then the whole record cleared
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F2BD0, 46 bytes, plain `ret`.
// Leaf: ??0SNamedZeroRecord
// Label: the catalog's "MirroredCriminalStatus" is propagated and wrong (CTCHeroStats::MirroredCriminalStatus is the 0x44-byte
//        record of 0x0057A5DD; this object is 0x2c bytes and holds a CCharString). No caller, jump or absolute reference reaches
//        0x006F2BD0 (an unreferenced COMDAT copy) and no other function inlines this sequence, so the class and member names are
//        descriptive; it sits among the LText tree helpers (0x006F2350..0x006F2BB0).
// Callees: CCharString::CCharString() @ 0x0099E4B0 (zeroes its pointer, counts the string).
// Shape: the member constructors (the string, then six zero words through one zero register), then a body
//        `memset(this, 0, sizeof(*this))` -- the intrinsic `mov ecx,0Bh ; rep stosd` from `this`.
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma intrinsic(memset)
class CCharString {
public:
    void* m_pStringData;
    CCharString();                                   // 0x0099E4B0
};
struct SNamedZeroRecord {
    long        Header[4];                           // +0x00
    CCharString Name;                                // +0x10
    long        Value0;                              // +0x14
    long        Value1;                              // +0x18
    long        Value2;                              // +0x1c
    long        Value3;                              // +0x20
    long        Value4;                              // +0x24
    long        Value5;                              // +0x28
    SNamedZeroRecord();
};
SNamedZeroRecord::SNamedZeroRecord()
    : Value0(0), Value1(0), Value2(0), Value3(0), Value4(0), Value5(0)
{
    memset(this, 0, sizeof(*this));
}