// CThing::UpdateThingScanInclusion() -- keep the thing's mapwho entry in step with whether it should be placed in the map
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C8C00, 143 bytes. __fastcall, `ret`.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020; CThing virtual slot 17 (+0x44, 0x004C7B80; the bool
//          query whose PDB name is most likely IsToBePlacedInMap() const -- not verified); CTCPhysicsBase::GetMapwhoPosition()
//          0x006AFCB0 (C3DVector by value); CTCMapwho::PlaceInMapwho(const C3DVector&) 0x0071A710; CTCMapwho::OnDie()
//          0x0071A730.
// Shape: interface 0x3A (CTCMapwho, bitset word 1 bit 26: `test dword ptr [edi+24h],4000000h`) fetched into a named local
//        (esi); PhysicsTC (+0x60) read once into ebp; not-yet-placed + physics + query true -> PlaceInMapwho(position);
//        else placed + physics + query false -> OnDie() (the `else if` re-tests the +0x50 bit 0 flag on the fall-through).
// Layout: retail CThing = Ego_r.pdb - 0x10 (bitset +0x20, interface map +0x44, PhysicsTC +0x60); CTCMapwho flag byte +0x50
//         bit 0 (descriptive name InMapwho).
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class C3DVector { public: float X, Y, Z; };
class CTCPhysicsBase { public:
    C3DVector GetMapwhoPosition() const;         // 0x006AFCB0
};
class CTCMapwho { public:
    unsigned char _pad00[0x50];
    unsigned char InMapwho : 1;                  // +0x50 bit 0
    void PlaceInMapwho(const C3DVector& position);   // 0x0071A710
    void OnDie();                                // 0x0071A730
};
class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual bool IsToBePlacedInMap() const;      // slot 17 (+0x44) 0x004C7B80
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
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
    void UpdateThingScanInclusion();
};
void CThing::UpdateThingScanInclusion()
{
    CTCMapwho* mapwho;
    if (GetInterface(&mapwho, 0x3a)) {
        CTCPhysicsBase* physics = PhysicsTC;
        if (!mapwho->InMapwho && physics && IsToBePlacedInMap())
            mapwho->PlaceInMapwho(physics->GetMapwhoPosition());
        else if (mapwho->InMapwho && physics && !IsToBePlacedInMap())
            mapwho->OnDie();
    }
}