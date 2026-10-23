// Grade: DIFFER(598v608) -- seatName CCharString homed in dead arg6 slot (retail own frame slot, sub esp,8; this in ebp)
// CTCTavernTable::GetSeat(CThing*, const C3DVector*, const CRightHandedSet*, long*, long*, const CCharString*) -- find a free "ACTION_POINT_SIT_" (or the given) named action point at the table for the user and turn its name's trailing number (after the SIT / GAME / HERO prefix) into a zero-based seat index
// Leaf: ?GetSeat@CTCTavernTable
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B8840, 606 bytes (+ int3 padding), `ret 0x18`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / operator=(const CCharString&) 0x0099EFB0 / CCharString(const
//   CCharString&) 0x0099EC30 / CCharString() 0x0099E4B0 / GetSubString(unsigned long, unsigned long) const 0x0099EC70 /
//   ~CCharString 0x0099EAE0; GFCharStringToInt 0x0099E7F0 (`__fastcall`); strstr 0x00BFEB3A; CVectorMap LowerBound
//   0x0040F020 (interface 0x2E); CTCNamedUserActionPoints::GetFreeNamedActionPoint 0x007FF190 / GetName 0x007FEFC0.
// Layout (retail CTCTavernTable = Ego_r.pdb less 0x14 after the four 16->12 vectors and the 20->16 vector<bool>):
//   ActionPointSitStrlen +0x48, ActionPointGameMasterStrlen +0x4c, ActionPointHeroStrlen +0x50, AvailableForUse +0x5c.
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CTCBase;

extern const char g_EmptyString[];                  // 0x0129AAF4
struct CCharStringData {
    char* Text;                                     // +0x00
    long Length;                                    // +0x04
};
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    CCharString GetSubString(unsigned long start, unsigned long length) const;   // 0x0099EC70
    const char* GetString() const { return PStringData ? PStringData->Text : g_EmptyString; }
    long GetLength() const { return PStringData ? PStringData->Length : 0; }
};
long __fastcall GFCharStringToInt(const CCharString& text);   // 0x0099E7F0
extern "C" char* strstr(const char* text, const char* find);  // 0x00BFEB3A

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCNamedUserActionPoints {
    bool GetFreeNamedActionPoint(CThing* user, const C3DVector* pos, const CRightHandedSet* rhs, CCharString name,
                                 long* point, long flags, bool free);   // 0x007FF190
    void GetName(long point, CCharString* name);    // 0x007FEFC0
};

struct CTCTavernTable {
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x48 - 0x08];
    long ActionPointSitStrlen;                      // +0x48
    long ActionPointGameMasterStrlen;               // +0x4c
    long ActionPointHeroStrlen;                     // +0x50
    long ActionPointTankardStrlen;                  // +0x54
    long ActionPointMaidStrlen;                     // +0x58
    bool AvailableForUse;                           // +0x5c
    bool GetSeat(CThing* user, const C3DVector* pos, const CRightHandedSet* rhs, long* point, long* seat,
                 const CCharString* pointName);
};

bool CTCTavernTable::GetSeat(CThing* user, const C3DVector* pos, const CRightHandedSet* rhs, long* point, long* seat,
                             const CCharString* pointName)
{
    if (!AvailableForUse)
        return false;
    CCharString name("ACTION_POINT_SIT_");
    if (pointName)
        name = *pointName;
    CTCNamedUserActionPoints* points;
    ParentThing->GetInterface(&points, 0x2e);
    if (points->GetFreeNamedActionPoint(user, pos, rhs, name, point, 0, true)) {
        CCharString seatName;
        points->GetName(*point, &seatName);
        if (strstr(seatName.GetString(), CCharString("SIT").GetString()))
            *seat = GFCharStringToInt(seatName.GetSubString(ActionPointSitStrlen, seatName.GetLength()));
        else if (strstr(seatName.GetString(), CCharString("GAME").GetString()))
            *seat = GFCharStringToInt(seatName.GetSubString(ActionPointGameMasterStrlen, seatName.GetLength()));
        else if (strstr(seatName.GetString(), CCharString("HERO").GetString()))
            *seat = GFCharStringToInt(seatName.GetSubString(ActionPointHeroStrlen, seatName.GetLength()));
        if (*seat > 0) {
            --*seat;
            return true;
        }
    }
    return false;
}
