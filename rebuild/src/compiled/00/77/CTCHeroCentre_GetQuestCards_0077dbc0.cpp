// CTCHeroCentre::GetQuestCards(vector<CIntelligentPointer<CThing>>& cards) const -- copy the quest-card pointers into `cards` (cleared first); false when there are none
// Leaf: GetQuestCards
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077DBC0, length 116 bytes (+12 int3 padding), `ret 4`.
// Ego_r.pdb: ?GetQuestCards@CTCHeroCentre@@QBE_NAAV?$vector@V?$CIntelligentPointer@VCThing@@@@...@std@@@Z
// Callees: the out-of-line STLport vector<CIntelligentPointer<CThing>>::erase(first, last) @ 0x004AC0F0 (manifest
// "std::vector::clear"; called as cards.erase(cards.begin(), cards.end())), vector::resize(unsigned) @ 0x00739B40 (manifest
// label names the CParticleEmitter instantiation; same code), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 / SetPItem
// @ 0x00A01B90 (the inline CIntelligentPointer operator=).
// Shape: `if (!empty()) { ...; return true; } return false;` (the false exit sits after the body); empty test on the loaded begin/end, resize to size(), then an index loop with STLport `*(begin() + i)` element access
// (size re-read each pass); the destination begin is read before the source element's GetPItem.
// Layout: retail QuestCards +0x18 (12-byte vector; engine/CTCHeroCentre.h has the 16-byte PDB layout).
class CThing;
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
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
typedef CIntelligentPointer<CThing> CThingPointer;
struct CThingPointerVector {                    // STLport vector<CIntelligentPointer<CThing>>
    CThingPointer* _M_start;
    CThingPointer* _M_finish;
    CThingPointer* _M_end_of_storage;
    CThingPointer* begin() { return _M_start; }
    CThingPointer* end() { return _M_finish; }
    const CThingPointer* begin() const { return _M_start; }
    const CThingPointer* end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    bool empty() const { return begin() == end(); }
    CThingPointer& operator[](unsigned int n) { return *(begin() + n); }
    const CThingPointer& operator[](unsigned int n) const { return *(begin() + n); }
    CThingPointer* erase(CThingPointer* first, CThingPointer* last);   // 0x004AC0F0
    void resize(unsigned int n);                                        // 0x00739B40
    void clear() { erase(begin(), end()); }
};
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCHeroCentre : public CTCBase {
public:
    void*               PDef;                   // +0x0c
    long                CurrentGUIMode;         // +0x10
    unsigned long       FrameQuestCardListFrom; // +0x14
    CThingPointerVector QuestCards;             // +0x18
    bool GetQuestCards(CThingPointerVector& cards) const;
};

bool CTCHeroCentre::GetQuestCards(CThingPointerVector& cards) const
{
    cards.clear();
    if (!QuestCards.empty()) {
        cards.resize(QuestCards.size());
        for (unsigned int i = 0; i < QuestCards.size(); ++i)
            cards[i] = QuestCards[i];
        return true;
    }
    return false;
}