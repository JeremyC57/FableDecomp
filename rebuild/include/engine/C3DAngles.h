#ifndef FABLE_ENGINE_C3DANGLES_H
#define FABLE_ENGINE_C3DANGLES_H
// Both original PDBs: class, twelve bytes, three named public floats.
#ifdef FABLE_RETAIL_C3DANGLES_CTOR
// Chunk 771 (row 0x006A5D90): Ego_r.pdb / FableWin.pdb ??0C3DAngles@@QAE@MMM@Z, inline in retail (the AngleXY / AngleYZ /
// AngleXZ temporary built in place before CRightHandedSet(const C3DAngles&) 0x00A229E0).
class C3DAngles {public:float AngleXY,AngleYZ,AngleXZ;C3DAngles(){}
    C3DAngles(float angleXY,float angleYZ,float angleXZ):AngleXY(angleXY),AngleYZ(angleYZ),AngleXZ(angleXZ){}};
#else
class C3DAngles {public:float AngleXY,AngleYZ,AngleXZ;C3DAngles(){}};
#endif
#endif
