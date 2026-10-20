// CTCTavernGame::ClearBetAndMoney() -- reset every primitive handle of the bet, money and winnings displays (telling the primitive when the last handle goes)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E9F30, 208 bytes (plain `ret`).
// Callees: the engine primitive's virtual slot 3 (+0xC, passed the handle, when this is its last handle) and slot 1 (destroy,
// at refcount 0); no direct calls.
// Layout (retail; the PDB's 16-byte vectors are 12 here, see the landed ClearBox 0x005EC5C0): BetHandles +0x5c,
// MoneyHandles +0x68, WinningsHandles +0x74 (PDB +0x6c/+0x7c/+0x8c), each a vector<CEnginePrimitiveHandle>; a handle is
// 8 bytes (vptr + the counted engine primitive, count at +4).
// Shape: three copies of the same inline handle Reset() (the ClearBox model: null-safe GetRefCount() == 1 notification, then
// the release with both zero stores); unlike ClearBox the vectors are not emptied, only their handles are cleared.
struct CEnginePrimitiveHandle;
struct CEnginePrimitive {
    virtual void v00();
    virtual void Destroy();                                     // slot 1
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0xc)
    long RefCount;                                              // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CEnginePrimitiveHandle {
    virtual ~CEnginePrimitiveHandle();
    CEnginePrimitive* P;                                        // +0x04
    long GetRefCount() const { return P ? P->RefCount : 0; }
    void Reset()
    {
        if (GetRefCount() == 1)
            P->OnLastHandleReleased(this);
        if (P) {
            P->Release();
            P = 0;
        }
        P = 0;
    }
};
struct CHandleVector {                          // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    CEnginePrimitiveHandle* begin() { return _M_start; }
    CEnginePrimitiveHandle* end() { return _M_finish; }
};
struct CTCTavernGame {
    void* __vfptr;                              // +0x00 (CTCBase)
    void* ParentThing;                          // +0x04
    unsigned char _pad08[0x44 - 0x08];
    CHandleVector Handles;                      // +0x44
    CHandleVector BoxHandles;                   // +0x50
    CHandleVector BetHandles;                   // +0x5c
    CHandleVector MoneyHandles;                 // +0x68
    CHandleVector WinningsHandles;              // +0x74
    void ClearBetAndMoney();
};

void CTCTavernGame::ClearBetAndMoney()
{
    CEnginePrimitiveHandle* h;
    for (h = BetHandles.begin(); h != BetHandles.end(); ++h)
        h->Reset();
    for (h = MoneyHandles.begin(); h != MoneyHandles.end(); ++h)
        h->Reset();
    for (h = WinningsHandles.begin(); h != WinningsHandles.end(); ++h)
        h->Reset();
}