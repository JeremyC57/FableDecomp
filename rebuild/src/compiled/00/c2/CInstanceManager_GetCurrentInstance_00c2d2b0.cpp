// CInstanceManager::GetCurrentInstance() -- the instance now playing: null while a request is pending (+0x33c), else &m_instances[m_current] (two 0x180-byte instances at +0x18, index at +0x318)
// Leaf: ?GetCurrentInstance@CInstanceManager@@QAEPAVCInstance@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D2B0, 30 bytes, __thiscall, plain `ret`: mov al,[ecx+33Ch] ;
//   test al,al ; je ; xor eax,eax ; ret ; mov eax,[ecx+318h] ; lea eax,[eax+eax*2] ; shl eax,7 ; lea eax,[eax+ecx+18h]
// Naming (sweep1, 2026-10-25): was landed as `CNavSwitchableLeafNode::GetRegion` (a propagated manifest label) built as a
//   fake `__fastcall GetRegion(self)` global; descriptive (no PDB covers libpk). FINDINGS chunk 436: the landed
//   CInstanceManager::GetCurrentInfo 0x00C2D420 calls it as GetCurrentInstance and keeps this exact body as its same-TU
//   stand-in; the layout is the libpk CInstanceManager of `.\CInstanceManager.cpp` (CInstanceManager::Assign
//   0x00C2D5E0 sets +0x318 and +0x33c, FindQuietestInstance 0x00C2D520 walks the instances).
class CInstance {                               // 0x180 bytes: one playing instance
public:
    unsigned char _body[0x180];
};

struct SThemeRequest {                          // 0x1c bytes
    long m_words[7];
};

class CInstanceManager {
public:
    unsigned char _pad_0x000[0x18];
    CInstance     m_instances[2];               // +0x18
    long          m_current;                    // +0x318 index of the current instance
    unsigned char _pad_0x31c[4];
    SThemeRequest m_pending;                    // +0x320 the queued request
    bool          m_bPending;                   // +0x33c a request is queued
    CInstance* GetCurrentInstance();
};

CInstance* CInstanceManager::GetCurrentInstance()
{
    if (m_bPending)
        return 0;
    return &m_instances[m_current];
}
