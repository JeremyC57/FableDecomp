// NCameraModes::CScriptModuleSpline::OnInitLeaf() -- set the camera point's spline to the module's node range and start the spline index at the start node's fraction, then run one Update without letting it advance the index
// Retail: Fable.exe (TLC, VC7.1) @ 0x008540F0, 143 bytes, plain `ret`.
// Leaf: ?OnInitLeaf@CScriptModuleSpline
// Name: vtable 0x01275A6C slot 8 (CScriptModuleBase::OnInitLeaf); FableWin.pdb ?OnInitLeaf@CScriptModuleSpline@NCameraModes@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCamPoint, no null test), CVectorMap<ETCInterfaceType, CTCBase*>::
//          LowerBound(const long*) @ 0x0040F020 (the inlined GetTC<CTCCameraPointScriptedSpline> for type 0x3f, bit 31 of the
//          bitset word +0x24, `jns`), CTCCameraPointScriptedSpline::InitialiseSpline(long startNode, long endNode) @ 0x0075D050
//          (landed; its only caller is this function, 0x00854136), the module's own virtual Update (slot 1, +0x04).
// Layout: Ego_r.pdb CScriptModuleSpline (landed IsFinished 0x00853260): Index +0x4c, NodeToStartOn +0x5c, NodeToEndOn +0x60;
//         the spline's KeyCameras vector of 0x38-byte CKeyCamera at +0x68 (landed InitialiseSpline).
// Shape: Index = (float)start / (size() - 1) (`fild ; fidiv` of a frame int); the index is saved in a local across the
//        virtual Update and stored back.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                               // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetTC(T** ppOut, long interfaceType)
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
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
struct CKeyCamera { unsigned char _body[0x38]; };
class CTCCameraPointScriptedSpline { public:
    unsigned char _pad00[0x68];
    CKeyCamera*   KeyCameras_first;                  // +0x68 vector<CKeyCamera>
    CKeyCamera*   KeyCameras_last;                   // +0x6c
    CKeyCamera*   KeyCameras_end;                    // +0x70
    long NumKeyCameras() const { return KeyCameras_last - KeyCameras_first; }
    void InitialiseSpline(long startNode, long endNode);   // 0x0075D050
};
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
namespace NCameraModes {
class CBase;
class CScriptModuleBase { public:                           // vtable 0x01255F74, 0x4c bytes
    virtual ~CScriptModuleBase();                    // slot 0
    virtual void Update() = 0;                       // slot 1
    virtual bool IsFinished() const;                 // slot 2
    virtual bool IsShortenOnOcclusion() const;       // slot 3
    virtual bool IsStretchAroundHero() const;        // slot 4
    virtual bool GetModuleFOV(float& fov) const;     // slot 5
    virtual bool GetModuleRollAngle(float& angle) const;   // slot 6
    virtual bool ResetCameraOnEnd() const;           // slot 7
    virtual void OnInitLeaf();                       // slot 8
    CBaseIntelligentPointer PCamPoint;               // +0x04
    CBaseIntelligentPointer PThingOrigin;            // +0x0c
    C3DVector           CoordOrigin;                 // +0x14
    CRightHandedSet     CoordAxis;                   // +0x20
    CBase*              POwner;                      // +0x38
    void*               PCameraManager;              // +0x3c
    bool                SelfTerminate;               // +0x40
    bool                ShortenOnCollision;          // +0x41
    long                CutOutOverride;              // +0x44
    float               FOVOverride;                 // +0x48
    CThing* GetPCameraPoint() const { return (CThing*)PCamPoint.GetPItem(); }
};
struct CScriptModuleSpline : CScriptModuleBase {    // vtable 0x01275A6C
    float Index;                                     // +0x4c
    float CurrentSplineFOV;                          // +0x50
    float CurrentSplineRollAngle;                    // +0x54
    bool  ReallyFinished;                            // +0x58
    bool  FinishedSpline;                            // +0x59
    long  NodeToStartOn;                             // +0x5c
    long  NodeToEndOn;                               // +0x60
    virtual void OnInitLeaf();
};
void CScriptModuleSpline::OnInitLeaf()
{
    CTCCameraPointScriptedSpline* spline;
    if (GetPCameraPoint()->GetTC(&spline, 0x3f)) {
        spline->InitialiseSpline(NodeToStartOn, NodeToEndOn);
        Index = (float)NodeToStartOn / (spline->NumKeyCameras() - 1);
    }
    float index = Index;
    Update();
    Index = index;
}
}