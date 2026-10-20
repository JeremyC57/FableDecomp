// CTCTavernBar::GetRandomBarSlot(CThing& creature, C3DVector& position, CRightHandedSet& orientation, long& actionPoint, long& slot) -- pick a random jug slot and claim the matching free "ACTION_POINT_BARMAN_0<n>" named action point for the creature (position, orientation, action point index)
// Leaf: GetRandomBarSlot
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BA3C0, 290 bytes, `ret 14h`. Ego_r.pdb
//   `?GetRandomBarSlot@CTCTavernBar@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@AAJ3@Z`.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (interface 0x2E = CTCNamedUserActionPoints), CTCBase::GetPRandomSeed() const
//   @ 0x00686DA0, GFROR13 @ 0x00497890 (`__fastcall`), CCharString() @ 0x0099E4B0, CCharString(const char*, long) @ 0x0099EBF0
//   ("ACTION_POINT_BARMAN" @ 0x0126DCD4), CCharString::operator+=(const char*) @ 0x0099F100 ("_0" @ 0x0126DCBC),
//   GFIntToCharString(long) @ 0x0099F830 (`__fastcall`, hidden result in ecx), CCharString::operator+=(const CCharString&)
//   @ 0x0099F0A0, ~CCharString @ 0x0099EAE0, the by-value copy @ 0x0099EC30, CTCNamedUserActionPoints::
//   GetFreeNamedActionPoint(CThing*, C3DVector*, CRightHandedSet*, CCharString, long*, long, bool) @ 0x007FF190.
// Layout: Ego_r.pdb CTCTavernBar Jugs (vector<CIntelligentPointer<CThingObject> >, 8-byte elements) +0x0C; retail CThing
//   interface bitset +0x20 (0x2E = word +0x24 bit 14), map +0x44.
// Shape: the uninitialised out pointer of the out-parameter GetInterface (a miss reads whatever the dead slot holds); the
//   jug count taken before the seed; the landed WorldRandomRange step inline (`range ? seed % range : 0`); a default
//   CCharString declared first and never used; the point name built by += and passed by value (declared copy ctor).
class CTCBase;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { float m[6]; };
struct CCharString {
    void* m_data;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const char* text);          // 0x0099F100
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);   // 0x0099F830
unsigned long __fastcall GFROR13(unsigned long value);  // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
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
    bool GetFreeNamedActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* rh, CCharString name, long* index,
                                 long flags, bool reserve);   // 0x007FF190
};
struct CJugPointer { void* m_vfptr; void* m_pItem; };    // CIntelligentPointer<CThingObject>
struct CTCTavernBar {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    CJugPointer*  Jugs_start;                           // +0x0c vector<CIntelligentPointer<CThingObject> >
    CJugPointer*  Jugs_finish;                          // +0x10
    CJugPointer*  Jugs_end_of_storage;                  // +0x14
    unsigned long* GetPRandomSeed() const;              // 0x00686DA0 (CTCBase)
    bool GetRandomBarSlot(CThing& creature, C3DVector& position, CRightHandedSet& orientation, long& actionPoint, long& slot);
};

bool CTCTavernBar::GetRandomBarSlot(CThing& creature, C3DVector& position, CRightHandedSet& orientation, long& actionPoint, long& slot)
{
    CTCNamedUserActionPoints* points;
    ParentThing->GetInterface(&points, 0x2e);
    unsigned long count = Jugs_finish - Jugs_start;
    slot = WorldRandomRange(*GetPRandomSeed(), count);
    CCharString unused;
    CCharString pointName("ACTION_POINT_BARMAN", -1);
    pointName += "_0";
    pointName += GFIntToCharString(slot + 1);
    if (points->GetFreeNamedActionPoint(&creature, &position, &orientation, pointName, &actionPoint, 0, true))
        return true;
    return false;
}
