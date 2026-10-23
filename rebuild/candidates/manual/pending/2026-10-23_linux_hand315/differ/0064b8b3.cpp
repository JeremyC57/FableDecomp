// Grade: DIFFER(3v1426) -- parked unreached (skeleton only): /Os ebp-frame target panel: button-map icon checks, demon-door face def-name compares (CREATURE_DEMON_DOOR_FACE_01 / NW variant) and target health float scaling (10 x87 ops); not reached beyond decode.
// CDrawTargetInfo::Draw() -- PARK SKELETON (decoded only)
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064B8B3, length 1426 bytes.
// Decode: /Os ebp-frame target panel: button-map icon checks, demon-door face def-name compares (CREATURE_DEMON_DOOR_FACE_01 / NW variant) and target health float scaling (10 x87 ops); not reached beyond decode.
#pragma optimize("s", on)
struct CDrawTargetInfo {
    unsigned char _pad[0x100];
    void Draw(void* context);
};

void CDrawTargetInfo::Draw(void* context)
{
}
