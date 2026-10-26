// CQuestManager::GetBoast(long id, CCharString scriptName) -- the boast with this ID (and, unless scriptName is empty, this script) from the Boasts list, or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF7A0, length 100 bytes (padded to 112), `ret 8`.
// Callees: CCharString::operator!=(const char*) @ 0x0099E960 (with "" @ 0x0122D70E), CCharString::operator!=(const
// CCharString&) @ 0x0099E900 (the manifest's "less<CCharString>::operator()" label is propagated), the by-value
// parameter's ~CCharString @ 0x0099EAE0 (callee-destroyed on both exits).
// Layout: Boasts (list<CBoast>, one head pointer) at +0x68 (PDB +0xa4); node {next, prev, CBoast +8} with the PDB CBoast
// (the landed AreAllBoastsProcessed 0x004AF9C0 model).
// Shape: the head is re-read for every end test, the ID is loaded once in the preheader (ebx); the string tests are an
// `||` of the inline operator== forms (`!(a != b)`), the argument string on the left.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);           // (by-value parameter; built by the caller)
    ~CCharString();                                  // 0x0099EAE0
    bool operator!=(const char* text) const;         // 0x0099E960
    bool operator!=(const CCharString& other) const; // 0x0099E900
    bool operator==(const char* text) const { return !(*this != text); }
    bool operator==(const CCharString& other) const { return !(*this != other); }
};
class CWideString { public:
    void* PStringData;
};
struct CBoast {                                    // CQuestManager::CBoast (PDB, 32 bytes)
    long        ID;                                // +0x00
    long        Cost;                              // +0x04
    long        Reward;                            // +0x08
    CCharString ScriptName;                        // +0x0c
    CWideString Text;                              // +0x10
    CWideString Title;                             // +0x14
    bool        Evil;                              // +0x18
    bool        Failed;                            // +0x19
    bool        Completed;                         // +0x1a
    long        GroupIndex;                        // +0x1c
};
struct CBoastNode {                                // STLport list node
    CBoastNode* next;
    CBoastNode* prev;
    CBoast      value;                             // +0x08
};
class CQuestManager { public:
    unsigned char _pad_0x00[0x68];
    CBoastNode*   Boasts;                          // +0x68 list<CBoast> (head node; PDB +0xa4)
    CBoastNode*   AcceptedBoasts;                  // +0x6c list<CBoast> (head node; PDB +0xb0)
    CBoast* GetBoast(long id, CCharString scriptName);
};
CBoast* CQuestManager::GetBoast(long id, CCharString scriptName)
{
    for (CBoastNode* node = Boasts->next; node != Boasts; node = node->next) {
        if (node->value.ID == id && (scriptName == "" || scriptName == node->value.ScriptName))
            return &node->value;
    }
    return 0;
}