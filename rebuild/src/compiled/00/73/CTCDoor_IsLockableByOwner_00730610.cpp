#include "engine/CTCDoor.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefPointeeBase {
    char pad[0x3a];
    unsigned char lockedFlag;
};
bool __fastcall CTCDoor_IsLockableByOwner(CTCDoor* door) {
    CDefPointeeBase* o = door->PDef_Object;
    if (o != 0)
        return o->lockedFlag == 0;
    return true;
}