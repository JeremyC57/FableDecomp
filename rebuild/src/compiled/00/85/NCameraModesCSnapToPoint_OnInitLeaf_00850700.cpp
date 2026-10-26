// NCameraModes::CSnapToPoint::OnInitLeaf() -- take the fixed camera point's settings: track the observed thing, or else look along the point's fixed look vector
// Retail: Fable.exe (TLC, VC7.1) @ 0x00850700, 128 bytes, plain `ret`.
// Leaf: ?OnInitLeaf@CSnapToPoint
// Name: vtable 0x0124DE5C slot 24; FableWin.pdb ?OnInitLeaf@CSnapToPoint@NCameraModes@@UAEXXZ (slot 25 0x0056E390 is the empty
//       OnDieLeaf; slots 28/29 read back TrackThing +0xc0 and LookDirection +0xc1, so this is the leaf that fills them).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCameraPoint +0xa4), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//          (const long*) const @ 0x004365B0 (the inlined const PeekTC for type 0x3f, bit 31 of the bitset word +0x24, `jns`),
//          the component's virtual slot 28 (+0x70, GetCameraPointType; 0 = fixed point, landed GetPointType<CTCCameraPointFixedPoint>
//          0x00850780).
// Layout: Ego_r.pdb CTCCameraPointFixedPoint TrackThing +0x40, LookVector +0x41 and CSnapToPoint TrackThing +0xe4, LookDirection
//         +0xe5 (retail +0xc0/+0xc1: the root is 0x24 narrower) -- both byte-packed, hence `#pragma pack(1)` for these classes.
// Shape: the inline const PeekTC and GetPointType<T> out-parameter queries with their results ignored (as the landed
//        CFixedDolly::OnInitLeaf 0x0087B6F0): both out pointers are uninitialised and share the lookup key's frame word, so the
//        absent and wrong-type paths read it (`mov esi,[esp+8]` / `mov ecx,[esp+8]`); TrackThing is stored before its test.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
};
struct CInterfaceMap {                               // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool PeekTC(const T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
#pragma pack(push, 1)
class CTCCameraPointDefinitionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetCameraPointType() const;         // slot 28 (+0x70)
    unsigned char _body04[0x40 - 0x04];
    template <class T> __forceinline bool GetPointType(const T** out) const
    {
        if (GetCameraPointType() == T::POINT_TYPE_ID) {
            *out = static_cast<const T*>(this);
            return true;
        }
        return false;
    }
};
class CTCCameraPointFixedPoint : public CTCCameraPointDefinitionBase { public:
    enum { POINT_TYPE_ID = 0 };
    bool      TrackThing;                            // +0x40
    C3DVector LookVector;                            // +0x41
};
namespace NCameraModes {
class CBase { public:
    virtual void v00();
    unsigned char _body[0xa0];                       // +0x04..+0xa3 (retail)
};
struct CCameraPointModeBase : CBase {
    CBaseIntelligentPointer PCameraPoint;            // +0xa4 CIntelligentPointer<const CThing>
    C3DVector Pos;                                   // +0xac
    float     FOV;                                   // +0xb8
    bool      CutInto;                               // +0xbc
    bool      CutOutOf;                              // +0xbd
    unsigned char _padbe[2];
    const CThing* GetPCameraPoint() const { return (const CThing*)PCameraPoint.GetPItem(); }
};
struct CSnapToPoint : CCameraPointModeBase {
    bool      TrackThing;                            // +0xc0
    C3DVector LookDirection;                         // +0xc1
    virtual void OnInitLeaf();
};
#pragma pack(pop)
void CSnapToPoint::OnInitLeaf()
{
    const CTCCameraPointDefinitionBase* point;
    GetPCameraPoint()->PeekTC(&point, 0x3f);
    const CTCCameraPointFixedPoint* fixedPoint;
    point->GetPointType(&fixedPoint);
    TrackThing = fixedPoint->TrackThing;
    if (!TrackThing)
        LookDirection = fixedPoint->LookVector;
}
}