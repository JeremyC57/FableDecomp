// Behaviour test for 004afe20: src/004afe20.cpp copied verbatim; removes the first boast with the id (and the script
// name unless empty), then every boast sharing a group bit with it and the same script name rule; stamps the frame.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CQuestManager::RemoveBoast(long id, CCharString scriptName) -- remove the boast with this id and script name from the boast list
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AFE20, 345 bytes, `ret 8` (by-value CCharString destroyed at the end).
// Callees: CCharString::operator!=(const char*) 0x0099E960 / (const CCharString&) 0x0099E900 (first pass), CBasicString<char>::
//   operator==(const char*) 0x004115A0 and Compare 0x00411570 (both given their landed-equivalent bodies as same-TU
//   stand-ins), ~CWideString 0x0099B510, ~CCharString 0x0099EAE0, free 0x00BFEA14, CWorld::GetFrame 0x0049D870.
// Layout: CQuestManager FrameBoastsLastUpdated +0x64, Boasts +0x68 (retail 0x3c below Ego_r.pdb); CBoast from the PDB.
// Shape: the group pass uses the inline CCharString == ("" and CCharString). The same-TU 0x004115A0 body tells VC7.1 the
//   call writes no memory, so scriptName's block stays in ebp across it (an extern declaration reloads it). The inline
//   ==("") null arm is `if (strcmp(text, "") == 0) return true; return false;`, which branches off the repe cmpsb flags.
#include <string.h>
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
class CWorld { public: static long __fastcall GetFrame(); };               // 0x0049D870
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body, same-TU stand-in)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                          // CBasicString<char>, 13 bytes
    char*         PData;                           // +0x00
    unsigned long StringLength;                    // +0x04
    unsigned long DataLength : 31;                 // +0x08
    bool          UseFastExtend : 1;               // +0x0c
    bool operator==(const char* text) const;       // 0x004115A0
};
class CCharStringData { public:
    CBasicStringChar Data;                         // +0x00
    long             NoRefs;                       // +0x0d
};
#pragma pack(pop)
#pragma auto_inline(off)
bool CBasicStringChar::operator==(const char* text) const   // 0x004115A0 (same-TU stand-in: reads only)
{
    return Compare_00411570(PData, text) ? false : true;
}
#pragma auto_inline(on)
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                                  // 0x0099EAE0
    bool operator!=(const char* text) const;         // 0x0099E960
    bool operator!=(const CCharString& other) const; // 0x0099E900
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        if (PStringData->Data.StringLength != other.PStringData->Data.StringLength)
            return false;
        return Compare_00411570(PStringData->Data.PData, other.PStringData->Data.PData) ? false : true;
    }
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                  // 0x0099B510
};
struct CBoast {                                    // CQuestManager::CBoast (PDB, 32 bytes)
    long        ID;                                // +0x00
    long        Cost;                              // +0x04
    long        Reward;                            // +0x08
    CCharString ScriptName;                        // +0x0c
    CWideString Text;                              // +0x10
    CWideString Title;                             // +0x14
    bool        Evil;                              // +0x18
    bool        Failed;                            // +0x19
    bool        Completed;                         // +0x1a
    long        GroupIndex;                        // +0x1c
};
struct CBoastNode {                                // STLport list node
    CBoastNode* next;
    CBoastNode* prev;
    CBoast      value;                             // +0x08
};
inline CBoastNode* EraseBoast(CBoastNode* node)    // STLport list::erase(iterator)
{
    CBoastNode* next = node->next;
    CBoastNode* prev = node->prev;
    prev->next = next;
    next->prev = prev;
    node->value.~CBoast();
    free(node);
    return next;
}
class CQuestManager { public:
    unsigned char _pad_0x00[0x64];
    unsigned long FrameBoastsLastUpdated;          // +0x64 (PDB +0xa0)
    CBoastNode*   Boasts;                          // +0x68 list<CBoast> (head node; PDB +0xa4)
    CBoastNode*   AcceptedBoasts;                  // +0x6c
    void RemoveBoast(long id, CCharString scriptName);
};
void CQuestManager::RemoveBoast(long id, CCharString scriptName)
{
    for (CBoastNode* node = Boasts->next; node != Boasts; node = node->next) {
        if (node->value.ID != id)
            continue;
        if (scriptName != "" && scriptName != node->value.ScriptName)
            continue;
        long group = node->value.GroupIndex;
        EraseBoast(node);
        if (group) {
            for (CBoastNode* other = Boasts->next; other != Boasts; ) {
                if ((other->value.GroupIndex & group) && (scriptName == "" || scriptName == other->value.ScriptName))
                    other = EraseBoast(other);
                else
                    other = other->next;
            }
        }
        break;
    }
    FrameBoastsLastUpdated = CWorld::GetFrame();
}
// ---- end source ----
static int g_strDtors, g_wideDtors;
static const char* TextOf(const CCharString& s) { return s.PStringData ? s.PStringData->Data.PData : ""; }
bool CCharString::operator!=(const char* text) const { return strcmp(TextOf(*this), text) != 0; }
bool CCharString::operator!=(const CCharString& other) const { return strcmp(TextOf(*this), TextOf(other)) != 0; }
CCharString::~CCharString() { ++g_strDtors; }
CWideString::~CWideString() { ++g_wideDtors; }
long CWorld::GetFrame() { return 4242; }
static CCharStringData MakeData(char* text) { CCharStringData d; memset(&d, 0, sizeof(d)); d.Data.PData = text; d.Data.StringLength = (unsigned long)strlen(text); return d; }
static CBoastNode g_head;
static void Append(long id, CCharStringData* name, long group)
{
    CBoastNode* n = (CBoastNode*)malloc(sizeof(CBoastNode));
    memset(n, 0, sizeof(*n));
    n->value.ID = id; n->value.ScriptName.PStringData = name; n->value.GroupIndex = group;
    n->prev = g_head.prev; n->next = &g_head; g_head.prev->next = n; g_head.prev = n;
}
static int Ids(long* out) { int k = 0; for (CBoastNode* n = g_head.next; n != &g_head; n = n->next) out[k++] = n->value.ID; return k; }
static void Call(CQuestManager& qm, long id, CCharStringData* name) { CCharString s; s.PStringData = name; qm.RemoveBoast(id, s); }
int main()
{
    static char t1[] = "s1", t1b[] = "s1", t2[] = "s2", tz[] = "zz", te[] = "";
    CCharStringData d1 = MakeData(t1), d1b = MakeData(t1b), d2 = MakeData(t2), dz = MakeData(tz), de = MakeData(te);
    g_head.next = g_head.prev = &g_head;
    static long raw[0x70 / 4]; memset(raw, 0, sizeof(raw));
    CQuestManager& qm = *(CQuestManager*)raw; qm.Boasts = &g_head;
    Append(1, &d1, 0); Append(2, &d1, 3); Append(3, &d1, 1); Append(4, &d2, 2); Append(5, &d1, 4);
    long ids[8]; int n;
    Call(qm, 2, &d1b);                       // B removed; C shares group 1 with "s1"; D is "s2"; E has no shared bit
    n = Ids(ids); CHECK(n == 3 && ids[0] == 1 && ids[1] == 4 && ids[2] == 5);
    CHECK(qm.FrameBoastsLastUpdated == 4242);
    CHECK(g_wideDtors == 4);
    Call(qm, 5, &de);                        // "" name: any script; group 4 has nobody else
    n = Ids(ids); CHECK(n == 2 && ids[0] == 1 && ids[1] == 4);
    qm.FrameBoastsLastUpdated = 0;
    Call(qm, 1, &dz);                        // name mismatch: nothing removed, frame still stamped
    n = Ids(ids); CHECK(n == 2 && qm.FrameBoastsLastUpdated == 4242);
    Append(6, &d2, 2); Append(7, &d1, 2);
    Call(qm, 4, 0);                          // null name: D removed, then every group-2 boast whatever its script
    n = Ids(ids); CHECK(n == 1 && ids[0] == 1);
    if (fails == 0) printf("004afe20_TEST PASS\n"); else printf("004afe20_TEST FAIL\n");
    return fails;
}