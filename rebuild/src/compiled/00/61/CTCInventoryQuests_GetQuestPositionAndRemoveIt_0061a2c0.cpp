// CTCInventoryQuests::GetQuestPositionAndRemoveIt(long questDefIndex) -- finds the live quest thing of the given def in the quest list, erases it and returns its former position (-1 when absent)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A2C0, length 153 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the `__fastcall` STLport
// __copy<CIntelligentPointer<CThing>*>(first, last, result, const random_access_iterator_tag&, int*) @ 0x004AA610
// (manifest "CBaseIntelligentPointer_CopyRange"), the element's virtual destructor (slot 0, flag 0).
// Shape: as the landed CTCHeroCentre::RemoveQuestCard (0x0077D400): the inline IsThingAlive test (null, then the dead bit 0 of
// CThing +0x91) before the def-index compare (unsigned short +0x8C), the position `it - begin()` taken before the inline STLport
// erase(iterator), whose tag temporary lands in the dead `questDefIndex` slot.
// Layout: retail CTCInventoryQuests quest list at +0x178 (12-byte vector of 8-byte CIntelligentPointer<CThing>; local model).
struct random_access_iterator_tag { random_access_iterator_tag() {} };
class CThing {
public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                    // +0x8c
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  Dead : 1;                    // +0x91 bit 0
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
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
typedef CIntelligentPointer<CThing> CThingPointer;
CThingPointer* __fastcall __copy(CThingPointer* first, CThingPointer* last, CThingPointer* result,
                                 const random_access_iterator_tag& tag, int* distance);   // 0x004AA610
struct CThingPointerVector {                    // STLport vector<CIntelligentPointer<CThing> >
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

class CTCInventoryQuests {
public:
    unsigned char       _pad00[0x178];
    CThingPointerVector Quests;                 // +0x178
    long GetQuestPositionAndRemoveIt(long questDefIndex);
};

long CTCInventoryQuests::GetQuestPositionAndRemoveIt(long questDefIndex)
{
    for (CThingPointer* it = Quests.begin(); it != Quests.end(); ++it) {
        CThing* quest = it->GetPItem();
        if (IsThingAlive(quest) && quest->DefIndex == questDefIndex) {
            long position = it - Quests.begin();
            Quests.erase(it);
            return position;
        }
    }
    return -1;
}