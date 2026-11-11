# NF22 header opt-ins

Each file here is a full copy of the current repo header (rebuild/include/...) with `#ifdef` additions only (no line removed).
Rows that use them define the macro before the include; grade with `FABLE_HDR_OVERLAY=$S/w22/agentNF22/hdr`.

| Header | Macro | What it adds | Rows |
|---|---|---|---|
| fable_string.h | `FABLE_RETAIL_CWIDESTRING_NOSTRINGS` | `protected: static long NoStrings;` in CWideString's default branch: `?NoStrings@CWideString@@1JA` (0x013BCA20), both PDBs | 0099b510 0099b6b0 0099b720 (+ tests 0099b6b0 / 0099b720); the other NoStrings binders keep their existing local CWideString models, which gain the same protected static |
| fable_string.h | `FABLE_RETAIL_CCHARSTRINGDATA_FRIEND` | `friend class CCharString;` in the retail CCharStringData: NoRefs is protected (Ego_r TPI), yet operator= 0x0099EFB0 / the copy constructor 0x0099EC30 bump it in place | 0099efb0 |
| engine/CTCFlammable.h | `FABLE_RETAIL_CTCFLAMMABLE_FIREPOINTS` | Ego_r members `PDef` +0x0C and `std::vector<NFire::CFirePoint> FirePoints` +0x10 (retail STLport, 12 bytes) | 00780410 |
| engine/CParticleSystem.h | `FABLE_RETAIL_CPARTICLESYSTEM_PEEKEMITTERTRANSFORM` | `private: const CMatrix3x4& PeekEmitterTransform() const;` (`?PeekEmitterTransform@CParticleSystem@NParticleEngine@@ABEABVCMatrix3x4@@XZ`, 0x00AF3D80, both PDBs) | 00af3d80 |
| engine/CParticleEmitter.h | `FABLE_RETAIL_CPARTICLESYSTEM_PEEKEMITTERTRANSFORM` | `friend class CParticleSystem;` (retail branch): PeekEmitterTransform / PeekEmitterVelocity read the protected Transform / Velocity off the parent emitter, and neither PDB names an emitter accessor | 00af3d80 |
| engine/CPlayer.h | `FABLE_RETAIL_PLAYER_MODES_LIST` | `PlayerModes` +0x210 typed `std::list<EPlayerMode>` (its Ego_r type; retail STLport list, 4 bytes) instead of `unsigned char[4]`; includes EPlayerMode.h + stlport/fable_containers.h | 00633be0 |
| engine/CCombatManager.h | `FABLE_RETAIL_CCOMBATMANAGER_PERFORMSEQUENCERESET` | `private: void PerformSequenceReset(CThingCreatureBase&);` (`?PerformSequenceReset@CCombatManager@@AAEXAAVCThingCreatureBase@@@Z`, 0x006EA2C0, both PDBs) | 006ea430 |

Header debt noticed (not changed): engine/CTCFlammable.h does not compile on its own (it names ETCInterfaceType / TCI_FLAMMABLE
without including engine/ETCInterfaceType.h; row 0x00780410 includes it first).
