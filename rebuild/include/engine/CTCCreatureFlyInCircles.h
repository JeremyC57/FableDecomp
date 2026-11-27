#ifndef FABLE_ENGINE_CTCCREATUREFLYINCIRCLES_H
#define FABLE_ENGINE_CTCCREATUREFLYINCIRCLES_H
// New header (w24B, row 0x007DDA30 CTCCreatureFlyInCircles::QueueSwoop; hand-written, one model, no class macro). RTTI
// .?AVCTCCreatureFlyInCircles@@ : CTCBase : CBaseClassNonCopyable : CBaseClass (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb
// CTCCreatureFlyInCircles in order and spelling (struct_layouts_egor.tsv, 124 bytes there); retail is 0x78 because the retail
// CCardinalSpline is 0x14 (STLport 12-byte array; Ego_r 0x18), so the members after SwoopSpline sit at Ego_r - 4. Retail reads:
// Creature +0x0C, Mode +0x34, Transitioning +0x38, ModeChangeQueued +0x39, Active +0x3A (FrameUpdate 0x007DE030), PBurninationEffect
// +0x3B (the class is byte-packed there: the constructor 0x007DDF90 builds the pointer at +0x3B), HasBeenHit +0x43, PSwoopTarget
// +0x44, SwoopSpline +0x4C, SwoopTargOffset +0x60, SwoopProgress +0x6C, SwoopDelta +0x70 (QueueSwoop 0x007DDA30), LastExplodeTick +0x74.
// Methods: Ego_r.pdb / FableWin.pdb spellings and access (QAE public, IAE protected, U virtual overrides of CTCBase slots 0 / 8 / 9 /
// 21 / 22). EFlyMode: the PDB member type; its enumerator names are not in the name lists, so none are declared (the constructor
// stores 1, QueueSwoop 2).
// Needs FABLE_RETAIL_CTCBASE, FABLE_RETAIL_CINTELLIGENTPOINTER and FABLE_RETAIL_DEF_CLASS_BASE-free CCardinalSpline (engine/CCardinalSpline.h).
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCardinalSpline.h"
#include "engine/C3DVector.h"
class CThing;
class CThingCreatureBase;
#pragma pack(push, 1)
class CTCCreatureFlyInCircles : public CTCBase {
public:
#ifdef FABLE_RETAIL_CTCCREATUREFLYINCIRCLES_UPDATESWOOP
    // Ego_d TPI 0x2c165, original enum values. Retail stores 1 at 007dd947.
    enum EFlyMode { eHover, eFly, eSwoop };
#else
    enum EFlyMode { };                                                   // Ego_r.pdb member type (enumerators unnamed, see above)
#endif
    CTCCreatureFlyInCircles(CThing& thing);                              // 0x007DDF90 ??0CTCCreatureFlyInCircles@@QAE@AAVCThing@@@Z
    virtual ~CTCCreatureFlyInCircles();                                  // slot 0 (scalar deleting 0x007DE420)
    virtual bool HasFrameUpdate() const;                                 // slot 8 0x007DE410
    virtual void FrameUpdate();                                          // slot 9 0x007DE030
    virtual ETCInterfaceType GetInterfaceType() const;                   // slot 21 0x007DE3E0
    virtual CCharString GetClassTypeName() const;                        // slot 22 0x007DE3F0
    void InitFlightCircle(const C3DVector& centre, float radius, float speed);   // 0x007DD430 ?InitFlightCircle@..@@QAEXABVC3DVector@@MM@Z
    void ChangeRadius(float radius, long ticks);                         // ?ChangeRadius@CTCCreatureFlyInCircles@@QAEXMJ@Z
    void ChangeSpeed(float speed, long ticks);                           // ?ChangeSpeed@CTCCreatureFlyInCircles@@QAEXMJ@Z
    void QueueSwoop(const CThingCreatureBase& target, const C3DVector& start, const C3DVector& middle, const C3DVector& end);   // 0x007DDA30
    bool HasBeenHitSinceLastCheck();                                     // ?HasBeenHitSinceLastCheck@CTCCreatureFlyInCircles@@QAE_NXZ
protected:
    void ChangeMode();                                                   // 0x007DDCC0 ?ChangeMode@CTCCreatureFlyInCircles@@IAEXXZ
    void PlayLoopAnim();                                                 // 0x007DDDA0 ?PlayLoopAnim@CTCCreatureFlyInCircles@@IAEXXZ
    void UpdateSwoop(C3DVector& position, C3DVector& facing);            // 0x007DD540 ?UpdateSwoop@..@@IAEXAAVC3DVector@@0@Z
    void UpdateFlight(C3DVector& position, C3DVector& facing);           // 0x007DD270 ?UpdateFlight@..@@IAEXAAVC3DVector@@0@Z
    void CheckForHits();                                                 // 0x007DD990 ?CheckForHits@CTCCreatureFlyInCircles@@IAEXXZ
#ifdef FABLE_RETAIL_CTCCREATUREFLYINCIRCLES_UPDATESWOOP
    // Ego_d TPI 0xac43 is an lvalue reference. FableWin's success-assert string uses Creature.GetTCGraphicAppearance().
    CThingCreatureBase& Creature;
#else
    CThingCreatureBase* Creature;                                        // +0x0c
#endif
    C3DVector Centre;                                                    // +0x10
    float Speed;                                                         // +0x1c
    float DeltaSpeed;                                                    // +0x20
    long  SpeedChangeTicks;                                              // +0x24
    float CurrentRadius;                                                 // +0x28
    float DeltaRadius;                                                   // +0x2c
    long  RadiusChangeTicks;                                             // +0x30
    EFlyMode Mode;                                                       // +0x34
    bool  Transitioning;                                                 // +0x38
    bool  ModeChangeQueued;                                              // +0x39
    bool  Active;                                                        // +0x3a
    CIntelligentPointer<CThing> PBurninationEffect;                      // +0x3b
    bool  HasBeenHit;                                                    // +0x43
    CIntelligentPointer<const CThingCreatureBase> PSwoopTarget;          // +0x44
    CCardinalSpline SwoopSpline;                                         // +0x4c
    C3DVector SwoopTargOffset;                                           // +0x60
    float SwoopProgress;                                                 // +0x6c
    float SwoopDelta;                                                    // +0x70
    long  LastExplodeTick;                                               // +0x74
public:
    static void CheckRetailLayout() {
#ifdef FABLE_RETAIL_CTCCREATUREFLYINCIRCLES_UPDATESWOOP
        FABLE_STATIC_ASSERT(offsetof(CTCCreatureFlyInCircles, Centre) == 0x10);
#else
        FABLE_STATIC_ASSERT_AT(64, offsetof(CTCCreatureFlyInCircles, Creature) == 0x0c);
#endif
        FABLE_STATIC_ASSERT_AT(65, offsetof(CTCCreatureFlyInCircles, Mode) == 0x34);
        FABLE_STATIC_ASSERT_AT(66, offsetof(CTCCreatureFlyInCircles, PBurninationEffect) == 0x3b);
        FABLE_STATIC_ASSERT_AT(67, offsetof(CTCCreatureFlyInCircles, PSwoopTarget) == 0x44);
        FABLE_STATIC_ASSERT_AT(68, offsetof(CTCCreatureFlyInCircles, SwoopSpline) == 0x4c);
        FABLE_STATIC_ASSERT_AT(69, offsetof(CTCCreatureFlyInCircles, SwoopTargOffset) == 0x60);
        FABLE_STATIC_ASSERT_AT(70, offsetof(CTCCreatureFlyInCircles, SwoopDelta) == 0x70);
    }
};
#pragma pack(pop)
FABLE_STATIC_ASSERT_AT(74, sizeof(CTCCreatureFlyInCircles) == 0x78);
#endif // FABLE_ENGINE_CTCCREATUREFLYINCIRCLES_H
