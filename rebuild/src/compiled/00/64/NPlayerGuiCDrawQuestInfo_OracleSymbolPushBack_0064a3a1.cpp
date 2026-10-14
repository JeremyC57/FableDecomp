#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::OracleSymbolPushBack(const CCharString&) -- resolve the symbol to a sprite index and append it to the oracle-symbol list when it is a real sprite
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064A3A1, length 38 bytes.
// Callees: CDrawQuestInfo::GetScriptSpriteBySymbol(const CCharString&) @ 0x0064A37B, list<long>::push_back(const long&) @ 0x0042B687 (on OracleSymbols +0x40;
// the body inserts before the head node = end(), so this is push_back; the sibling @ 0x0064A3C7 inserts before head->next = push_front).
// Shape: a frame-pointer size TU; the sprite index is a local whose address is passed on, and VC homes it in the dead `symbol` argument slot
// (`mov [ebp+8],eax ; lea eax,[ebp+8]`); `jle` skips non-positive indices.
struct CCharString;

struct CRetailLongList {                           // VC6 Dinkumware list<long>: one sentinel pointer
    void* head;
    void push_back(const long& value);             // 0x0042B687
};

namespace NPlayerGui {

struct CDrawQuestInfo {
    unsigned char   _pad[0x40];
    CRetailLongList OracleSymbols;                 // +0x40
    long GetScriptSpriteBySymbol(const CCharString& symbol);   // 0x0064A37B
    void OracleSymbolPushBack(const CCharString& symbol);
};

void CDrawQuestInfo::OracleSymbolPushBack(const CCharString& symbol)
{
    long sprite = GetScriptSpriteBySymbol(symbol);
    if (sprite > 0)
        OracleSymbols.push_back(sprite);
}

}