// CWideStringPairOwner_004abde0::`scalar deleting destructor'(unsigned int) -- non-virtual: destroy the two wide strings of the pair at +4 (second first), then the flag-tested operator delete
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ABDE0, length 43 bytes.
// Callees: CWideString::~CWideString @ 0x0099B510 twice, operator delete @ 0x00BFE9BC. Shape: the pair's address is formed with `lea edi,[esi+4]` and the second
// string's with `lea ecx,[edi+4]`: the two strings are one pair member. The chunk label "CEngineSubPrimitiveText" is propagated; the class is named by address.
struct CWideString {
    void* m_pData;
    ~CWideString();                                // 0x0099B510
};

struct SWideStringPair_004abde0 {
    CWideString m_first;
    CWideString m_second;
};

struct CWideStringPairOwner_004abde0 {
    long                     m_0;
    SWideStringPair_004abde0 m_pair;               // +0x04
    ~CWideStringPairOwner_004abde0() { }
};

// The delete-expression that makes the compiler emit the scalar deleting destructor.
void Destroy_CWideStringPairOwner_004abde0(CWideStringPairOwner_004abde0* object)
{
    delete object;
}
