// CSubtitleRenderer::CSetTextInfo::CSetTextInfo(const CSetTextInfo& other) -- copy constructor: copy-construct the four strings, then copy the flags, time and two switches
// Leaf: ??0CSetTextInfo@CSubtitleRenderer@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004935A7, 81 bytes (`ret 4`).
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720, four times with `lea eax,[edi+N]`
//   (the source object kept in edi).
// Identity: the Ego_r.pdb name set holds `??0CSetTextInfo@CSubtitleRenderer@@QAE@ABV01@@Z`; callers are the subtitle
//   deque's push_back 0x004941C0 and _M_push_back_aux_v 0x00494106 (twice) and 0x00493911.
// Shape: memberwise copy in declaration order; the float is copied as a dword, the bools through al. The body sits
//   at an unaligned address in a size-optimised module: `#pragma optimize("s", on)` is needed (the default /O2 build
//   schedules the copies differently: same length, different order).
// Family (owner item 1g, sweep2): CSubtitleRenderer's TextInfoQueue, Ego_r.pdb `queue<CSubtitleRenderer::CSetTextInfo,
//   deque<..> >`, over STLport 4.6 deque<CSubtitleRenderer::CSetTextInfo> in a size-optimised TU. The rows are the
//   CSetTextInfo copy ctor 0x004935A7, deque::pop_front 0x00493CF5, _M_pop_front_aux 0x00493C25, _M_push_back_aux_v
//   0x00494106 and push_back 0x004941C0; queue::push's out-of-line copy 0x004942BE is a bare `jmp` to push_back. The
//   element is the nested class CSubtitleRenderer::CSetTextInfo (Ego_r.pdb, 28 bytes; the name sets mangle it
//   `VCSetTextInfo@CSubtitleRenderer@@`): its destructor 0x0043E3D0 is landed under that name, and the landed
//   _M_pop_front_aux 0x00493C25 binds it. These rows had modelled a non-nested CSetTextInfo and flat stand-in
//   deques, so they bound `??1CSetTextInfo@@QAE@XZ`, `??0CSetTextInfo@@QAE@ABU0@@Z` and
//   `?InitPrimitiveSlot@@YIXPAX@Z` for the same retail functions.
#pragma optimize("s", on)
#include <stdlib.h>                                     // malloc @ 0x00BFEA0E, free @ 0x00BFEA14
#include <new>

class CWideString { public:
    void* PStringData;                                  // +0x00
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};

class CSubtitleRenderer {
public:
    class CSetTextInfo {                                // Ego_r.pdb, 28 bytes (a class: mangled V)
    public:
        CWideString   Text;                             // +0x00
        CWideString   Button1Text;                      // +0x04
        CWideString   Button2Text;                      // +0x08
        CWideString   Button3Text;                      // +0x0C
        unsigned long DisplayFlags;                     // +0x10
        float         DisplayTime;                      // +0x14
        bool          UpdateWhilePaused;                // +0x18
        bool          ChangePlayerMode;                 // +0x19
        CSetTextInfo(const CSetTextInfo& other);        // 0x004935A7
        ~CSetTextInfo();                                // 0x0043E3D0
    };
};

CSubtitleRenderer::CSetTextInfo::CSetTextInfo(const CSetTextInfo& other)
    : Text(other.Text)
    , Button1Text(other.Button1Text)
    , Button2Text(other.Button2Text)
    , Button3Text(other.Button3Text)
    , DisplayFlags(other.DisplayFlags)
    , DisplayTime(other.DisplayTime)
    , UpdateWhilePaused(other.UpdateWhilePaused)
    , ChangePlayerMode(other.ChangePlayerMode)
{
}
