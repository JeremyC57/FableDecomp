// IsCameraModeInterruptable(CCountedPointer<const NCameraModes::CBase> mode) -- false for camera modes of type 0x0A, 0x0B or 0x0F; true otherwise (also for no mode)
// Leaf: ?IsCameraModeInterruptable@?A0x
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
class CCPPointerInfo {
public:
    long          m_count;                                  // +0x00
    void (CCountedItem::*m_pfnDestroy)();                   // +0x04
    CCountedItem* m_pItem;                                  // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->m_count == 0) {
                (Info->m_pItem->*Info->m_pfnDestroy)();
                operator delete(Info);
            }
        }
    }
    T* Get() const { return Data; }
    T* operator->() const { return Data; }
};

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