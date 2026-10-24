// CEnvironment::ConsoleRestoreDefaultTheme(CEnvironment& env, const vector<CGenericVar>& args) -- console: transition (channel, priority) back to the default theme over a duration, even while a theme is forced
// Leaf: ?ConsoleRestoreDefaultTheme@CEnvironment@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B3B30, length 80 bytes (68 code + int3 pad), private static
//   __fastcall (`CI`): ecx = env, edx = args, plain `ret`.
// Callees: NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::TransitionToDefaultTheme(float) @ 0x00886400,
//   reached through CEnvironment::TransitionToDefaultTheme(long, long, float) (0x006B3880, inlined here).
// Identity: Ego_r.pdb `?ConsoleRestoreDefaultTheme@CEnvironment@@CIXAAV1@ABV?$vector@VCGenericVar@@...@Z` (name
//   set only); the catalog's "EnvironmentThemePriority_ClearFlag" describes the first store.
// Layout (local model): as 0x006B3880 -- the engine header keeps PriorityStackChannels opaque at the donor's
//   16-byte CArray, retail strides are 12 / 0x30. CGenericVar (0x1c) = vptr, Type +4, ValString +8,
//   ValSLONG +0xc, ValULONG +0x10, ValBool +0x14, ValFloat +0x18 (Ego_r layout, matches the strides here:
//   args[1].ValSLONG = +0x28, args[2].ValFloat = +0x50).
// Shape: ForcedEnvironmentTheme cleared, the inlined TransitionToDefaultTheme folds its forced test away
//   (constant false), the range checks run on args[0] / args[1], and ForcedEnvironmentTheme is set again.
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

class CCharString { public: void* PData; };

class CGenericVar {
public:
    void*         _vftable;
    long          Type;                                     // +0x04
    CCharString   ValString;                                // +0x08
    long          ValSLONG;                                 // +0x0c
    unsigned long ValULONG;                                 // +0x10
    bool          ValBool;                                  // +0x14
    float         ValFloat;                                 // +0x18
};

namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T& operator[](unsigned int n) const { return *(_M_start + n); }
};
}
using std::vector;

enum { ENVIRONMENT_CHANNEL_COUNT = 6, ENVIRONMENT_PRIORITY_COUNT = 5 };

class CEnvironment {
public:
    unsigned char _pad00[0x3e];
    bool          ForcedEnvironmentTheme;                    // +0x3e
    unsigned char _pad3f[0x48 - 0x3f];
    CArray<NEnvironmentInternal::CEnvironmentThemePriorityStack> PriorityStackChannels;   // +0x48
    void TransitionToDefaultTheme(long channel, long priority, float duration)
    {
        if (ForcedEnvironmentTheme)
            return;
        if (channel < 0 || channel >= ENVIRONMENT_CHANNEL_COUNT)
            return;
        if (priority < 1 || priority >= ENVIRONMENT_PRIORITY_COUNT)
            return;
        PriorityStackChannels[channel].Ents[priority].TransitionToDefaultTheme(duration);
    }
private:
    static void __fastcall ConsoleRestoreDefaultTheme(CEnvironment& env, const vector<CGenericVar>& args);
};

void __fastcall CEnvironment::ConsoleRestoreDefaultTheme(CEnvironment& env, const vector<CGenericVar>& args)
{
    env.ForcedEnvironmentTheme = false;
    env.TransitionToDefaultTheme(args[0].ValSLONG, args[1].ValSLONG, args[2].ValFloat);
    env.ForcedEnvironmentTheme = true;
}