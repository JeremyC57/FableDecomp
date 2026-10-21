// Grade: DIFFER(464v465) -- the two script loops get each other's register shapes: retail loop 1 keeps the array base from the size test in esi (i ebx, offset ebp, ebp pushed in the prologue), loop 2 reloads it; ours is the other way round (ebp pushed before loop 2)
// NThingManager::CEntityRuntimePersistence::ClearPersistenceDataForSection(const CCharString&) -- clear the section's data from every valid level- and game-persistent entity script, then erase every persisted dead entity whose section-name CRC matches the section's
// Leaf: ClearPersistenceDataForSection
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00520390, length 480 bytes (body 0x1cf), `ret 4`.
// Callees: the per-script clear @ 0x0051F480 (thiscall: section, script info), the STLport tree erase rebalance
//   _Rb_global::_Rebalance_for_erase @ 0x0042A34E (cdecl: node, &root, &leftmost, &rightmost), free @ 0x00BFEA14.
// Globals: CRC-32 table @ 0x0129A168, the empty string @ 0x0129AAF4.
// Layout: PDB CEntityRuntimePersistence with the retail 12-byte CArray: LevelPersistentEntityScripts +0,
//   GamePersistentEntityScripts +0xc, PersistedDeadEntities (STLport map: header pointer +0x18, node count +0x1c);
//   CEntityScriptInfo 0x18 bytes (Valid +0x14); map node: parent +4, left +8, right +0xc, CPersistedDeadEntity at +0x18
//   (SectionNameCRC +0xc, so node +0x24). CCharString: string data pointer {text +0, length +4}.
extern "C" void __cdecl free(void* block);

struct CCharStringData {
    const char*   Text;                             // +0x00
    unsigned long Length;                           // +0x04
};

struct CCharString {
    CCharStringData* PStringData;
    unsigned long Length() const { return PStringData ? PStringData->Length : 0; }
    const char* CStr() const { return PStringData ? PStringData->Text : ""; }
};

extern unsigned long g_CRCTable_0129a168[256];      // 0x0129A168

__forceinline unsigned long GetCRC(const char* data, unsigned long length)
{
    unsigned long crc = 0;
    if (length == 0)
        return 0;
    for (unsigned long i = 0; i < length; ++i)
        crc = g_CRCTable_0129a168[(unsigned char)data[i] ^ (crc & 0xff)] ^ (crc >> 8);
    return crc;
}

template <class T> struct CArray {
    T* First;
    T* Last;
    T* End;
    unsigned long size() const { return Last - First; }
    T& operator[](unsigned long index) { return First[index]; }
};

struct SRbNode {
    long     Color;                                 // +0x00
    SRbNode* Parent;                                // +0x04
    SRbNode* Left;                                  // +0x08
    SRbNode* Right;                                 // +0x0c
};

SRbNode* __cdecl Rebalance_for_erase(SRbNode* node, SRbNode*& root, SRbNode*& leftmost, SRbNode*& rightmost);   // 0x0042A34E

inline SRbNode* NextNode(SRbNode* node)
{
    if (node->Right != 0) {
        node = node->Right;
        while (node->Left != 0)
            node = node->Left;
    } else {
        SRbNode* parent = node->Parent;
        while (node == parent->Right) {
            node = parent;
            parent = parent->Parent;
        }
        if (node->Right != parent)
            node = parent;
    }
    return node;
}

namespace NThingManager {
struct CEntityRuntimePersistence {
    struct CEntityScriptInfo {
        CCharString   Script;                       // +0x00
        unsigned char SaveData[0xc];                // +0x04 CArray<unsigned char>
        long          UncompressedSize;             // +0x10
        bool          Valid;                        // +0x14
    };
    struct SDeadNode : SRbNode {
        unsigned __int64 Key;                       // +0x10
        unsigned __int64 UID;                       // +0x18
        bool             GamePersistent;            // +0x20
        unsigned long    SectionNameCRC;            // +0x24
    };
    CArray<CEntityScriptInfo> LevelPersistentEntityScripts;   // +0x00
    CArray<CEntityScriptInfo> GamePersistentEntityScripts;    // +0x0c
    SRbNode*                  DeadHeader;                     // +0x18
    unsigned long             DeadCount;                      // +0x1c
    void ClearScriptForSection(const CCharString& section, CEntityScriptInfo* info);   // 0x0051F480
    __forceinline void ClearScriptsForSection(CArray<CEntityScriptInfo>& scripts, const CCharString& section)
    {
        for (unsigned long i = 0; i < scripts.size(); ++i) {
            if (scripts[i].Valid)
                ClearScriptForSection(section, &scripts[i]);
        }
    }
    void EraseDead(SRbNode* node)
    {
        SRbNode* erased = Rebalance_for_erase(node, DeadHeader->Parent, DeadHeader->Left, DeadHeader->Right);
        if (erased)
            free(erased);
        --DeadCount;
    }
    void ClearPersistenceDataForSection(const CCharString& section);
};
}

using namespace NThingManager;

void CEntityRuntimePersistence::ClearPersistenceDataForSection(const CCharString& section)
{
    ClearScriptsForSection(LevelPersistentEntityScripts, section);
    ClearScriptsForSection(GamePersistentEntityScripts, section);
    unsigned long crc = GetCRC(section.CStr(), section.Length());
    for (SRbNode* node = DeadHeader->Left; node != DeadHeader;) {
        if (static_cast<SDeadNode*>(node)->SectionNameCRC == crc) {
            SRbNode* erase = node;
            node = NextNode(node);
            EraseDead(erase);
        } else {
            node = NextNode(node);
        }
    }
}
