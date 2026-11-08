#ifndef FABLE_ENGINE_C3DMESHVOLUME_H
#define FABLE_ENGINE_C3DMESHVOLUME_H
// C3DMeshVolume (chunk 868, row 0x00A2F740): Ego_r.pdb struct_layouts_egor.tsv -- CCharString Name +0x0,
// CCountedPointer<CContainmentVolume> ConvexVolume +0x4; sizeof 0xC. FableWin.pdb: ??0C3DMeshVolume@@QAE@XZ (retail 0x00A0A3D0, landed),
// ??0C3DMeshVolume@@QAE@ABVC3DMeshFileConvexVolumeChunk@@PAVCMovableResourceMemoryPool@@@Z (retail 0x00A2E9A0, landed: the chunk's name
// copied, a new CContainmentVolume initialised from the chunk's planes), ??0C3DMeshVolume@@QAE@ABV0@@Z / ??4 (the implicit copy and
// assignment: inline in retail, 0x00A2FD45..0x00A2FD6D copies one into a pair in place), ??1C3DMeshVolume@@QAE@XZ (retail 0x00A32690),
// ?GetName@..@@QBE?AVCCharString@@XZ, ?GetConvexVolume@..@@QBEPBVCContainmentVolume@@XZ, the containment queries
// ?IsContainingPoint / ?IsContainingSphere / ?IsContainingPartOfSphere / ?PeekRoughProportionOfSphereContained, ?LoadBinary, ?SaveBinary.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
class CContainmentVolume;
class CMovableResourceMemoryPool;
class CMemoryDataInputStream;
class CDataOutputStream;
class C3DMeshFileConvexVolumeChunk;
class C3DMeshVolume {
public:
    C3DMeshVolume();                                                                             // 0x00A0A3D0
    C3DMeshVolume(const C3DMeshFileConvexVolumeChunk& chunk, CMovableResourceMemoryPool* pool);   // 0x00A2E9A0
    // No user destructor: ??1C3DMeshVolume@@QAE@XZ is the implicit one (retail 0x00A32690 is its unreferenced COMDAT, landed; the
    // callers expand it in place -- 0x00A2FDD1..0x00A2FE00 releases the counted pointer and calls ~CCharString 0x0099EAE0).
    CCharString GetName() const { return Name; }
    const CContainmentVolume* GetConvexVolume() const { return ConvexVolume.Data; }
    bool IsContainingPoint(const C3DVector& point) const;
    bool IsContainingSphere(const C3DVector& center, float radius) const;
    bool IsContainingPartOfSphere(const C3DVector& center, float radius) const;
    float PeekRoughProportionOfSphereContained(const C3DVector& center, float radius) const;
    bool LoadBinary(CMemoryDataInputStream&, CMovableResourceMemoryPool*);
    void SaveBinary(CDataOutputStream&);
private:
    CCharString Name;                                                    // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;                    // +0x04
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshVolume) == 0xC);
#endif // FABLE_ENGINE_C3DMESHVOLUME_H
