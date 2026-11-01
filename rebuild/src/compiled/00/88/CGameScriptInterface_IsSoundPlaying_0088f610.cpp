class CGameScriptInterface
{
public:
    bool __fastcall IsSoundPlaying(unsigned long soundId) const;
};

struct CGameScriptInterface_SoundProvider
{
    void** vtable;
};

class CASoundManager;
extern CASoundManager* PSoundManager;

typedef bool (__fastcall *FnIsSoundPlaying)(CGameScriptInterface_SoundProvider*, unsigned long);

bool __fastcall CGameScriptInterface::IsSoundPlaying(unsigned long soundId) const
{
    return ((FnIsSoundPlaying)((CGameScriptInterface_SoundProvider*)PSoundManager)->vtable[15])(((CGameScriptInterface_SoundProvider*)PSoundManager), soundId);
}