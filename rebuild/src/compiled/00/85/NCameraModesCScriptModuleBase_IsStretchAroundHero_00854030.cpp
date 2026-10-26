// NCameraModes::CScriptModuleBase::IsStretchAroundHero() const -- stretch the camera around the hero when the live camera point says the hero is its subject, or when the module's origin thing is the main player
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- PeekTC ends in the F88 form (`if (found != last && !(type < found->type)) it = found; else it = last;` then one `*ppOut = it->component`), which VC7.1 tail-duplicates so the component loads straight into ecx for the slot-30 call as retail does
// Retail: Fable.exe (TLC, VC7.1) @ 0x00854030, 178 bytes, plain `ret`.
// Leaf: ?IsStretchAroundHero@CScriptModuleBase@NCameraModes@@UBE_NXZ
// Name: vtable 0x01255F74 slot 4; FableWin.pdb ?IsStretchAroundHero@CScriptModuleBase@NCameraModes@@UBE_NXZ (the remaining base virtual
//       once slots 2/3/5/6/7/8 are placed by their overriders, see GetModuleFOV 0x00851DD0).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the inline GetPCameraPoint() re-fetches the camera point for every
//          use), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound(const long*) const @ 0x004365B0 (the inlined const PeekTC for
//          the camera-point component, type 0x3f = bit 31 of the bitset word +0x24, tested with `jns`).
// Layout: Ego_r.pdb CScriptModuleBase (0x4c; landed CScriptModuleSpline::IsFinished 0x00853260): PCamPoint +0x04, PThingOrigin
//         +0x0c, CutOutOverride +0x44 (-1 = no override); CThing dead flag +0x91 bit 0; CTCCameraPointDefinitionBase CutOutOf +0x0d.
//          Also the component's virtual slot 30 (+0x78, a const bool query; IsHeroTheSubject is the FableWin.pdb virtual of
//          CTCCameraPointDefinitionBase that fits), CThingCreatureBase::GetPlayer @ 0x004C7A10, CPlayer::IsMainPlayer @ 0x00487520.
//          The origin thing must have type byte +0x10 == 2 before it is asked for its player.
// Shape: two positive `&&` chains, each through separate intelligent-pointer fetches (the camera point three times, the origin
//        four), sharing the `mov al,1` and `xor al,al` tails.
class CTCBase;
class CPlayer { public:
    bool IsMainPlayer() const;                       // 0x00487520
};
struct CInterfaceMap {                               // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    __declspec(noinline) SEntry* LowerBound(const long* type)   // same-TU read-only stand-in (the template body every retail TU instantiates)
    {
        SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
    __declspec(noinline) const SEntry* LowerBound(const long* type) const   // same-TU read-only stand-in (the template body every retail TU instantiates)
    {
        const SEntry* lo = first;
        int len = last - first;
        while (len > 0) {
            int half = len >> 1;
            const SEntry* middle = lo + half;
            if (middle->type < *type) {
                lo = middle + 1;
                len = len - half - 1;
            }
            else
                len = half;
        }
        return lo;
    }
};
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
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* found = map->LowerBound(&type);
            const CInterfaceMap::SEntry* it;
            if (found != map->last && !(interfaceType < found->type))
                it = found;
            else
                it = map->last;
            *ppOut = (const T*)it->component;
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
    CThing* GetPThingOrigin() const { return PThingOrigin.Get(); }
};
bool CScriptModuleBase::IsStretchAroundHero() const
{
    if (GetPCameraPoint() && !GetPCameraPoint()->IsDead()) {
        const CTCCameraPointDefinitionBase* point;
        if (GetPCameraPoint()->PeekTC(&point, 0x3f) && point->IsHeroTheSubject())
            return true;
    }
    if (GetPThingOrigin() && !GetPThingOrigin()->IsDead() && GetPThingOrigin()->m_type == 2
        && GetPThingOrigin()->GetPlayer()->IsMainPlayer())
        return true;
    return false;
}
}