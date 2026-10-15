// Behaviour test for CTCSoundPlayer::CTCSoundPlayer (0x0066AC80): base built with the parent, the emitter pointer set to
// null, both strings default-constructed, clip distance and material index zero.
#include <cstdio>

struct CThing { int id; };
struct CThingSoundEmitter;
static CThing* g_baseParent = 0;
static int g_setPItemCalls = 0;
static int g_stringCtors = 0;

struct CTCBase {
    CThing* ParentThing;
    virtual ~CTCBase() { }
    CTCBase(CThing* parent) : ParentThing(parent) { g_baseParent = parent; }
};

struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem((void*)1) { }
    void SetPItem(void* item) { ++g_setPItemCalls; m_pItem = item; }
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(CThingSoundEmitter* item) : CBaseIntelligentPointer() { SetPItem((void*)item); }
};

struct CCharString {
    const char* m_pData;
    CCharString() : m_pData("") { ++g_stringCtors; }
};

struct CTCSoundPlayer : CTCBase {
    CIntelligentPointer PAttachedEmitter;
    CCharString         VoiceName;
    CCharString         ExtraTag;
    short               AnimCriteriaClipDistance;
    long                LastMaterialIndex;
    CTCSoundPlayer(CThing* parent);
};

CTCSoundPlayer::CTCSoundPlayer(CThing* parent)
    : CTCBase(parent)
    , PAttachedEmitter(0)
    , AnimCriteriaClipDistance(0)
    , LastMaterialIndex(0)
{
}

int main()
{
    CThing thing;
    CTCSoundPlayer p(&thing);
    if (g_baseParent != &thing || g_setPItemCalls != 1 || p.PAttachedEmitter.m_pItem != 0 || g_stringCtors != 2
        || p.AnimCriteriaClipDistance != 0 || p.LastMaterialIndex != 0) {
        std::printf("0066ac80_TEST FAIL\n"); return 1;
    }
    std::printf("0066ac80_TEST PASS\n");
    return 0;
}