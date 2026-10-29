// CCreatureAction_BeDazed::Initialise() -- base Initialise, then on the creature's mode manager (interface type 49): restart the current mode when it is already the dazed mode (type 67), else add that mode
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008ED850, length 111 bytes (+ int3 padding).
// Callees: CCreatureActionBase::Initialise @ 0x00694D10 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature
// at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 49 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 (manifest "CPixelShader::CountReferences" is propagated), the mode's virtual slot 1
// (its type), CTCCreatureModeManager::RestartCurrentMode @ 0x0073A840 (manifest "CNavQuadTreeNode::RemoveNeighbours" is propagated: it
// exits/re-enters the current mode through its virtuals), CTCCreatureModeManager::AddMode(unsigned long) @ 0x0073AE10.
// Shape: the GetInterface result is IGNORED, so the manager is read back from the frame slot on the not-present path
// (`mov esi,[esp+4]`) and taken from the register on the found path (`jmp` joins them); the type compare keeps `this`=manager in
// ecx for both tail calls. Retail CCreatureActionBase is 0xa8 bytes (engine/CCreatureAction_BeDazed.h is the PDB's 0xb4-byte
// layout), so `this` is modelled locally.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                      // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase;
struct CTCGraphicAppearance;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x14];
    CTCPhysicsBase* m_pPhysics;             // +0x60
    CTCGraphicAppearance* m_pGraphicAppearance;   // +0x64
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }                           // inline accessors: the member is
    CTCGraphicAppearance* GetGraphicAppearance() const { return m_pGraphicAppearance; }   //   loaded straight into ecx
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
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
struct CMode {
    virtual void v00();
    virtual long GetModeType();             // slot 1
};
class CTCCreatureModeManager { public:             // the interface-49 component
    CMode* GetCurrentMode();                // 0x0073A6E0
    void RestartCurrentMode();              // 0x0073A840
    bool AddMode(unsigned long mode);       // 0x0073AE10
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00694D10
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};

class CCreatureAction_BeDazed : public CCreatureActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_BeDazed::Initialise()
{
    CCreatureActionBase::Initialise();
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetInterface(&pModes, 49);   // result unchecked: the manager is assumed present
    if (pModes->GetCurrentMode()->GetModeType() == 67)
        pModes->RestartCurrentMode();
    else
        pModes->AddMode(67);
}