// IsCameraModeInterruptable(CCountedPointer<const NCameraModes::CBase> mode) -- false for camera modes of type 0x0A, 0x0B or 0x0F; true otherwise (also for no mode)
// Leaf: ?IsCameraModeInterruptable@?A0x
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members m_count->RefCount, m_pItem->DeleteFunc are the header CCPPointerInfo's (same offsets).
// (Leaf is a prefix: the anonymous-namespace hash varies with the file path; it excludes the Call... wrapper.)
//
// Retail 0x00697A50, 95 bytes (+ int3 padding to 96), `ret 8` (__fastcall TU: the 8-byte counted pointer is a class
//   passed by value, so it goes on the stack and the callee cleans it).
// Callees: the mode's virtual slot 2 (+0x08, its type); on the last release the pointer-info's destroy member
//   pointer (+4, called on the item +8) and operator delete @ 0x00BFE9BC (the import thunk) on the info block.
// Callers: 0x0069AC4F, 0x0069B670 (the camera-manager mode switches).
// Shape: `if (mode.Get()) { type = mode->GetType(); if (type == 0x0B || type == 0x0A || type == 0x0F) return false; }
//   return true;` -- the by-value parameter's destructor runs at each return, so retail carries two copies of the
//   `if (Info && --Info->m_count == 0) { (Info->m_pItem->*Info->m_pfnDestroy)(); delete Info; }` release.
// Name: Ego_r.pdb `?IsCameraModeInterruptable@?A0xa59f4854@@YI_NV?$CCountedPointer@$$CBVCBase@NCameraModes@@@@@Z`
//   (a file-static of the camera-manager TU).  The type values are the camera-mode enum (names not recovered).
// Model: CCPPointerInfo / CCountedPointer as the landed 0x008C2550 (the destroy is a member pointer on CCountedItem).
namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    virtual void v01();
    virtual long GetType() const;                           // slot 2 (+0x08)
};
}

class CCountedItem { };
#include "engine/CCountedPointer.h"


namespace {
bool __fastcall IsCameraModeInterruptable(CCountedPointer<const NCameraModes::CBase> mode)
{
    if (mode.Get()) {
        long type = mode->GetType();
        if (type == 0x0B || type == 0x0A || type == 0x0F)
            return false;
    }
    return true;
}
}

bool __fastcall CallIsCameraModeInterruptable(const CCountedPointer<const NCameraModes::CBase>& mode)
{
    return IsCameraModeInterruptable(mode);
}