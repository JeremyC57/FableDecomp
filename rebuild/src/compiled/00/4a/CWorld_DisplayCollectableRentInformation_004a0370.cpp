// CWorld::DisplayCollectableRentInformation() -- total the collectable and maximum rent over every buyable house that
// is currently rented; when there is rent to collect and the hero is alive, format TEXT_GUI_COLLECTABLE_RENT with
// the two totals into the hero's region display (the extra area-name line)
// Leaf: DisplayCollectableRentInformation
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A0370, 400 bytes, `ret`.
// Callees: CThingSearchTools::GetAllLeafThings(filter, vector&) 0x004AD780 (the buyable-house instantiation; the
//   manifest names the CThingAICreature copy), the const CVectorMap::LowerBound 0x004365B0 (inlined GetInterface on a
//   const thing) and the non-const 0x0040F020 (hero), CTCBuyableHouse::GetCurrentCollectableRentValue 0x006C0160 /
//   GetMaxCollectableRentValue 0x006C0270, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature
//   0x00487DC0, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CDataBank::GetTextBySymbol 0x009C95E0 on
//   the main game component's text bank (+0x14), the __fastcall ConstructString(CWideString&, long, long) 0x009C7B80,
//   CWideString::operator= 0x0099B7D0 / ~CWideString 0x0099B510, free 0x00BFEA14 (vector storage).
// Layout: rebuild/include/engine/CWorld.h (PlayerManager +0x0c, PThingSearchTools +0x20); CTCBuyableHouse Rented
//   +0x12 (engine/CTCBuyableHouse.h); CTCRegionDisplay CurrentAreaNameExtra +0x10 (Ego_r.pdb, interface 0x8E as the
//   landed 0x007795A0); CThing interface bitset +0x20 / map +0x44 / Dead bit +0x91 (landed GetInterface model).
// Shape: GetInterface writes through an out pointer and the house pointer is used without a null test (its home
//   slot is shared with the filter and the lookup key, hence the load before the loop).
#define CWorld CWorld_PDB
#include "engine/CWorld.h"
#undef CWorld
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    const CWideString& operator=(const CWideString& other);  // 0x0099B7D0
    ~CWideString();                                    // 0x0099B510
};
bool __fastcall ConstructString(CWideString& text, long value0, long value1);   // 0x009C7B80
namespace NGameText {
class CDataBank { public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
using NGameText::CDataBank;
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                            // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                 // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);              // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;  // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                             // +0x91 bit 0 Dead
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
inline bool IsThingAlive(const CThing* thing) { return thing && !(thing->Flags91 & 1); }
class CTCBuyableHouse { public:
    unsigned char _pad00[0x12];
    bool          Rented;                              // +0x12
    long GetCurrentCollectableRentValue();             // 0x006C0160
    long GetMaxCollectableRentValue();                 // 0x006C0270
};
class CTCRegionDisplay { public:
    unsigned char _pad00[0x10];
    CWideString   CurrentAreaNameExtra;                // +0x10
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                         // 0x00487DC0
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
template <class T> struct vector {                     // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
class CThingFilter_HasTC { public:
    long Type;                                         // ETCInterfaceType
    CThingFilter_HasTC(long type) : Type(type) { }
};
class CThingSearchTools { public:
    void GetAllLeafThings(const CThingFilter_HasTC& filter, vector<const CThing*>& things);   // 0x004AD780
};
class CWorld : public CWorld_PDB { public:
    void DisplayCollectableRentInformation();
};
const CThing** CollectLeafThings(long type, const CThing** first, const CThing** last);
// Same-TU stand-in for GetAllLeafThings<buyable house> 0x004AD780 (retail's LTCG knowledge): the house vector's
// address does not escape the search.
#pragma auto_inline(off)
void CThingSearchTools::GetAllLeafThings(const CThingFilter_HasTC& filter, vector<const CThing*>& things)
{
    things._M_finish = CollectLeafThings(filter.Type, things._M_start, things._M_end_of_storage);
}
#pragma auto_inline(on)
void CWorld::DisplayCollectableRentInformation()
{
    vector<const CThing*> houses;
    PThingSearchTools_ptr->GetAllLeafThings(CThingFilter_HasTC(0xd5), houses);
    unsigned int count = houses.size();
    long collectableRent = 0;
    long maxRent = 0;
    for (unsigned int i = 0; i < count; ++i) {
        const CThing* thing = houses[i];
        CTCBuyableHouse* house;
        thing->GetInterface(&house, 0xd5);
        if (house->Rented) {
            collectableRent += house->GetCurrentCollectableRentValue();
            maxRent += house->GetMaxCollectableRentValue();
        }
    }
    if (collectableRent > 0) {
        CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
        if (IsThingAlive(hero)) {
            CTCRegionDisplay* display;
            if (hero->GetInterface(&display, 0x8e) && display) {
                CWideString text = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_COLLECTABLE_RENT");
                ConstructString(text, collectableRent, maxRent);
                display->CurrentAreaNameExtra = text;
            }
        }
    }
}