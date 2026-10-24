// CAnimFilterOf3Groups::~CAnimFilterOf3Groups() -- destroy the three animation-group names (Group3, Group2, Group1), then the CAnimFilterBase root
// Leaf: ??1CAnimFilterOf3Groups
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00739DF0, length 35 bytes (+ int3 padding), plain `ret`. Callers: the scalar
// deleting destructors of NCreatureMode::CModeStandFront/Left/Right (0x0073E3E0/0x0073E4D0/0x0073E5C0) and
// CModeSitChairStudious (0x009108E0) call it on their `Filter` member at +0x68 (`lea ecx,[esi+0x68]`) before the mode base
// destructor -- Ego_r.pdb CModeStandFront has `CAnimFilterOf3Groups Filter` at +0x68 (104). The manifest label
// (_Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo>>>) is propagated and wrong.
// Layout: Ego_r.pdb CAnimFilterOf3Groups (0x1c) = CAnimFilterBase {vptr}, CCharString Group1 +4, Group2 +8, Group3 +0xc,
// ulong Group1CRC/Group2CRC/Group3CRC +0x10..+0x18.
// Callees: CCharString::~CCharString @ 0x0099EAE0 (x3); vtable 0x0122FFCC = CAnimFilterBase (rtti-vtables.tsv).
// Shape: the filter declares no destructor (no own vtable store, GOTCHAS wave 25); the base's inline user destructor
// contributes the closing CAnimFilterBase vtable store.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};

class CAnimFilterBase {                                      // vtable 0x0122FFCC
public:
    virtual ~CAnimFilterBase() { }
    virtual bool IsAnimAllowed(const void* anim) const;      // slot 1 (the filter test)
};

class CAnimFilterOf3Groups : public CAnimFilterBase {        // 0x1c bytes
public:
    CCharString   Group1;                                    // +0x04
    CCharString   Group2;                                    // +0x08
    CCharString   Group3;                                    // +0x0c
    unsigned long Group1CRC;                                 // +0x10
    unsigned long Group2CRC;                                 // +0x14
    unsigned long Group3CRC;                                 // +0x18
};

void DestroyFilter(CAnimFilterOf3Groups* filter)             // emits the implicit destructor; not part of the retail body
{
    filter->CAnimFilterOf3Groups::~CAnimFilterOf3Groups();
}