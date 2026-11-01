# h17_deque.py: naming wave 17, family stl -- 0x004427D0 ~deque: the landed STLport 4.6 deque text kept; the placeholder element
# CUnprovenElement_004427d0 is the proven CSubtitleRenderer::CSetTextInfo (c675: ~CSubtitleRenderer 0x00443E20 calls it on the
# +0x34 queue<CSetTextInfo> TextInfoQueue; Ego_r.pdb CSetTextInfo 28 bytes, members as below). Name-only.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all
A = '004427d0'
edit(A, 'src', '''// std::deque<CUnprovenElement_004427d0>::~deque() -- STLport deque destructor: destroy every element, then free the node buffers and the map
// Leaf: ??1?$deque@VCUnprovenElement_004427d0@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004427D0, 150 bytes, thiscall, plain `ret`.
// Identity (descriptive): callers a texture-cache destructor 0x00443E20 and a tail jump from 0x00443350; the 0x1C-byte element (four CWideStrings from +0x0C) is not named.
''', '''// std::deque<CSubtitleRenderer::CSetTextInfo>::~deque() -- STLport deque destructor: destroy every element, then free the node buffers and the map
// Leaf: ??1?$deque@VCSetTextInfo@CSubtitleRenderer@@V?$allocator@VCSetTextInfo@CSubtitleRenderer@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004427D0, 150 bytes, thiscall, plain `ret`.
// Identity (proven, chunk c675): ~CSubtitleRenderer 0x00443E20 calls it on the +0x34 queue<CSetTextInfo> (Ego_r.pdb member
//   TextInfoQueue: queue<CSubtitleRenderer::CSetTextInfo, deque<...> >); a tail jump from 0x00443350 also reaches it.
// Naming wave 17: the placeholder element CUnprovenElement_004427d0 replaced by the proven CSubtitleRenderer::CSetTextInfo
//   (name-only: the bytes are unchanged; catalog CBankFileAsyncData::FreeBuffer was wrong).
''')
edit(A, 'src', '''class CUnprovenElement_004427d0 {                       // 0x1C bytes: its destroy loop runs four ~CWideString at +0x0C..+0x18
public:
    unsigned char _data[0x1C];
};
''', '''class CWideStringData;
class CWideString {
public:
    CWideStringData* PStringData;
};
class CSubtitleRenderer {
public:
    class CSetTextInfo {                                // Ego_r.pdb, 0x1C bytes
    public:
        CWideString Text;                               // +0x00
        CWideString Button1Text;                        // +0x04
        CWideString Button2Text;                        // +0x08
        CWideString Button3Text;                        // +0x0C
        unsigned long DisplayFlags;                     // +0x10
        float DisplayTime;                              // +0x14
        bool UpdateWhilePaused;                         // +0x18
        bool ChangePlayerMode;                          // +0x19
    };
};
''')
edit(A, 'src', 'template deque<CUnprovenElement_004427d0 >::~deque();', 'template deque<CSubtitleRenderer::CSetTextInfo >::~deque();')
