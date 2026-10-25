// _global::InitTimerGlobals() -- resets a libpk 12-byte static record at 0x0143BC18: the count word to 1 and both links to the anchor object at 0x01443000
// Leaf: InitTimerGlobals
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C33770, 26 bytes, `ret`. Not a dynamic initialiser: the catalog's `$E2` is a
//   propagated label; the only reference is a direct call from the landed StartTimer 0x00C1C1A0, which declares it as
//   `InitTimerGlobals()` (an inline-constructed static would be folded into .data by VC7.1, not run as code).
// Shape: the constant 1 is stored first, the anchor address goes through eax for the two link stores. The record's
//   real name is not recoverable (descriptive model).
struct CAnchor_01443000 { long m_value; };
extern CAnchor_01443000 g_anchor_01443000;    // 0x01443000

struct CStaticRecord_0143bc18 {
    long              m_count;                // +0 (0x0143BC18)
    CAnchor_01443000* m_pFirst;               // +4 (0x0143BC1C)
    CAnchor_01443000* m_pLast;                // +8 (0x0143BC20)
};
extern CStaticRecord_0143bc18 g_record_0143bc18;   // 0x0143BC18

void InitTimerGlobals()
{
    g_record_0143bc18.m_count = 1;
    g_record_0143bc18.m_pFirst = &g_anchor_01443000;
    g_record_0143bc18.m_pLast = &g_anchor_01443000;
}