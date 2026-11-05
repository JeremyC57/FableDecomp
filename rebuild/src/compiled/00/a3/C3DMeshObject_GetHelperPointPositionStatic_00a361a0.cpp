// C3DMeshObject::GetHelperPointPositionStatic(const CCharString&,unsigned long) -- transform a named helper's position by the object's matrix.
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail 0x00A361A0, 193 bytes. CRC(seed=0) 0x004014A0; mesh-stat GetHelperPoint 0x00A2E8C0.
// PDB PositionMatrix starts +0x10; mesh stats counted pointer Data is +8. Empty string oracle 0x0129AAF4.
#ifdef _MSC_VER
#include "fable_string.h"
#else
#define __fastcall
class CCharStringData { public: char* text; unsigned int unknown04,unknown08; unsigned char flags0C; int owners; };
class CCharString { public: CCharString(); ~CCharString(); CCharStringData* storage_; };
#endif
// Header exposes the real data record; the one-pointer object view supplies native inline accessors.
struct CCharStringNativeView {
    CCharStringData* PStringData;
    const char* c_str() const { return PStringData == 0 ? "" : PStringData->text; }
    unsigned long length() const { return PStringData == 0 ? 0 : PStringData->unknown04; }
};
#include "engine/C3DVector.h"
class C3DHelperPoint { public: C3DVector Position; long BoneIndex; };
class C3DMeshStats { public: public:
    const C3DHelperPoint* GetHelperPoint(unsigned long, unsigned long) const;   // 0x00A2E8C0
};
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
}; // 0x004014A0
struct CMeshPointer { C3DMeshStats* Data; void* Info; C3DMeshStats* operator->() const { return Data; } };
class C3DMeshObject { public:
    void* __vftable;
    void* MeshInfo;
    CMeshPointer Mesh;
    float PositionMatrix[12];
    C3DVector GetHelperPointPositionStatic(const CCharString&,unsigned long);
};
C3DVector C3DMeshObject::GetHelperPointPositionStatic(const CCharString& name,unsigned long index)
{
    const CCharStringNativeView& text=*reinterpret_cast<const CCharStringNativeView*>(&name);
    const C3DVector& position = Mesh->GetHelperPoint(CCRC::Calc(0,text.c_str(),text.length()),index)->Position;
    C3DVector point(position);
    // Transform the copied point in place, preserving the input coordinates needed later.
    const float x = point.X;
    const float y = point.Y;
    point.X = x*PositionMatrix[0] + y*PositionMatrix[3] + point.Z*PositionMatrix[6] + PositionMatrix[9];
    point.Y = x*PositionMatrix[1] + y*PositionMatrix[4] + point.Z*PositionMatrix[7] + PositionMatrix[10];
    point.Z = x*PositionMatrix[2] + y*PositionMatrix[5] + point.Z*PositionMatrix[8] + PositionMatrix[11];
    return point;
}