// CEnvironment::TransitionToDefaultTheme(long channel, long priority, float duration) -- fade one channel's priority layer back to its default theme
// Leaf: ?TransitionToDefaultTheme@CEnvironment@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B3880, length 80 bytes (66 code + int3 pad), thiscall `ret 0xc`.
// Callees: NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::TransitionToDefaultTheme(float) @ 0x00886400.
// Identity: Ego_r.pdb `?TransitionToDefaultTheme@CEnvironment@@QAEXJJM@Z` (name set only; donor-keyed address);
//   the catalog's "CEnvironmentThemePriorityStackEntry_TransitionToDefault" names the callee, not this body.
// Layout (local model): engine/CEnvironment.h has ForcedEnvironmentTheme at +0x3e but keeps PriorityStackChannels
//   (+0x48) opaque as the 16-byte DONOR CArray; retail strides prove a 12-byte CArray {first,last,end}
//   (channel stack stride 12 = `lea edx,[edx+edx*2]` / `*4`) over 0x30-byte entries (`lea ecx,[eax+eax*2]` /
//   `shl ecx,4`), against the donor's 16 / 56 bytes, so the header does not fit the bytes.
// Shape: bool test `mov al,[ecx+0x3e] ; test al,al`, then 0 <= channel < 6 and 1 <= priority < 5 as four
//   early-out compares to the shared `ret 0xc`; the inlined CArray operator[] pair loads Channels.first.
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry {
public:
    unsigned char _pad00[0x30];
    void TransitionToDefaultTheme(float duration);          // 0x00886400
};
}

template <class T> class CArray {                           // retail 12-byte array (STLport vector body)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](long index) { return _M_start[index]; }
};

namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStack {
public:
    CArray<CEnvironmentThemePriorityStackEntry> Ents;
};
}

enum { ENVIRONMENT_CHANNEL_COUNT = 6, ENVIRONMENT_PRIORITY_COUNT = 5 };

class CEnvironment {
public:
    unsigned char _pad00[0x3e];
    bool          ForcedEnvironmentTheme;                    // +0x3e
    unsigned char _pad3f[0x48 - 0x3f];
    CArray<NEnvironmentInternal::CEnvironmentThemePriorityStack> PriorityStackChannels;   // +0x48
    void TransitionToDefaultTheme(long channel, long priority, float duration);
};

void CEnvironment::TransitionToDefaultTheme(long channel, long priority, float duration)
{
    if (ForcedEnvironmentTheme)
        return;
    if (channel < 0 || channel >= ENVIRONMENT_CHANNEL_COUNT)
        return;
    if (priority < 1 || priority >= ENVIRONMENT_PRIORITY_COUNT)
        return;
    PriorityStackChannels[channel].Ents[priority].TransitionToDefaultTheme(duration);
}