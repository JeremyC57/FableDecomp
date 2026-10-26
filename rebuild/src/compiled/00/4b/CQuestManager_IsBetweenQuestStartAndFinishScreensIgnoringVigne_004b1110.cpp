// CQuestManager::IsBetweenQuestStartAndFinishScreensIgnoringVignettes() -- true when a quest between its start and finish screens has a live quest card whose quest is not a vignette
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B1110, length 133 bytes (padded to 144).
// Callees: GetActiveQuestCardFromScriptName(const CCharString&) @ 0x004B0C80 (on `this`), CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound const @ 0x004365B0 (component kind 0x6c, the quest card), the card component's vignette getter
// @ 0x007022E0 (`return Data(+0xc)->byte +0x59`; the manifest label "CTCShotDefinitionBase::CanBeHitByMeleeStrike" is a
// byte-twin propagation).
// Layout: retail BetweenStartAndEndScreenQuests (STLport vector<CCharString>, 4-byte elements) at +0x4c/+0x50 (PDB
// +0x6c); the card thing's dead bit +0x91 bit 0, mask word +0x2c (kind 0x6c = bit 12), component map +0x44.
// Shape: `for (unsigned long i = 0; i < size(); ++i)` with size() recomputed (`sar 2`, unsigned compare) and the
// element as `*(begin() + i)`; the three guards are one `&&` chain; the find's key temporary is the push-ecx frame word.
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation; key by address)
    __forceinline CTCBase_Retail* Find(long kind) const               // the inlined map `find`: end entry when absent
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCBase_Retail* GetTC(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
class CCharString { public:
    void* m_pStringData;
};
template <class T> struct CRetailVector {          // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
class CTCQuestCard { public:
    bool IsVignette() const;                       // 0x007022E0
};
class CQuestManager { public:
    unsigned char             _pad_0x00[0x4c];
    CRetailVector<CCharString> BetweenStartAndEndScreenQuests;   // +0x4c
    CThing_Retail* GetActiveQuestCardFromScriptName(const CCharString& name);   // 0x004B0C80
    bool IsBetweenQuestStartAndFinishScreensIgnoringVignettes();
};
bool CQuestManager::IsBetweenQuestStartAndFinishScreensIgnoringVignettes()
{
    for (unsigned long i = 0; i < BetweenStartAndEndScreenQuests.size(); ++i) {
        CThing_Retail* card = GetActiveQuestCardFromScriptName(BetweenStartAndEndScreenQuests[i]);
        if (card && !card->m_dead && card->HasTC(0x6c)) {
            if (!((CTCQuestCard*)GetTC(card, 0x6c))->IsVignette())
                return true;
        }
    }
    return false;
}