// CSoundEmitterLH::CActiveSound::CActiveSound(void*, void*, void*, void*, bool, bool) -- CBase, the four argument words, the two flag bytes, this class's vtable, a cleared count, an empty name, a cleared word at +0x20
// Leaf: ??0CActiveSound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAB850, length 84 bytes (+ int3 padding).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CCharString::CCharString() @ 0x0099E4B0 (on +0x1c).
// Identity: the manifest label "CTCAISummon::CTCAISummon" is propagated; the vtable stored is 0x0129E67C =
// CSoundEmitterLH::CActiveSound (rtti-vtables.tsv). This is the overload next to the landed
// CActiveSound(a, b, const char* name, d, e, f) @ 0x00AAB8B0: here the third argument is stored as the
// word at +0x18 (the landed overload clears it), the name string is default-constructed and the word
// at +4 starts at 0 (1 in the landed overload). No PDB layout exists for the nested class; member
// names are descriptive and follow the landed sibling.
//
// Shape: `ret 18h` = six stack arguments; the argument words and bytes are loaded in a shuffled order
// around the vtable store (scheduling of the plain init-list members), the string member's
// `lea ecx,[esi+1Ch]` sits before the +4 / +0x14 / +0x18 stores, the trailing word follows the string.
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};

class CCharString { public:
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
};

struct CSoundEmitterLH {
    struct CActiveSound : CBaseClassNonCopyable {                  // vtable 0x0129E67C
        long        m_references;                  // +0x04
        void*       m_pA;                          // +0x08
        void*       m_pB;                          // +0x0c
        void*       m_pD;                          // +0x10
        bool        m_e;                           // +0x14
        bool        m_f;                           // +0x15
        unsigned char _pad16[2];
        void*       m_18;                          // +0x18
        CCharString m_name;                        // +0x1c
        long        m_20;                          // +0x20
        CActiveSound(void* a, void* b, void* c, void* d, bool e, bool f);
    };
};

CSoundEmitterLH::CActiveSound::CActiveSound(void* a, void* b, void* c, void* d, bool e, bool f)
    : CBaseClassNonCopyable()
    , m_references(0)
    , m_pA(a), m_pB(b), m_pD(d)
    , m_e(e), m_f(f)
    , m_18(c)
    , m_name()
    , m_20(0)
{
}