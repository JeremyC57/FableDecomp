#ifndef FABLE_ENGINE_CTHINGTRACKNODE_H
#define FABLE_ENGINE_CTHINGTRACKNODE_H
#include "engine/CThingGameObject.h"
// Retail opt-in (Codex pass 12): original PDB sole base and full native allocation.
// NextNode, PreviousNode, NextNodeUID, PreviousNodeUID, TrackName, IsStart, IsEnd; native allocation527360.
class CThingTrackNode:public CThingGameObject {
private:
    union { CThingTrackNode* NextNode; long NextNodeThingFileID; }; // +d0, ctor837ca7
    union { CThingTrackNode* PreviousNode; long PreviousNodeThingFileID; }; // +d4, ctor837cad
    unsigned __int64 NextNodeUID; // +d8, ctor837cb3/837cb9
    unsigned __int64 PreviousNodeUID; // +e0, ctor837cbf/837cc5
    CCharString TrackName; // +e8, ctor837c94
    bool IsStart; // +ec, ctor837ccb
    bool IsEnd; // +ed, ctor837cd1
    unsigned char _pad_0xee[2];
public: static CClass* __fastcall Alloc();
#ifdef FABLE_RETAIL_CTHINGTRACKNODE_FLIP
    void FlipTrackDirection(); // Ego_r public nonconst ?FlipTrackDirection@CThingTrackNode@@QAEXXZ
#endif
#ifdef FABLE_RETAIL_CTHINGTRACKNODE_UPDATEFACING
    // Opt-in (chunk Q18, row 0x00837220): Ego_r.pdb / FableWin.pdb ?UpdateFacingDirection@CThingTrackNode@@QAEXXZ (public). Declaration only.
    void UpdateFacingDirection();                                       // 0x00837220
#endif
#ifdef FABLE_RETAIL_CTHINGTRACKNODE_LINK
    // Opt-in (chunk Q16, row 0x008378D0): Ego_r.pdb / FableWin.pdb ?SetAsLinkedTo@CThingTrackNode@@QAEXPAV1@@Z (public). Declaration only.
    void SetAsLinkedTo(CThingTrackNode* other);                         // 0x008378D0
#endif
#ifdef FABLE_RETAIL_CTHINGTRACKNODE
    // Retail opt-in (chunk 779, row 0x00837E40): the Ego_r.pdb member set used by the landed rows -- ?ConstructFromLevelScript@CThingTrackNode@@UAE_NPBD@Z
    // (vtable 0x01272CE4 slot 4 = 0x00837E40, the override of CThingGameObject's 0x00838930), ?ConstructFromParams@CThingTrackNode@@IAE_NJABVC3DVector@@VCCharString@@@Z
    // (protected, 0x00836C50 landed), ?Create@CThingTrackNode@@SIPAV1@JABVC3DVector@@VCCharString@@@Z (static __fastcall, 0x008383D0),
    // ?SetTrackName@CThingTrackNode@@QAEXABVCCharString@@@Z (0x00836EE0 landed), ?PeekTrackName@CThingTrackNode@@QBEABVCCharString@@XZ,
    // ?IsTrackStart@ / ?IsTrackEnd@ (QBE_NXZ, inline flag reads). The virtual OnInitialActivate / WorldUpdate overrides are not declared here
    // (their CThing base slots are not modelled in the shared chain yet). The layout above is the retail one (PDB 0x18 higher, landed 0x00836D00).
    // Needs the retail chain (FABLE_RETAIL_CTHING, _CTHINGPHYSICAL, _CTHINGGAMEOBJECT and their _LEVELSCRIPT opt-ins) for the virtual override.
public:
#ifdef FABLE_RETAIL_CTHINGTRACKNODE_CTOR
    // Retail opt-in (chunk 871, row 0x008383D0 CThingTrackNode::Create): Ego_r.pdb ??0CThingTrackNode@@QAE@XZ, out of line in retail at
    // 0x00837C70 (landed; it stores vtable 0x01272CE4) and called by Create through `new (false) CThingTrackNode`. A declaration only.
    CThingTrackNode();                                                  // 0x00837C70
#endif
    virtual bool ConstructFromLevelScript(const char* script);          // 0x00837E40
    static CThingTrackNode* __fastcall Create(long defIndex,const C3DVector& position,CCharString trackName);   // 0x008383D0
    void SetTrackName(const CCharString& trackName);                     // 0x00836EE0
    const CCharString& PeekTrackName() const { return TrackName; }
    bool IsTrackStart() const { return IsStart; }
    bool IsTrackEnd() const { return IsEnd; }
#ifdef FABLE_RETAIL_CTHINGTRACKNODE_PEEK_NEXT
    // Original FableWin0202f650, both donor class records; native traversal00860aa0.
    const CThingTrackNode* PeekNextNode()const{return NextNode;}
#endif
protected:
    bool ConstructFromParams(long defIndex,const C3DVector& position,CCharString trackName);   // 0x00836C50
public:
#endif
#ifdef FABLE_RETAIL_CTHINGTRACKNODE_VSLOTS
    // agentFAM03 (2026-11-11): retail virtuals missing from this declaration, declaration only (no layout change).
    // Slots: the TLC vtable aligned with the Xbox 360 Test PDB order on TLC-named slots; spellings: Ego_r / FableWin name sets.
public:
    virtual bool IsEntitySerialisable() const;  // slot 22, 0x008384E0
public:
#endif
};
FABLE_STATIC_ASSERT(sizeof(CThingTrackNode)==0xf0);
#endif
