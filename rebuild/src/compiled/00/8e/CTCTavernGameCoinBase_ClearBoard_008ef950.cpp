// CTCTavernGameCoinBase::ClearBoard() -- kills every owned coin thing that is still alive, then empties the coin list
// Leaf: ClearBoard
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EF950, 171 bytes (plain `ret`). Ego_r.pdb
// `?ClearBoard@CTCTavernGameCoinBase@@QAEXXZ`.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (x3 per coin); CThing::Kill(bool) 0x004C9B80 (true);
//          STLport __copy<Coin*>(first, last, result, const random_access_iterator_tag&, int*) 0x008F02E0 (`__fastcall`,
//          `ret 0Ch`; manifest "_Move<Coin*,Coin*>"); Coin's virtual destructor (slot 0, explicit call with flags 0) per
//          erased coin.
// Layout: retail CTCTavernGameCoinBase::Coins (vector<Coin>, 12 bytes) +0xfc (PDB +0x114; CTCTavernGame is 0xf0 bytes in
//         retail). Coin (Ego_r.pdb, 0x3c bytes): vptr, Owned +0x24, Thing (CIntelligentPointer<CThing>) at the odd offset
//         +0x25 (the pointer classes are `#pragma pack(1)`, Coin itself is not), Radius +0x30, Owner +0x34, Sound +0x38.
// Shape: the IS_THING_ALIVE idiom (GetPItem null test, then the dead bit of the re-fetched thing) before the Owned test,
//        a third GetPItem for the Kill; the shared ebx = 1 serves both the dead-bit mask and the `true` push; the clear is
//        the inline STLport `erase(begin(), end())`: `__copy(last, finish, first, tag, (int*)0)`, the range destroy, then
//        `finish = i` (as the landed CTCTavernGame::ClearBox 0x005EC5C0).
class CBaseObject;

class CThing { public:
    unsigned char _pad_0x00[0x91];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    void Kill(bool immediately);                // 0x004C9B80
};

#pragma pack(push, 1)
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
public:

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)

class Coin { public:                                   // 0x3c bytes
    virtual ~Coin();
    unsigned char _pad_0x04[0x20];
    bool          Owned;                        // +0x24
    CIntelligentPointer<CThing> Thing;             // +0x25
    float         Radius;                       // +0x30
    long          Owner;                        // +0x34
    unsigned long Sound;                        // +0x38
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
Coin* __fastcall __copy(Coin* first, Coin* last, Coin* result, const random_access_iterator_tag& tag, int* distance);   // 0x008F02E0

struct CCoinVector {                            // STLport vector<Coin> (12 bytes)
    Coin* _M_start;
    Coin* _M_finish;
    Coin* _M_end_of_storage;
    Coin* begin() { return _M_start; }
    Coin* end() { return _M_finish; }
    static void _Destroy(Coin* first, Coin* last)
    {
        for (; first != last; ++first)
            first->~Coin();
    }
    Coin* erase(Coin* first, Coin* last)
    {
        Coin* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _Destroy(i, _M_finish);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};

struct CTCTavernGameCoinBase {
    virtual void v00();
    unsigned char _body04[0xfc - 0x04];         // CTCTavernGame (retail 0xf0) + Arrow, Arrow1, Arrow2
    CCoinVector   Coins;                        // +0xfc
    void ClearBoard();
};

void CTCTavernGameCoinBase::ClearBoard()
{
    for (Coin* coin = Coins.begin(); coin != Coins.end(); ++coin) {
        if (coin->Thing.GetPItem() && !coin->Thing.GetPItem()->IsDead() && coin->Owned)
            coin->Thing.GetPItem()->Kill(true);
    }
    Coins.clear();
}