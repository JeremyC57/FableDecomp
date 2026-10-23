// CGameScriptInterface::SetDeathRecoveryMarkerName
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCharString
{
    CCharString& operator=(const CCharString& other);
};
struct DeathRecoveryMarkerNameInterface
{
    char pad00[0x170];
    CCharString markerName;
    void SetMarkerName(const CCharString& n) { markerName = n; }
};
struct DeathRecoveryMarkerNameNode
{
    int key;
    DeathRecoveryMarkerNameInterface* value;
};

struct DeathRecoveryMarkerNameMap
{
    DeathRecoveryMarkerNameNode* begin;
    DeathRecoveryMarkerNameNode* end;

    DeathRecoveryMarkerNameNode* lower_bound(const int* key);
    __forceinline DeathRecoveryMarkerNameInterface* Find(int kind)   // the inlined map find: end when the key is absent
    {
        int key = kind;
        DeathRecoveryMarkerNameNode* found = lower_bound(&key);
        if (found == end || found->key > kind)
            found = end;
        return found->value;
    }
};

struct CPlayerManager
{
    void* GetTarget();
};

template <class T> __forceinline bool GetTCOut(unsigned char* thing, int kind, T** out)
{
    if (*(unsigned char*)(thing + 0x20) & 0x10) {
        *out = ((DeathRecoveryMarkerNameMap*)(thing + 0x44))->Find(kind);
        return true;
    }
    return false;
}

extern "C" void* __fastcall ResolveDeathRecoveryMarkerTarget(void* source);

void __fastcall CGameScriptInterface_SetDeathRecoveryMarkerName(
    CGameScriptInterface* self,
    unsigned long,
    const CCharString& name)
{
    void* source = self->PlayerManager->GetTarget();
    unsigned char* target =
        (unsigned char*)ResolveDeathRecoveryMarkerTarget(source);

    if (target == 0)
        return;
    if (*(unsigned char*)(target + 0x91) & 0x01)
        return;
    DeathRecoveryMarkerNameInterface* markerInterface;
    if (GetTCOut(target, 4, &markerInterface))
        markerInterface->markerName = name;
}
