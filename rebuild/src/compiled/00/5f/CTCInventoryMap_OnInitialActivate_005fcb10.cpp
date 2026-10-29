// CTCInventoryMap::OnInitialActivate() -- after the base hook, re-activates every teleporter whose UID was serialised with the map screen (live, non-dead things carrying the teleporter component) and drops the serialised UID list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FCB10, length 208 bytes (+ int3 padding); speed TU (no frame pointer). CTCInventoryMap vtable
// 0x01257A9C slot 6 (OnInitialActivate).
// Callees: CTCInventoryBase::OnInitialActivate() @ 0x005BD1EB, CTCBase::GetThingManager() @ 0x00686AE0, CThingManager::GetPThingWithUID(unsigned
// __int64) @ 0x0051ED80 (manifest "CStopWatch::GetTicks" is propagated), CVectorMap::LowerBound @ 0x0040F020 (the inline CThing::GetInterface
// for interface 0xDD = bit 29 of the bitset dword at +0x38), CTCTeleporter::SetAsActive(bool) @ 0x007DFEB0, the count block's destroy
// pointer-to-function (`mov ecx,[eax+8] ; call [eax+4]`) and operator delete @ 0x00BFE9BC for the CCountedPointer reset.
// Layout: retail SerialisedTeleporterUIDs (CCountedPointer<vector<unsigned __int64>>: Data +0x200, Info +0x204; PDB +0x254 with 16-byte
// containers before it); CThing dead flag +0x91 bit 0, interface bitset +0x20, interface map +0x44 (local models).
// Shape: the vector's end is re-read every iteration (the loop runs `it != Data->_M_finish`); the UID is a named __int64 local (hi word in ecx, low in eax, pushed in that order; dereferencing the iterator in the call swaps the registers); the
// GetInterface key is homed in the frame word; the counted reset is the landed CCountedPointer::Reset body (count touched through the
// re-read block, destroy through the block's function pointer, `delete` the block, then Info and Data zeroed last-to-first).
struct SCountBlock {
    long m_references;                                                       // +0
    void (__fastcall* m_pfnDestroy)(void*);                                  // +4
    void* m_pObject;                                                         // +8
};
struct CUIDVector {                                                          // std::vector<unsigned __int64> (12 bytes)
    unsigned __int64* _M_start;
    unsigned __int64* _M_finish;
    unsigned __int64* _M_end_of_storage;
};
struct CSerialisedUIDPointer {                                               // CCountedPointer<std::vector<unsigned __int64>>
    CUIDVector*  Data;                                                       // +0
    SCountBlock* Info;                                                       // +4
    void Reset()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);      // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char IsDead : 1;                   // +0x91 bit 0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)((void*)it->second);
            return true;
        }
        return false;
    }
};
class CThingManager { public:
    CThing* GetPThingWithUID(unsigned __int64 uid);            // 0x0051ED80
};
class CTCTeleporter { public:                               // interface type 0xDD
    void SetAsActive(bool active);                   // 0x007DFEB0
};
class CTCBase
{
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void OnInitialActivate();                // slot 6 @ 0x005BD1EB
protected:
    CThingManager& GetThingManager();   // 0x00686AE0
};

struct CTCInventoryBase_Map : public CTCBase {
    virtual void OnInitialActivate();

    unsigned char _pad04[0x200 - 0x04];
};
class CTCInventoryMap : public CTCInventoryBase_Map { public:
    CSerialisedUIDPointer SerialisedTeleporterUIDs;  // +0x200
    virtual void OnInitialActivate();
};
void CTCInventoryMap::OnInitialActivate()
{
    CTCInventoryBase_Map::OnInitialActivate();
    CUIDVector* uids = SerialisedTeleporterUIDs.Data;
    if (uids) {
        for (unsigned __int64* it = uids->_M_start; it != uids->_M_finish; ++it) {
            __int64 uid = *it;
            CThing* thing = GetThingManager().GetPThingWithUID(uid);
            if (thing && !thing->IsDead) {
                CTCTeleporter* teleporter;
                if (thing->GetInterface(&teleporter, 0xDD))
                    teleporter->SetAsActive(true);
            }
        }
        SerialisedTeleporterUIDs.Reset();
    }
}