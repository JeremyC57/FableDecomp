// SBoastMapEntry::`scalar deleting destructor'(unsigned int) -- destroys a keyed CBoast record (Title, Text, ScriptName at +0x18/+0x14/+0x10) and frees it for flag bit 0
// Leaf: ??_GSBoastMapEntry
// Retail: Fable.exe 0x0056CBA0, 49 bytes. No vtable store: a non-polymorphic record. The manifest label
// "CThingSwitch::`vector_deleting_destructor'" is propagated. The destroyed offsets are Ego_r.pdb CBoast's
// (ScriptName +0x0c, Text +0x10, Title +0x14) shifted by one word, so the record is modelled as a
// pair<long, CBoast> (a map entry; the key type is inferred from the 4-byte shift, not proven).
// Callees: CWideString::~CWideString 0x0099B510 (twice), CCharString::~CCharString 0x0099EAE0,
// operator delete 0x00BFE9BC.
// Shape: a non-virtual `??_G` emitted for the `delete` in the helper below; CBoast's implicit destructor
// inlined (members in reverse declaration order, addressed straight off esi).
class CCharString { public:
    void* m_pData;
    ~CCharString();                                      // 0x0099EAE0
};

class CWideString { public:
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

struct SBoastMapEntry {                                  // std::pair<const long, CBoast>
    long   first;                                        // +0x00
    CBoast second;                                       // +0x04
};

void DestroyBoastMapEntry(SBoastMapEntry* entry)
{
    delete entry;
}