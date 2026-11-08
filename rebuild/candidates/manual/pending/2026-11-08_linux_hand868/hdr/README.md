# hdr/ overlay -- chunk 868 (agent868)

Every file here is the repo header plus an additive `#ifdef FABLE_RETAIL_*` opt-in (declarations only, no layout change for
any existing includer), or a new header. Carried over from hand-back 863 unchanged: `engine/CQ_OrchardFarmRaidScript.h`
(new) and the `FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSINMARKER` block of `engine/CQ_BanditCampBossBattleScript.h`
(used by the parked differ/00d11930.cpp and differ/00dcfa60.cpp). Neither matched row (0x00D18DF0, 0x00ECBDF0) needs any
overlay header: both build on the repo tree as it is.

New this chunk, all for the 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate shared-header rebuild
(differ/007a1b20.cpp, parked 2712v2840):

| header | opt-in macro | what it adds | evidence |
|---|---|---|---|
| engine/CThing.h | FABLE_RETAIL_CTHING_VIRTUALS | vtable slots 17..45 (IsToBePlacedInMap .. SetPositionUsingBrain) + forward decls of their parameter types | PDB-named local model in landed rebuild/src/compiled/00/83/CThingDeadCreature_OnCreate_00835fd0.cpp; CThingGameObject.h STANDINGON slots 38/39 agree |
| engine/CThingPhysical.h | FABLE_RETAIL_CTHINGPHYSICAL_VIRTUALS | slots 46..59 (IsSolidToThing .. ModifyHealth; PeekPMaterial returns CDefPointer<const CMaterialDef>) | same local model; needs the CThing chain |
| engine/CThingGameObject.h | FABLE_RETAIL_CTHINGGAMEOBJECT_VIRTUALS | slots 60..64 (DrawIsTargeted .. IsCover) so `OnHit` is slot 65 = +0x104 | retail `call [edx+104h]` at 0x007A228C / 0x007A2436 |
| engine/CTCWillLightning.h | FABLE_RETAIL_CTCWILLLIGHTNING_BEAM | `class CLightningBeam;` + `StartLoopingSound()` (0x007A0400) in the class; the naturally aligned `CTCWillLightning::CLightningBeam` (sizeof 0x7c, STLport list at +0x4c) after it | struct_layouts_egor.tsv (PDB 0x84 with the 8-byte-larger Dinkumware list); every offset read by 0x007A1B20 (m_fTime +0x44, m_lChildren +0x4c, m_bTemporary +0x50, m_fDelay +0x68, m_nFrameCount +0x70, PDef +0x74, SuccessfulHit +0x78) |
| engine/CHitParameters.h | FABLE_RETAIL_CHITPARAMETERS_LIGHTNING | 9-arg ctor (0x00789238), `AddDamageWithAttribute(EDamageAttribute,float)` (0x00868CA0), inline `SetCreateDustParticleEffectOnHit(bool)`; `enum EDamageAttribute;` forward | FableWin.pdb ctor name; labels; landed 0x007E2600 sibling models |
| engine/CTCEntityEvents.h | FABLE_RETAIL_CTCENTITYEVENTS_ADDEVENT | `AddEvent(NEntityEvents::CEventBase*)` (0x008113C0) | Ego_r.pdb / FableWin.pdb name; the row's call after `new CEventHitBySpecialAbility` |
| engine/CTCSoundPlayer.h | FABLE_RETAIL_CTCSOUNDPLAYER_UNATTACHED | `PlayUnattachedCriteria(const CCharString&,const C3DVector&,bool,bool)` (0x0066BDF0) | labels + landed will-lightning rows |
| engine/NCreatureMode.h | FABLE_RETAIL_NCREATUREMODE_QUERIES | `__fastcall IsUnderground(const CThing&)` 0x0085F120, `IsLightningable` 0x0085FAA0 | labels_*.tsv + PDB names |
| engine/CSpecialAbilitiesLightningSpellDef.h | (new, always on) | `: public CDefPointeeBase` (so CDefPointer<const T>::Get compiles), pad to +0x110, CDefString InitSound/LoopingSound/EndHeroSound/EndAttackeeSound +0x110..+0x11c | landed ctor / StartLoopingSound 0x007A0400 read +0x110/+0x114; the row reads +0x11c; PDB order |
| engine/NEntityEvents_CEventHitBySpecialAbility.h | (new) | ctor `(const CHitParameters&, EHeroAbility)` 0x00811830, sizeof 0x18 | struct_layouts_egor.tsv sizeof; `push 18h ; new` in the row |
| engine/CTCElectrocutedEffect.h | (new) | `void Reset(); // 0x007CE7A0` | labels_*.tsv + PDB |

Pack: CLightningBeam is deliberately NOT `#pragma pack(1)` -- the PDB layout has no odd offsets and a packed declaration makes
VC7.1 spill the float members through a frame temporary (r1: 2728 bytes, frame 0xc4; r2 proved pack irrelevant to the remaining
residue). `CCRC::Calc` is declared only inside CSpecialEffectsDef.h's GETEFFECT_BODY opt-in, so the row declares it locally.

## Round 2 (coordinator 2026-11-08r): the 0x00A2F740 C3DMeshStats::InitialiseFromFileData rebuild (differ/00a2f740.cpp, DIFFER(3059v3051) md 720)

New headers (always on, PDB layouts from ghidra_out/struct_layouts_egor.tsv, addresses from the landed rows / labels):

| header | what it holds | evidence |
|---|---|---|
| engine/C3DMeshStats.h | `: public CMovableResource` (needs FABLE_RETAIL_CMOVABLERESOURCE); MemoryPool +4, StatsMemoryHandle +8, Name +0xC, BoundingSphere +0x10, BoundingBox +0x20, `CPreAllocVectorMap<ulong,C3DHelperPoint>` HelperPoints +0x38, `CPreAllocVectorMap<ulong,C3DDummyObject>` DummyObjects +0x48, `CVectorMap<ulong,C3DMeshVolume>` MeshVolumes +0x58, `CArray<C3DGeneratorObject>` MeshGenerators +0x68, `char*` PackedNames +0x74, `unsigned short` PackedNamesSize +0x78, `bool` AnimatedFlag +0x7A, sizeof 0x7C; every PDB method declared with its landed address; private AllocateBuffers 0x00A2E760 / PackNames 0x00A2EEF0 / UnpackNames | struct_layouts_egor.tsv; the row's stores; landed AllocateBuffers / PackNames rows |
| engine/C3DMeshFile.h | `: public CChunkedFile`, ctor, `CCountedPointer<const C3DMeshFileRootChunk> GetRoot() const` 0x00A32620 (+ non-const), sizeof 0x14 | landed GetRoot row; PDB |
| engine/C3DHelperPoint.h | Position +0, `long` BoneIndex +0xC, sizeof 0x10; ctors `()` / `(const C3DVector&, unsigned long)`; GetPosition / GetBoneIndex | struct_layouts_egor.tsv; the row's 4-dword copy |
| engine/C3DDummyObject.h | CMatrix3x4 Transform +0, `long` BoneIndex +0x30, sizeof 0x34; ctors `()` / `(const CMatrix3x4&, unsigned long)`; PeekTransform / GetPosition / GetBoneIndex (members private: a public variant, r12, was byte-identical and was reverted) | struct_layouts_egor.tsv; the row's `rep movsd` 0xC dwords + bone store |
| engine/C3DGeneratorObject.h | `: public C3DDummyObject`; CCharString ObjectName +0x34, `unsigned long` BankIndex +0x38, `bool` UseLocalOrigin +0x3C, sizeof 0x40; ctor `(const C3DDummyObject&, bool useLocalOrigin, const CCharString& objectName, unsigned long bankIndex)`; getters; LoadBinary / SaveBinary | struct_layouts_egor.tsv; the push_back 0x00A345D0 argument build in the row |
| engine/C3DMeshVolume.h | CCharString Name +0, `CCountedPointer<CContainmentVolume>` ConvexVolume +4, sizeof 0xC; ctors 0x00A0A3D0 / `(const C3DMeshFileConvexVolumeChunk&, CMovableResourceMemoryPool*)` 0x00A2E9A0; NO user destructor (0x00A32690 is the implicit one's unreferenced COMDAT; declaring it makes the row call it); GetName by value, GetConvexVolume, containment queries, LoadBinary / SaveBinary | struct_layouts_egor.tsv; landed ctor row; the row's inline release |
| engine/CPreAllocVectorMap.h | `template<K,V,CompareType=CKeyPairCompareLess<K,V>>`: Compare +0, Elements +4, Size +8, Dirty +0xC; Initialise, begin/end, size, GetByteSize, GetAsCArray, SetDirty, `Sort()` (= `std::sort(Elements, Elements+Size, Compare); Dirty = false;`), LowerBound / UpperBound declared, Count, Find, RelocateData | struct_layouts_egor.tsv; the row's std::sort 0x00A355F0 / 0x00A35150 calls + `mov byte [this+44h],0` |
| engine/CCRC.h | `extern unsigned long g_CRCTable[256]; // 0x0129A168` (no PDB name); `CCRC::Calc(seed, data, byteCount)` inline under FABLE_RETAIL_CCRC_CALC_INLINE (the 0x004014A0 table loop), else declared; CalcFromString 0x00AEAA10 / CalcFromPathname 0x00AEAA70 | the row expands the loop three times (0x00A2F85B..); landed 0x004014A0 |

Additive opt-ins on repo headers (declarations only; `#ifdef FABLE_RETAIL_*`):

| header | opt-in macro | what it adds | evidence |
|---|---|---|---|
| fable_string.h (root) | FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE | `GetCRC()` as an inline member `return CCRC::Calc(0, GetAsCharArray(), GetLength());` (+ `#include "engine/CCRC.h"` under the same guard at the top), else the existing declaration | the row inlines GetCRC + Calc three times |
| fable_string.h (root) | FABLE_RETAIL_CCHARSTRING_GETASLOWERCASE | `CCharString GetAsLowerCase() const; // 0x0099ED10` | Ego_r.pdb / FableWin.pdb; the row's call at 0x00A2FA76 |
| stlport/fable_containers.h | FABLE_RETAIL_STLP_COPY | `template<class T> T* copy(T*, T*, T*)` and the const-T* overload dispatching on `__type_traits<T>::has_trivial_assignment_operator` to `__copy_trivial` (memmove) / `__copy<I,O,int>` | STLport 4.6 `copy`; the row's 0x00A31150 / 0x00A311B0 calls and the memmove |
| engine/C3DMeshFileHelpersChunk.h | FABLE_RETAIL_C3DMESHFILEHELPERSCHUNK_FINDERS | FindFirstPoint / FindNextPoint / FindFirstDummy / FindNextDummy / FindFirstVolumeW / FindNextVolumeW, const (0x00AAA270 / 0x00AAA300 / 0x00AAA550 / 0x00AAA5E0 / 0x00AAA830 / 0x00AAA8C0) and non-const, + forward decls | fablewin_pdb_names.tsv; the landed finder rows |
| engine/C3DMeshFileRootChunk.h | FABLE_RETAIL_C3DMESHFILEROOTCHUNK_HELPERS | HasHelpers 0x00A9B2C0, GetHelpers const 0x00A9B3D0 / non-const | landed rows; PDB |
| engine/C3DMeshFileHelperPointChunk.h | FABLE_RETAIL_C3DMESHFILEHELPERPOINTCHUNK_GETTERS | GetPosition (by value), GetSubMeshIndex, GetBoneIndex, `const CCharString& GetName()` | PDB; the row's reads |
| engine/C3DMeshFileDummyObjectChunk.h | FABLE_RETAIL_C3DMESHFILEDUMMYOBJECTCHUNK_GETTERS | `CMatrix3x4 GetTransform() const`, GetUseLocalOrigin | PDB; the row's `rep movsd` |
| engine/C3DMeshFileConvexVolumeChunk.h | FABLE_RETAIL_C3DMESHFILECONVEXVOLUMECHUNK_GETTERS | GetName, GetPlaneCount, PeekPlanes | PDB |
| engine/C3DMeshFileXBoundingSphereChunk.h | FABLE_RETAIL_C3DMESHFILEXBOUNDINGSPHERECHUNK_GETTERS | GetCenter, GetRadius | PDB; the row's XBDS block |
| engine/C3DMeshFileXBoundingBoxChunk.h | FABLE_RETAIL_C3DMESHFILEXBOUNDINGBOXCHUNK_GETTERS | GetCenter, GetDiagonal | PDB; the row's XBDB block |
| engine/C3DBoundingSphere.h | FABLE_RETAIL_C3DBOUNDINGSPHERE_SET | `void Set(const C3DVector& center, float radius)` (unused by the final row: it routes the radius through x87) | kept for completeness |
| engine/C3DBoundingBox.h | FABLE_RETAIL_C3DBOUNDINGBOX_SETCENTERDIAG (inside FABLE_RETAIL_C3DBOUNDINGBOX) | `SetMinMax(min, max)` and `__forceinline void SetCenterDiag(const C3DVector& center, const C3DVector& diagonal) { MinCoord = center - diagonal; MaxCoord = center + diagonal; }` -- __forceinline under the owner's 2026-11-01 FableWin math-helper rule (plain inline is called out of line at the guarded XBDB site, r3/r6; retail inlines, min stored first) | the row's XBDB block 0x00A3031A.. |

Lane-only: `fable_definition_table.h` is a byte-identical copy of the repo header. Reason: `engine/CDefString.h` (repo) includes
`"fable_definition_table.h"`, which quote-includes `"fable_string.h"` and so finds the REPO copy in its own directory before the overlay,
and the two `#pragma once` files then redefine every string class (it broke the 0x007A1B20 build). The copy is a no-op at landing;
drop it if the landing applies hdr/ by diff. Also dropped this round: a temporary `public:` on C3DDummyObject (r12 proved it inert).
