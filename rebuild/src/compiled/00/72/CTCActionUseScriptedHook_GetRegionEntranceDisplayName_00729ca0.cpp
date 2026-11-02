// CTCActionUseScriptedHook::GetRegionEntranceDisplayName() const -- the display name of the region holding the map that the connected entrance's position lies in
// Leaf: ?GetRegionEntranceDisplayName@CTCActionUseScriptedHook@@QBEABVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00729CA0, 75 bytes, thiscall, tail `jmp CRegion::GetDisplayName` 0x006BBF20.
// Identity (proven, two sources): Ego_r.pdb / FableWin.pdb ?GetRegionEntranceDisplayName@CTCActionUseScriptedHook@@QBEABVCCharString@@XZ
//   (Ego_r 0x007403C0 right after GetRegionEntranceName 0x00740370, which follows SetEntranceAsLocked 0x00740350); retail has the
//   same run: the landed SetEntranceAsLocked 0x00729C30, GetRegionEntranceName 0x00729C50, then this row. Its body is
//   GetRegionEntranceName's with the last step CRegion::GetDisplayName (Ego_r/FableWin ?GetDisplayName@CRegion@@QBEABVCCharString@@XZ,
//   bound at 0x006BBF20 by the landed 0x0057A989) instead of the Name member. Sole caller 0x0064DF94 (player-GUI drawing).
// Callees: ?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ 0x00686D20 (x3), ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
//   0x00A01B50, ?GetPos@CThing@@QBEABVC3DVector@@XZ 0x004C73D0, CWorldMap vtable slot 16 (+0x40) GetMapNumberAt(const C3DVector&)
//   (the C3DVector overload 0x004FC0C0 in vtable 0x01244AEC; slot 17 is the C2DCoordI one, reverse declaration order, as the landed
//   CWorldMap rows model it), ?GetRegionNumberMapIsIn@CWorldMap@@QBEJJ@Z 0x004FC190, ?GetRegion@CWorldMap@@QBEABVCRegion@@J@Z
//   0x004FC180, ?GetDisplayName@CRegion@@QBEABVCCharString@@XZ 0x006BBF20 (tail).
// Layout: CTCActionUseScriptedHook as in 0x00729A50 (Ego_r.pdb members, 0x50; PEntranceConnectedTo +0x20). engine/CTCBase.h retail branch (+ hdr/ overlay: PeekWorldMap). engine/CRegion.h (retail 0x58 layout) through the
//   layout-rename include, adding the PDB member function. CWorldMap: engine/CWorldMap.h flattens the vptr, so a local model with
//   the RTTI bases (CBaseClassNonCopyable +0, CIDrawWorldMap +4), the primary-vtable slots 0..19 and opaque members to the retail
//   sizeof 0xD8 (`push 0D8h` before the ctor call at 0x004A6E8C).
// Shape: as the landed GetRegionEntranceName 0x00729C50: the first world-map fetch (and its vtable, kept in ebx) happens BEFORE the
//   argument is computed, so the map is a named local; the outer two calls evaluate their argument first and fetch the map again.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                 // CTCBase : CBaseClassNonCopyable (vtable 0x0125BE0C)
#include "fable_string.h"                                   // CCharString
#define CRegion CRegionLayout
#include "engine/CRegion.h"                                 // retail CRegion layout (0x58)
#undef CRegion

class C3DVector { public: float X, Y, Z; };
class C2DCoordI;
class C2DBoxI;
class CAFile;
class CMap;
class CASuspendableProcess;
template <class T> class CCountedPointer;

class CThing { public:                                      // reached only through pointers here; retail sizeof 0x98
    const C3DVector& GetPos() const;                        // 0x004C73D0 ?GetPos@CThing@@QBEABVC3DVector@@XZ
    unsigned long Body_[0x98 / 4];                          // CThing members (not modelled)
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                             // vtable 0x0129C95C (vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50 ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    T* operator->() const { return GetPItem(); }
};

class CRegion : public CRegionLayout { public:
    const CCharString& GetDisplayName() const;              // 0x006BBF20 ?GetDisplayName@CRegion@@QBEABVCCharString@@XZ
};

class CIDrawWorldMap { public:                              // RTTI base at +4 (vtable 0x01244AB4); its slots are not used here
    virtual long DrawGetFrame() const;
};
class CWorldMap : public CBaseClassNonCopyable, public CIDrawWorldMap {   // vtable 0x01244AEC (RTTI .?AVCWorldMap@@)
public:
    virtual ~CWorldMap();                                                   // slot 0
    virtual void DeleteAllLevels();                                         // slot 1
    virtual void DeleteAllRegions();                                        // slot 2
    virtual void LoadFromFile(CAFile& file, long level);                    // slot 3
    virtual void SaveToFile(CAFile& file);                                  // slot 4
    virtual void RemoveLevel(long map);                                     // slot 5
    virtual void LoadTopologyFromFile(long map, CCountedPointer<CAFile> file, CASuspendableProcess& process);   // slot 6
    virtual void PostLoadTopology(long map);                                // slot 7
    virtual void ForceLoad(long map);                                       // slot 8
    virtual void ForceUnload(long map);                                     // slot 9
    virtual bool IsMapLoaded(long map) const;                               // slot 10
    virtual bool IsMapUsed(long map) const;                                 // slot 11
    virtual long GetGranularity() const;                                    // slot 12
    virtual const C2DBoxI& GetDimensions() const;                           // slot 13
    virtual CMap& GetMapAt(const C2DCoordI& pos);                           // slot 14
    virtual const CMap& PeekMapAt(const C2DCoordI& pos) const;              // slot 15
    // overloaded virtuals are laid out in reverse declaration order (the C3DVector form is slot 16)
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;                // slot 17 (+0x44) 0x004FC080
    virtual long GetMapNumberAt(const C3DVector& pos) const;                // slot 16 (+0x40) 0x004FC0C0
    virtual CMap& GetMap(long map);                                         // slot 18
    virtual const CMap& PeekMap(long map) const;                            // slot 19
    long GetRegionNumberMapIsIn(long map) const;                            // 0x004FC190 ?GetRegionNumberMapIsIn@CWorldMap@@QBEJJ@Z
    const CRegion& GetRegion(long region) const;                            // 0x004FC180 ?GetRegion@CWorldMap@@QBEABVCRegion@@J@Z
    unsigned long Members_[(0xD8 - 0x8) / 4];                               // +0x08 World .. (retail members, not modelled)
};

class CTCActionUseScriptedHook : public CTCBase {           // vtable 0x0126664C
public:
    const CCharString& GetRegionEntranceDisplayName() const;
    bool                        Usable;                     // +0x0C
    bool                        ForceConfirmation;          // +0x0D
    bool                        WaitingForConfirmation;     // +0x0E
    bool                        PerformAction;              // +0x0F
    bool                        TeleportToRegionEntrance;   // +0x10
    unsigned __int64            EntranceConnectedToUID;     // +0x18
    CIntelligentPointer<CThing> PEntranceConnectedTo;       // +0x20
    bool                        EntranceLocked;             // +0x28
    unsigned __int64            CameraTrackUID;             // +0x30
    CIntelligentPointer<CThing> PCameraTrack;               // +0x38
    CCharString                 SoundName;                  // +0x40
    CCharString                 AnimationName;              // +0x44
    long                        ReplacementObject;          // +0x48
    bool                        ReversedOnMiniMap;          // +0x4C
    bool                        HiddenOnMiniMap;            // +0x4D
};
FABLE_STATIC_ASSERT(sizeof(CTCActionUseScriptedHook) == 0x50);

const CCharString& CTCActionUseScriptedHook::GetRegionEntranceDisplayName() const
{
    const CWorldMap& worldMap = PeekWorldMap();
    long mapNumber = worldMap.GetMapNumberAt(PEntranceConnectedTo->GetPos());
    return PeekWorldMap().GetRegion(PeekWorldMap().GetRegionNumberMapIsIn(mapNumber)).GetDisplayName();
}