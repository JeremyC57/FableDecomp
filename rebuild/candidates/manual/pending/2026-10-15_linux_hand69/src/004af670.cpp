// CQuestManager::GetAvailableQuestCardDefNameFromScriptName(const CCharString&) const -- the definition name of the available quest card whose script name equals the given one, or the shared empty string
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AF670, length 84 bytes.
// Callees: Compare(const char*, const char*) @ 0x00411570 (`__fastcall`, the inlined CCharString ==); the empty
// string returned on a miss lives at 0x013BD804 (as in the landed GetRegisteredQuestFromIndex @ 0x004AF3C0).
// Layout: AvailableQuestCardTypes (list<CQuestCard>, one head pointer in the shipped build) at +0x58 (PDB +0x7c);
// the node is {next, prev, CQuestCard {DefName +8, ScriptName +0xc, Replayable +0x10, FeatIndex +0x14}}.
// Shape: the head is held in ebx (a named local); the key's string block is loaded once AFTER the empty test and
// kept in esi across the Compare calls: a local initialised inside the `first != head` guard of a do/while (a
// local declared before the loop is scheduled above the test; a read inside the loop is re-done per iteration);
// the node's name on the left of the compare (`cmp eax,esi`), `neg ; sbb ; inc` for `Compare == 0`;
// the found exit returns `&node->DefName` (`lea eax,[edi+8]`).
struct CCharStringData { const char* m_pData; unsigned long m_length; };
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

struct CCharString {
    CCharStringData* PStringData;
};

__forceinline bool SameText(const CCharStringData* a, const CCharStringData* b)
{
    if (a == b)
        return true;
    if (a && b && a->m_length == b->m_length)
        return Compare_00411570(a->m_pData, b->m_pData) == 0;
    return false;
}

extern CCharString g_EmptyCharString;          // 0x013BD804

struct CQuestCard {
    CCharString DefName;                       // +0x00
    CCharString ScriptName;                    // +0x04
    bool        Replayable;                    // +0x08
    long        FeatIndex;                     // +0x0c
};

struct CQuestCardNode {
    CQuestCardNode* next;
    CQuestCardNode* prev;
    CQuestCard      value;                     // +0x08
};

struct CQuestManager {
    unsigned char   _pad_0x00[0x58];
    CQuestCardNode* AvailableQuestCardTypes;   // +0x58 list<CQuestCard> (head node)
    const CCharString& GetAvailableQuestCardDefNameFromScriptName(const CCharString& scriptName) const;
};

const CCharString& CQuestManager::GetAvailableQuestCardDefNameFromScriptName(const CCharString& scriptName) const
{
    CQuestCardNode* head = AvailableQuestCardTypes;
    CQuestCardNode* node = head->next;
    if (node != head) {
        const CCharStringData* key = scriptName.PStringData;
        do {
            if (SameText(node->value.ScriptName.PStringData, key))
                return node->value.DefName;
            node = node->next;
        } while (node != head);
    }
    return g_EmptyCharString;
}
