// CTCInventoryAbilities::InitialiseExpressions() -- sizes the received-expression order to the number of EXPRESSION definitions (new entries zeroed)
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F3E3E, length 63 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CTCBase::GetDefinitions() @ 0x006869D0,
//          CDefinitionManager::GetNumberOfDefsOfType(const CCharString&) @ 0x009AD3B0 (manifest label
//          "LTextSystem::GetIDFromSymbol" is propagated; the body counts the type's def list),
//          std::vector<long>::resize(unsigned long) @ 0x004433F0, CCharString::~CCharString @ 0x0099EAE0.
//          The literal "EXPRESSION" sits at 0x01235D94.
// Pragmas: frame-pointer size TU (`optimize("y", off)` + `("s", on)`; `push ecx` frame word, `leave`).
// Layout: engine/CTCInventoryAbilities.h is the Ego_r.pdb layout (16-byte vectors); in the shipped class
//         ReceivedExpressionOrder (vector<long>) is at +0x1BC (PDB +0x1F8).
// Shape: the type name is an IMPLICIT conversion temporary (`"EXPRESSION"` through the defaulted length,
//        re-addressed with `lea eax,[ebp-4]` after its constructor), built before GetDefinitions() is called.

class CCharString { public:
    char* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CDefinitionManager { public:
    long GetNumberOfDefsOfType(const CCharString& typeName);   // 0x009AD3B0
};

struct CLongVector {                              // std::vector<long>
    long* first;
    long* last;
    long* end_of_storage;
    void resize(unsigned long size);              // 0x004433F0
};

struct CTCInventoryAbilities_Retail {
    unsigned char _pad_0x000[0x1BC];
    CLongVector   ReceivedExpressionOrder;        // +0x1BC (PDB +0x1F8)

    CDefinitionManager* GetDefinitions();         // 0x006869D0 (CTCBase)
    void InitialiseExpressions();
};

void CTCInventoryAbilities_Retail::InitialiseExpressions()
{
    ReceivedExpressionOrder.resize(GetDefinitions()->GetNumberOfDefsOfType("EXPRESSION"));
}