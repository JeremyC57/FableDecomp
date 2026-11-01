// C3DMeshVolume::~C3DMeshVolume() -- implicit destructor: release the counted ConvexVolume (+0x04), then the Name string by tail jump
// Leaf: ??1C3DMeshVolume@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A32690, 60 bytes, thiscall, `jmp` tail; no call or address reference (an unreferenced
//   COMDAT kept by the linker, no /OPT:REF).
// Identity: the C3DMeshFile / C3DMeshStats TU whose landed rows are C3DMeshVolume's own implicit members and containers:
//   0x00A325E0 (an unreferenced {CCharString(); Data = Info = 0} constructor: C3DMeshVolume's implicit default ctor),
//   this row, ~pair<long, C3DMeshVolume> 0x00A326D0 (inlines this same sequence at +4), pair<unsigned long,
//   C3DMeshVolume>() 0x00A327E0 and C3DMeshVolume::operator= 0x00A32960 (Ego_r.pdb ??4C3DMeshVolume@@QAEAAV0@ABV0@@Z).
//   Ego_r.pdb C3DMeshVolume (12 bytes): CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4;
//   FableWin.pdb names ??1C3DMeshVolume@@QAE@XZ. A COMDAT is kept once per image, so this copy (the one in the class's
//   own TU, next to its constructor) is the only ~C3DMeshVolume; the identical bodies in the CMeshBankManager TU
//   (0x00A0A3F0 / 0x00A0A5D0) are CMeshDataBankInfo's. The manifest label (_Dest_val<..C3DSkeletalMorphTarget..>) is
//   propagated and wrong.
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete 0x00BFE9BC (the block),
//   CCharString::~CCharString 0x0099EAE0 (tail).
// Layout: no engine/C3DMeshVolume.h; the Ego_r.pdb layout over engine/CCountedPointer.h.
#include "engine/CCountedPointer.h"

class CCharStringData;
class CCharString {
public:
    ~CCharString();                                             // 0x0099EAE0
    CCharStringData* PStringData;
};

class CContainmentVolume;

class C3DMeshVolume {                                           // Ego_r.pdb, 12 bytes
public:
    CCharString                         Name;                   // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;           // +0x04
};

void EmitMeshVolumeDestructor(C3DMeshVolume* volume)            // emits the implicit ??1; not a retail function
{
    volume->C3DMeshVolume::~C3DMeshVolume();
}