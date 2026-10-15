// CTCHeroPodium::GetCrowdPos(const C3DVector&, C3DVector*) -- claims a crowd slot (at most eight) and hands back its position (always the origin)
// Grade: DIFFER(85v85) -- only the frame size differs: retail `sub esp,10h` (4 dead bytes above the 12-byte zero vector temporary), the lane `sub esp,0Ch`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00771D50, 85 bytes (+11 int3 padding). Single caller 0x00957D86
// (passes the asking thing's position and an out vector).
//
// Shape: false (`xor al,al`) once NumberOfCrowdOccupants reaches 8; otherwise a zero C3DVector temporary is built in
// the 16-byte frame and copied into *pos component by component with integer moves (`mov [esp],0 ; mov edx,[esp] ;
// mov [eax],edx`, a bitwise implicit assignment of a class with a user float constructor), the first occupation slot
// is set to 1, the occupant count bumped in memory (`inc dword ptr [ecx+0Ch]`) and true returned (`mov al,1`).
// Layout: NumberOfCrowdOccupants +0x0C, CrowdOccupation's first pointer +0x10 (PDB layout; retail vector 12 bytes).

struct C3DVector {
    float x, y, z;
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) {}
};

struct CTCHeroPodium_Retail {
    void*         __vftable;                   // +0x00
    unsigned char _pad_0x04[0x8];              // +0x04 CTCBase
    long          NumberOfCrowdOccupants;      // +0x0C
    long*         CrowdOccupation_First;       // +0x10 vector<long>::_First

    bool GetCrowdPos(const C3DVector& from, C3DVector* pos);
};

bool CTCHeroPodium_Retail::GetCrowdPos(const C3DVector& from, C3DVector* pos)
{
    if (NumberOfCrowdOccupants == 8)
        return false;
    *pos = C3DVector(0.0f, 0.0f, 0.0f);
    CrowdOccupation_First[0] = 1;
    ++NumberOfCrowdOccupants;
    return true;
}
