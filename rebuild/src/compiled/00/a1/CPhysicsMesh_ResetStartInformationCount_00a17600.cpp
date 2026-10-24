// CPhysicsMesh::ResetStartInformationCount() -- zero the static physics-mesh intersection statistics block
// Leaf: ?ResetStartInformationCount@CPhysicsMesh@@SIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A17600, 17 bytes, plain `ret` (`rep stosd` of 12 dwords at 0x013D2780).
// Identity: Ego_r.pdb / FableWin.pdb name `?ResetStartInformationCount@CPhysicsMesh@@SIXXZ` (static __fastcall, no
//   args), sibling of `?GetInformation@CPhysicsMesh@@SIABVCPhysicsInfomationOutputData@@XZ`; the 48-byte block is
//   CPhysicsInfomationOutputData (PDB layout: bool + ten longs + float = 0x30). Its only caller is
//   CWorld::UpdateFrame (call at 0x004A5CE2), so the catalog label "$E1" (dynamic initializer) is wrong; the
//   function sits in the CPhysicsMesh TU (next to HasBoundingSphereIntersectedFromTo 0x00A17620). The static's own
//   name is not in the PDB name sets: "Information" is a descriptive name.
// Shape: the intrinsic memset (count 0x0C dwords loaded into ecx before the zero eax).
extern "C" void* __cdecl memset(void* dst, int value, unsigned int count);
#pragma intrinsic(memset)

class CPhysicsInfomationOutputData {
public:
    bool UsingSegmentation;                         // +0x00
    long TotalSphereIntersectionCalls;              // +0x04
    long TotalPointIntersectionCalls;               // +0x08
    long TotalSuccessfulSphereBoundingBoxTests;     // +0x0C
    long TotalSuccessfulSphereBoundingSphereTests;  // +0x10
    long TotalSuccessfulSphereIntersectionTests;    // +0x14
    long TotalSuccessfulPointBoundingBoxTests;      // +0x18
    long TotalSuccessfulPointBoundingSphereTests;   // +0x1C
    long TotalSuccessfulPointIntersectionTests;     // +0x20
    long TotalNoSphereTrianglesTested;              // +0x24
    long TotalNoPointTrianglesTested;               // +0x28
    float TotalTime;                                // +0x2C
};

class CPhysicsMesh {
public:
    static void __fastcall ResetStartInformationCount();
    static CPhysicsInfomationOutputData Information;   // 0x013D2780 (descriptive name)
};

void __fastcall CPhysicsMesh::ResetStartInformationCount()
{
    memset(&Information, 0, sizeof(CPhysicsInfomationOutputData));
}