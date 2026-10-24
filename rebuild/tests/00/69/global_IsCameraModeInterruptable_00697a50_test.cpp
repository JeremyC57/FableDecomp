// Behaviour test for 0x00697A50 IsCameraModeInterruptable: modes of type 0x0A/0x0B/0x0F are not interruptable,
// every other type and an empty pointer are; the by-value counted pointer drops its reference on return and the
// last reference destroys the mode and frees the info block.
#include <cstdio>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif

namespace NCameraModes {
class CBase {
public:
    long Type;
    explicit CBase(long type) : Type(type) {}
    virtual ~CBase() {}
    virtual long GetType() const { return Type; }
};
}

static int g_destroyed = 0;
class CCountedItem { public: void Destroy() { ++g_destroyed; } };
class CCPPointerInfo {
public:
    long          m_count;
    void (CCountedItem::*m_pfnDestroy)();
    CCountedItem* m_pItem;
};

template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data, CCPPointerInfo* info) : Data(data), Info(info) {}
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

static bool __fastcall IsCameraModeInterruptable(CCountedPointer<const NCameraModes::CBase> mode)
{
    if (mode.Get()) {
        long type = mode->GetType();
        if (type == 0x0B || type == 0x0A || type == 0x0F)
            return false;
    }
    return true;
}

static bool Check(long type, bool expected)
{
    NCameraModes::CBase mode(type);
    CCountedItem item;
    CCPPointerInfo* info = (CCPPointerInfo*)operator new(sizeof(CCPPointerInfo));
    info->m_count = 1;
    info->m_pfnDestroy = &CCountedItem::Destroy;
    info->m_pItem = &item;
    int before = g_destroyed;
    bool ok;
    {
        CCountedPointer<const NCameraModes::CBase> held(&mode, info);
        bool result = IsCameraModeInterruptable(held);
        // (the by-value copy is gone once the call statement ends, on either ABI)
        ok = result == expected && info->m_count == 1 && g_destroyed == before;
    }
    return ok && g_destroyed == before + 1;
}

int main()
{
    if (!Check(0x0A, false) || !Check(0x0B, false) || !Check(0x0F, false)) { std::printf("00697a50_TEST FAIL\n"); return 1; }
    if (!Check(0x00, true) || !Check(0x0C, true) || !Check(0x10, true) || !Check(0x09, true)) { std::printf("00697a50_TEST FAIL\n"); return 1; }
    CCountedPointer<const NCameraModes::CBase> none(0, 0);
    if (!IsCameraModeInterruptable(none)) { std::printf("00697a50_TEST FAIL\n"); return 1; }
    std::printf("00697a50_TEST PASS\n");
    return 0;
}