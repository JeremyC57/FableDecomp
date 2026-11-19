// CMainGameComponent::CheckSync(const CGameEventPackage* package) -- network sync check: take the world checksum, then for every sync packet (type 1) in the package copy its payload into a read buffer and read the three 4-byte sync words back (the comparison is compiled out in the retail build)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004165E8, length 136 bytes.
// Grade: DIFFER(138v136) -- retail keeps the loop index in esi and re-reads the package argument from [ebp+8] for every call (no `this`/package register); ours caches the package in esi and the index in edi (+2 bytes for the extra push/pop) | tried: const reference, pointer, a one-word handle class by value
// Flags: /GS
// Callees: CMainGameComponent::PeekWorldChecksum @ 0x00416392 (result unused), CEventPackage::GetNoPackets @ 0x009F16E0,
//          CEventPackage::GetPacket(ulong) @ 0x009F16D0, CEventPacket::CopyToBuffer(CReadBuffer*) @ 0x009F17A0,
//          CReadBuffer::Read(ulong, void*) @ 0x009F18F0, __security_check_cookie @ 0x00BFE9F9.
// Shape: /GS (cookie at [ebp-4] next to the buffer's 0x20-byte array); size TU with a frame pointer
// (`optimize("s", on)` + `("y", off)`); the 0x2c-byte read buffer's inline constructor zeroes its length, read
// position and spare word with the shared `xor esi,esi`; the packet count is re-read every iteration.
// Leaf: ?CheckSync@CMainGameComponent@@IAEXPBVCGameEventPackage@@@Z
// NF95 2026-11-19 (q11320): PDB names (Ego_r.pdb 0x004158BE / FableWin.pdb 0x018A8354 ?CheckSync@CMainGameComponent@@IAEXPBVCGameEventPackage@@@Z,
//   protected, the package by const pointer): the read buffer is CGameEventDataPackage (GetData 0x009F18F0, Ego_r.pdb 0x00B454F0), the
//   packet CGameEvent (GetDataPackage 0x009F17A0, Ego_r.pdb 0x00B453F0; Type +0x00) and the package CGameEventPackage (Ego_r.pdb
//   GetEvent 0x00B45330 / GetNoEvents 0x00B45340, in that order = retail 0x009F16D0 / 0x009F16E0). Were the stand-ins CReadBuffer::Read,
//   CEventPacket::CopyToBuffer, CEventPackage::GetPacket / GetNoPackets and a const CEventPackage& parameter.
// NF97 2026-11-19 (port): the buffer, event and package are the shared headers engine/CGameEventDataPackage.h
//   (FABLE_RETAIL_CGAMEEVENTDATAPACKAGE), engine/CGameEvent.h (FABLE_RETAIL_CGAMEEVENT_DATAPACKAGE) and engine/CGameEventPackage.h
//   (FABLE_RETAIL_CGAMEEVENTPACKAGE + _GETEVENTS), no longer local models.
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#define FABLE_RETAIL_CGAMEEVENT_DATAPACKAGE
#define FABLE_RETAIL_CGAMEEVENTPACKAGE
#define FABLE_RETAIL_CGAMEEVENTPACKAGE_GETEVENTS
#pragma optimize("s", on)
#pragma optimize("y", off)
#include "engine/CGameEventDataPackage.h"
#include "engine/CGameEventPackage.h"
class CMainGameComponent { public:
    unsigned long PeekWorldChecksum() const;             // 0x00416392
protected:
    void CheckSync(const CGameEventPackage* package);
};

void CMainGameComponent::CheckSync(const CGameEventPackage* package)
{
    CGameEventDataPackage buffer;
    PeekWorldChecksum();
    for (unsigned long i = 0; i < package->GetNoEvents(); ++i)
    {
        const CGameEvent* packet = package->GetEvent(i);
        if (packet->Type == 1)
        {
            unsigned long worldChecksum, frame, random;
            packet->GetDataPackage(&buffer);
            buffer.GetData(4, &worldChecksum);
            buffer.GetData(4, &frame);
            buffer.GetData(4, &random);
        }
    }
}