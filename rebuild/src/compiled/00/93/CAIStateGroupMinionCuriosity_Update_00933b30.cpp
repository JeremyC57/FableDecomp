// CAIStateGroup_MinionCuriosity::Update() -- walk to within the creature's extended sight radius of the
// curiosity position, then terminate.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00933B30, length 95 bytes.
// Callees: CVectorMap::LowerBound @ 0x004365B0 (the inlined interface lookup, type 0x34 = bit 20 of the bitset
// dword at +0x24; this module links the second copy of LowerBound), CTCPerceiveThing::GetExtendedSightRadius
// @ 0x007448F0 (float in st0, stored straight into the pushed argument slot), MoveToCuriosityProximity(float)
// @ 0x00933A40 (own member, bool result dropped), CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: uninitialised component local, untested lookup, both paths merge (`jmp`) into the call. Base layout
// modelled locally (PDB names); no engine header for CAIStateGroup_MinionCuriosity.
class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound -- the same shape the landed
// GetInterface_0x.. bodies use, here inlined into the caller through GetTC<T>().
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;      // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18 CThingAICreature*
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
public:
    void Terminate();   // 0x008FCF40
};

class CTCPerceiveThing { public:                       // interface type 0x34 component
    float GetExtendedSightRadius();             // 0x007448F0
};

struct CAIStateGroup_MinionCuriosity : CAIStateGroupBase {
    float CuriosityPos_X, CuriosityPos_Y, CuriosityPos_Z;   // +0x14 C3DVector
    bool MoveToCuriosityProximity(float radius);            // 0x00933A40
    virtual void Update();
};

void CAIStateGroup_MinionCuriosity::Update()
{
    CTCPerceiveThing* pPerceive;
    PBrain->Creature->GetInterface(&pPerceive, 0x34);
    MoveToCuriosityProximity(pPerceive->GetExtendedSightRadius());
    Terminate();
}