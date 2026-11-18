# Selected-object relocation review

Instruction alignment links an emitted operand to a retail instruction. Binding evidence is reported for each proposed pair. A parked row still needs byte verification before landing.

| Offset | Emitted symbol | Retail instruction | Target | Evidence |
| --- | --- | --- | --- | --- |
| +0x33 | `?ForceToVel@Mass@@QAE?AVC2DVector@@V2@0@Z` | 0x008EECA2 | 0x008EEB40 | Both PDBs and the native same-TU ForceToVel body |
| +0x65 | `?ForceToVel@Mass@@QAE?AVC2DVector@@V2@0@Z` | 0x008EECD4 | 0x008EEB40 | Both PDBs and the native same-TU ForceToVel body |
| +0x93 | `__real@3f800000` | 0x008EED01 | 0x0122DED8 | IEEE literal payload |
| +0x1F5 | `__real@3f800000` | 0x008EEE63 | 0x0122DED8 | IEEE literal payload |
| +0x220 | `__real@00000000` | 0x008EEE8E | 0x0122DEDC | IEEE literal payload |
| +0x264 | `?ApplyImpulse@Mass@@QAEXVC2DVector@@0@Z` | 0x008EEED3 | 0x008EEA80 | Both PDBs and the native same-TU ApplyImpulse body |
| +0x29F | `?ApplyImpulse@Mass@@QAEXVC2DVector@@0@Z` | 0x008EEF0E | 0x008EEA80 | Both PDBs and the native same-TU ApplyImpulse body |
| +0x35D | `?ForceToVel@Mass@@QAE?AVC2DVector@@V2@0@Z` | 0x008EEFCC | 0x008EEB40 | Both PDBs and the native same-TU ForceToVel body |
| +0x38F | `?ForceToVel@Mass@@QAE?AVC2DVector@@V2@0@Z` | 0x008EEFFE | 0x008EEB40 | Both PDBs and the native same-TU ForceToVel body |
| +0x3BF | `__real@00000000` | 0x008EF02D | 0x0122DEDC | IEEE literal payload |
| +0x41E | `?ApplyImpulse@Mass@@QAEXVC2DVector@@0@Z` | 0x008EF08D | 0x008EEA80 | Both PDBs and the native same-TU ApplyImpulse body |
| +0x45B | `?ApplyImpulse@Mass@@QAEXVC2DVector@@0@Z` | 0x008EF0CA | 0x008EEA80 | Both PDBs and the native same-TU ApplyImpulse body |

12 supported pairs out of 12. See the separate manual audit where supplied.
