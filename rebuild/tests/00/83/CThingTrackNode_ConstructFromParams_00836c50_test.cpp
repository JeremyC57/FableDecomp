// Behaviour test for 0x00836C50 CThingTrackNode::ConstructFromParams: the source body verbatim (comments and CRT declarations dropped), mocks for its callees.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#define std tstd
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#define __int64 long long
#endif
#define protected public
class C3DVector;
class CThingPhysical;
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);       // 0x0099EFB0
};

class CThingGameObject {                                    // retail 0xd0 bytes
public:
    unsigned char _pad_0x00[0xd0];
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& position, long a3, unsigned long a4, const CCharString& scriptName,
                             CThingPhysical* creator, CThingPhysical* originalCreator, bool a8);   // 0x008388D0
};

class CThingTrackNode : public CThingGameObject {
public:
    CThingTrackNode* PNextNode;                             // +0xd0
    CThingTrackNode* PPreviousNode;                         // +0xd4
    unsigned __int64 NextNodeUID;                           // +0xd8
    unsigned __int64 PreviousNodeUID;                       // +0xe0
    CCharString      TrackName;                             // +0xe8
    bool             IsStart;                               // +0xec
    bool             IsEnd;                                 // +0xed
protected:
    bool ConstructFromParams(long defIndex, const C3DVector& position, CCharString trackName);
};

bool CThingTrackNode::ConstructFromParams(long defIndex, const C3DVector& position, CCharString trackName)
{
    bool constructed;
    {
        CCharString noScriptName("");
        constructed = CThingGameObject::ConstructFromParams(defIndex, position, 0, 0, noScriptName, 0, 0, true);
    }
    if (constructed) {
        PNextNode = 0;
        PPreviousNode = 0;
        NextNodeUID = 0;
        PreviousNodeUID = 0;
        IsStart = false;
        IsEnd = false;
        TrackName = trackName;
        return true;
    }
    return false;
}

static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
class C3DVector { public: float X, Y, Z; };
CCharString::CCharString(const char* text, long length) : PStringData((void*)text) { char b[64]; sprintf(b, "ctor(%s,%ld)", text, length); Log(b); }
CCharString::~CCharString() { char b[64]; sprintf(b, "dtor(%s)", (const char*)PStringData); Log(b); }
const CCharString& CCharString::operator=(const CCharString& other) { PStringData = other.PStringData; Log("assign"); return *this; }
static bool g_result = true;
bool CThingGameObject::ConstructFromParams(long defIndex, const C3DVector& position, long a3, unsigned long a4, const CCharString& scriptName,
                                           CThingPhysical* creator, CThingPhysical* originalCreator, bool a8)
{
    char b[96]; sprintf(b, "base(%ld,%g,%ld,%lu,'%s',%d,%d,%d)", defIndex, position.X, a3, a4, (const char*)scriptName.PStringData,
                        creator == 0, originalCreator == 0, (int)a8);
    Log(b); return g_result;
}
int main()
{
    CThingTrackNode* n = (CThingTrackNode*)malloc(sizeof(CThingTrackNode));
    memset(n, 0x77, sizeof(CThingTrackNode)); n->TrackName.PStringData = (void*)"old";
    C3DVector pos; pos.X = 2.5f; pos.Y = 0; pos.Z = 0;
    CCharString* name = (CCharString*)malloc(sizeof(CCharString)); name->PStringData = (void*)"track";
    int ok = n->ConstructFromParams(12, pos, *name);
    ok &= strstr(g_log, "ctor(,-1);base(12,2.5,0,0,'',1,1,1);dtor();assign;") == g_log;
    ok &= n->PNextNode == 0 && n->PPreviousNode == 0 && n->NextNodeUID == 0 && n->PreviousNodeUID == 0 && !n->IsStart && !n->IsEnd;
    ok &= strcmp((const char*)n->TrackName.PStringData, "track") == 0;
    memset(n, 0x77, sizeof(CThingTrackNode)); n->TrackName.PStringData = (void*)"old"; g_log[0] = 0; g_result = false;
    ok &= !n->ConstructFromParams(3, pos, *name);
    ok &= strstr(g_log, "ctor(,-1);base(3,2.5,0,0,'',1,1,1);dtor();") == g_log && strstr(g_log, "assign") == 0;
    ok &= n->IsEnd != false && strcmp((const char*)n->TrackName.PStringData, "old") == 0;
    if (!ok) printf("log %s\n", g_log);
    if (ok) printf("00836c50_TEST PASS\n");
    return ok ? 0 : 1;
}