// CTCHeroCentre::RemoveQuestCard(CThing* card) -- find the live quest card in the QuestCards vector and erase it (shift the tail down, destroy the last intelligent pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077D400, length 104 bytes (+8 int3 padding), `ret 4` (the final virtual
// destructor call tail-jumps with the argument slot rewritten to 0).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the `__fastcall` STLport
// __copy<CIntelligentPointer<CThing>*>(first, last, result, const random_access_iterator_tag&, int*) @ 0x004AA610
// (manifest "CBaseIntelligentPointer_CopyRange"), the element's virtual destructor (slot 0, flag 0).
// Shape: the inline IsThingAlive test (null, then the dead bit 0 of CThing +0x91) before the identity compare; the inline
// STLport erase(iterator): `if (pos + 1 != end()) __copy(pos + 1, end(), pos, random_access_iterator_tag(), (int*)0);
// --_M_finish; _Destroy(_M_finish);` with the tag temporary in the dead `card` slot (card lives in ebp).
// Layout: engine/CTCHeroCentre.h is the PDB layout (16-byte vectors); retail: PDef +0x0c, CurrentGUIMode +0x10,
// FrameQuestCardListFrom +0x14, QuestCards +0x18 (12-byte vector of 8-byte CIntelligentPointer<CThing>), QuestCardPositions +0x24.
struct random_access_iterator_tag { random_access_iterator_tag() {} };
class CThing {
public:
    unsigned char _pad00[0x91];
    unsigned char Dead : 1;                     // +0x91 bit 0
};
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    void* PData;
    CThing* GetPItem() const;                   // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};
typedef CIntelligentPointer<CThing> CThingPointer;
CThingPointer* __fastcall __copy(CThingPointer* first, CThingPointer* last, CThingPointer* result,
                                 const random_access_iterator_tag& tag, int* distance);   // 0x004AA610
struct CThingPointerVector {                    // STLport vector<CIntelligentPointer<CThing>>
    CThingPointer* _M_start;
    CThingPointer* _M_finish;
    CThingPointer* _M_end_of_storage;
    CThingPointer* begin() { return _M_start; }
    CThingPointer* end() { return _M_finish; }
    void erase(CThingPointer* pos)
    {
        if (pos + 1 != _M_finish)
            __copy(pos + 1, _M_finish, pos, random_access_iterator_tag(), (int*)0);
        --_M_finish;
        _M_finish->~CThingPointer();
    }
};
inline bool IsThingAlive(const CThing* thing) { return thing && !thing->Dead; }

struct CDefPointerHeroCentre { void* Object; };
class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCHeroCentre : public CTCBase {
public:
    CDefPointerHeroCentre PDef;                 // +0x0c
    long                  CurrentGUIMode;       // +0x10
    unsigned long         FrameQuestCardListFrom;   // +0x14
    CThingPointerVector   QuestCards;           // +0x18
    void RemoveQuestCard(CThing* card);
};

void CTCHeroCentre::RemoveQuestCard(CThing* card)
{
    for (CThingPointer* it = QuestCards.begin(); it != QuestCards.end(); ++it) {
        CThing* thing = it->GetPItem();
        if (IsThingAlive(thing) && card == thing) {
            QuestCards.erase(it);
            return;
        }
    }
}
