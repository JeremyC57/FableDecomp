// CQuestManager::AreAllBoastsProcessed(CCharString&) const -- true unless an accepted boast of the given script is neither failed nor completed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF9C0, length 68 bytes.
// Callees: CCharString::operator!=(const CCharString&) const @ 0x0099E900 (out of line; the manifest's
// "less<CCharString>::operator()" label is propagated -- the body returns true for different strings).
// Identity: FableWin.pdb ?AreAllBoastsProcessed@CQuestManager@@QBE_NAAVCCharString@@@Z.
// Layout: AcceptedBoasts (list<CBoast>, one head pointer) at +0x6c (PDB +0xb0); the node is {next, prev, CBoast}
// with CBoast {ID +8, Cost +0xc, Reward +0x10, ScriptName +0x14, Text +0x18, Title +0x1c, Evil +0x20,
// Failed +0x21, Completed +0x22, GroupIndex +0x24} (PDB CBoast, 32 bytes).
// Shape: the head is re-read from the member for every end test; the argument pointer is loaded once in the
// preheader (ebx); the three skip tests share one `jne` target; `mov al,1` and `xor al,al` exits are separate.
class CCharString { public:
    void* PStringData;
    bool operator!=(const CCharString& other) const;   // 0x0099E900
};
class CWideString { public:
    void* PStringData;
};

struct CBoast {
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

struct CBoastNode {
    CBoastNode* next;
    CBoastNode* prev;
    CBoast      value;                             // +0x08
};

class CQuestManager { public:
    unsigned char _pad_0x00[0x6c];
    CBoastNode*   AcceptedBoasts;                  // +0x6c list<CBoast> (head node)
    bool AreAllBoastsProcessed(CCharString& scriptName) const;
};

bool CQuestManager::AreAllBoastsProcessed(CCharString& scriptName) const
{
    for (CBoastNode* node = AcceptedBoasts->next; node != AcceptedBoasts; node = node->next) {
        if (node->value.ScriptName != scriptName || node->value.Failed || node->value.Completed)
            continue;
        return false;
    }
    return true;
}