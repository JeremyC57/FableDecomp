// CCRC::CalcAndLookup(const CCharString& name, long* value) -- hash a name with the engine CRC and look the hash up in the owner's CRC -> value map; on a hit store the mapped value and return true
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00669D30, length 100 bytes (ret 8; 178 call sites, e.g. 0x0066385F).
// Callees: Calc @ 0x004014A0 (`__fastcall(seed, data, length)`, the engine's CRC-32 kernel, seed 0), the STLport
// map's find @ 0x006699C0 (manifest "BinaryTree_FindNode": `find(iterator* result, const unsigned long& key)`,
// header pointer at the map's +0, keys at node+0x10, mapped values at node+0x14).
// The owning class is unknown (the manifest name is kept); the map sits at +0x28.
// Shape: the name's length and text come from two null-case-first CCharString accessors (each tests the data block
// on its own; the length is evaluated first because Calc's stack argument is pushed first); the hash is homed in the
// dead `name` argument slot (`mov [esp+14h],eax`) and passed to find by address with the 4-byte iterator result in
// the frame; end() is the header pointer re-read after the call.
struct CCharStringData {
    const char*   PData;                      // +0x00
    unsigned long StringLength;               // +0x04
};
extern const char g_EmptyString[];            // 0x0129AAF4
class CCharString {
public:
    CCharStringData* PStringData;             // +0x00
    unsigned long GetLength() const { if (PStringData == 0) return 0; return PStringData->StringLength; }
    const char* GetText() const { if (PStringData == 0) return g_EmptyString; return PStringData->PData; }
};
unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0

struct SCRCMapNode {                          // STLport _Rb_tree_node<pair<const unsigned long, long> >
    long          Color;                      // +0x00
    SCRCMapNode*  Parent;                     // +0x04
    SCRCMapNode*  Left;                       // +0x08
    SCRCMapNode*  Right;                      // +0x0c
    unsigned long Key;                        // +0x10
    long          Value;                      // +0x14
};
struct SCRCMapIterator {
    SCRCMapNode* Node;
};
struct CCRCMap {                              // map<unsigned long, long>
    SCRCMapNode*  Header;                     // +0x00
    unsigned long NodeCount;                  // +0x04
    SCRCMapIterator* Find(SCRCMapIterator* result, const unsigned long& key);   // 0x006699C0
};

class CCRC {
public:
    unsigned char _pad00[0x28];
    CCRCMap       Map;                        // +0x28
    bool CalcAndLookup(const CCharString& name, long* value);
};

bool CCRC::CalcAndLookup(const CCharString& name, long* value)
{
    unsigned long crc = Calc(0, name.GetText(), name.GetLength());
    SCRCMapIterator it;
    Map.Find(&it, crc);
    if (it.Node != Map.Header) {
        *value = it.Node->Value;
        return true;
    }
    return false;
}
