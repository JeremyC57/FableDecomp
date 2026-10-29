// CAIStateGroup_TavernKeeper::GetOldJugPoint() -- with the bar alive, ask its named-action-point component
// (interface 0x2E) for the free point "ACTION_POINT_BARMAN_0<JugSlot+1>" and store it as the jug point.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00968860, length 233 bytes (+ int3 padding). The catalog files it
// under CAIStateGroup_TavernServer; the string, the field offsets (PDB TavernKeeper: JugActionPointIndex +0x34,
// JugPos +0x38, JugRHSet +0x44, JugSlot +0x5c) and its address among the keeper bodies make it the keeper's twin
// of CAIStateGroup_TavernServer::GetOldJugPoint (0x009672C0).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PBar +0x1c, three times), CVectorMap::LowerBound
// @ 0x0040F020 (inlined out-parameter GetInterface, 0x2E = bit 14 of +0x24; the miss path reads the never-written
// slot), CCharString(const char*, long) @ 0x0099EBF0, operator+=(const char*) @ 0x0099F100, GFIntToCharString
// @ 0x0099F830 (__fastcall, hidden result), operator+=(const CCharString&) @ 0x0099F0A0, ~CCharString @ 0x0099EAE0,
// the by-value copy @ 0x0099EC30, CTCNamedUserActionPoints::GetFreeNamedActionPoint @ 0x007FF190 (creature, &JugPos,
// &JugRHSet, CCharString name, &JugActionPointIndex, 0, true). Literals 0x0126DCD4 "ACTION_POINT_BARMAN", 0x0126DCBC "_0".
// Shape: early `return false` for a dead bar; the component pointer preset to null (`xor ebx,ebx`, no miss-path load); the creature a named local read before the by-value string copy;
// the integer string temporary shares the lookup-key slot. Engine header is padding only, so a local model.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;


// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020.
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
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
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
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

struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CIntelligentPointer<CThing> PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PPub;               // +0x24 CIntelligentPointer<CThingBuilding>
    CIntelligentPointer<CThing> PPayee;             // +0x2c CIntelligentPointer<CThingAICreature>
    long JugActionPointIndex;                   // +0x34
    C3DVector JugPos;                           // +0x38
    CRightHandedSet JugRHSet;                   // +0x44
    long JugSlot;                               // +0x5c
    bool GetOldJugPoint();
};

bool CAIStateGroup_TavernKeeper::GetOldJugPoint()
{
    if (!IS_THING_ALIVE(PBar))
        return false;
    CTCNamedUserActionPoints* points = 0;
    PBar.GetPItem()->GetInterface(&points, 0x2E);
    CCharString name("ACTION_POINT_BARMAN", -1);
    name += "_0";
    name += GFIntToCharString(JugSlot + 1);
    CThing* creature = PBrain->PCreature;
    return points->GetFreeNamedActionPoint(creature, &JugPos, &JugRHSet, name, &JugActionPointIndex, 0, true);
}