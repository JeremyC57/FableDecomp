// NCameraModes::CScriptModuleBase::IsStretchAroundHero() const -- stretch the camera around the hero when the live camera point says the hero is its subject, or when the module's origin thing is the main player
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- PeekTC ends in the F88 form (`if (found != last && !(type < found->type)) it = found; else it = last;` then one `*ppOut = it->component`), which VC7.1 tail-duplicates so the component loads straight into ecx for the slot-30 call as retail does
// Retail: Fable.exe (TLC, VC7.1) @ 0x00854030, 178 bytes, plain `ret`.
// Leaf: ?IsStretchAroundHero@CScriptModuleBase@NCameraModes@@UBE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 004365b0: ?LowerBound@CInterfaceMap@@QBEPBUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QBEPBU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
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
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    CThing* Get() const { return (CThing*)GetPItem(); }
};
#include "engine/C3DVector.h"
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
        const const CTCCameraPointDefinitionBase* point;
        if (GetPCameraPoint()->PeekTC(TCI_CAMERA_POINT, &point) && point->IsHeroTheSubject())
            return true;
    }
    if (GetPThingOrigin() && !GetPThingOrigin()->IsDead() && GetPThingOrigin()->GetType() == 2
        && GetPThingOrigin()->PeekPlayer().IsMainPlayer())
        return true;
    return false;
}
}