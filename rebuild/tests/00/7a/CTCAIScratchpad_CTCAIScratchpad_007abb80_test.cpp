// Behaviour test for CTCAIScratchpad::CTCAIScratchpad @ 0x007ABB80: the base keeps the owner, the four
// strings are default-constructed (empty) and the def pointer is null.
#include <cstdio>
#include <new>

class CThing { public: int unused; };
class CAIScratchpadDef { public: int unused; };

static int g_stringCtors = 0;
static const char g_empty[1] = { 0 };

struct CTCBase_Calls {
    virtual ~CTCBase_Calls() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
};

class CCharString { public:
    const char* m_pData;
    CCharString() : m_pData(g_empty) { ++g_stringCtors; }
};

template<class T>
class CDefPointer { public:
    const T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

class CTCAIScratchpad : public CTCBase_Calls { public:
    unsigned long ThankingPhrase;
    CCharString   Action;
    bool          FaceHeroForAction;
    CCharString   TargetName;
    CCharString   TableName;
    CCharString   SeatName;
    CDefPointer<CAIScratchpadDef> PDef;
    CTCAIScratchpad(CThing* owner) : CTCBase_Calls(owner) {}
};

int main()
{
    CThing owner;
    static CAIScratchpadDef junk;
    unsigned char storage[sizeof(CTCAIScratchpad)];
    for (unsigned i = 0; i < sizeof(storage); ++i) storage[i] = 0xCD;
    CTCAIScratchpad* p = new (storage) CTCAIScratchpad(&owner);
    bool ok = p->ParentThing == &owner && g_stringCtors == 4 && p->Action.m_pData == g_empty
           && p->TargetName.m_pData == g_empty && p->TableName.m_pData == g_empty
           && p->SeatName.m_pData == g_empty && p->PDef.m_pDef == 0;
    (void)junk;
    p->~CTCAIScratchpad();
    if (!ok) { std::printf("007abb80_TEST FAIL\n"); return 1; }
    std::printf("007abb80_TEST PASS\n");
    return 0;
}