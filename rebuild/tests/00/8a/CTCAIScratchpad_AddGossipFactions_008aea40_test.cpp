// CTCAIScratchpad::AddGossipFactions(CCharString name,CCharString value) -- Append a gossip value in its category.
// Leaf: ?AddGossipFactions@CTCAIScratchpad@@SIXVCCharString@@0@Z
// Test (NF208, 2026-11-20): the row on the shared headers (fable_string.h, stlport/fable_containers.h, stlport/fable_map.h,
//   engine/CTCAIScratchpad.h); the harness defines the CCharString members the header declares (a reference count per string id),
//   the static map and its out-of-line operator[] (0x007AEBE0 in retail), and drives push_back through both the in-place and the
//   _M_insert_overflow (0x00433530) paths.
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#include "stlport/fable_containers.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCAISCRATCHPAD_GOSSIP_MAPS
#define FABLE_RETAIL_CTCAISCRATCHPAD_ADDGOSSIPFACTIONS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCAIScratchpad.h"
namespace std { template<> vector<CCharString>& map<CCharString, vector<CCharString> >::operator[](const CCharString& key); }

void FABLE_FASTCALL CTCAIScratchpad::AddGossipFactions(CCharString name,CCharString value){GossipFactions[name].push_back(value);}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static long gRefs[256];
static long gLive;
static int gCalls, gBad;
static unsigned long gExpectedKey;
static std::vector<CCharString>* gValues;
static unsigned long IdOf(const CCharString& s) { void* p; memcpy(&p, &s, sizeof p); return (unsigned long)(size_t)p; }
static void SetId(CCharString* s, unsigned long id) { void* p = (void*)(size_t)id; memcpy(s, &p, sizeof p); }
CCharString::CCharString() { SetId(this, 0); }
CCharString::CCharString(const char* text, long) { unsigned long id = (unsigned char)text[0]; SetId(this, id); if (id) { ++gRefs[id]; ++gLive; } }
CCharString::CCharString(const CCharString& other) { unsigned long id = IdOf(other); SetId(this, id); if (id) { ++gRefs[id]; ++gLive; } }
CCharString::~CCharString() { unsigned long id = IdOf(*this); if (id) { --gRefs[id]; --gLive; } SetId(this, 0); }
bool CCharString::operator<(const CCharString& other) const { return IdOf(*this) < IdOf(other); }

std::map<CCharString, std::vector<CCharString> > CTCAIScratchpad::GossipFactions;
namespace std {
template<> vector<CCharString>& map<CCharString, vector<CCharString> >::operator[](const CCharString& key)
{
    ++gCalls;
    if (this != &CTCAIScratchpad::GossipFactions || IdOf(key) != gExpectedKey) gBad = 1;
    return *gValues;
}
}

static int Run()
{
    std::vector<CCharString> values;
    gValues = &values;
    CCharString key("k"), a("a"), b("b"), c("c");
    gExpectedKey = 'k';
    CTCAIScratchpad::AddGossipFactions(key, a);                      // empty vector: the overflow path
    if (gBad || gCalls != 1 || values.size() != 1 || IdOf(values[0]) != 'a') return 1;
    CTCAIScratchpad::AddGossipFactions(key, b);                      // grows again (capacity 1)
    CTCAIScratchpad::AddGossipFactions(key, c);                      // grows to 4
    CTCAIScratchpad::AddGossipFactions(key, a);                      // in place
    if (gBad || gCalls != 4 || values.size() != 4) return 2;
    if (IdOf(values[0]) != 'a' || IdOf(values[1]) != 'b' || IdOf(values[2]) != 'c' || IdOf(values[3]) != 'a') return 3;
    if (gRefs['k'] != 1 || gRefs['a'] != 3 || gRefs['b'] != 2 || gRefs['c'] != 2) return 4;
    return 0;
}

int main()
{
    int r = Run();
    if (r) { printf("008aea40_TEST FAIL %d\n", r); return r; }
    if (gLive != 0) { printf("008aea40_TEST FAIL leak %ld\n", gLive); return 9; }
    printf("008aea40_TEST PASS\n");
    return 0;
}
