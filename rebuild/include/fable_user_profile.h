#pragma once

#include "fable_string.h"

class CUserProfileManager
{
public:
    CUserProfileManager();

    __forceinline static void* operator new(unsigned int size)
    {
        return ::operator new(size);           // 0x00BFEA1A MSVCR71 operator new
    }

    void SetProfileName(
        const CWideString& profileName,
        bool loadProfile);

private:
    void Load();
    void Save();

    void* primaryVTable_;
    void* secondaryVTable_;
    CWideString profileName_;
    fable_u8 opaque0C_[0x134];
};

struct CCountedUserProfileManager
{
    CUserProfileManager* object;
    void* reference;

    void operator=(CUserProfileManager* value);
};

struct CUserProfileManagerSingleton
{
    static CUserProfileManager& FABLE_FASTCALL Get();
};

extern CCountedUserProfileManager
    g_CUserProfileManagerSingleton_013B7D4C;

FABLE_STATIC_ASSERT(sizeof(CUserProfileManager) == 0x140);
FABLE_STATIC_ASSERT(sizeof(CCountedUserProfileManager) == 0x08);
