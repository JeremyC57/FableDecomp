// CQuestManager::IsBoastTaken(long id, CCharString scriptName) -- true when the AcceptedBoasts list holds a boast with this ID (and, unless scriptName is empty, this script)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF810, length 99 bytes (padded to 112), `ret 8`.
// Callees: CCharString::operator!=(const char*) @ 0x0099E960 (with "" @ 0x0122D70E), CCharString::operator!=(const
// CCharString&) @ 0x0099E900 (called on the node's ScriptName), the by-value parameter's ~CCharString @ 0x0099EAE0.
// Layout: AcceptedBoasts (list<CBoast>) at +0x6c (PDB +0xb0); node {next, prev, CBoast +8}.
// Shape: as GetBoast (0x004AF7A0) with the second comparison written node-first (`node->ScriptName == scriptName`);
// separate `mov al,1` / `xor al,al` exits.
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
    bool IsBoastTaken(long id, CCharString scriptName);
};
bool CQuestManager::IsBoastTaken(long id, CCharString scriptName)
{
    for (CBoastNode* node = AcceptedBoasts->next; node != AcceptedBoasts; node = node->next) {
        if (node->value.ID == id && (scriptName == "" || node->value.ScriptName == scriptName))
            return true;
    }
    return false;
}