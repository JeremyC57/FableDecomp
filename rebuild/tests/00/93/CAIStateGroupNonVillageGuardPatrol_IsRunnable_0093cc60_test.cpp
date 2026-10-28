// Behaviour test for 0093cc60: src/0093cc60.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CAIStateGroup_NonVillageGuardPatrol::IsRunnable()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_NonVillageGuardPatrol::IsRunnable() -- runnable when a "MoveTrack" track node lies in the 10m square around the creature: latch the nearest one as the patrol node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093CC60, length 336 bytes (+ int3 padding).
// Callees: CThing::GetPosition @ 0x004C73D0 (five times), __ftol2 @ 0x00BFEA70, CCharString(const char*, long)
//          @ 0x0099EBF0 ("MoveTrack" @ 0x012925BC) / copy ctor @ 0x0099EC30 / ~CCharString @ 0x0099EAE0,
//          CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPBestThingInRoughArea<
//          CThingFilter_HasTrackName, CThingCompare_Nearest, CThingTrackNode> @ 0x0093CDB0, CBaseIntelligentPointer
//          ctor/SetPItem/GetPItem/dtor (temporary vtable 0x0125A894).
// Globals: 5.0f @ 0x012462A8.
// Shape: the box is a named local whose four truncated bounds evaluate right to left (maxY, maxX, minY, minX) with the
//          creature loaded once; the track name is a named CCharString copied into the filter temporary; arguments
//          right to left (comparator copies the position, then the filter); the hit is latched through a temporary.
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04) with the
//          patrol node intelligent pointer at +0x14.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) { X = v.X; Y = v.Y; Z = v.Z; }
};
class CThing { public:
    const C3DVector& GetPosition();             // 0x004C73D0
};
class CThingTrackNode;
class C2DBoxI { public:
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) { }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CThingFilter_HasTrackName { public:
    CCharString Name;
    CThingFilter_HasTrackName(const CCharString& name) : Name(name) { }
};
class CThingCompare_Nearest { public:
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
class CThingSearchTools { public:
    bool GetPBestThingInRoughArea(const C2DBoxI& area, const CThingFilter_HasTrackName& filter,
                                  const CThingCompare_Nearest& compare, CThingTrackNode** out);   // 0x0093CDB0
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the work GetPBestThingInRoughArea does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for CThingSearchTools::GetPBestThingInRoughArea @ 0x0093CDB0 (compiled in retail's TU): reads its const reference / pointer-to-const
// arguments, updates its out-parameters, and lets no argument escape.
__declspec(noinline) bool CThingSearchTools::GetPBestThingInRoughArea(const C2DBoxI& area, const CThingFilter_HasTrackName& filter, const CThingCompare_Nearest& compare, CThingTrackNode** out)
{
    *out = 0;
    long best = NearSink_s7_0(this, *(const long*)&area + *(const long*)&filter + *(const long*)&compare);
    if (best)
        *(long*)out = best;
    return *out != 0;
}
#pragma auto_inline(on)
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerTrackNode : CBaseIntelligentPointer {   // CIntelligentPointer<CThingTrackNode>, vtable 0x0125A894
    virtual void v00();
    __forceinline CIntelligentPointerTrackNode(CThingTrackNode* const& item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
    __forceinline CIntelligentPointerTrackNode& operator=(const CIntelligentPointerTrackNode& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
struct CAIStateGroup_NonVillageGuardPatrol : CAIStateGroupBase {
    CIntelligentPointerTrackNode PNode;         // +0x14 (descriptive)
    virtual bool IsRunnable();
};

bool CAIStateGroup_NonVillageGuardPatrol::IsRunnable()
{
    C2DBoxI area((long)(GetCreature()->GetPosition().X - 5.0f), (long)(GetCreature()->GetPosition().Y - 5.0f),
                 (long)(GetCreature()->GetPosition().X + 5.0f), (long)(GetCreature()->GetPosition().Y + 5.0f));
    CThingTrackNode* node = 0;
    bool found;
    {
        CCharString track("MoveTrack");
        found = PeekThingSearchTools()->GetPBestThingInRoughArea(area, CThingFilter_HasTrackName(track),
            CThingCompare_Nearest(GetCreature()->GetPosition()), &node);
    }
    if (found) {
        PNode = CIntelligentPointerTrackNode(node);
        return true;
    }
    return false;
}

// ---- stubs and driver ----
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerTrackNode::v00() {}
#include <stdlib.h>
CAIStateGroupBase::~CAIStateGroupBase() {}
static C3DVector* g_posPtr = 0; static int g_posCalls = 0;
const C3DVector& CThing::GetPosition() { ++g_posCalls; return *g_posPtr; }
static const char* g_text = 0; static int g_strCtor = 0, g_strCopy = 0, g_strDtor = 0;
CCharString::CCharString(const char* text, long length) { ++g_strCtor; g_text = text; PStringData = (void*)text; }
CCharString::CCharString(const CCharString& other) { ++g_strCopy; PStringData = other.PStringData; }
CCharString::~CCharString() { ++g_strDtor; }
static CThingSearchTools g_tools;
CThingSearchTools* CAIStateGroupBase::PeekThingSearchTools() { return &g_tools; }
static long g_best = 0; static long g_seen = 0;
long NearSink_s7_0(const void* self, long seen) { g_seen = seen; return g_best; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static int g_ipCtor = 0, g_ipDtor = 0;
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { ++g_ipCtor; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ipDtor; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
int main()
{
    static unsigned char storeG[sizeof(CAIStateGroup_NonVillageGuardPatrol)];
    CAIStateGroup_NonVillageGuardPatrol& g = *(CAIStateGroup_NonVillageGuardPatrol*)storeG;
    static CAIBrain brain; static CThing creature; brain.PCreature = &creature; g.PBrain = &brain;
    static C3DVector* pos = (C3DVector*)malloc(sizeof(C3DVector)); pos->X = 12.7f; pos->Y = -3.2f; pos->Z = 1.0f; g_posPtr = pos;
    g_best = 0; bool r = g.CAIStateGroup_NonVillageGuardPatrol::IsRunnable();
    CHECK(r == false); CHECK(g_posCalls == 5); CHECK(strcmp(g_text, "MoveTrack") == 0); CHECK(g_strCopy == 1); CHECK(g_strDtor == 2);
    CHECK(g_seen == 7 + (long)g_text + *(const long*)&pos->X); CHECK(g_ipCtor == 0); CHECK(g.PNode.m_pItem == 0);   // box MinX = (long)(12.7 - 5) = 7
    g_best = 0x1234; r = g.CAIStateGroup_NonVillageGuardPatrol::IsRunnable();
    CHECK(r == true); CHECK(g.PNode.m_pItem == (CThing*)0x1234); CHECK(g_ipCtor == 1); CHECK(g_ipDtor == 1);
    if (fails == 0) printf("0093cc60_TEST PASS\n"); else printf("0093cc60_TEST FAIL\n");
    return fails;
}