// CShaderDataBank::CShaderDataBank() -- CBankFile base, this vtable, an empty 12-byte vector at +0x110, then the embedded CShaderBankStateBlock's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62020, length 48 bytes.
// Callees: CBankFile::CBankFile @ 0x009CD480.
// Vtables: 0x0129D644 CShaderDataBank (this), 0x0129D630 CShaderBankStateBlock (the member at
// +0x11c, whose inline constructor stores only its vtable). rtti-vtables.tsv names both; the
// manifest label "CBedDef" is propagated garbage.
// Shape: the derived vtable store precedes the zero stores because the zeroed block is a member
// object (a retail vector, first/last/end) with an inline constructor; the polymorphic member
// is constructed after it in declaration order.
struct CBankFile {
    virtual void v00();
    unsigned char _body[0x10c];                 // +0x004..+0x10f
    CBankFile();                                // 0x009CD480
};

template <class T>
struct CRetailVector {                          // VC6-era Dinkumware vector: first / last / end
    T* first;
    T* last;
    T* end;
    CRetailVector() : first(0), last(0), end(0) { }
};

struct CShaderBankStateBlock {                  // vtable 0x0129D630
    virtual void v00();
    CShaderBankStateBlock() { }
};

struct CShaderDataBank : CBankFile {            // vtable 0x0129D644
    CRetailVector<void*>  m_shaders;            // +0x110
    CShaderBankStateBlock m_stateBlock;         // +0x11c
    virtual void v00();
    CShaderDataBank();
};

CShaderDataBank::CShaderDataBank()
    : CBankFile(), m_shaders(), m_stateBlock()
{
}