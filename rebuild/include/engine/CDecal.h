#ifndef FABLE_ENGINE_CDECAL_H
#define FABLE_ENGINE_CDECAL_H
#ifdef FABLE_RETAIL_CDECAL_NO_CONTAINERS
#ifdef FABLE_RETAIL_CDECAL
#error "FABLE_RETAIL_CDECAL_NO_CONTAINERS is defined instead of FABLE_RETAIL_CDECAL, not with it"
#endif
// Add-only per-TU opt-in (NF451, 2026-11-25; row 0x00AF7C80, CDecal's deleting destructor, which calls ~CDecal 0x00AF7C10 and the
// class operator delete 0x00AF8AD0): NParticleEngine::CDecal as FABLE_RETAIL_CDECAL below declares it, without its two CArray
// (STLport vector) members PrimIDList +0x5C / ParentHandleList +0x68 and without engine/CArray.h, which pulls in STLport and adds
// /DEFAULTLIB:"libcp" to the object's .drectve. Define this INSTEAD of FABLE_RETAIL_CDECAL. Same members, names and order up to
// +0x5C; sizeof is 0x5C here, not 0x74. Declarations only: no bodies, no accessors.
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
namespace NParticleEngine {
class CDecal {
public:
    ~CDecal();                                                           // 0x00AF7C10
    virtual void Delete();                                               // slot 0 0x00AF7CA0
    static void* __fastcall operator new(unsigned int);
    static void __fastcall operator delete(void*);                       // 0x00AF8AD0
    float GetInterpolatedLifetimerInSeconds(float) const;
private:
    C3DVector                             Position;                      // +0x04
    CRightHandedSet                       RHSet;                         // +0x10
    long                                  Lifetimer;                     // +0x28
    float                                 CreationTime;                  // +0x2c
    long                                  MaxLife;                       // +0x30
    unsigned char                         FlickerIndex;                  // +0x34
    unsigned char                         FlickerTimeOffset;             // +0x35
    float                                 SizeFactor[2];                 // +0x38
    float                                 AlphaFactor[2];                // +0x40
    float                                 GraphicFrame;                  // +0x48
    long                                  UpdateTimer;                   // +0x4c
    float                                 MinLife;                       // +0x50
    float                                 MinSize;                       // +0x54
    bool                                  Active : 1;                    // +0x58 bit 0
    bool                                  SizeCollisionThisFrame : 1;
    bool                                  AlphaCollisionThisFrame : 1;
    bool                                  LandscapeOnlyDecal : 1;
};
}
FABLE_STATIC_ASSERT_AT(76, sizeof(NParticleEngine::CDecal) == 0x5C);
#else
#ifdef FABLE_RETAIL_CDECAL
// Retail opt-in (chunk 761, row 0x00AF7C10 ~CDecal): the polymorphic retail class NParticleEngine::CDecal (RTTI
// .?AVCDecal@NParticleEngine@@, no bases; vtable 0x0129F4FC: slot 0 Delete 0x00AF7CA0). Ego_r.pdb: ??1CDecal@NParticleEngine@@QAE@XZ
// (public, NOT virtual), ?Delete@CDecal@NParticleEngine@@UAEXXZ (virtual), class operator new / delete (static __fastcall
// ??2 / ??3 ..SI..), ?GetInterpolatedLifetimerInSeconds@..QBEMM@Z. Members: Ego_r names and order; retail offsets with the 12-byte
// CArray (STLport vector): PrimIDList +0x5C (its out-of-line ~vector<CArray<long> > 0x00AF78D0 is the dtor's tail jump),
// ParentHandleList +0x68 (destroyed inline: virtual element dtors, then the block free), sizeof 0x74.
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CArray.h"
#include "engine/CEnginePrimitiveHandle.h"
#ifdef FABLE_RETAIL_CDECAL_PREPARE
#include "engine/GFFastFloatToLong.h"
#ifndef FABLE_RETAIL_CPARTICLE_SPRITE
// Same mechanism as engine/CParticle.h (FABLE_RETAIL_CPARTICLE_SPRITE, which supplies these when both are on): with
// FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL, SetPrimitiveInfo's one-argument resizes of the two id vectors forward to the two-argument
// overloads (retail 0x00B1FD94 -> 0x00AFE910, 0x00B1FDB4 -> 0x004428F0) while the handle overload stays called (0x00B1FDD5 -> 0x00443F90).
namespace std {
template<> inline void vector<long,allocator<long> >::resize(size_type n){resize(n,long());}
template<> inline void vector<CArray<long>,allocator<CArray<long> > >::resize(size_type n){resize(n,CArray<long>());}
}
#endif
#endif
namespace NParticleEngine {
class CDecal {
public:
    ~CDecal();                                                           // 0x00AF7C10
    virtual void Delete();                                               // slot 0 0x00AF7CA0
    static void* __fastcall operator new(unsigned int);
    static void __fastcall operator delete(void*);
    float GetInterpolatedLifetimerInSeconds(float) const;
#ifdef FABLE_RETAIL_CDECAL_PASS22_CLEARALL
    // Retail opt-in (Codex pass 22): FableWin ClearAll and the native two-container clear.
    // This row defines the original public method out of line; earlier opt-ins are unchanged.
    void ClearAll();
#endif
#ifdef FABLE_RETAIL_CDECAL_CLEAR
    // Opt-in (row 0x00AF5430 CParticleSystem::ClearDecal): FableWin.pdb accessors, all inline in retail (no retail copies):
    // ?GetParentHandle@CDecal@NParticleEngine@@QBE_NJAAVCEnginePrimitiveHandle@@@Z, ?GetPrimID@CDecal@NParticleEngine@@QBE_NJJAAJ@Z,
    // ?ClearParentHandle@CDecal@NParticleEngine@@QAEXJ@Z, ?ClearAll@CDecal@NParticleEngine@@QAEXXZ. The bounds tests are written
    // size-first (retail `cmp size,group ; jbe`). ClearParentHandle is forced inline: retail expands it in ClearDecal's loop
    // (0x00AF555A..0x00AF55D3), VC7.1's inline budget otherwise calls it there. ClearAll's PrimIDList.clear() calls the
    // out-of-line vector<CArray<long> >::erase 0x00AF7910; ParentHandleList.clear() expands (needs the __copy 0x00440260 declared).
    bool GetParentHandle(long group, CEnginePrimitiveHandle& handle) const {
        if (ParentHandleList.size() > group) { handle = ParentHandleList[group]; return !handle.IsNull(); }
        return false;
    }
    bool GetPrimID(long group, long index, long& id) const {
        if (PrimIDList.size() > group && PrimIDList[group].size() > index) { id = PrimIDList[group][index]; return id >= 0; }
        return false;
    }
    FABLE_FORCEINLINE void ClearParentHandle(long group) {
        if (ParentHandleList.size() > group) { ParentHandleList[group].Clear(); PrimIDList[group].clear(); }
    }
    void ClearAll() { ParentHandleList.clear(); PrimIDList.clear(); }
#endif
#ifdef FABLE_RETAIL_CDECAL_PREPARE
    // Opt-in (wfD6, row 0x00B1F200 CPSCDecalRenderer::PreparePrimitives): FableWin.pdb inline accessors of CDecal (no retail
    // copy of any of them; the row reads the members in place): ?GetInterpolatedLifetimer@CDecal@NParticleEngine@@QBEMM@Z
    // (`fild [+28h] ; fadd`), ?GetMaxLife@..@QBEJXZ, ?IsActive@..@QBE_NXZ, ?GetMinLife@..@QBEMXZ / ?SetMinLife@..@QAEXM@Z,
    // ?GetAlphaFactor@..@QBEMM@Z / ?GetSizeFactor@..@QBEMM@Z ((F[0] - F[1]) * t + F[1]), ?GetFlickerIndex@..@QBEEXZ,
    // ?GetFlickerTimeOffset@..@QBEEXZ, ?GetNearestGraphicFrame@..@QBEJXZ (GFFastFloatToLong of +48h, retail 0x00B1FB95),
    // ?IsLandscapeOnlyDecal@..@QBE_NXZ, ?GetPosition@..@QBEABVC3DVector@@XZ, ?GetRightHandedSet@..@QBEABVCRightHandedSet@@XZ,
    // ?SetPrimitiveInfo@..@QAEXJJABVCEnginePrimitiveHandle@@J@Z (the text of CParticle::SetPrimitiveInfo, FABLE_RETAIL_CPARTICLE_SPRITE).
    float GetInterpolatedLifetimer(float fraction) const { return Lifetimer + fraction; }
    long GetMaxLife() const { return MaxLife; }
#ifdef FABLE_RETAIL_CDECAL_ISACTIVE_MAXLIFE
    // hand3c nested opt-in (row 0x00B1D550 CPSCDecalRenderer::DecalCollides): the FableWin.pdb ?IsActive@CDecal@NParticleEngine@@QBE_NXZ
    // body (FableWin 0x02F56430: `cmp [this+30h],-1 ; jle false`, then bit 0 of +0x58), expanded in retail (0x00B1D684..0x00B1D695).
    bool IsActive() const { return MaxLife > -1 && Active; }
#else
    bool IsActive() const { return Active; }
#endif
    float GetMinLife() const { return MinLife; }
    void SetMinLife(float minLife) { MinLife = minLife; }
    float GetAlphaFactor(float t) const { return (AlphaFactor[0] - AlphaFactor[1]) * t + AlphaFactor[1]; }
    float GetSizeFactor(float t) const { return (SizeFactor[0] - SizeFactor[1]) * t + SizeFactor[1]; }
#ifdef FABLE_RETAIL_CDECAL_POOLING
    // hand3c nested opt-in (row 0x00B1D790 CPSCDecalRenderer::PoolDecal): FableWin.pdb bodies, all expanded in retail --
    // ?GetGraphicFrame@..@QBEMXZ (0x02F575F0) / ?SetGraphicFrame@..@QAEXM@Z (0x02F575D0) on +0x48, ?GetAlphaFactor@..@QBEMXZ (0x02F576F0,
    // AlphaFactor[0]), ?IncrementSizeFactor@..@QAEXM@Z (0x02F57610) / ?IncrementAlphaFactor@..@QAEXM@Z (0x02F57680: once per frame -- set
    // the collision bit, UpdateTimer = Lifetimer, keep the old factor in [1], add to [0]) and ?Deactivate@..@QAEXXZ (0x02F57710, clears
    // the active bit). Bodies only: no layout change.
    float GetGraphicFrame() const { return GraphicFrame; }
    void SetGraphicFrame(float frame) { GraphicFrame = frame; }
    float GetAlphaFactor() const { return AlphaFactor[0]; }
    void IncrementSizeFactor(float increment) {
        if (!SizeCollisionThisFrame) {
            SizeCollisionThisFrame = true;
            UpdateTimer = Lifetimer;
            SizeFactor[1] = SizeFactor[0];
            SizeFactor[0] = SizeFactor[0] + increment;
        }
    }
    void IncrementAlphaFactor(float increment) {
        if (!AlphaCollisionThisFrame) {
            AlphaCollisionThisFrame = true;
            UpdateTimer = Lifetimer;
            AlphaFactor[1] = AlphaFactor[0];
            AlphaFactor[0] = AlphaFactor[0] + increment;
        }
    }
    void Deactivate() { Active = false; }
#endif
#ifdef FABLE_RETAIL_CDECAL_GETSIZEFACTOR_NOARG
    // hand3c nested opt-in (row 0x00B1D550): FableWin.pdb ?GetSizeFactor@CDecal@NParticleEngine@@QBEMXZ (FableWin 0x02F56D60: `fld [this+38h]`),
    // the no-argument overload, inline in retail (0x00B1D705 `fmul [edx+38h]`). Body only: no layout change.
    float GetSizeFactor() const { return SizeFactor[0]; }
#endif
    unsigned char GetFlickerIndex() const { return FlickerIndex; }
    unsigned char GetFlickerTimeOffset() const { return FlickerTimeOffset; }
    long GetNearestGraphicFrame() const { return GFFastFloatToLong(GraphicFrame); }
    bool IsLandscapeOnlyDecal() const { return LandscapeOnlyDecal; }
    const C3DVector& GetPosition() const { return Position; }
    const CRightHandedSet& GetRightHandedSet() const { return RHSet; }
#ifndef FABLE_RETAIL_CDECAL_CLEAR
    bool GetPrimID(long group, long index, long& id) const {
        if (PrimIDList.size() > (unsigned long)group && PrimIDList[group].size() > (unsigned long)index) { id = PrimIDList[group][index]; return id >= 0; }
        return false;
    }
#endif
    void SetPrimitiveInfo(long list, long index, const CEnginePrimitiveHandle& parent, long id)
    {
        if (PrimIDList.size() <= (unsigned long)list)
            PrimIDList.resize(list + 1);
        if (PrimIDList[list].size() <= (unsigned long)index)
            PrimIDList[list].resize(index + 1);
        PrimIDList[list][index] = id;
        if (ParentHandleList.size() <= (unsigned long)list)
            ParentHandleList.resize(list + 1);
        ParentHandleList[list] = parent;
    }
#endif
#ifdef FABLE_RETAIL_CDECAL_UPDATE
 // Both PDB accessors; native B0E66x onward uses these exact field locations.
    void SetPosition(const C3DVector& position) { Position = position; }
    void SetRightHandedSet(const CRightHandedSet& set) { RHSet = set; }
    void SetLandscapeOnlyDecal(bool landscape) { LandscapeOnlyDecal = landscape; }
    void SetLifetimer(long lifetimer) { Lifetimer = lifetimer; }
    void SetMaxLife(long maxLife) { MaxLife = maxLife; }
    void SetFlickerIndex(unsigned char index) { FlickerIndex = index; }
    void SetFlickerTimeOffset(unsigned char offset) { FlickerTimeOffset = offset; }
    void Activate() { Active = true; }
private:
#endif
    C3DVector                             Position;                      // +0x04
    CRightHandedSet                       RHSet;                         // +0x10
    long                                  Lifetimer;                     // +0x28
    float                                 CreationTime;                  // +0x2c
    long                                  MaxLife;                       // +0x30
    unsigned char                         FlickerIndex;                  // +0x34
    unsigned char                         FlickerTimeOffset;             // +0x35
    float                                 SizeFactor[2];                 // +0x38
    float                                 AlphaFactor[2];                // +0x40
    float                                 GraphicFrame;                  // +0x48
    long                                  UpdateTimer;                   // +0x4c
    float                                 MinLife;                       // +0x50
    float                                 MinSize;                       // +0x54
    bool                                  Active : 1;                    // +0x58 bit 0
    bool                                  SizeCollisionThisFrame : 1;
    bool                                  AlphaCollisionThisFrame : 1;
    bool                                  LandscapeOnlyDecal : 1;
    CArray<CArray<long> >                 PrimIDList;                    // +0x5c
    CArray<CEnginePrimitiveHandle>        ParentHandleList;              // +0x68
};
}
FABLE_STATIC_ASSERT_AT(75, sizeof(NParticleEngine::CDecal) == 0x74);
#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CDecal = 0x7c.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CDecal = 0x74.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
struct CDecal {
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    float         Position_X;                                // +0x04 C3DVector::X
    float         Position_Y;                                // +0x08 C3DVector::Y
    float         Position_Z;                                // +0x0c C3DVector::Z
    unsigned char RHSet[0x18];                               // +0x10 CRightHandedSet
    long          Lifetimer;                                 // +0x28
    float         CreationTime;                              // +0x2c
    long          MaxLife;                                   // +0x30
    unsigned char FlickerIndex;                              // +0x34
    unsigned char FlickerTimeOffset;                         // +0x35
    unsigned char _pad_0x36[0x2];                            // +0x36
    float         SizeFactor[2];                             // +0x38
    float         AlphaFactor[2];                            // +0x40
    float         GraphicFrame;                              // +0x48
    long          UpdateTimer;                               // +0x4c
    float         MinLife;                                   // +0x50
    float         MinSize;                                   // +0x54
    unsigned char LandscapeOnlyDecal[0x2];                   // +0x58 bitfield group: bool:1
    unsigned char _pad_0x5a[0x2];                            // +0x5a
    unsigned char PrimIDList[0xc];                           // +0x5c CArray<CArray<long>_>
    unsigned char ParentHandleList[0xc];                     // +0x68 CArray<CEnginePrimitiveHandle>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(112, sizeof(CDecal) == 0x74);
FABLE_STATIC_ASSERT_AT(113, offsetof(CDecal, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(114, offsetof(CDecal, Position_X) == 0x4);
FABLE_STATIC_ASSERT_AT(115, offsetof(CDecal, Position_Y) == 0x8);
FABLE_STATIC_ASSERT_AT(116, offsetof(CDecal, Position_Z) == 0xc);
FABLE_STATIC_ASSERT_AT(117, offsetof(CDecal, RHSet) == 0x10);
FABLE_STATIC_ASSERT_AT(118, offsetof(CDecal, Lifetimer) == 0x28);
FABLE_STATIC_ASSERT_AT(119, offsetof(CDecal, CreationTime) == 0x2c);
FABLE_STATIC_ASSERT_AT(120, offsetof(CDecal, MaxLife) == 0x30);
FABLE_STATIC_ASSERT_AT(121, offsetof(CDecal, FlickerIndex) == 0x34);
FABLE_STATIC_ASSERT_AT(122, offsetof(CDecal, FlickerTimeOffset) == 0x35);
FABLE_STATIC_ASSERT_AT(123, offsetof(CDecal, SizeFactor) == 0x38);
FABLE_STATIC_ASSERT_AT(124, offsetof(CDecal, AlphaFactor) == 0x40);
FABLE_STATIC_ASSERT_AT(125, offsetof(CDecal, GraphicFrame) == 0x48);
FABLE_STATIC_ASSERT_AT(126, offsetof(CDecal, UpdateTimer) == 0x4c);
FABLE_STATIC_ASSERT_AT(127, offsetof(CDecal, MinLife) == 0x50);
FABLE_STATIC_ASSERT_AT(128, offsetof(CDecal, MinSize) == 0x54);
FABLE_STATIC_ASSERT_AT(129, offsetof(CDecal, LandscapeOnlyDecal) == 0x58);
FABLE_STATIC_ASSERT_AT(130, offsetof(CDecal, PrimIDList) == 0x5c);
FABLE_STATIC_ASSERT_AT(131, offsetof(CDecal, ParentHandleList) == 0x68);

#endif // FABLE_RETAIL_CDECAL

#endif // FABLE_RETAIL_CDECAL_NO_CONTAINERS
#endif // FABLE_ENGINE_CDECAL_H
