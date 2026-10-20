// CQuestManager::IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(const CCharString& region) -- true when a quest between its start and finish screens has a live, non-vignette quest card whose region is the given (non-empty) region
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B1340, length 270 bytes, `ret 4`.
// Callees: CCharString::operator!=(const char*) 0x0099E960 (the empty-region guard), GetActiveQuestCardFromScriptName
// 0x004B0C80, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 (TC 0x6c, the quest card),
// CTCQuestCard::IsVignette 0x007022E0, CTCQuestCard::GetRegionName 0x007025A0 (by value), Compare 0x00411570
// (`__fastcall`, the inlined CCharString ==), ~CCharString 0x0099EAE0. Literal "" 0x0122D70E.
// Layout: retail BetweenStartAndEndScreenQuests (STLport vector<CCharString>) +0x4c (PDB +0x6c); CThing dead bit
// +0x91 bit 0, TC mask +0x20 (0x6c = word 3 bit 12), Components +0x44.
// Shape: the landed sibling IsBetweenQuestStartAndFinishScreensIgnoringVignettes (0x004B1110) loop and map model,
// with `T* tc = 0; if (HasTC) tc = GetTC;` for the `xor esi,esi` preload, nested ifs (an `&&` makes the temporary
// conditional, with a construction flag) and an inline
// CCharString == whose length+Compare tail is a nested inline IsEqual (its own `xor al,al ; mov bl,al` false exit);
// the match bool survives the temporary's destructor in bl.
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

struct CCharStringData {                               // the shared string block
    const char*   Data;                                // +0x00
    unsigned long Length;                              // +0x04
    bool IsEqual(const CCharStringData& other) const
    {
        if (Length != other.Length)
            return false;
        return Compare_00411570(Data, other.Data) == 0;
    }
};
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                    // 0x0099EAE0
    bool operator!=(const char* text) const;           // 0x0099E960
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = PStringData;
        CCharStringData* b = other.PStringData;
        if (a == b)
            return true;
        if (a == 0 || b == 0)
            return false;
        return a->IsEqual(*b);
    }
};

struct CTCBase_Retail;
struct STCEntry_Retail {                               // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                                 // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                        // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                              // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                         // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCBase_Retail* GetTC(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }

template <class T> struct CRetailVector {              // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CTCQuestCard {
    bool IsVignette() const;                           // 0x007022E0
    CCharString GetRegionName();                       // 0x007025A0
};
struct CQuestManager {
    unsigned char             _pad_0x00[0x4c];
    CRetailVector<CCharString> BetweenStartAndEndScreenQuests;   // +0x4c
    CThing_Retail* GetActiveQuestCardFromScriptName(const CCharString& name);   // 0x004B0C80
    bool IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(const CCharString& region);
};

bool CQuestManager::IsActiveQuestBetweenQuestStartAndFinishScreensInRegion(const CCharString& region)
{
    if (region != "") {
        for (unsigned long i = 0; i < BetweenStartAndEndScreenQuests.size(); ++i) {
            CThing_Retail* card = GetActiveQuestCardFromScriptName(BetweenStartAndEndScreenQuests[i]);
            if (card && !card->m_dead) {
                CTCQuestCard* tc = 0;
                if (card->HasTC(0x6c))
                    tc = (CTCQuestCard*)GetTC(card, 0x6c);
                if (!tc->IsVignette()) {
                    if (tc->GetRegionName() == region)
                        return true;
                }
            }
        }
    }
    return false;
}
