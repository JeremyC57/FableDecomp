// CWorld::EAPlayerRespawn(const CGameEvent* event) -- game-event action: unpack a one-byte flag from the event packet, then either auto-load the respawn save or reset to the front end
// Grade: DIFFER(110v110) -- frame only: retail homes the one-byte flag in the dead packet-argument slot ([esp+38h], 0x30 frame), ours gives it its own frame byte (0x34 frame, every offset +4); same instructions otherwise. Tried: function-scope and block-scope flag local. Lane gap: short-lived local in a dead argument slot.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006294A0, length 110 bytes (`ret 4`, /GS).
// Label correction: the catalog's "CRenderManager2D::AddLineToDrawBuffer" is propagated. `this` is a CWorld (both
// callees are CWorld members) and the only reference is the static event-handler table initialiser at 0x0121BD2E
// (`mov eax,6294A0h ; mov [13B9488h],eax`); the method name is descriptive.
// Callees: CEventPacket::CopyToBuffer(SPacketBuffer*) @ 0x009F17A0 (landed), the packet reader ReadBytes(count, dest)
// @ 0x009F18F0 (as in the landed CGameEvent::ReadData8 0x00628E80), CWorld::AutoLoadRespawn @ 0x004A5E90,
// CWorld::ResetToFrontEnd @ 0x0049DCC0; __security_check_cookie @ 0x00BFE9F9 (cookie 0x0139C8A8).
// Shape: the 0x2c-byte packet buffer is a local whose inline constructor clears the length and the two cursor words
// (+0x20/+0x24/+0x28, one zero register); the flag byte is homed in the dead packet-argument slot and compared with 1
// (`cmp byte ptr [esp+38h],1`), each arm ending in its own cookie check.
// Flags: /GS
// Leaf: ?EAPlayerRespawn@CWorld@@QAEXPBVCGameEvent@@@Z
// NF95 2026-11-19 (q11320): the name is Ego_r.pdb 0x0066D810 / FableWin.pdb 0x023DA740 ?EAPlayerRespawn@CWorld@@QAEXPBVCGameEvent@@@Z; the
//   landed event-table initialiser 0x0121B9B0 binds it (slot 8 GAME_EVENT_PLAYER_RESPAWN). The buffer is CGameEventDataPackage (Ego_r.pdb;
//   GetData 0x009F18F0) filled by CGameEvent::GetDataPackage 0x009F17A0 (Ego_r.pdb 0x00B453F0). Was the descriptive
//   HandleLeaveWorldPacket(CEventPacket*) on the stand-ins SPacketBuffer::ReadBytes / CEventPacket::CopyToBuffer.
// NF97 2026-11-19 (port): the buffer and the event are the shared headers engine/CGameEventDataPackage.h
//   (FABLE_RETAIL_CGAMEEVENTDATAPACKAGE) and engine/CGameEvent.h (FABLE_RETAIL_CGAMEEVENT_DATAPACKAGE), no longer local models.
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#define FABLE_RETAIL_CGAMEEVENT_DATAPACKAGE
#include "engine/CGameEventDataPackage.h"
#include "engine/CGameEvent.h"

class CWorld { public:
    bool AutoLoadRespawn();                  // 0x004A5E90
    void ResetToFrontEnd();                        // 0x0049DCC0
    void EAPlayerRespawn(const CGameEvent* event);
};

void CWorld::EAPlayerRespawn(const CGameEvent* event)
{
    CGameEventDataPackage buffer;
    event->GetDataPackage(&buffer);
    bool autoLoad;
    buffer.GetData(1, &autoLoad);
    if (autoLoad == true)
        AutoLoadRespawn();
    else
        ResetToFrontEnd();
}