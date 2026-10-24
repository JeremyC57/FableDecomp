// CBrainBehaviourSet_00887330::Init(long name) -- one default behaviour, named, priority 1.0, then the set's state cleared
// Leaf: Init
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

struct CBrainBehaviour {
    long  Name;                                 // +0x00 CDefString
    float Priority;                             // +0x04
    long  ScriptAIPriority;                     // +0x08
    CBrainBehaviour() { memset(this, 0, sizeof(*this)); }
};

struct CRetailVector_CBrainBehaviour {          // VC6-era Dinkumware vector: first / last / end
    CBrainBehaviour* first;
    CBrainBehaviour* last;
    CBrainBehaviour* end;
    void resize(unsigned long count, const CBrainBehaviour& value);   // 0x00889440
};

struct CBrainBehaviourSet_00887330 {
    long                           m_current;   // +0x00
    long                           m_04;        // +0x04
    CRetailVector_CBrainBehaviour  m_behaviours;// +0x08
    unsigned char                  _pad[8];     // +0x14..+0x1b
    bool                           m_dirty;     // +0x1c
    void Init(long name);
};

void CBrainBehaviourSet_00887330::Init(long name)
{
    CBrainBehaviour value;
    m_behaviours.resize(1, value);
    m_behaviours.first->Name = name;
    m_behaviours.first->Priority = 1.0f;
    m_current = 0;
    m_dirty = false;
}