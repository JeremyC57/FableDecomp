// CTCWallMount::OnSerialise(CPersistContext* context, bool flag) -- persist the bought-for amount and, when saving, refresh TrophyID from the live mounted trophy's UID (0 otherwise) before transferring it
// Leaf: OnSerialise
// Retail: Fable.exe @ 0x007DE530, 147 bytes (`ret 8`).
// Callees: CPersistContext::Transfer<long>(const char*, long&, const long&) 0x00410BE0; CBaseIntelligentPointer::GetPItem
//          0x00A01B50 (x3: alive test, dead test, UID read); CPersistContext::Transfer<unsigned __int64>(const char*,
//          unsigned __int64&, const unsigned __int64&) 0x0044BB00. Strings 0x0126FBF8 "BoughtForAmount", 0x0126FBEC
//          "TrophyID".
// Layout: Ego_r.pdb CTCWallMount (no shrunken containers, offsets hold): BoughtForAmount +0x0c, Trophy
//         (CIntelligentPointer<CThing>) +0x10, TrophyID (ulong64) +0x18; CPersistContext::Mode +0x18 (1/3 = saving);
//         CThing::UID +0x18, Dead flag +0x91 bit 0.
// Shape: each zero default is a temporary in the same frame slot (ebp = 0 shared with the TrophyID reset); IsSaving is the
//        inline `Mode == 1 || Mode == 3`.
class CThing {
public:
    unsigned char    _pad_0x00[0x18];
    unsigned __int64 UID;                       // +0x18
    unsigned char    _pad_0x20[0x71];
    unsigned char    Flags91;                   // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
};
class CBaseObject;
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CPersistContext { public:
    unsigned char _pad_0x00[0x18];
    long Mode;                                  // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, long& value, const long& defaultValue);                          // 0x00410BE0
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);  // 0x0044BB00
};

class CTCBase { public:                                // 0x0c bytes (a real base keeps the vptr from padding to 8)
    virtual void v00();
    CThing*          ParentThing;               // +0x04
    unsigned char    _pad_0x08[0x4];
};
class CTCWallMount : public CTCBase { public:
    long             BoughtForAmount;           // +0x0c
    CIntelligentPointer<CThing> Trophy;             // +0x10
    unsigned __int64 TrophyID;                  // +0x18
    void OnSerialise(CPersistContext* context, bool flag);
};

void CTCWallMount::OnSerialise(CPersistContext* context, bool flag)
{
    context->Transfer("BoughtForAmount", BoughtForAmount, 0);
    if (context->IsSaving()) {
        TrophyID = 0;
        if (Trophy.GetPItem() && !Trophy.GetPItem()->IsDead())
            TrophyID = Trophy.GetPItem()->UID;
    }
    context->Transfer("TrophyID", TrophyID, 0);
}