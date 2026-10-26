// SQuestPersistEntry::`scalar deleting destructor'(unsigned int) -- destroys a pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> (the info's two strings, then the key) and frees it for flag bit 0
// Leaf: ??_GSQuestPersistEntry
// Retail: Fable.exe 0x004B77A0, 52 bytes. No vtable store: a non-polymorphic record. The manifest label
// "CThingSwitch::`vector_deleting_destructor'" is propagated over the byte-identical deleting destructors;
// the body sits among the pair<CCharString, CRuntimeQuestPersistence::CPersistInfo> helpers (0x004B7490,
// 0x004B7650, 0x004B7800) and Ego_r.pdb gives that pair 12 bytes (first +0, second +4), so the quest
// CPersistInfo is two CCharStrings (member names descriptive).
// Callees: CCharString::~CCharString 0x0099EAE0 (three times), operator delete 0x00BFE9BC.
// Shape: identical to 0x006FB730 -- a non-virtual `??_G` from a `delete` helper; the inner record's
// implicit destructor inlined with its address in edi.
class CCharString { public:
    void* m_pData;
    ~CCharString();                                      // 0x0099EAE0
};

struct CQuestPersistInfo {                               // NQuestManager::CRuntimeQuestPersistence::CPersistInfo
    CCharString m_questName;                             // +0x00
    CCharString m_persistScript;                         // +0x04
};

struct SQuestPersistEntry {                              // std::pair<CCharString, CPersistInfo>
    CCharString       first;                             // +0x00
    CQuestPersistInfo second;                            // +0x04
};

void DestroyQuestPersistEntry(SQuestPersistEntry* entry)
{
    delete entry;
}