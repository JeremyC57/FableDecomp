// NCameraModes::CScriptModuleBase::ResetCameraOnEnd() const -- cut back out of the module at its end: the script's cut-out override when set, else the live camera point's CutOutOf flag
// Retail: Fable.exe (TLC, VC7.1) @ 0x00853FB0, 123 bytes, plain `ret`.
// Leaf: ?ResetCameraOnEnd@CScriptModuleBase
// Name: vtable 0x01255F74 slot 7; FableWin.pdb ?ResetCameraOnEnd@CScriptModuleBase@NCameraModes@@UBE_NXZ (CScriptModuleCameraOnAStick
//       and CScriptModuleFOVZoom override slot 7, both PDB ResetCameraOnEnd overriders; SetCutOutOverride sets the override).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the inline GetPCameraPoint() re-fetches the camera point for every
//          use), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound(const long*) const @ 0x004365B0 (the inlined const PeekTC for
//          the camera-point component, type 0x3f = bit 31 of the bitset word +0x24, tested with `jns`).
// Layout: Ego_r.pdb CScriptModuleBase (0x4c; landed CScriptModuleSpline::IsFinished 0x00853260): PCamPoint +0x04, PThingOrigin
//         +0x0c, CutOutOverride +0x44 (-1 = no override); CThing dead flag +0x91 bit 0; CTCCameraPointDefinitionBase CutOutOf +0x0d.
// Shape: `return CutOutOverride == 1;` (sete into a cleared register); the camera point is null-tested and dead-tested through
//        separate GetPCameraPoint() calls, then the component flag; one shared `return false` tail.
class CTCBase;
class CPlayer { public:
    bool IsMainPlayer() const;                       // 0x00487520
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                               // a thing's sorted {type, component} vector map at +0x44
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);            // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char m_type;                            // +0x10 thing type (2 here: a creature that can be the player)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_dead : 1;                        // +0x91 bit 0
    unsigned char m_91_1 : 7;
    bool IsDead() const { return m_dead; }
    template<class T> __forceinline bool PeekTC(const T** ppOut, long interfaceType) const   // FableWin.pdb ??$PeekTC@...@CThing@@QBE_NPAPB...@Z
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (const T*)it->second;
            return true;
        }
        return false;
    }
    CPlayer* GetPlayer();                            // 0x004C7A10 (CThingCreatureBase::GetPlayer)
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    CThing* Get() const { return (CThing*)GetPItem(); }
};
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CTCCameraPointDefinitionBase { public:                // retail = Ego_r.pdb offsets for these bytes
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual long GetCameraPointType() const;         // slot 28 (+0x70)
    virtual void v29();
    virtual bool IsHeroTheSubject() const;           // slot 30 (+0x78) (FableWin.pdb virtual; slot inferred)
    CThing*       m_pThing;                          // +0x04
    unsigned char _pad08[0x0c - 0x08];
    bool          CutInto;                           // +0x0c
    bool          CutOutOf;                          // +0x0d
};
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
    CIntelligentPointer_CThing PCamPoint;            // +0x04
    CIntelligentPointer_CThing PThingOrigin;         // +0x0c
    C3DVector           CoordOrigin;                 // +0x14
    CRightHandedSet     CoordAxis;                   // +0x20
    CBase*              POwner;                      // +0x38
    void*               PCameraManager;              // +0x3c
    bool                SelfTerminate;               // +0x40
    bool                ShortenOnCollision;          // +0x41
    long                CutOutOverride;              // +0x44
    float               FOVOverride;                 // +0x48
    CThing* GetPCameraPoint() const { return PCamPoint.Get(); }
};
bool CScriptModuleBase::ResetCameraOnEnd() const
{
    if (CutOutOverride != -1)
        return CutOutOverride == 1;
    if (GetPCameraPoint() && !GetPCameraPoint()->IsDead()) {
        const CTCCameraPointDefinitionBase* point;
        if (GetPCameraPoint()->PeekTC(&point, 0x3f) && point->CutOutOf)
            return true;
    }
    return false;
}
}