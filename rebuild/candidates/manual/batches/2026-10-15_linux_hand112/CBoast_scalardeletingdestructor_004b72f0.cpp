// CBoast::`scalar deleting destructor'(unsigned int) -- destroys Title, Text (CWideString) and ScriptName (CCharString) and frees the boast for flag bit 0
// Leaf: ??_GCBoast
// Retail: Fable.exe 0x004B72F0, 49 bytes. No vtable store: a non-polymorphic record. The manifest label
// "CThingSwitch::`vector_deleting_destructor'" is propagated; the offsets are Ego_r.pdb CBoast's (ScriptName
// +0x0c, Text +0x10, Title +0x14) and the body sits just before CBoast::CBoast 0x004B73A0.
// Callees: CWideString::~CWideString 0x0099B510 (twice), CCharString::~CCharString 0x0099EAE0,
// operator delete 0x00BFE9BC.
// Shape: a non-virtual `??_G` emitted for the `delete` in the helper below; members destroyed in
// reverse declaration order.
struct CCharString {
    void* m_pData;
    ~CCharString();                                      // 0x0099EAE0
};

struct CWideString {
    void* m_pData;
    ~CWideString();                                      // 0x0099B510
};

struct CBoast {
    long        ID;                                      // +0x00
    long        Cost;                                    // +0x04
    long        Reward;                                  // +0x08
    CCharString ScriptName;                              // +0x0c
    CWideString Text;                                    // +0x10
    CWideString Title;                                   // +0x14
    bool        Evil;                                    // +0x18
    bool        Failed;                                  // +0x19
    bool        Completed;                               // +0x1a
    long        GroupIndex;                              // +0x1c
};

void DestroyBoast(CBoast* boast)
{
    delete boast;
}
