// Behaviour test for the implicit ~CCreatureAction_DragonFireBlastBase (0x009536D0): the Line1 handle notifies its primitive
// when it holds the last handle reference and drops it (deleting it on the last count), then PShot, then the combat-action base.
#include <stdio.h>
static int g_notified = 0, g_deletedPrims = 0, g_shotDtors = 0, g_baseDtors = 0;
class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase { public:
    unsigned long IVRefCount;
    CEnginePersistentPrimitiveBase() : IVRefCount(0) { }
    virtual ~CEnginePersistentPrimitiveBase() { }
    virtual void Delete() const { g_deletedPrims++; }
    virtual void NotifyReleased(const CEnginePrimitiveHandle&) { g_notified++; }
};
class CEnginePrimitiveHandle { public:
    CEnginePersistentPrimitiveBase* Data;
    CEnginePrimitiveHandle() : Data(0) { }
    unsigned long GetRefCount() const { return Data ? Data->IVRefCount : 0; }
    void Reset(CEnginePersistentPrimitiveBase* object)
    {
        CEnginePersistentPrimitiveBase* old = Data;
        if (old) { if (--old->IVRefCount == 0) old->Delete(); Data = 0; }
        Data = object;
        if (object) ++object->IVRefCount;
    }
    void Clear() { if (GetRefCount() == 1) Data->NotifyReleased(*this); Reset(0); }
    virtual ~CEnginePrimitiveHandle() { Clear(); }
};
class CShotPointer { public: virtual ~CShotPointer() { g_shotDtors++; } };
class CCombatActionBase { public: virtual ~CCombatActionBase() { g_baseDtors++; } };
class CCreatureAction_DragonFireBlastBase : public CCombatActionBase { public:
    CShotPointer PShot;
    CEnginePrimitiveHandle Line1;
};
int main()
{
    CEnginePersistentPrimitiveBase prim;
    {
        CCreatureAction_DragonFireBlastBase action;
        action.Line1.Reset(&prim);
    }
    if (g_notified != 1 || g_deletedPrims != 1 || prim.IVRefCount != 0 || g_shotDtors != 1 || g_baseDtors != 1)
        { printf("009536d0_TEST FAIL\n"); return 1; }
    {
        CCreatureAction_DragonFireBlastBase action;      // a primitive another handle still holds: no notify, no delete
        action.Line1.Reset(&prim);
        ++prim.IVRefCount;
    }
    if (g_notified != 1 || g_deletedPrims != 1 || prim.IVRefCount != 1 || g_shotDtors != 2 || g_baseDtors != 2)
        { printf("009536d0_TEST FAIL\n"); return 1; }
    {
        CCreatureAction_DragonFireBlastBase action;      // no line: nothing to release
    }
    if (g_notified != 1 || g_deletedPrims != 1 || g_shotDtors != 3 || g_baseDtors != 3) { printf("009536d0_TEST FAIL\n"); return 1; }
    printf("009536d0_TEST PASS\n");
    return 0;
}