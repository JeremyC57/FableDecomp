// C3DMeshLODInfo::OnResourceLoaded() -- with file data present, build the mesh; when that fails remove this LOD from the owning mesh info
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A25470, length 48 bytes.
// Callees: 0x00A24520 C3DMeshLODInfo::BuildMesh(bool, bool), 0x00A249A0 C3DMeshInfo::RemoveLOD(unsigned long, bool)
//
#define C3DMeshLODInfo C3DMeshLODInfoPdb
#include "engine/C3DMeshLODInfo.h"
#undef C3DMeshLODInfo   // PDB layout agrees with retail (Mesh +0x1c, FileData +0x24, MeshInfo +0x2c, LOD +0x30)

class CCPPointerInfo { public:                                  // PDB: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
class C3DMeshInfo { public:
private:
    void RemoveLOD(unsigned long lod, bool keepStats);   // 0x00A249A0 ?RemoveLOD@C3DMeshInfo@@ABEXK_N@Z (private; the LOD info is its friend)
    friend class C3DMeshLODInfo;
public:
};

struct C3DMeshLODInfo : C3DMeshLODInfoPdb {
    bool BuildMesh(bool fromFileData, bool stats);       // 0x00A24520
    void OnResourceLoaded();
};
void C3DMeshLODInfo::OnResourceLoaded()
{
    if (FileData_Data != 0) {
        if (!BuildMesh(true, false))
            MeshInfo->RemoveLOD(LOD, false);
    }
}