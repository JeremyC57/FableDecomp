#ifndef FABLE_ENGINE_TCONSTRAINTTYPE_H
#define FABLE_ENGINE_TCONSTRAINTTYPE_H
// TConstraintType (FableWin TPI; ?IsControlType@CParticleProgram@@AAE_NW4TConstraintType@@@Z in both PDBs): the record type of
// each CParticleProgram::Constraints block (0x00AE0070 compares the header word with 2 and 3).
enum TConstraintType { CONSTRAINT_REPEAT=0, CONSTRAINT_END_REPEAT=1, CONSTRAINT_DISTANCE=2, CONSTRAINT_MIN_MAX_DISTANCE=3,
    CONSTRAINT_UNBEND=4, CONSTRAINT_SPHERE_COLLISION=5, CONSTRAINT_CYLINDER_COLLISION=6 };
#endif
