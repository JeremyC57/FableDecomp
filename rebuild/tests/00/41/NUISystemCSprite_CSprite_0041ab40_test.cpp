// Behaviour test for NUISystem::CSprite::~CSprite @ 0x0041AB40: the sprite clears its engine primitive handle (the primitive is
// told it was released when this handle held the last reference, then the reference is dropped and the primitive deleted at
// zero), and the texture counted pointer releases its data when its count reaches zero.
#include <stdio.h>
static int g_notified = 0, g_deleted = 0, g_textureFreed = 0, g_baseDtor = 0;
class CCPPointerInfo { public:
    unsigned int RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T* Data;
    CCPPointerInfo* Info;
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
};
class CBaseClass { public: virtual ~CBaseClass() { } };
class CEnginePrimitiveHandle;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() const { g_deleted++; }
    mutable unsigned int IVRefCount;
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase { public:
    virtual void NotifyReleased(const CEnginePrimitiveHandle&) { g_notified++; }
};
template <class T> class CIVCountedPointer { public:
    CIVCountedPointeeBase* Object;
    T* operator->() const { return static_cast<T*>(Object); }
    unsigned int GetRefCount() const { return Object ? Object->IVRefCount : 0; }
    void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    void Reset(T* object) { Release(); Object = object; if (object) ++object->IVRefCount; }
    ~CIVCountedPointer() { Release(); }
};
class CEnginePrimitiveHandle : public CBaseClass { public:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
    unsigned int GetReferenceCount() const { return Data.GetRefCount(); }
    void Clear()
    {
        if (GetReferenceCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
    virtual ~CEnginePrimitiveHandle() { Clear(); }
};
class CTexture;
namespace NUISystem {
class CChangingStateComponent { public: virtual ~CChangingStateComponent() { g_baseDtor++; } };
class CSprite : public CChangingStateComponent { public:
    CEnginePrimitiveHandle    Handle;
    float                     Angle;
    CCountedPointer<CTexture> PTexture;
    virtual ~CSprite();
};
CSprite::~CSprite()
{
    Handle.Clear();
}
}   // namespace NUISystem
static void FreeTexture(void*) { g_textureFreed++; }
static int fail(int line) { printf("0041ab40_TEST FAIL\n"); printf("line %d\n", line); return 1; }
int main()
{
    CEnginePersistentPrimitiveBase primitive;
    primitive.IVRefCount = 1;
    CCPPointerInfo* info = new CCPPointerInfo;
    info->RefCount = 1; info->DeleteFunc = FreeTexture; info->Data = 0;
    NUISystem::CSprite* sprite = new NUISystem::CSprite;
    sprite->Handle.Data.Object = &primitive;
    sprite->PTexture.Data = 0; sprite->PTexture.Info = info;
    delete sprite;
    if (g_notified != 1 || g_deleted != 1 || primitive.IVRefCount != 0 || g_textureFreed != 1 || g_baseDtor != 1)
        return fail(__LINE__);
    // shared primitive and texture: no notification, no delete, counts drop by one
    primitive.IVRefCount = 3;
    CCPPointerInfo shared; shared.RefCount = 2; shared.DeleteFunc = FreeTexture; shared.Data = 0;
    sprite = new NUISystem::CSprite;
    sprite->Handle.Data.Object = &primitive;
    sprite->PTexture.Data = 0; sprite->PTexture.Info = &shared;
    delete sprite;
    if (g_notified != 1 || g_deleted != 1 || primitive.IVRefCount != 2 || shared.RefCount != 1 || g_textureFreed != 1)
        return fail(__LINE__);
    printf("0041ab40_TEST PASS\n");
    return 0;
}