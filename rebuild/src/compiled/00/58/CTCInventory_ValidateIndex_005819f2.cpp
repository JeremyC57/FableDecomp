// CTCInventory::ValidateIndex(long) -- evaluates the main player's 8-byte-record count (a check whose result is dropped) and returns the index unchanged
// Leaf: ValidateIndex
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005819F2, 27 bytes.
// Callees: 0x00686D50 GetWorld (free: `return g_pGame->m_30;`, 0x013B8A1C), 0x00449960
// GetMainPlayer (__fastcall: `return GetPlayer(m_1c);`), 0x00445950 GetCount (__fastcall:
// `(m_7c8 - m_7c4) >> 3`).
//
// The manifest label (a CPersistContextReadSection constructor) is propagated and wrong: the body
// never touches `this` (ecx is overwritten by the first call's result), takes one dword argument
// (`ret 4`) and returns it. Its only caller is CTCInventory::UpdateContextSensitiveItems
// (0x00591B7F), which passes an index found in a global table (-1 skips the call), and its
// neighbours are CTCInventory methods. The chain world -> main player -> +0xC object -> GetCount()
// is kept only because the callees are out of line: a range check whose comparison compiled away
// (the result is dropped), so the source calls the chain as a statement.
struct CItemList {
    unsigned long GetCount();              // 0x00445950
};

class CPlayer { public:
    unsigned char _pad[0x0C];
    CItemList*    m_pItems;                // +0x0C
};

class CWorld { public:
    CPlayer* GetMainPlayer();              // 0x00449960
};

CWorld* GetWorld();                        // 0x00686D50

class CTCInventory { public:
    long ValidateIndex(long index);
};

long CTCInventory::ValidateIndex(long index)
{
    GetWorld()->GetMainPlayer()->m_pItems->GetCount();
    return index;
}