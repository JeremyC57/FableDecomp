// CTCWeaponTrail::CalculateInterpolatedWeaponHelpers(long trail, float time, C3DVector* end, C3DVector* start) -- interpolate the weapon trail's start/end helper positions between the two stored samples at the given time
// Grade: DIFFER(421v421) -- size and calls equal; retail holds the parent in eax via lea, ours in ebp; the else-arm call uses a different register
// Leaf: CalculateInterpolatedWeaponHelpers
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
struct CCharStringData { CBasicStringChar Data; long NoRefs; };
#pragma pack(pop)
struct CCharString {
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
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Forward; C3DVector Up; };
struct CMeshCountBlock;
struct CBaseClass {                                         // vtable 0x01231710
    CBaseClass();                                           // 0x0099A2D0
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
struct C3DMeshInfo;
struct C3DMeshObject : CBaseClass {                         // vtable 0x01239860, 0x78 bytes
    const C3DMeshInfo* MeshInfo;                            // +0x04
    void*              MeshObject;                          // +0x08 CCountedPointer<const C3DMeshStats>
    CMeshCountBlock*   MeshCount;                           // +0x0c
    unsigned char      _pad10[0x40 - 0x10];
    bool               Valid;                               // +0x40
    unsigned char      _pad41[0x78 - 0x41];
    C3DMeshObject() : MeshObject(0), MeshCount(0) { Valid = false; }
    virtual ~C3DMeshObject();                               // 0x004C3200 (out-of-line copy)
    C3DVector GetHelperPointPositionStatic(const CCharString& name, unsigned long index);   // 0x00A361A0
};
struct CTCGraphicAppearance {
    bool GetRenderMeshObject(C3DMeshObject& out, const C3DVector& position, const CRightHandedSet& orientation) const;   // 0x004BC750
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector* position, float time);   // 0x004C0560
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long& type) const;             // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad50[0x64 - 0x50];
    CTCGraphicAppearance* PGraphicAppearance;               // +0x64
    template <class T> __forceinline bool GetInterface(T** out, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *out = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct CTCCarryable {                                       // TC 0x47
    unsigned char _pad00[0x10];
    CThing* PCarrier;                                       // +0x10
    bool GetCarriedPositionInfo(C3DVector* position, CRightHandedSet* orientation, bool forceUpdate, float time);   // 0x007055D0
};
struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
};
struct CTCWeaponTrail : CTCBase {
    unsigned char _pad08[0x54 - 0x08];
    CHelperNameVector HelperNames;                          // +0x54 retail 12-byte vector
    void CalculateInterpolatedWeaponHelpers(long trail, float time, C3DVector* end, C3DVector* start);
};

void CTCWeaponTrail::CalculateInterpolatedWeaponHelpers(long trail, float time, C3DVector* end, C3DVector* start)
{
    long secondIndex = 0;
    if (HelperNames[trail].first == HelperNames[trail].second)
        secondIndex = 1;
    CTCCarryable* carryable;
    if (ParentThing->GetInterface(&carryable, 0x47) && carryable->PCarrier) {
        {
            C3DVector position;
            CRightHandedSet orientation;
            carryable->GetCarriedPositionInfo(&position, &orientation, false, time);
            C3DMeshObject meshObject;
            ParentThing->PGraphicAppearance->GetRenderMeshObject(meshObject, position, orientation);
            *start = meshObject.GetHelperPointPositionStatic(HelperNames[trail].first, 0);
            *end = meshObject.GetHelperPointPositionStatic(HelperNames[trail].second, secondIndex);
        }
    } else {
        ParentThing->PGraphicAppearance->GetHelperPoint(HelperNames[trail].first, 0, 0, end, time);
        ParentThing->PGraphicAppearance->GetHelperPoint(HelperNames[trail].second, secondIndex, 0, start, time);
    }
}
