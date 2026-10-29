// CSoundEmitterLH::CActiveSound::CActiveSound(void* a, void* b, const char* name, void* d, bool e, bool f) -- CBase, the words at +8 / +0xc / +0x10, the flag bytes at +0x14 / +0x15, this class's vtable, a reference count of 1 at +4, the cleared word at +0x18, the name string at +0x1c built out of line, the cleared word at +0x20
// Leaf: ??0CActiveSound@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AAB8B0, length 87 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CCharString::CCharString(const char*) @ 0x0099EC30 (on +0x1c). Vtable 0x0129E67C = CSoundEmitterLH::CActiveSound
// (rtti-vtables.tsv). Shape: the six arguments are loaded in a shuffled order around the vtable store; the string's `push ; lea ecx` sits before the +4 / +0x14 /
// +0x18 stores; `ret 18h`. The chunk label "CEngineSubPrimitiveAnimatedMesh" is propagated.
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                       // 0x0099A2F0
};

class CCharString { public:
    void* m_pData;
    CCharString(const char* text);                 // 0x0099EC30
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
        long        m_18;                          // +0x18
        CCharString m_name;                        // +0x1c
        long        m_20;                          // +0x20
        CActiveSound(void* a, void* b, const char* name, void* d, bool e, bool f);
    };
};

CSoundEmitterLH::CActiveSound::CActiveSound(void* a, void* b, const char* name, void* d, bool e, bool f)
    : CBaseClassNonCopyable()
    , m_references(1)
    , m_pA(a), m_pB(b), m_pD(d)
    , m_e(e), m_f(f)
    , m_18(0)
    , m_name(name)
    , m_20(0)
{
}