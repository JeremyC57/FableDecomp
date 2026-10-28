#include <cstdio>
#if !defined(_MSC_VER)
#  define __fastcall
#  define __forceinline inline
#endif

// Standalone behavior fixture for
// CGameScriptInterface::SetDeathRecoveryMarkerName @ 0x00896f60.
class CCharString
{ public:
    const char* text;
    const CCharString& operator=(const CCharString& other);
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

class CPlayerManager
{ public:
    void* GetTarget();
};

class CGameScriptInterface                        // test-local view (engine/CGameScriptInterface.h is 32-bit only)
{ public:
    void* VTable;                                  // +0x00
    void* World;                                   // +0x04
    void* Component;                               // +0x08
    void* DisplayEngine;                           // +0x0c
    void* DefinitionManager;                       // +0x10
    CPlayerManager* PlayerManager;                 // +0x14
};

template <class T> __forceinline bool GetTCOut(unsigned char* thing, int kind, T** out)
{
    if (*(unsigned char*)(thing + 0x20) & 0x10) {
        *out = ((DeathRecoveryMarkerNameMap*)(thing + 0x44))->Find(kind);
        return true;
    }
    return false;
}


static void* g_sourceThis;
static void* g_sourceResult;
static void* g_resolveArgument;
static void* g_resolveResult;
static void* g_lowerBoundThis;
static int g_lowerBoundKey;
static DeathRecoveryMarkerNameNode* g_lowerBoundResult;
static void* g_assignmentThis;
static const CCharString* g_assignmentSource;
static int g_assignmentCount;

void* CPlayerManager::GetTarget()
{
    g_sourceThis = this;
    return g_sourceResult;
}

extern "C" void* __fastcall ResolveDeathRecoveryMarkerTarget(void* source)
{
    g_resolveArgument = source;
    return g_resolveResult;
}

DeathRecoveryMarkerNameNode*
DeathRecoveryMarkerNameMap::lower_bound(const int* key)
{
    g_lowerBoundThis = this;
    g_lowerBoundKey = *key;
    return g_lowerBoundResult;
}

const CCharString& CCharString::operator=(const CCharString& other)
{
    g_assignmentThis = this;
    g_assignmentSource = &other;
    ++g_assignmentCount;
    text = other.text;
    return *this;
}

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

static void ResetRecorders()
{
    g_sourceThis = 0;
    g_sourceResult = 0;
    g_resolveArgument = 0;
    g_resolveResult = 0;
    g_lowerBoundThis = 0;
    g_lowerBoundKey = -1;
    g_lowerBoundResult = 0;
    g_assignmentThis = 0;
    g_assignmentSource = 0;
    g_assignmentCount = 0;
}

static void ClearBytes(void* data, unsigned int size)
{
    unsigned char* bytes = (unsigned char*)data;
    for (unsigned int index = 0; index < size; ++index)
        bytes[index] = 0;
}

int main()
{
    int failures = 0;
    CGameScriptInterface self;
    CPlayerManager sourceObject;
    CCharString input;
    unsigned char target[0x98];
    DeathRecoveryMarkerNameNode found;
    DeathRecoveryMarkerNameNode end;
    DeathRecoveryMarkerNameInterface foundInterface;
    DeathRecoveryMarkerNameInterface endInterface;

    ClearBytes(&self, sizeof(self));
    self.PlayerManager = &sourceObject;
    input.text = "Oakvale";
    found.key = 4;
    found.value = &foundInterface;
    end.key = 0;
    end.value = &endInterface;

    // Null resolution stops after both source-chain calls.
    ResetRecorders();
    g_sourceResult = (void*)0x11110000;
    CGameScriptInterface_SetDeathRecoveryMarkerName(&self, 0, input);
    if (g_sourceThis != &sourceObject ||
        g_resolveArgument != (void*)0x11110000 ||
        g_assignmentCount != 0)
    {
        std::printf("null resolution path failed\n");
        ++failures;
    }

    // Disabled/dead targets are rejected by byte +0x91 bit 0.
    ResetRecorders();
    ClearBytes(target, sizeof(target));
    target[0x91] = 0x01;
    g_sourceResult = (void*)0x22220000;
    g_resolveResult = target;
    CGameScriptInterface_SetDeathRecoveryMarkerName(&self, 0, input);
    if (g_lowerBoundThis != 0 || g_assignmentCount != 0)
    {
        std::printf("flags91 gate failed\n");
        ++failures;
    }

    // Targets without interface-map flag +0x20 bit 4 are rejected.
    ResetRecorders();
    ClearBytes(target, sizeof(target));
    g_sourceResult = (void*)0x33330000;
    g_resolveResult = target;
    CGameScriptInterface_SetDeathRecoveryMarkerName(&self, 0, input);
    if (g_lowerBoundThis != 0 || g_assignmentCount != 0)
    {
        std::printf("flags20 gate failed\n");
        ++failures;
    }

    // A lower-bound entry with key <= 4 supplies the interface.
    ResetRecorders();
    ClearBytes(target, sizeof(target));
    target[0x20] = 0x10;
    DeathRecoveryMarkerNameMap* map =
        (DeathRecoveryMarkerNameMap*)(target + 0x44);
    map->end = &end;
    found.key = 4;
    found.value = &foundInterface;
    g_lowerBoundResult = &found;
    g_sourceResult = (void*)0x44440000;
    g_resolveResult = target;
    CGameScriptInterface_SetDeathRecoveryMarkerName(&self, 0, input);
    if (g_lowerBoundThis != map ||
        g_lowerBoundKey != 4 ||
        g_assignmentCount != 1 ||
        g_assignmentThis != &foundInterface.markerName ||
        g_assignmentSource != &input ||
        foundInterface.markerName.text != input.text)
    {
        std::printf("found entry path failed\n");
        ++failures;
    }

    // A key above 4 falls back to the end entry.
    ResetRecorders();
    found.key = 5;
    found.value = &foundInterface;
    end.value = &endInterface;
    g_lowerBoundResult = &found;
    g_sourceResult = (void*)0x55550000;
    g_resolveResult = target;
    CGameScriptInterface_SetDeathRecoveryMarkerName(&self, 0, input);
    if (g_assignmentCount != 1 ||
        g_assignmentThis != &endInterface.markerName)
    {
        std::printf("upper-bound fallback failed\n");
        ++failures;
    }

    // lower_bound returning end uses that same end entry.
    ResetRecorders();
    g_lowerBoundResult = &end;
    g_sourceResult = (void*)0x66660000;
    g_resolveResult = target;
    CGameScriptInterface_SetDeathRecoveryMarkerName(&self, 0, input);
    if (g_assignmentCount != 1 ||
        g_assignmentThis != &endInterface.markerName)
    {
        std::printf("end entry path failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf("FSE2_00896f60_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("FSE2_00896f60_TEST PASS\n");
    return 0;
}
