// CTCTavernGameCardPairs::GetRandomCard(long* suit, long* value, long dealt, long total) -- draw random deck entries until one fits: in the second half of the deal it must pair with its mirrored first-half card (same value, different suit); in the first half it must share neither suit nor value with any card already dealt
// Grade: DIFFER(468v465) -- near miss; open gaps: (1) the derived card vptr store lands before the base Handle/flags init (retail stores it last); (2) mirror index folds to [edx+eax*4-4] where retail does sub/dec; (3) the inner suit/value search exits through the it!=last check rather than jumping straight back to the draw (goto forms thread the jumps but move the search loop below the epilogue, 461v465)
// Retail: Fable.exe @ 0x007D75A0, 465 bytes (`ret 0x10`, no EH frame).
// Callees: GetCardDef 0x008F0B20, card-entry base ctor 0x009FBEC0 / dtor 0x009FC550, GetRandomSeed 0x00686DA0,
//   GFROR13 0x00497890 (__fastcall), vector operator= 0x00431F90, free 0x00BFEA14
extern "C" void free(void* p);
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
#pragma pack(push, 1)
struct CByteVector {                                    // STLport vector (12 bytes) + 1 allocator byte
    void* First;
    void* Last;
    void* EndOfStorage;
    unsigned char Alloc;
    CByteVector() : First(0), Last(0), EndOfStorage(0) { }
    ~CByteVector() { if (First) free(First); }
    CByteVector& operator=(const CByteVector& o);       // 0x00431F90
};
struct CNameField {
    CByteVector V;                                      // +0x00
    bool        Used;                                   // +0x0d
    CNameField() : Used(false) { }
    CNameField& operator=(const CNameField& o) { V = o.V; Used = o.Used; return *this; }
};
struct CResourceBase {                                  // 0x24-byte polymorphic base (descriptive)
    CResourceBase();                                    // 0x009FBEC0
    ~CResourceBase();                                   // 0x009FC550
    virtual void v00();
    long Data[6];                                       // +0x04
    long Handle;                                        // +0x1c
    long Extra;                                         // +0x20
    unsigned char Flag0 : 1;                            // +0x24
    unsigned char Flag1 : 1;
    unsigned char Flag2 : 1;
};
struct CResourceMid : CResourceBase {                   // vtable 0x01230BA0
    CResourceMid() { Flag0 = 0; Flag1 = 0; Flag2 = 0; Handle = 0; }
    ~CResourceMid() { }
};
struct CTavernCard : CResourceMid {                     // 68-byte deck entry, vtable 0x01243A44
    long          Id;                                   // +0x25
    CNameField    Name;                                 // +0x29 (vector + flag at +0x36)
    unsigned char _pad37[0x3c - 0x37];
    long          Suit;                                 // +0x3c
    long          Value;                                // +0x40
    CTavernCard() : Id(-1) { }
    ~CTavernCard() { }
    __forceinline CTavernCard& operator=(const CTavernCard& o)
    {
        for (int k = 0; k < 6; ++k) Data[k] = o.Data[k];
        Handle = o.Handle;
        Extra = o.Extra;
        Flag0 = o.Flag0;
        Flag1 = o.Flag1;
        Flag2 = o.Flag2;
        Id = o.Id;
        Name = o.Name;
        Suit = o.Suit;
        Value = o.Value;
        return *this;
    }
};
#pragma pack(pop)
struct CTavernGameCardBaseDef : CDefBase {
    unsigned char _pad08[0x78 - 0x08];
    CTavernCard* DeckFirst;                             // +0x78
};
struct CDealtCard {
    unsigned char _pad00[4];
    long Suit;                                          // +0x04
    long Value;                                         // +0x08
};
long __fastcall GFROR13(unsigned long v);               // 0x00497890
struct CTCTavernGameCardPairs {
    virtual void v00();
    unsigned char _pad04[0x128 - 0x04];
    CDealtCard** DealtFirst;                            // +0x128
    CDealtCard** DealtLast;                             // +0x12c
    CDealtCard* Dealt(unsigned long n) const { return *(DealtFirst + n); }
    unsigned long* GetRandomSeed();                     // 0x00686DA0
    void GetCardDef(CDefPointer<CTavernGameCardBaseDef>* def);   // 0x008F0B20
    void GetRandomCard(long* suit, long* value, long dealt, long total);
};

void CTCTavernGameCardPairs::GetRandomCard(long* suit, long* value, long dealt, long total)
{
    CDefPointer<CTavernGameCardBaseDef> def;
    GetCardDef(&def);
    CTavernCard card;
    long half = total / 2;
    for (;;) {
        {
            unsigned long* seed = GetRandomSeed();
            *seed = *seed * 0x24a1 + 0x24df;
            *seed = GFROR13(*seed);
            card = def->DeckFirst[(unsigned long)*seed % 52];
        }
        if (dealt > half) {
            CDealtCard* mirror = Dealt(dealt - half - 1);
            if (mirror->Value != card.Value || mirror->Suit == card.Suit)
                continue;
        } else {
            CDealtCard** it;
            for (it = DealtFirst; it != DealtLast; ++it) {
                if ((*it)->Suit == card.Suit)
                    break;
                if ((*it)->Value == card.Value)
                    break;
            }
            if (it != DealtLast)
                continue;
        }
        break;
    }
    *suit = card.Suit;
    *value = card.Value;
}
