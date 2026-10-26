// CWorld::HandleLeaveWorldPacket(CEventPacket* packet) -- network/game event handler: unpack a one-byte flag from the event packet, then either auto-load the region being left or reset to the front end
// Grade: DIFFER(110v110) -- frame only: retail homes the one-byte flag in the dead packet-argument slot ([esp+38h], 0x30 frame), ours gives it its own frame byte (0x34 frame, every offset +4); same instructions otherwise. Tried: function-scope and block-scope flag local. Lane gap: short-lived local in a dead argument slot.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006294A0, length 110 bytes (`ret 4`, /GS).
// Label correction: the catalog's "CRenderManager2D::AddLineToDrawBuffer" is propagated. `this` is a CWorld (both
// callees are CWorld members) and the only reference is the static event-handler table initialiser at 0x0121BD2E
// (`mov eax,6294A0h ; mov [13B9488h],eax`); the method name is descriptive.
// Callees: CEventPacket::CopyToBuffer(SPacketBuffer*) @ 0x009F17A0 (landed), the packet reader ReadBytes(count, dest)
// @ 0x009F18F0 (as in the landed CGameEvent::ReadData8 0x00628E80), CWorld::AutoLoadRegionLeaving @ 0x004A5E90,
// CWorld::ResetToFrontEnd @ 0x0049DCC0; __security_check_cookie @ 0x00BFE9F9 (cookie 0x0139C8A8).
// Shape: the 0x2c-byte packet buffer is a local whose inline constructor clears the length and the two cursor words
// (+0x20/+0x24/+0x28, one zero register); the flag byte is homed in the dead packet-argument slot and compared with 1
// (`cmp byte ptr [esp+38h],1`), each arm ending in its own cookie check.
// Flags: /GS
// Leaf: HandleLeaveWorldPacket
struct SPacketBuffer {
    unsigned char m_payload[0x20];                 // +0x00
    long          m_length;                        // +0x20
    long          m_a;                             // +0x24
    long          m_b;                             // +0x28
    SPacketBuffer() : m_length(0), m_a(0), m_b(0) {}
    void ReadBytes(long count, void* destination); // 0x009F18F0
};

struct CEventPacket {
    long CopyToBuffer(SPacketBuffer* buffer);      // 0x009F17A0
};

class CWorld { public:
    bool AutoLoadRegionLeaving();                  // 0x004A5E90
    void ResetToFrontEnd();                        // 0x0049DCC0
    void HandleLeaveWorldPacket(CEventPacket* packet);
};

void CWorld::HandleLeaveWorldPacket(CEventPacket* packet)
{
    SPacketBuffer buffer;
    packet->CopyToBuffer(&buffer);
    bool autoLoad;
    buffer.ReadBytes(1, &autoLoad);
    if (autoLoad == true)
        AutoLoadRegionLeaving();
    else
        ResetToFrontEnd();
}