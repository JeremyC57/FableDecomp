// CQuestManager::SetBoastAsFailed(long id, CCharString scriptName) -- mark the accepted boast with this ID (and, unless scriptName is empty, this script) as failed
// Leaf: SetBoastAsFailed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF880, length 99 bytes (padded to 112), `ret 8`.
// Label correction: the catalog calls both 0x004AF880 and 0x004AF8F0 "SetBoastAsCompleted"; this one sets CBoast::Failed
// (node +0x21 = CBoast +0x19), the twin at 0x004AF8F0 sets Completed (+0x1a).
// Callees: CCharString::operator!=(const char*) @ 0x0099E960 (with "" @ 0x0122D70E), CCharString::operator!=(const
// CCharString&) @ 0x0099E900, the by-value parameter's ~CCharString @ 0x0099EAE0.
// Layout: AcceptedBoasts (list<CBoast>) at +0x6c (PDB +0xb0); node {next, prev, CBoast +8}.
// History: three plain forms (flat loop, pointer-returning helper, end-node helper) differed; this twin form of
// 0x004AF8F0 was then applied and verified once.
// Shape: the search is a __forceinline FindAcceptedBoast(id, name, CBoast*& out) returning bool; only that form gives
// retail's ebx push on the non-empty path and the store straight on the hit (an inline loop in the body pushes ebx at entry,
// a pointer-returning helper adds `lea ; test` before the store).
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);           // (by-value parameter; built by the caller)
    ~CCharString();                                  // 0x0099EAE0
    bool operator!=(const char* text) const;         // 0x0099E960
    bool operator!=(const CCharString& other) const; // 0x0099E900
    bool operator==(const char* text) const { return !(*this != text); }
    bool operator==(const CCharString& other) const { return !(*this != other); }
};
struct CWideString {
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
struct CQuestManager {
    unsigned char _pad_0x00[0x68];
    CBoastNode*   Boasts;                          // +0x68 list<CBoast> (head node; PDB +0xa4)
    CBoastNode*   AcceptedBoasts;                  // +0x6c list<CBoast> (head node; PDB +0xb0)
    __forceinline bool FindAcceptedBoast(long id, const CCharString& scriptName, CBoast*& boast)
    {
        for (CBoastNode* node = AcceptedBoasts->next; node != AcceptedBoasts; node = node->next) {
            if (node->value.ID == id && (scriptName == "" || scriptName == node->value.ScriptName)) {
                boast = &node->value;
                return true;
            }
        }
        return false;
    }
    void SetBoastAsFailed(long id, CCharString scriptName);
};
void CQuestManager::SetBoastAsFailed(long id, CCharString scriptName)
{
    CBoast* boast;
    if (FindAcceptedBoast(id, scriptName, boast))
        boast->Failed = true;
}
