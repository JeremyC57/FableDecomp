// Grade: DIFFER(953v967) -- flow and calls match; register allocation differs (retail points ebp / prefix length esi / seat edi with actionPoint& reloaded from the arg each use; ours CSEs it into ebp), switch case blocks laid out in a different order
// CTCTavernTable::GetServingPoint(CThing&, C3DVector&, CRightHandedSet&, long&, long&, CThingObject**, ServingMode, bool) -- walk the table's sit (or game-master) action points from the last one tried, take the seat number from each point's name and, if the seat suits the serving mode (an empty tankard on a seat whose used flag matches, or a seat with no / some food), find a free maid action point for that seat; returns the seat (0-based) on success
// Leaf: ?GetServingPoint@CTCTavernTable
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B8CC0, 965 bytes (+ int3 padding), `ret 0x20`.
// Callees: CTCNamedUserActionPoints::GetFullNamedActionPoint 0x007FEFE0 / GetFreeNamedActionPoint 0x007FF190 / GetName
//   0x007FEFC0 / the point reservation 0x007FEF40 (manifest label "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera"
//   is propagated); CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / (const CCharString&) 0x0099EC30 /
//   ~ 0x0099EAE0 / GetSubString 0x0099EC70 / operator+= 0x0099F0A0; operator+(const char*, const CCharString&) 0x0099F690;
//   GFCharStringToInt 0x0099E7F0; CBaseIntelligentPointer::GetPItem 0x00A01B50; LowerBound 0x0040F020 (interface 0xBA).
// Layout (retail CTCTavernTable, 12-byte vectors): Tankards +0x1c, Food +0x28, SeatsCurrentlyUsed +0x34 (byte vector),
//   ActionPointSitStrlen +0x48, ActionPointGameMasterStrlen +0x4c, AvailableForUse +0x5c.
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CThingObject;
struct CStringData { long _r0; long Length; };
struct CCharString {
    CStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    long Length() const
    {
        if (PStringData)
            return PStringData->Length;
        return 0;
    }
    CCharString GetSubString(unsigned long start, unsigned long count) const;   // 0x0099EC70
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
long __fastcall GFCharStringToInt(const CCharString& text);                     // 0x0099E7F0

class CTCBase;
struct CThing;
struct CTCNamedUserActionPoints {
    bool GetFullNamedActionPoint(CThing* user, CCharString prefix, long* index, long start);   // 0x007FEFE0
    bool GetFreeNamedActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* rhs, CCharString prefix, long* index,
                                 long start, bool reserve);                                    // 0x007FF190
    void Reserve(long index);                           // 0x007FEF40
    void GetName(long index, CCharString* name);        // 0x007FEFC0
};
struct CTCTankardState {                                // interface 0xBA (descriptive)
    unsigned char _pad00[0x0c];
    float Contents;                                     // +0x0c
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    __forceinline SEntry* InlineFind(long type)
    {
        SEntry* it = first;
        long count = last - first;
        while (count > 0) {
            long half = count / 2;
            if (it[half].type < type) {
                it += half + 1;
                count -= half + 1;
            } else
                count = half;
        }
        if (it == last || type < it->type)
            it = last;
        return it;
    }
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned long HasTC(long t) const { return m_presentBits[t >> 5] & (1UL << (t & 31)); }
    template<class T> __forceinline T* GetInterface(long wanted)
    {
        if (HasTC(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
        }
        return 0;
    }
};
struct CBaseIntelligentPointer {
    void* _vfptr;
    void* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CPointerVector {
    CBaseIntelligentPointer* _M_start;
    CBaseIntelligentPointer* _M_finish;
    CBaseIntelligentPointer* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CBaseIntelligentPointer& operator[](unsigned long i) const { return _M_start[i]; }
};
struct CBoolVector {
    bool* _M_start;
    bool* _M_finish;
    bool* _M_end_of_storage;
    bool Get(unsigned long i) const { return (unsigned long)(_M_finish - _M_start) > i ? _M_start[i] : false; }
};

struct CTCTavernTable {
    enum ServingMode { SERVE_DRINK, SERVE_FOOD, CLEAR_FOOD };
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x1c - 0x08];
    CPointerVector Tankards;                            // +0x1c
    CPointerVector Food;                                // +0x28
    CBoolVector SeatsCurrentlyUsed;                     // +0x34
    long NumPeopleSeated;                               // +0x40
    bool SomeoneTalking;                                // +0x44
    bool SomeoneLaughing;                               // +0x45
    long ActionPointSitStrlen;                          // +0x48
    long ActionPointGameMasterStrlen;                   // +0x4c
    long ActionPointHeroStrlen;                         // +0x50
    long ActionPointTankardStrlen;                      // +0x54
    long ActionPointMaidStrlen;                         // +0x58
    bool AvailableForUse;                               // +0x5c
    bool GetServingPoint(CThing& thing, C3DVector& pos, CRightHandedSet& rhs, long& actionPoint, long& seatIndex,
                         CThingObject** object, ServingMode mode, bool sitting);
};

bool CTCTavernTable::GetServingPoint(CThing& thing, C3DVector& pos, CRightHandedSet& rhs, long& actionPoint, long& seatIndex,
                                     CThingObject** object, ServingMode mode, bool sitting)
{
    if (!AvailableForUse)
        return false;
    long start = 0;
    CTCNamedUserActionPoints* points;
    for (;;) {
        if (ParentThing->HasTC(0x2e))
            points = (CTCNamedUserActionPoints*)ParentThing->m_interfaces.InlineFind(0x2e)->component;
        long prefixLength;
        if (sitting) {
            if (!points->GetFullNamedActionPoint(&thing, "ACTION_POINT_SIT_", &actionPoint, start)) {
                if (!points->GetFullNamedActionPoint(&thing, "ACTION_POINT_GAME_MASTER_", &actionPoint, start)) {
                    actionPoint = -1;
                    return false;
                }
                prefixLength = ActionPointGameMasterStrlen;
            } else
                prefixLength = ActionPointSitStrlen;
        } else {
            if (!points->GetFreeNamedActionPoint(&thing, &pos, &rhs, "ACTION_POINT_SIT_", &actionPoint, start, true)) {
                actionPoint = -1;
                return false;
            }
            prefixLength = ActionPointSitStrlen;
            points->Reserve(actionPoint);
        }
        CCharString name;
        points->GetName(actionPoint, &name);
        CCharString seatText = name.GetSubString(prefixLength, name.Length());
        long seat = GFCharStringToInt(seatText);
        bool suitable;
        switch (mode) {
        case SERVE_DRINK: {
            if (seat <= 0 || (unsigned long)seat > Tankards.size())
                return false;
            if (!Tankards[seat - 1].GetPItem())
                return false;
            CTCTankardState* tankard = Tankards[seat - 1].GetPItem()->GetInterface<CTCTankardState>(0xba);
            if (!tankard)
                return false;
            suitable = tankard->Contents == 0.0f && SeatsCurrentlyUsed.Get(seat - 1) == sitting;
            break;
        }
        case SERVE_FOOD:
            if (seat <= 0 || (unsigned long)seat > Food.size())
                return false;
            suitable = Food[seat - 1].GetPItem() == 0;
            break;
        case CLEAR_FOOD:
            if (seat <= 0 || (unsigned long)seat > Food.size())
                return false;
            suitable = Food[seat - 1].GetPItem() != 0;
            break;
        default:
            suitable = false;
            break;
        }
        if (suitable) {
            if (mode == SERVE_DRINK)
                *object = (CThingObject*)Tankards[seat - 1].GetPItem();
            CCharString maidPrefix("ACTION_POINT_MAID_");
            maidPrefix += seatText.Length() > 1 ? seatText : "0" + seatText;
            if (points->GetFreeNamedActionPoint(&thing, &pos, &rhs, maidPrefix, &actionPoint, 0, true)) {
                seatIndex = seat - 1;
                return true;
            }
        }
        start = actionPoint + 1;
    }
}
