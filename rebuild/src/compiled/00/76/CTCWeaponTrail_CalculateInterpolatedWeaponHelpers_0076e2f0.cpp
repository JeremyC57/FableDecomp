// CTCWeaponTrail::CalculateInterpolatedWeaponHelpers(long trail, float time, C3DVector& end, C3DVector& start) const -- interpolate the weapon trail's start/end helper positions between the two stored samples at the given time
// Grade: RELOCATION_MATCH (retry wave 2 sec2): the retail const CVectorMap::LowerBound body (0x004365B0) in the TU as a same-TU noinline callee -- VC7.1 then keeps the parent in eax as retail (was DIFFER(421v421), parent in ebp). Respelled to the PDB: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound, the private const leaf taking C3DVector& (Ego_r ABEXJMAAVC3DVector@@0@Z), NGraphicAppearance::CTCGraphicAppearance, CTCCarryable::GetCarriedPositionInfo / C3DMeshObject::GetHelperPointPositionStatic const reference forms, engine/CBaseClass.h.
// Leaf: ?CalculateInterpolatedWeaponHelpers@CTCWeaponTrail@@ABEXJMAAVC3DVector@@0@Z
long __fastcall Compare(const char* a, const char* b);      // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    __forceinline bool operator==(const CBasicStringChar& other) const
    {
        if (StringLength != other.StringLength)
            return false;
        return Compare(PData, other.PData) == 0;
    }
};
class CCharStringData { public: CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
};
__forceinline bool operator==(const CCharString& a, const CCharString& b)
{
    if (a.PStringData == b.PStringData)
        return true;
    if (a.PStringData && b.PStringData)
        return a.PStringData->Data == b.PStringData->Data;
    return false;
}
struct CHelperNamePair { CCharString first; CCharString second; };
struct CHelperNameVector {
    CHelperNamePair* _M_start;
    CHelperNamePair* _M_finish;
    CHelperNamePair* _M_end_of_storage;
    CHelperNamePair* begin() const { return _M_start; }
    const CHelperNamePair& operator[](long n) const { return *(begin() + n); }
};
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Forward; C3DVector Up; };
struct CMeshCountBlock;
#include "engine/CBaseClass.h"
class C3DMeshInfo;
class C3DMeshObject : public CBaseClass { public:                         // vtable 0x01239860, 0x78 bytes
    const C3DMeshInfo* MeshInfo;                            // +0x04
    void*              MeshObject;                          // +0x08 CCountedPointer<const C3DMeshStats>
    CMeshCountBlock*   MeshCount;                           // +0x0c
    unsigned char      _pad10[0x40 - 0x10];
    bool               Valid;                               // +0x40
    unsigned char      _pad41[0x78 - 0x41];
    C3DMeshObject() : MeshObject(0), MeshCount(0) { Valid = false; }
    virtual ~C3DMeshObject();                               // 0x004C3200 (out-of-line copy)
    C3DVector GetHelperPointPositionStatic(const CCharString& name, long index) const;   // 0x00A361A0 (Ego_r QBE)
};
namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    bool GetRenderMeshObject(C3DMeshObject& out, const C3DVector& position, const CRightHandedSet& orientation) const;   // 0x004BC750
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& position, float time) const;   // 0x004C0560
};
}
using NGraphicAppearance::CTCGraphicAppearance;
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap {                                  // retail STLport vector<pair<K, V> > + comparator (only the used members)
public:
    typedef std::pair<K, V> value_type;
    value_type* _M_start;                           // +0x00
    value_type* _M_finish;                          // +0x04
    value_type* _M_end_of_storage;                  // +0x08
    const value_type* LowerBound(const K& key) const;     // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*> CInterfaceMap;
template <class K, class V, class C>
__declspec(noinline) const std::pair<K, V>* CVectorMap<K, V, C>::LowerBound(const K& key) const   // the retail body, out of line
{
    long count = _M_finish - _M_start;
    const value_type* first = _M_start;
    while (count > 0) {
        long half = count / 2;
        const value_type* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            count = count - half - 1;
        } else
            count = half;
    }
    return first;
}
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad50[0x64 - 0x50];
    CTCGraphicAppearance* PGraphicAppearance;               // +0x64
    template <class T> __forceinline bool GetInterface(T** out, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = map->LowerBound(type);
            *out = (T*)(it == map->_M_finish || interfaceType < it->first ? map->_M_finish : it)->second;
            return true;
        }
        return false;
    }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        return (it == map->_M_finish || interfaceType < it->first ? map->_M_finish : it)->second;
    }
};
class CTCCarryable { public:                                       // TC 0x47
    unsigned char _pad00[0x10];
    CThing* PCarrier;                                       // +0x10
    bool GetCarriedPositionInfo(C3DVector& position, CRightHandedSet& orientation, bool forceUpdate, float time) const;   // 0x007055D0
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
};
class CTCWeaponTrail : public CTCBase { public:
    unsigned char _pad08[0x54 - 0x08];
    CHelperNameVector HelperNames;                          // +0x54 retail 12-byte vector
private:
    void CalculateInterpolatedWeaponHelpers(long trail, float time, C3DVector& end, C3DVector& start) const;
};

void CTCWeaponTrail::CalculateInterpolatedWeaponHelpers(long trail, float time, C3DVector& end, C3DVector& start) const
{
    long secondIndex = 0;
    if (HelperNames[trail].first == HelperNames[trail].second)
        secondIndex = 1;
    CTCCarryable* carryable;
    if (ParentThing->GetInterface(&carryable, 0x47) && carryable->PCarrier) {
        {
            C3DVector position;
            CRightHandedSet orientation;
            carryable->GetCarriedPositionInfo(position, orientation, false, time);
            C3DMeshObject meshObject;
            ParentThing->PGraphicAppearance->GetRenderMeshObject(meshObject, position, orientation);
            start = meshObject.GetHelperPointPositionStatic(HelperNames[trail].first, 0);
            end = meshObject.GetHelperPointPositionStatic(HelperNames[trail].second, secondIndex);
        }
    } else {
        ParentThing->PGraphicAppearance->GetHelperPoint(HelperNames[trail].first, 0, 0, end, time);
        ParentThing->PGraphicAppearance->GetHelperPoint(HelperNames[trail].second, secondIndex, 0, start, time);
    }
}