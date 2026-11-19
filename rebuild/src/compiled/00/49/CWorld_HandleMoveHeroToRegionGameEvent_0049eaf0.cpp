// CWorld::HandleMoveHeroToRegionGameEvent(const CGameEvent&) -- unpack a region-move request (start position, start angle, three flags) from the game event and queue the region load
// Leaf: ?HandleMoveHeroToRegionGameEvent@CWorld@@QAEXABVCGameEvent@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049EAF0, 252 bytes (256 with padding), `ret 4`, /GS (cookie 0x0139C8A8).
//   PDB ?HandleMoveHeroToRegionGameEvent@CWorld@@IAEXPBVCGameEvent@@@Z.
// Callees: CGameEvent::GetData(CMemoryBuffer&) 0x009F17A0 (label "CEventPacket::CopyToBuffer"), CMemoryBuffer::Read
//   (unsigned long, void*) 0x009F18F0 (label "CircularBuffer_Read"), CWorld::SetAsLoadingRegion(const C3DVector&,
//   float, bool, bool, bool) 0x0049E2C0 (landed), __security_check_cookie 0x00BFE9F9.
// Shape: the landed CWorld::EAMoveCreature event idiom: a 44-byte CMemoryBuffer local (inline ctor zeroes its three
//   words), the payload read field by field (X, Y, Z, angle, three flag bytes; the last byte lands in the dead `event`
//   argument slot under the RTM compiler), the flags are char bytes normalised with `!= 0` (setne) for the bool
//   parameters, the position is a named C3DVector built after the reads. Flag names follow SetAsLoadingRegion.
// NF95 2026-11-19 (q11320): the unpacking buffer is Ego_r.pdb CGameEventDataPackage (engine/CGameEventDataPackage.h layout, 0x2C bytes)
//   read through ?GetData@CGameEventDataPackage@@QAEXJPAX@Z 0x009F18F0 (Ego_r.pdb 0x00B454F0 / FableWin.pdb 0x030A2200) and filled by
//   ?GetDataPackage@CGameEvent@@QBEXPAVCGameEventDataPackage@@@Z 0x009F17A0 (Ego_r.pdb 0x00B453F0); were the stand-ins
//   CMemoryBuffer::Read(unsigned long, void*) and CGameEvent::GetData(CMemoryBuffer&).
// NF97 2026-11-19 (port): the read buffer is the shared engine/CGameEventDataPackage.h class (FABLE_RETAIL_CGAMEEVENTDATAPACKAGE:
//   inline ctor zeroing Length / StartPos / EndPos, GetData 0x009F18F0), no longer a local model.
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#include "engine/CGameEventDataPackage.h"
class CGameEvent { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    void GetDataPackage(CGameEventDataPackage* package) const;     // 0x009F17A0
};
#include "engine/C3DVector.h"
class CWorld { public:
    void SetAsLoadingRegion(const C3DVector& pos, float angleXY, bool viaTeleporter, bool force, bool viaDoor);   // 0x0049E2C0
    void HandleMoveHeroToRegionGameEvent(const CGameEvent& event);
};
void CWorld::HandleMoveHeroToRegionGameEvent(const CGameEvent& event)
{
    CGameEventDataPackage buffer;
    event.GetDataPackage(&buffer);
    float x;
    buffer.GetData(4, &x);
    float y;
    buffer.GetData(4, &y);
    float z;
    buffer.GetData(4, &z);
    float angleXY;
    buffer.GetData(4, &angleXY);
    char viaTeleporter;
    buffer.GetData(1, &viaTeleporter);
    char force;
    buffer.GetData(1, &force);
    char viaDoor;
    buffer.GetData(1, &viaDoor);
    C3DVector pos(x, y, z);
    SetAsLoadingRegion(pos, angleXY, viaTeleporter != 0, force != 0, viaDoor != 0);
}