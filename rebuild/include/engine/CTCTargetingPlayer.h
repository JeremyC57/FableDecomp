// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifndef FABLE_ENGINE_CTCTARGETINGPLAYER_H
#define FABLE_ENGINE_CTCTARGETINGPLAYER_H
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER
// Retail opt-in (chunk 777, row 0x00803540 UpdateTargetedPositionRandomWalk): the polymorphic retail class on the shared
// CTCTargetingBase (RTTI CTCTargetingPlayer : CTCTargetingBase : CTCBase; vtable 0x0124280C), Ego_r.pdb member names and types at
// the retail offsets of the flattened default below (+0x38 .. +0xF4, sizeof 0xF8). Needs FABLE_RETAIL_CTCBASE and
// FABLE_RETAIL_DEF_CLASS_BASE (CTCTargetingBase's CDefPointer). Member functions: the protected
// ?UpdateTargetedPositionRandomWalk@CTCTargetingPlayer@@IAEXXZ (Ego_r.pdb 0x007FE840; retail 0x00803540, called from FrameUpdate
// at 0x00805814). No vtable is emitted from these declarations (no virtual is defined here).
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS3
// agentFAM05 (2026-11-11): the member type of the typed pointers below.
#include "engine/CIntelligentPointer.h"
#endif // FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS3
#include "engine/CTCTargetingBase.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#include "engine/NPlayerTargeting.h"
#endif
class CTCTargetingPlayer : public CTCTargetingBase {
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
public:
    // Original public overrides in both complete PDB types; existing base slots.
    virtual CThing* GetPTargetedThing()const;
    virtual void SetPTargetedThing(CThing*);
    virtual CThing* GetPTargetLockedThing()const;
    enum ETargetNextThingMethod { TARGET_NEXT_NULL=0,TARGET_NEXT_NEXT=1,TARGET_NEXT_LEFT=2,TARGET_NEXT_RIGHT=3 };
    typedef std::vector<NPlayerTargeting::CTargetPoint> TTargetPointList;
#endif
protected:
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    void UpdateTargetList();
    float GetFOVToBeInToBeTargetable()const;
#endif
    void UpdateTargetedPositionRandomWalk();                    // 0x00803540
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
private:
    static bool PlayerTargetingLineOfSightTest,PlayerTargetingBuildingVolumeTest;
#endif
    long          ZLockTargetSelectionTimer;                    // +0x38
    bool          TargetNextThing;                              // +0x3c
    bool          ForcedToHaveNoTarget;                         // +0x3d
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    ETargetNextThingMethod TargetNextThingMethod;                   // +0x40 ETargetNextThingMethod
#else
    unsigned char TargetNextThingMethod[0x4];                   // +0x40 ETargetNextThingMethod
#endif
    long          FrameSetAsTargetNextThing;                    // +0x44
    float         MaxTargetingDistance;                         // +0x48
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    NPlayerTargeting::CTargetPoint TargetedPoint;                          // +0x4c NPlayerTargeting::CTargetPoint
    TTargetPointList Targets;                                 // +0x68 vector<NPlayerTargeting::CTargetPoint>
    TTargetPointList RejectedTargetPoints;                    // +0x74 vector<NPlayerTargeting::CTargetPoint>
#else
    unsigned char TargetedPoint[0x1c];                          // +0x4c NPlayerTargeting::CTargetPoint
    unsigned char Targets[0xc];                                 // +0x68 vector<NPlayerTargeting::CTargetPoint>
    unsigned char RejectedTargetPoints[0xc];                    // +0x74 vector<NPlayerTargeting::CTargetPoint>
#endif
    long          CurrentZTargetIndex;                          // +0x80
    C3DVector     TargetedPos;                                  // +0x84
    C3DVector     OldTargetedPos;                               // +0x90
    C3DVector     TargetedPosOffset3D;                          // +0x9c
    C3DVector     OldTargetedPosOffset3D;                       // +0xa8
    C2DVector     TargetedPosOffset;                            // +0xb4
    C2DVector     DeltaTargetedPosOffset;                       // +0xbc
    C2DVector     DesiredTargetedPosOffset;                     // +0xc4
    C2DVector     TargetedPosOffsetSecondOrder;                 // +0xcc
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    CIntelligentPointer<CThing> PLastThreatenedThing;                    // +0xd4 CIntelligentPointer<CThing>
    CIntelligentPointer<CThing> PProjectileTargetedThing;                // +0xdc CIntelligentPointer<CThing>
    CIntelligentPointer<CThing> PTargetLockedThing;                      // +0xe4 CIntelligentPointer<CThing>
    CIntelligentPointer<CThing> PTargetOnlyThing;                        // +0xec CIntelligentPointer<CThing>
#else
    unsigned char PLastThreatenedThing[0x8];                    // +0xd4 CIntelligentPointer<CThing>
#ifndef FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS3 // agentFAM05 (2026-11-11)
    unsigned char PProjectileTargetedThing[0x8];                // +0xdc CIntelligentPointer<CThing>
    unsigned char PTargetLockedThing[0x8];                      // +0xe4 CIntelligentPointer<CThing>
#else // FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS3
    // agentFAM05 (2026-11-11): the two members typed as Ego_r.pdb declares them (CIntelligentPointer<CThing>, 8 bytes, same offsets),
    // for the getters below (`add ecx,0DCh / 0E4h ; jmp CBaseIntelligentPointer::GetPItem`, 0x004E231D / 0x004E2328).
    CIntelligentPointer<CThing> PProjectileTargetedThing;       // +0xdc
    CIntelligentPointer<CThing> PTargetLockedThing;             // +0xe4
#endif // FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS3
    unsigned char PTargetOnlyThing[0x8];                        // +0xec CIntelligentPointer<CThing>
#endif
    bool          LockedOntoEnemy;                              // +0xf4
public:
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, ZLockTargetSelectionTimer) == 0x38);
        FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPos) == 0x84);
        FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPosOffset3D) == 0x9c);
        FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPosOffset) == 0xb4);
        FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPosOffsetSecondOrder) == 0xcc);
        FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, LockedOntoEnemy) == 0xf4);
    }
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x004E231A
public:
#endif
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS3
// agentFAM05 (2026-11-11): retail virtuals this declaration lacks, declaration only (no layout change; the vtable exists). Slots from the TLC vtable
// 0x0124280C (agentFAM03 proof, re-verified by agentFAM05); spellings from the Ego_r / FableWin name sets.
public:
    virtual CThing* GetPProjectileTargetedThing() const;   // slot 35, 0x004E231D
    virtual CThing* GetPTargetLockedThing() const;         // slot 36, 0x004E2328
public:
#endif // FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS3
};
FABLE_STATIC_ASSERT(sizeof(CTCTargetingPlayer) == 0xf8);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCTargetingPlayer = 0x100.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCTargetingPlayer = 0xf8.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
class CTCTargetingPlayer {
public:
#ifndef FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
#endif
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    unsigned char _pad_0x10[0x4];                            // +0x10
    unsigned char _pad_0x14[0x4];                            // +0x14
    unsigned char _pad_0x18[0x4];                            // +0x18
    unsigned char _pad_0x1c[0x4];                            // +0x1c
    unsigned char _pad_0x20[0x4];                            // +0x20
    unsigned char _pad_0x24[0x4];                            // +0x24
    unsigned char _pad_0x28[0x4];                            // +0x28
    unsigned char _pad_0x2c[0x4];                            // +0x2c
    unsigned char _pad_0x30[0x4];                            // +0x30
    unsigned char _pad_0x34[0x4];                            // +0x34
    long          ZLockTargetSelectionTimer;                 // +0x38
    bool          TargetNextThing;                           // +0x3c
    bool          ForcedToHaveNoTarget;                      // +0x3d
    unsigned char _pad_0x3e[0x2];                            // +0x3e
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    ETargetNextThingMethod TargetNextThingMethod;                // +0x40 ETargetNextThingMethod
#else
    unsigned char TargetNextThingMethod[0x4];                // +0x40 ETargetNextThingMethod
#endif
    long          FrameSetAsTargetNextThing;                 // +0x44
    float         MaxTargetingDistance;                      // +0x48
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    NPlayerTargeting::CTargetPoint TargetedPoint;                       // +0x4c CTargetPoint
    TTargetPointList Targets;                              // +0x68 vector<NPlayerTargeting::CTargetPoint,std::allocator<NPlayerTargeting::CTargetPoint>_>
    TTargetPointList RejectedTargetPoints;                 // +0x74 vector<NPlayerTargeting::CTargetPoint,std::allocator<NPlayerTargeting::CTargetPoint>_>
#else
    unsigned char TargetedPoint[0x1c];                       // +0x4c CTargetPoint
    unsigned char Targets[0xc];                              // +0x68 vector<NPlayerTargeting::CTargetPoint,std::allocator<NPlayerTargeting::CTargetPoint>_>
    unsigned char RejectedTargetPoints[0xc];                 // +0x74 vector<NPlayerTargeting::CTargetPoint,std::allocator<NPlayerTargeting::CTargetPoint>_>
#endif
    long          CurrentZTargetIndex;                       // +0x80
    float         TargetedPos_X;                             // +0x84 C3DVector::X
    float         TargetedPos_Y;                             // +0x88 C3DVector::Y
    float         TargetedPos_Z;                             // +0x8c C3DVector::Z
    float         OldTargetedPos_X;                          // +0x90 C3DVector::X
    float         OldTargetedPos_Y;                          // +0x94 C3DVector::Y
    float         OldTargetedPos_Z;                          // +0x98 C3DVector::Z
    float         TargetedPosOffset3D_X;                     // +0x9c C3DVector::X
    float         TargetedPosOffset3D_Y;                     // +0xa0 C3DVector::Y
    float         TargetedPosOffset3D_Z;                     // +0xa4 C3DVector::Z
    float         OldTargetedPosOffset3D_X;                  // +0xa8 C3DVector::X
    float         OldTargetedPosOffset3D_Y;                  // +0xac C3DVector::Y
    float         OldTargetedPosOffset3D_Z;                  // +0xb0 C3DVector::Z
    float         TargetedPosOffset_X;                       // +0xb4 C2DVector::X
    float         TargetedPosOffset_Y;                       // +0xb8 C2DVector::Y
    float         DeltaTargetedPosOffset_X;                  // +0xbc C2DVector::X
    float         DeltaTargetedPosOffset_Y;                  // +0xc0 C2DVector::Y
    float         DesiredTargetedPosOffset_X;                // +0xc4 C2DVector::X
    float         DesiredTargetedPosOffset_Y;                // +0xc8 C2DVector::Y
    float         TargetedPosOffsetSecondOrder_X;            // +0xcc C2DVector::X
    float         TargetedPosOffsetSecondOrder_Y;            // +0xd0 C2DVector::Y
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
    CIntelligentPointer<CThing> PLastThreatenedThing;                 // +0xd4 CIntelligentPointer<CThing>
    CIntelligentPointer<CThing> PProjectileTargetedThing;             // +0xdc CIntelligentPointer<CThing>
    CIntelligentPointer<CThing> PTargetLockedThing;                   // +0xe4 CIntelligentPointer<CThing>
    CIntelligentPointer<CThing> PTargetOnlyThing;                     // +0xec CIntelligentPointer<CThing>
#else
    unsigned char PLastThreatenedThing[0x8];                 // +0xd4 CIntelligentPointer<CThing>
    unsigned char PProjectileTargetedThing[0x8];             // +0xdc CIntelligentPointer<CThing>
    unsigned char PTargetLockedThing[0x8];                   // +0xe4 CIntelligentPointer<CThing>
    unsigned char PTargetOnlyThing[0x8];                     // +0xec CIntelligentPointer<CThing>
#endif
    bool          LockedOntoEnemy;                           // +0xf4
    unsigned char _pad_0xf5[0x3];                            // +0xf5
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool HasFrameUpdate() const;  // slot 8, 0x004E231A
public:
#endif
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCTargetingPlayer) == 0xf8);
#ifndef FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, __vftable) == 0x0);
#endif
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, ZLockTargetSelectionTimer) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetNextThing) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, ForcedToHaveNoTarget) == 0x3d);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetNextThingMethod) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, FrameSetAsTargetNextThing) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, MaxTargetingDistance) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPoint) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, Targets) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, RejectedTargetPoints) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, CurrentZTargetIndex) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPos_X) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPos_Y) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPos_Z) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, OldTargetedPos_X) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, OldTargetedPos_Y) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, OldTargetedPos_Z) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPosOffset3D_X) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPosOffset3D_Y) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPosOffset3D_Z) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, OldTargetedPosOffset3D_X) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, OldTargetedPosOffset3D_Y) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, OldTargetedPosOffset3D_Z) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPosOffset_X) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPosOffset_Y) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, DeltaTargetedPosOffset_X) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, DeltaTargetedPosOffset_Y) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, DesiredTargetedPosOffset_X) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, DesiredTargetedPosOffset_Y) == 0xc8);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPosOffsetSecondOrder_X) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, TargetedPosOffsetSecondOrder_Y) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, PLastThreatenedThing) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, PProjectileTargetedThing) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, PTargetLockedThing) == 0xe4);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, PTargetOnlyThing) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CTCTargetingPlayer, LockedOntoEnemy) == 0xf4);

#endif // FABLE_RETAIL_CTCTARGETINGPLAYER
#endif // FABLE_ENGINE_CTCTARGETINGPLAYER_H
