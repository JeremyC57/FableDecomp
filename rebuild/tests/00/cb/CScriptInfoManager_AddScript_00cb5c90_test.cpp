// CScriptInfoManager::AddScript(const CScriptInfo& info, CCharString section) (0x00CB5C90)
// Leaf: ?AddScript@CScriptInfoManager@@IAEXABVCScriptInfo@@VCCharString@@@Z
// Test (NF208, 2026-11-20): the row on the shared headers (fable_string.h, stlport/fable_containers.h, engine/CScriptInfoManager.h,
//   engine/CTBaseSingleton.h); the harness defines the CCharString members the header declares (string data blocks from a pool, the
//   length set through a pointer to the protected member, live copies counted), CTBaseSingleton<CCodeSectionManager>::Get and
//   RegisterSection (0x00414CE0 / 0x009F05A0) as recorders, the out-of-line push_back of the Scripts vector (0x00CB7210) as a
//   recorder, and reaches the protected AddScript through a derived manager. CodeSectionNames runs both push_back paths.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "engine/CScriptInfoManager.h"
#define FABLE_RETAIL_CTBASESINGLETON_GET_DECL
#include "engine/CTBaseSingleton.h"

class CCodeSectionManager {
public:
    unsigned long RegisterSection(CCharString shortName, CCharString longName, bool loadNow, bool canUnload);   // 0x009F05A0
};

template <>
void std::vector<std::pair<CCharString, CScriptInfo> >::push_back(const std::pair<CCharString, CScriptInfo>& __x);   // 0x00CB7210

void CScriptInfoManager::AddScript(const CScriptInfo& info, CCharString section)
{
    if (section.GetLength() > 0) {
        CTBaseSingleton<CCodeSectionManager>::Get().RegisterSection(section, info.Name, false, false);
        CodeSectionNames.push_back(section);
    }
    Scripts.Insert(info.Name, info);
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static long gLive;
struct LenSetter : CBasicString<char> {
    static unsigned long CBasicString<char>::* Length() { return &LenSetter::StringLength; }
};
static CCharStringData gPool[16];
static int gPoolUsed;
static CCharStringData* DataOf(const CCharString& s) { CCharStringData* p; memcpy(&p, &s, sizeof p); return p; }
static void SetData(CCharString* s, CCharStringData* p) { memcpy(s, &p, sizeof p); }
CCharString::CCharString() { SetData(this, 0); }
CCharString::CCharString(const char* text, long)
{
    CCharStringData* d = &gPool[gPoolUsed++];
    d->Data.*(LenSetter::Length()) = (unsigned long)strlen(text);
    SetData(this, d); ++gLive;
}
CCharString::CCharString(const CCharString& other) { SetData(this, DataOf(other)); if (DataOf(other)) ++gLive; }
CCharString::~CCharString() { if (DataOf(*this)) --gLive; SetData(this, 0); }
const CCharString& CCharString::operator=(const CCharString& other) { if (DataOf(*this)) --gLive; SetData(this, DataOf(other)); if (DataOf(other)) ++gLive; return *this; }

static CCodeSectionManager gManager;
static int gRegistered;
static CCharStringData* gLastShort;
static CCharStringData* gLastLong;
static bool gLastLoad, gLastUnload;
template <> CCodeSectionManager& CTBaseSingleton<CCodeSectionManager>::Get() { return gManager; }
unsigned long CCodeSectionManager::RegisterSection(CCharString shortName, CCharString longName, bool loadNow, bool canUnload)
{
    ++gRegistered; gLastShort = DataOf(shortName); gLastLong = DataOf(longName); gLastLoad = loadNow; gLastUnload = canUnload;
    return 0;
}
static int gPushed;
static CCharStringData* gPushedKey;
static long gPushedID;
template <>
void std::vector<std::pair<CCharString, CScriptInfo> >::push_back(const std::pair<CCharString, CScriptInfo>& __x)
{
    ++gPushed; gPushedKey = DataOf(__x.first); gPushedID = __x.second.ID;
}
CScriptInfoManager::CScriptInfoManager() : CurrentScriptID(0) {}
// Scripts.Dirty is private in CVectorMap.h: the harness reads the byte at its retail offset (manager +0x11: Scripts +4, Dirty +0x0D).
static bool DirtyOf(const CScriptInfoManager& m) { bool b; memcpy(&b, (const char*)&m + 0x11, 1); return b; }
static void ClearDirty(CScriptInfoManager& m) { bool b = false; memcpy((char*)&m + 0x11, &b, 1); }
CScriptInfoManager::~CScriptInfoManager() {}
struct CTestManager : CScriptInfoManager {
    void Add(const CScriptInfo& info, CCharString section) { AddScript(info, section); }
};

static int Run()
{
    CCharString arena("ARENA"), empty(""), name("Q_Arena"), s("S");
    CScriptInfo a; a.AllocFunc = 0; a.MasterScriptDataAlloc = 0; a.Name = name; a.ID = 7; a.MasterScript = true;
    CTestManager m;
    ClearDirty(m);
    m.Add(a, arena);
    if (gRegistered != 1 || gLastShort != DataOf(arena) || gLastLong != DataOf(name) || gLastLoad || gLastUnload) return 1;
    if (m.CodeSectionNames.size() != 1 || DataOf(m.CodeSectionNames[0]) != DataOf(arena)) return 2;
    if (gPushed != 1 || gPushedKey != DataOf(name) || gPushedID != 7 || !DirtyOf(m)) return 3;
    m.Add(a, empty);                                     // empty section: not registered
    m.Add(a, CCharString());                             // null section: not registered
    if (gRegistered != 1 || m.CodeSectionNames.size() != 1 || gPushed != 3) return 4;
    for (int i = 0; i < 4; ++i) m.Add(a, s);             // grows through _M_insert_overflow
    if (gRegistered != 5 || m.CodeSectionNames.size() != 5 || DataOf(m.CodeSectionNames[4]) != DataOf(s) || gPushed != 7) return 5;
    return 0;
}

int main()
{
    int r = Run();
    if (r) { printf("00cb5c90_TEST FAIL %d\n", r); return r; }
    if (gLive != 0) { printf("00cb5c90_TEST FAIL leak %ld\n", gLive); return 9; }
    printf("00cb5c90_TEST PASS\n");
    return 0;
}
