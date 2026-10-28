// NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::SetOnlyTheme(long themeID) -- one zeroed theme weight, its id set, weight 1.0, then the entry's state cleared
// Leaf: SetOnlyTheme
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00887330, length 73 bytes (+ int3 padding).
// Callees: vector<CBrainBehaviour>::resize(unsigned long, const CBrainBehaviour&) @ 0x00889440 on
// the retail vector at +0x08 (first/last/end).
// Shape: a 12-byte CBrainBehaviour local (Ego_r.pdb: Name +0, Priority +4, ScriptAIPriority +8)
// is zero-constructed in the frame and passed by reference to resize(1, value); the first element
// is then named from the argument and given priority 1.0f (retail stores 0x3F800000, so Priority
// is a float in TLC where the PDB says long); the vector's `first` is re-read after each store
// (no type-based aliasing in VC7.1). Finally the dword at +0 and the byte at +0x1c are cleared.
// The manifest label "CBrainBehaviour::Init" is kept as the leaf; the owning class (0x1d bytes
// or more) is named by what the body does.

#include <string.h>

namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {        // STLport, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void resize(unsigned int __new_size, const T& __x);           // 0x00889440 ?resize@?$vector@UCThemeWeight@...@std@@QAEXIABU...@@Z
};
}
namespace NEnvironmentInternal {
class CEnvironmentThemePriorityStackEntry { public:
    struct CThemeWeight {                                        // Ego_r.pdb, 12 bytes
        long  ThemeID;                                           // +0x00
        float Weight;                                            // +0x04
        bool  LockWeight;                                        // +0x08
        CThemeWeight() { memset(this, 0, sizeof(*this)); }
    };
    long                       m_current;                        // +0x00
    long                       m_04;                             // +0x04
    std::vector<CThemeWeight>  Themes;                           // +0x08 (Ego_r.pdb CArray<CThemeWeight> Themes)
    unsigned char              _pad[8];                          // +0x14..+0x1b
    bool                       m_dirty;                          // +0x1c
    void SetOnlyTheme(long themeID);
};
}

void NEnvironmentInternal::CEnvironmentThemePriorityStackEntry::SetOnlyTheme(long themeID)
{
    CThemeWeight value;
    Themes.resize(1, value);
    Themes._M_start->ThemeID = themeID;
    Themes._M_start->Weight = 1.0f;
    m_current = 0;
    m_dirty = false;
}