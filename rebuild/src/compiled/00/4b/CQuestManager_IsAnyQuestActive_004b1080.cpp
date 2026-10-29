// CQuestManager::IsAnyQuestActive() -- true when an active quest card's quest component holds a non-vignette quest definition and no quest number has been assigned yet (field +0x14 negative)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B1080, length 143 bytes (padded to 144).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the card thing), CVectorMap<ETCInterfaceType, CTCBase*>::
// LowerBound const @ 0x004365B0 (component kind 0x6c, the quest card), the definition's release (virtual slot 1, +4).
// Layout: retail ActiveQuestCards (STLport list<CIntelligentPointer<CThing>>, node value at +8) at +0x5c (PDB +0x88);
// the card component holds an intrusively counted definition at +0xc (count at +4, vignette byte +0x59) and a long at
// +0x14.
// Shape: the component comes from an inline accessor returning an unset local when the kind is absent (GOTCHAS chunk
// 141): VC7.1 loads that local's frame word (shared with the find key) into esi once before the loop; the definition
// is a scoped counted-pointer copy whose count moves through inline AddRef()/DecRef() members (`inc/dec [ecx+4]` in
// memory; open-coded ++/-- go through a register), released on both paths; `QuestNumber <= -1` gives `cmp -1 ; jle`.
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry_Retail;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    const STCEntry_Retail* LowerBound(const ETCInterfaceType& kind) const;   // 0x004365B0 (the const instantiation; key by address)
    __forceinline CTCBase_Retail* Find(long kind) const               // the inlined map `find`: end entry when absent
    {
        const STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap_Retail;
class CBaseObject { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
__forceinline CTCBase_Retail* GetTC(const CBaseObject* thing, long kind) { return thing->m_tcs.Find(kind); }
class CQuestCardDef { public:                             // intrusively counted (vptr, count +4)
    virtual void v00();
    virtual void Release();                        // slot 1 (+4)
    long          m_refs;                          // +0x04
    void AddRef() { ++m_refs; }
    void DecRef() { if (--m_refs == 0) Release(); }
    unsigned char _pad08[0x59 - 0x08];
    bool          Vignette;                        // +0x59
};
struct CQuestCardDefPtr {
    CQuestCardDef* p;
    CQuestCardDefPtr(const CQuestCardDefPtr& other) : p(other.p) { if (p) p->AddRef(); }
    ~CQuestCardDefPtr() { if (p) p->DecRef(); }
};
class CTCQuestCard { public:
    unsigned char    _pad00[0xc];
    CQuestCardDefPtr Def;                          // +0x0c
    unsigned char    _pad10[4];
    long             QuestNumber;                  // +0x14
};
class CBaseIntelligentPointer { public:
    void* __vftable;
    void* m_pItem;
public:
    CBaseObject* GetPItem() const;   // 0x00A01B50
};
struct CCardNode {                                 // STLport list node
    CCardNode*                 next;
    CCardNode*                 prev;
    CBaseIntelligentPointer value;              // +0x08
};
__forceinline CTCQuestCard* GetQuestCard(CBaseObject* thing)
{
    CTCQuestCard* tc;
    if (thing->HasTC(0x6c))
        tc = (CTCQuestCard*)GetTC(thing, 0x6c);
    return tc;
}
class CQuestManager { public:
    unsigned char _pad_0x00[0x5c];
    CCardNode*    ActiveQuestCards;                // +0x5c
    bool IsAnyQuestActive();
};
bool CQuestManager::IsAnyQuestActive()
{
    for (CCardNode* node = ActiveQuestCards->next; node != ActiveQuestCards; node = node->next) {
        CTCQuestCard* card = GetQuestCard(node->value.GetPItem());
        if (card) {
            CQuestCardDefPtr def(card->Def);
            if (def.p && !def.p->Vignette && card->QuestNumber <= -1)
                return true;
        }
    }
    return false;
}