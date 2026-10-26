// CShaderDataBank::`scalar deleting destructor'(unsigned int) -- destructor inlined: own vtable, the shader entries deleted, the StateBlock member's inline destructor, the Shaders vector's storage freed, then the CBankFile base destructor; frees `this` when bit 0 of the flags is set
// Leaf: ??_GCShaderDataBank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62860, length 72 bytes.
// Callees: the shader-entry teardown @ 0x00A62050 (on `this`: walks the +0x110 pointer vector and
//          deletes each entry; manifest label "CMeshDataBank::FreeDeviceSpecificResources" is
//          propagated), free @ 0x00BFEA14, CBankFile::~CBankFile() @ 0x009CE180 (stores the CBankFile
//          vtable 0x0129B54C), operator delete @ 0x00BFE9BC.
// Vtables: 0x0129D644 = CShaderDataBank (this body is its slot 0), 0x0129C82C = CBankStateBlock
// (rtti-vtables.tsv) stored into the member at +0x11c.
// Layout: the retail bank is narrower than the PDB one (PDB Shaders +0x174 .. StateBlock +0x19c; the
// shipped CBankFile ends at +0x110 and the Ego-only parser members are absent): Shaders
// (vector<CShaderBankEntry*>, 12 bytes) +0x110, StateBlock +0x11c.
// Shape: `??_G` with the destructor inlined (`__forceinline virtual ~CShaderDataBank`): the vtable
// store, the body's teardown call, then the members in reverse order -- the state block's inline
// destructor leaves its own vtable, the vector's inline destructor frees its block under a null
// test -- then the base destructor. The `new` factory (inline constructor) pulls the vtable and the
// deleting destructor into the TU.

extern "C" void __cdecl free(void* block);                                // 0x00BFEA14

class CBankFile { public:
    virtual ~CBankFile();                                                 // 0x009CE180
    unsigned char _body[0x10c];                                           // +0x04 .. +0x110
};

class CShaderBankEntry;

struct CShaderEntryVector {                                               // retail vector<CShaderBankEntry*> (12 bytes)
    CShaderBankEntry** m_first;
    CShaderBankEntry** m_last;
    CShaderBankEntry** m_end;
    CShaderEntryVector() : m_first(0), m_last(0), m_end(0) {}
    ~CShaderEntryVector()
    {
        if (m_first)
            free(m_first);
    }
};

class CBankStateBlock { public:                                                  // vtable 0x0129C82C
    virtual ~CBankStateBlock() {}
};

class CShaderDataBank : public CBankFile { public:                                      // vtable 0x0129D644
    CShaderEntryVector Shaders;                                           // +0x110
    CBankStateBlock    StateBlock;                                        // +0x11c
    CShaderDataBank() {}
    void DeleteShaderEntries();                                           // 0x00A62050
    __forceinline virtual ~CShaderDataBank()
    {
        DeleteShaderEntries();
    }
};

CShaderDataBank* NewShaderDataBank()
{
    return new CShaderDataBank();
}