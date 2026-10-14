// CAnimFilterNamed_00739db0::~CAnimFilterNamed_00739db0() -- destroys the CCharString at +4 (@ 0x0099EAE0), then the inlined CAnimFilterBase destructor restores its vtable (0x0122FFCC)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00739DB0, length 19 bytes.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (frees the string data, decrements the global string count).
// The manifest label (C3DAnimationUtil::C3DAnimationUtil) is propagated and the callee is a
// destructor, so this is a destructor: the member destructor runs BEFORE the vtable store,
// which means the stored vtable belongs to the base (CAnimFilterBase per rtti-vtables.tsv,
// inline empty destructor) and the most-derived class owning the string stores no vtable of
// its own (novtable).
struct CCharString {
    void* PStringData;
    ~CCharString();                        // 0x0099EAE0
};

struct CAnimFilterBase {                   // vtable 0x0122FFCC
    virtual void v00();
    ~CAnimFilterBase() { }
};

struct __declspec(novtable) CAnimFilterNamed_00739db0 : CAnimFilterBase {
    CCharString m_name;                    // +4
    ~CAnimFilterNamed_00739db0();
};

CAnimFilterNamed_00739db0::~CAnimFilterNamed_00739db0()
{
}