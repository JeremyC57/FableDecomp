// CPlayerManager::GetMultiplayerColour(long player) const -- the fixed colour of multiplayer slot 0..3 (red, blue, yellow, green), white for any other slot
// Leaf: GetMultiplayerColour
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449B60, 124 bytes + the 4-entry jump table (0x00449BDC), `ret 8`.
// PDB: ?GetMultiplayerColour@CPlayerManager@@QBE?AVCRGBColour@@J@Z (const member, CRGBColour returned through the
// hidden result pointer at [esp+4]; `this` is never read).
// Each arm builds the colour with the inline (r, g, b, a) constructor straight into the result (CRGBColour = { uchar
// B, G, R, A }, D3DCOLOR byte order); VC keeps 0 in dl and 0xFF in cl and orders the byte stores per arm.
// Layout: `this` is the engine header CPlayerManager (unused); the member function is declared on a header-derived
// view because the generated header is data-only.
#include "engine/CPlayerManager.h"

class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

struct CPlayerManager_Methods : CPlayerManager {
    CRGBColour GetMultiplayerColour(long player) const;
};

CRGBColour CPlayerManager_Methods::GetMultiplayerColour(long player) const
{
    switch (player) {
    case 0: return CRGBColour(255, 0, 0, 255);
    case 1: return CRGBColour(0, 0, 255, 255);
    case 2: return CRGBColour(255, 255, 0, 255);
    case 3: return CRGBColour(0, 255, 0, 255);
    }
    return CRGBColour(255, 255, 255, 255);
}
