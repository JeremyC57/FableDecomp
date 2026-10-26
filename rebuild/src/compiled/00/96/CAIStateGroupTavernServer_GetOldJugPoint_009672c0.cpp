// CAIStateGroup_TavernServer::GetOldJugPoint() -- with the bar alive, ask its named-action-point component
// (interface 0x2E) for the free point "ACTION_POINT_MAID_0<JugIndex+1>" and store it as the bar point.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009672C0, length 233 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PBar +0x1c, three times), CVectorMap::LowerBound
// @ 0x0040F020 (inlined out-parameter GetInterface, 0x2E = bit 14 of +0x24; the miss path reads the never-written
// slot), CCharString(const char*, long) @ 0x0099EBF0, operator+=(const char*) @ 0x0099F100, GFIntToCharString
// @ 0x0099F830 (__fastcall, hidden result), operator+=(const CCharString&) @ 0x0099F0A0, ~CCharString @ 0x0099EAE0,
// the by-value copy @ 0x0099EC30, CTCNamedUserActionPoints::GetFreeNamedActionPoint @ 0x007FF190 (creature, &BarPos,
// &BarRHSet, CCharString name, &BarActionPointIndex, 0, true). Literals 0x0126DCC0 "ACTION_POINT_MAID", 0x0126DCBC "_0".
// Shape: early `return false` for a dead bar; the creature a named local read before the by-value string copy;
// the integer string temporary shares the lookup-key slot. Engine header is padding only, so a local model.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;


// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x70 - 0x4c];
    void* PDef;                                 // +0x70 CDefPointer<const CThingDef>
    unsigned char _pad2[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

class CRightHandedSet { public: float m[6]; };         // 0x18 bytes
class C3DVector { public: float X, Y, Z; };
class CCharString { public:                            // 4 bytes: the shared string data pointer
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator+=(const char* text);          // 0x0099F100
    CCharString& operator+=(const CCharString& other);  // 0x0099F0A0
};
CCharString __fastcall GFIntToCharString(long value);   // 0x0099F830 (hidden result in ecx)

class CTCNamedUserActionPoints { public:               // interface 0x2E
    bool GetFreeNamedActionPoint(CThing* creature, C3DVector* pos, CRightHandedSet* rhSet, CCharString name,
                                 long* actionPointIndex, long slot, bool reserve);   // 0x007FF190
};

struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PTable;             // +0x24 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PTankard;           // +0x2c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PPub;               // +0x34 CIntelligentPointer<CThingBuilding>
    long JugIndex;                              // +0x3c
    long ServingActionPointIndex;               // +0x40
    C3DVector ServingPos;                       // +0x44
    CRightHandedSet ServingRHSet;               // +0x50
    long BarActionPointIndex;                   // +0x68
    C3DVector BarPos;                           // +0x6c
    CRightHandedSet BarRHSet;                   // +0x78
    bool GetOldJugPoint();
};

bool CAIStateGroup_TavernServer::GetOldJugPoint()
{
    if (!IS_THING_ALIVE(PBar))
        return false;
    CTCNamedUserActionPoints* points;
    PBar.GetPItem()->GetInterface(&points, 0x2E);
    CCharString name("ACTION_POINT_MAID", -1);
    name += "_0";
    name += GFIntToCharString(JugIndex + 1);
    CThing* creature = PBrain->PCreature;
    return points->GetFreeNamedActionPoint(creature, &BarPos, &BarRHSet, name, &BarActionPointIndex, 0, true);
}