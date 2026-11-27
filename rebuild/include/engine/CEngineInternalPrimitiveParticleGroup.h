#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_H
#if defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP) && !defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_NO_CONTAINERS)
// Retail opt-in (chunk 2038, row 0x00BB8750 CEnginePrimitiveManagerParticleDecalGroup::RenderLandscapeDecalGroup): the typed class.
// Ego_r.pdb members in Ego_r order and names (struct_layouts_egor.tsv), retail offsets (the generated flattened model below: list 4,
// CArray 0xC, sizeof 0xAC). RTTI CEngineInternalPrimitiveParticleGroup : CEngineInternalPrimitiveBase (Ego_r: own members from +0x48 =
// sizeof CEngineInternalPrimitiveBase). Retail reads Primitives (STLport list<CEnginePrimitiveHandle>, the node pointer +0x48) at
// 0x00BB8A10 (`mov eax,[edx+48h] ; mov ecx,[eax]`, begin) and 0x00BB8ED7 (`cmp eax,[ecx+48h]`, end). Data members public (the
// renderers read them in place; the PDB access is not recorded by the layout dump).
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CArray.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CMatrix3x4.h"
enum EEngineEffectWaterStage;
class CEngineInternalPrimitiveParticleGroup : public CEngineInternalPrimitiveBase {
public:
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_ORIGINAL_TYPES
    // Retail opt-in (Codex pass22): both full original PDB class records.
    // Native Primitives is the four-byte STLport list at+0x48.
    typedef std::list<CEnginePrimitiveHandle> TPrimitiveList;
    typedef TPrimitiveList::iterator TPrimitiveIterator;
    typedef TPrimitiveList::const_iterator TPrimitiveConstIterator;
#endif
    // ?CPrimitiveIterator@CEngineInternalPrimitiveParticleGroup (Ego_r.pdb: bool Valid +0, the list iterator +4; 8 bytes).
    class CPrimitiveIterator {
    public:
        bool Valid;                                                      // +0x00
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_ORIGINAL_TYPES
        TPrimitiveIterator Iterator;
#else
        std::list<CEnginePrimitiveHandle>::iterator Iterator;            // +0x04
#endif
    };
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_ORIGINAL_TYPES
    typedef CArray<CPrimitiveIterator> TPrimitiveIteratorLookup;
    typedef std::list<long> TPrimitiveFreeIDList;
#ifdef FABLE_RETAIL_PARTICLE_GROUP_DESTRUCTOR_PASS29
protected:
    CEngineInternalPrimitiveParticleGroup(EEnginePrimitiveType,unsigned long);
#endif
    TPrimitiveList Primitives;
    TPrimitiveIteratorLookup PrimitiveIDToIterator;
    TPrimitiveFreeIDList FreePrimitiveIDs;
#else
    std::list<CEnginePrimitiveHandle> Primitives;                        // +0x48
    CArray<CPrimitiveIterator>        PrimitiveIDToIterator;             // +0x4c
    std::list<long>                   FreePrimitiveIDs;                  // +0x58
#endif
    C3DBoundingSphere                 BSphere;                           // +0x5c
    float                             DistanceFromCam;                   // +0x6c
    CMatrix3x4                        WorldMat;                          // +0x70
    bool                              ScreenDisplacement;                // +0xa0
    bool                              ReadZBuffer;                       // +0xa1
    EEngineEffectWaterStage           WaterStage;                        // +0xa4
    bool                              ZBufferWritable;                   // +0xa8
    bool                              ClipEffectToWeatherMask;           // +0xa9
    bool                              EnableDithering;                   // +0xaa
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_RTTI_DECLS
    // Opt-in (NF319, 2026-11-23): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base CEngineInternalPrimitiveBase chain declares at the same slot (no new slot).
    // 0x00BBBE60 vtable 0x012A59D4 slot 5: ?Update@CEngineInternalPrimitiveParticleGroup@@UAEXABVCEnginePrimitive@@K@Z (Ego_r.pdb + FableWin.pdb)
public:
    virtual void Update(const CEnginePrimitive&, unsigned long);      // 0x00BBBE60
public:
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_RTTI_SLOTS
    // Opt-in (NF325, 2026-11-24): declarations only, no layout change. The overrides the retail RTTI vftable 0x012A59D4 places under this
    // class (ghidra_out/labels_rtti_port.tsv; every spelling in Ego_r.pdb and FableWin.pdb), plus its two new virtuals in slot order
    // (slot 28 GetRenderLayerMask, slot 29 PrimTypeMatchesGroup). With _RTTI_DECLS (Update, slot 5) the emitted table matches retail
    // slot for slot (NF325 evidence/probe_*.txt; needs FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_SETCHILDPRIMITIVE_ORDER for slots 7/8).
public:
    virtual unsigned long AddChildPrimitive(const CEnginePrimitive&, unsigned long); // slot 6 0x00BBD1A0 ?AddChildPrimitive@CEngineInternalPrimitiveParticleGroup@@UAEKABVCEnginePrimitive@@K@Z
    virtual void SetChildPrimitive(unsigned long, CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long); // slot 7 0x00BBC310 ?SetChildPrimitive@CEngineInternalPrimitiveParticleGroup@@UAEXKAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z
    virtual void SetChildPrimitive(unsigned long, const CEnginePrimitive&, unsigned long); // slot 8 0x00BBCF70 ?SetChildPrimitive@CEngineInternalPrimitiveParticleGroup@@UAEXKABVCEnginePrimitive@@K@Z
    virtual void GetChildPrimitiveHandle(unsigned long, CEnginePrimitiveHandle&); // slot 9 0x00BBC3A0 ?GetChildPrimitiveHandle@CEngineInternalPrimitiveParticleGroup@@UAEXKAAVCEnginePrimitiveHandle@@@Z
    virtual void ClearChildPrimitive(unsigned long);                      // slot 10 0x00BBCE10 ?ClearChildPrimitive@CEngineInternalPrimitiveParticleGroup@@UAEXK@Z
    virtual void ClearAllChildPrimitives();                               // slot 11 0x00BBD200 ?ClearAllChildPrimitives@CEngineInternalPrimitiveParticleGroup@@UAEXXZ
    virtual CEnginePersistentPrimitiveBase* GetChildPrimitive(unsigned long); // slot 12 0x00BBC430 ?GetChildPrimitive@CEngineInternalPrimitiveParticleGroup@@UAEPAVCEnginePersistentPrimitiveBase@@K@Z
    virtual bool GetWorldPosition(C3DVector&);                            // slot 15 0x00BBBE40 ?GetWorldPosition@CEngineInternalPrimitiveParticleGroup@@UAE_NAAVC3DVector@@@Z
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere&);         // slot 18 0x00BBBE10 ?GetBoundingSphereWorldSpace@CEngineInternalPrimitiveParticleGroup@@UAE_NAAVC3DBoundingSphere@@@Z
    virtual unsigned long GetRenderLayerMask() const;                     // slot 28 0x00BBBDF0 ?GetRenderLayerMask@CEngineInternalPrimitiveParticleGroup@@UBEKXZ
    virtual bool PrimTypeMatchesGroup(long);                              // slot 29 0x00BBBF00 ?PrimTypeMatchesGroup@CEngineInternalPrimitiveParticleGroup@@UAE_NJ@Z
#endif
};
FABLE_STATIC_ASSERT_AT(39, sizeof(CEngineInternalPrimitiveParticleGroup) == 0xac);
#elif !defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineInternalPrimitiveParticleGroup = 0xc0.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineInternalPrimitiveParticleGroup = 0xac.
// Retail evidence: 2 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
struct CEngineInternalPrimitiveParticleGroup {
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
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
    unsigned char _pad_0x38[0x4];                            // +0x38
    unsigned char _pad_0x3c[0x4];                            // +0x3c
    unsigned char _pad_0x40[0x4];                            // +0x40
    unsigned char _pad_0x44[0x4];                            // +0x44
    unsigned char Primitives[0x4];                           // +0x48 list<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char PrimitiveIDToIterator[0xc];                // +0x4c CArray<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator>
    unsigned char FreePrimitiveIDs[0x4];                     // +0x58 list<long,std::allocator<long>_>
    unsigned char BSphere[0x10];                             // +0x5c C3DBoundingSphere
    float         DistanceFromCam;                           // +0x6c
    unsigned char WorldMat[0x30];                            // +0x70 CMatrix3x4
    bool          ScreenDisplacement;                        // +0xa0
    bool          ReadZBuffer;                               // +0xa1
    unsigned char _pad_0xa2[0x2];                            // +0xa2
    unsigned char WaterStage[0x4];                           // +0xa4 EEngineEffectWaterStage
    bool          ZBufferWritable;                           // +0xa8
    bool          ClipEffectToWeatherMask;                   // +0xa9
    bool          EnableDithering;                           // +0xaa
    unsigned char _pad_0xab[0x1];                            // +0xab
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(87, sizeof(CEngineInternalPrimitiveParticleGroup) == 0xac);
FABLE_STATIC_ASSERT_AT(88, offsetof(CEngineInternalPrimitiveParticleGroup, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(89, offsetof(CEngineInternalPrimitiveParticleGroup, Primitives) == 0x48);
FABLE_STATIC_ASSERT_AT(90, offsetof(CEngineInternalPrimitiveParticleGroup, PrimitiveIDToIterator) == 0x4c);
FABLE_STATIC_ASSERT_AT(91, offsetof(CEngineInternalPrimitiveParticleGroup, FreePrimitiveIDs) == 0x58);
FABLE_STATIC_ASSERT_AT(92, offsetof(CEngineInternalPrimitiveParticleGroup, BSphere) == 0x5c);
FABLE_STATIC_ASSERT_AT(93, offsetof(CEngineInternalPrimitiveParticleGroup, DistanceFromCam) == 0x6c);
FABLE_STATIC_ASSERT_AT(94, offsetof(CEngineInternalPrimitiveParticleGroup, WorldMat) == 0x70);
FABLE_STATIC_ASSERT_AT(95, offsetof(CEngineInternalPrimitiveParticleGroup, ScreenDisplacement) == 0xa0);
FABLE_STATIC_ASSERT_AT(96, offsetof(CEngineInternalPrimitiveParticleGroup, ReadZBuffer) == 0xa1);
FABLE_STATIC_ASSERT_AT(97, offsetof(CEngineInternalPrimitiveParticleGroup, WaterStage) == 0xa4);
FABLE_STATIC_ASSERT_AT(98, offsetof(CEngineInternalPrimitiveParticleGroup, ZBufferWritable) == 0xa8);
FABLE_STATIC_ASSERT_AT(99, offsetof(CEngineInternalPrimitiveParticleGroup, ClipEffectToWeatherMask) == 0xa9);
FABLE_STATIC_ASSERT_AT(100, offsetof(CEngineInternalPrimitiveParticleGroup, EnableDithering) == 0xaa);

#endif // FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_NO_CONTAINERS
// Add-only per-TU opt-in (NF459, 2026-11-26, rulings r456b / r457b on r451a): the typed class above without STLport. Define it TOGETHER
// with FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP (the Sprite and 2D group headers define that one themselves) over the polymorphic
// root (FABLE_RETAIL_DEF_CLASS_BASE): it then replaces the typed view. The typed view's three STLport containers and its C3DBoundingSphere
// (engine/C3DBoundingSphere.h includes stlport/fable_containers.h) pull in <new>, which adds /DEFAULTLIB:"libcp" to the object's .drectve
// (retail's objects never had it; r450a). Here those four members keep their names and offsets as opaque bytes (real type in the comment,
// as the generated flat model below spells them); every other member, the RTTI_DECLS / RTTI_SLOTS declarations and sizeof 0xAC are the
// typed view's. It adds the out-of-line destructor 0x00BBA140 ??1CEngineInternalPrimitiveParticleGroup@@UAE@XZ (Ego_r.pdb + FableWin.pdb;
// virtual: it overrides the root CIVCountedPointeeBase destructor, no new slot), which the six particle-group deleting destructors
// (0x00BBA240 Decal, 0x00BBF180 SpriteTrail, 0x00BC2240 Sprite, 0x00BC3BE0 Mesh, 0x00BC53E0 Line, 0x00BE1B30 2D) call directly. For TUs
// that only name the class or call that destructor (no member construction or destruction). Declarations only.
#if !defined(FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP) || !defined(FABLE_RETAIL_DEF_CLASS_BASE)
#error "FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_NO_CONTAINERS needs FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP and FABLE_RETAIL_DEF_CLASS_BASE"
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_ORIGINAL_TYPES
#error "FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_NO_CONTAINERS cannot be combined with FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_ORIGINAL_TYPES (STLport members)"
#endif
#include "rebuild_abi.h"
#include <stddef.h>
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CMatrix3x4.h"
enum EEngineEffectWaterStage;
class CEnginePrimitive; class CEnginePersistentPrimitiveBase; class C3DVector; class C3DBoundingSphere;
class CEngineInternalPrimitiveParticleGroup : public CEngineInternalPrimitiveBase {
public:
    unsigned char                     Primitives[0x4];                   // +0x48 std::list<CEnginePrimitiveHandle>
    unsigned char                     PrimitiveIDToIterator[0xc];        // +0x4c CArray<CEngineInternalPrimitiveParticleGroup::CPrimitiveIterator>
    unsigned char                     FreePrimitiveIDs[0x4];             // +0x58 std::list<long>
    unsigned char                     BSphere[0x10];                     // +0x5c C3DBoundingSphere
    float                             DistanceFromCam;                   // +0x6c
    CMatrix3x4                        WorldMat;                          // +0x70
    bool                              ScreenDisplacement;                // +0xa0
    bool                              ReadZBuffer;                       // +0xa1
    EEngineEffectWaterStage           WaterStage;                        // +0xa4
    bool                              ZBufferWritable;                   // +0xa8
    bool                              ClipEffectToWeatherMask;           // +0xa9
    bool                              EnableDithering;                   // +0xaa
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_RTTI_DECLS
public:
    virtual void Update(const CEnginePrimitive&, unsigned long);      // 0x00BBBE60
public:
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_RTTI_SLOTS
public:
    virtual unsigned long AddChildPrimitive(const CEnginePrimitive&, unsigned long); // slot 6 0x00BBD1A0
    virtual void SetChildPrimitive(unsigned long, CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long); // slot 7 0x00BBC310
    virtual void SetChildPrimitive(unsigned long, const CEnginePrimitive&, unsigned long); // slot 8 0x00BBCF70
    virtual void GetChildPrimitiveHandle(unsigned long, CEnginePrimitiveHandle&); // slot 9 0x00BBC3A0
    virtual void ClearChildPrimitive(unsigned long);                      // slot 10 0x00BBCE10
    virtual void ClearAllChildPrimitives();                               // slot 11 0x00BBD200
    virtual CEnginePersistentPrimitiveBase* GetChildPrimitive(unsigned long); // slot 12 0x00BBC430
    virtual bool GetWorldPosition(C3DVector&);                            // slot 15 0x00BBBE40
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere&);         // slot 18 0x00BBBE10
    virtual unsigned long GetRenderLayerMask() const;                     // slot 28 0x00BBBDF0
    virtual bool PrimTypeMatchesGroup(long);                              // slot 29 0x00BBBF00
#endif
public:
    virtual ~CEngineInternalPrimitiveParticleGroup();                    // 0x00BBA140
};
FABLE_STATIC_ASSERT_AT(209, sizeof(CEngineInternalPrimitiveParticleGroup) == 0xac);
FABLE_STATIC_ASSERT_AT(210, offsetof(CEngineInternalPrimitiveParticleGroup, Primitives) == 0x48);
FABLE_STATIC_ASSERT_AT(211, offsetof(CEngineInternalPrimitiveParticleGroup, BSphere) == 0x5c);
FABLE_STATIC_ASSERT_AT(212, offsetof(CEngineInternalPrimitiveParticleGroup, WorldMat) == 0x70);
FABLE_STATIC_ASSERT_AT(213, offsetof(CEngineInternalPrimitiveParticleGroup, WaterStage) == 0xa4);
#endif // FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_NO_CONTAINERS
#endif // FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_H
