// CAudioParamBlock::~CAudioParamBlock() -- restore the vtable and destroy the two embedded CString name arrays (the 8 pitch-factor names at +0x3a4, then the 16 gain names at +0x364) through the EH vector destructor iterator
// Leaf: ??1CAudioParamBlock@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0A9A0, length 109 bytes. Built under /EHsc.
// Naming (sweep1, 2026-10-25): was landed as CVtable012b1e84::~CVtable012b1e84 (the catalog's "CJoystick::~CJoystick"
//   is propagated: CJoystick's vtable is 0x0129C0EC); descriptive (no PDB covers this library). Vtable 0x012B1E84 is
//   stored only here and by the constructor 0x00C0B200 (chunk 431's CAudioParamBlock, renamed this pass), and slot 0
//   is CAudioParamBlock::SetSlot 0x00C0B380; ~CAudioOwner 0x00C09380 calls this on its +0x34 block (0x00C09432). The
//   two arrays are the names the constructor gives the parameters ("G<i>" at +0x364, "P<j>" at +0x3a4); their element
//   destructor 0x00C0BA40 is the landed CString::~CString.
// Callees: `eh vector destructor iterator' @ 0x00BFEED8 (element size 4, counts 8 and 16), CString::~CString
// @ 0x00C0BA40.
// Shape: member arrays of a class with a destructor are destroyed last-declared first; the EH state is 0 while the first
// array is still alive and -1 for its own destruction; `this` is spilled to the frame for the unwind funclet. A virtual
// function with a non-virtual destructor keeps the plain `??1` (as the landed 0x00C23FE0).
class CString {                                    // 4 bytes: the library's MFC-style string
public:
    char* m_pchData;
    ~CString();                                    // 0x00C0BA40
};
struct CAudioParamBlock {                          // vtable 0x012B1E84
    virtual void v00();                            // slot 0: SetSlot 0x00C0B380
    unsigned char    _pad04[0x364 - 4];            // +0x004 the 16 ramped gains and the 8 pitch factors
    CString          m_gainNames[16];              // +0x364
    CString          m_pitchNames[8];              // +0x3a4
    ~CAudioParamBlock();
};

CAudioParamBlock::~CAudioParamBlock()
{
}
