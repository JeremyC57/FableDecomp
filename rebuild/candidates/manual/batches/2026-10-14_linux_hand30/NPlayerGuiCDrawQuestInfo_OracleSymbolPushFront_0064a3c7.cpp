#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuestInfo::OracleSymbolPushFront(const CCharString&) -- resolve the symbol to a sprite index and prepend it to the oracle-symbol list when it is a real sprite
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064A3C7, length 38 bytes.
// Callees: CDrawQuestInfo::GetScriptSpriteBySymbol(const CCharString&) @ 0x0064A37B, list<long>::push_front(const long&) @ 0x00655ECE (on OracleSymbols +0x40; manifest "list<C2DCoordI>::_Construct_n" is propagated;
// the body inserts before the head node = end(), so this is push_front; the sibling @ 0x0064A3C7 inserts before head->next = push_front).
// Shape: a frame-pointer size TU; the sprite index is a local whose address is passed on, and VC homes it in the dead `symbol` argument slot
// (`mov [ebp+8],eax ; lea eax,[ebp+8]`); `jle` skips non-positive indices.
struct CCharString;

struct CRetailLongList {                           // VC6 Dinkumware list<long>: one sentinel pointer
    void* head;
    void push_front(const long& value);             // 0x00655ECE
};

namespace NPlayerGui {

struct CDrawQuestInfo {
    unsigned char   _pad[0x40];
    CRetailLongList OracleSymbols;                 // +0x40
    long GetScriptSpriteBySymbol(const CCharString& symbol);   // 0x0064A37B
    void OracleSymbolPushFront(const CCharString& symbol);
};

void CDrawQuestInfo::OracleSymbolPushFront(const CCharString& symbol)
{
    long sprite = GetScriptSpriteBySymbol(symbol);
    if (sprite > 0)
        OracleSymbols.push_front(sprite);
}

}
