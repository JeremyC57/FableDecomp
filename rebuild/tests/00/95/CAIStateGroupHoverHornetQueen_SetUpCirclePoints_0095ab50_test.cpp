// CAIStateGroup_HoverHornetQueen::SetUpCirclePoints() -- rebuild the circle markers from the live "WaspQueenCirclePos<n>" track things (n = 1, 2, ... until one is missing) within 60 units of the queen, and set the circle radius to the farthest marker's distance from the arena centre plus 12
//   template instance, manifest <CThingFilter_HasDefGlobalIndex, CThing>) as a same-TU `template <class TFilter, class T>
//   __declspec(noinline)` body: VC7.1 then knows only that call writes `marker`, keeps the one marker load in esi across the
//   GetPosition call and drops the null half of the loop test. The body is a STAND-IN (the real search walks the thing-map
//   cells; FindFirstInArea is a placeholder declaration) -- flag it for review. (2) The loop is
//   `do { lookup; if (IS_THING_ALIVE(marker)) { ... } } while (IS_THING_ALIVE(marker));` with marker declared outside and
//   nulled per iteration; retail's loop-bottom `test [esi+91h],1` is that while test with the null half folded.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095AB50, length 448 bytes (+ int3 padding; /GS: the 32-byte name buffer).
// Callees: vector::erase(first, last) @ 0x004AC0F0 (the inline clear), sprintf @ 0x00BFEB10 ("WaspQueenCirclePos%d" @ 0x01294ECC),
// CCharString(const char*, long) @ 0x0099EBF0 / copy ctor @ 0x0099EC30 / ~CCharString @ 0x0099EAE0, CThing::GetPosition
// @ 0x004C73D0, __ftol2 @ 0x00BFEA70, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::
// GetPFirstThingInRoughArea @ 0x007FD110 (manifest instance <CThingFilter_HasDefGlobalIndex, CThing>; here the filter carries
// a track name), CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C), the vector's _M_insert_overflow @ 0x004AC940, __security_check_cookie
// @ 0x00BFE9F9 (cookie @ 0x0139C8A8).
// Globals: 12.0f @ 0x01294EC8, the circle radius float @ 0x0138D2D8 (.bss, unnamed in the PDB dump; descriptive name).
// Flags: /GS
// Layout: HoverHornetQueen from Ego_r.pdb (base 0x14): ArenaCentrePos +0x28, CircleMarkers +0x50 (PDB +0x54: the Things vector
// at +0x40 is 12 bytes in STLport).
#ifndef _MSC_VER
#define __declspec(x)
#define __cdecl
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
class C3DVector { public:
    float X, Y, Z;
};
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition();             // 0x004C73D0
};
#define IS_THING_ALIVE(p) ((p) != 0 && !((p)->m_flags91 & 1))
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
class CThingSearchTools { public:
    template <class TFilter, class T> bool GetPFirstThingInRoughArea(const C2DBoxI& area, const TFilter& filter, T** out) const;   // 0x007FD110
    CThing* FindFirstInArea(const C2DBoxI& area, const void* filter) const;   // stand-in placeholder (not a retail symbol)
};
// Same-TU stand-in for 0x007FD110 (see the Grade line): only its visibility matters to the caller's code.
template <class TFilter, class T> __declspec(noinline) bool CThingSearchTools::GetPFirstThingInRoughArea(const C2DBoxI& area, const TFilter& filter, T** out) const
{
    CThing* thing = FindFirstInArea(area, &filter);
    if (thing == 0)
        return false;
    *out = (T*)thing;
    return true;
}
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct __false_type { __false_type() { } };
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* where) { return where; }
#else
inline void* operator new(__SIZE_TYPE__, void* where) { return where; }
#endif
template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* erase(T* first, T* last);                        // 0x004AC0F0
    void clear() { erase(_M_start, _M_finish); }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long fill_len, bool atend);   // 0x004AC940
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
};
extern float g_WaspQueenCircleRadius;           // 0x0138D2D8
struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase {
    unsigned char _pad14[0x1c - 0x14];
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
    bool bOneTime;                              // +0x34
    CThing* PEmitter;                           // +0x38
    CThing* PEmitter2;                          // +0x3c
    CRetailVector<CIntelligentPointer> Things;  // +0x40
    long CurrentCirclePoint;                    // +0x4c
    CRetailVector<CIntelligentPointer> CircleMarkers;   // +0x50
    long EnemyKnockDownFrame;                   // +0x5c
    void SetUpCirclePoints();
};

void CAIStateGroup_HoverHornetQueen::SetUpCirclePoints()
{
    long index = 0;
    CircleMarkers.clear();
    float maxDistance = 0.0f;
    CThing* marker;
    do {
        ++index;
        marker = 0;
        char name[32];
        sprintf(name, "WaspQueenCirclePos%d", index);
        {
            CCharString track(name);
            CThingFilter_HasTrackName filter(track);
            const C3DVector& pos = GetCreature()->GetPosition();
            long x = (long)pos.X;
            long y = (long)pos.Y;
            PeekThingSearchTools()->GetPFirstThingInRoughArea(C2DBoxI(x - 60, y - 60, x + 60, y + 60), filter, &marker);
        }
        if (IS_THING_ALIVE(marker)) {
            const C3DVector& pos = marker->GetPosition();
            float dx = pos.X - ArenaCentrePos.X;
            float dy = pos.Y - ArenaCentrePos.Y;
            float distance = (float)sqrt(dx * dx + dy * dy);
            if (distance > maxDistance)
                maxDistance = distance;
            CircleMarkers.push_back(CIntelligentPointer(marker));
        }
    } while (IS_THING_ALIVE(marker));
    g_WaspQueenCircleRadius = maxDistance + 12.0f;
}

// ---- behaviour-test stubs ----
float g_WaspQueenCircleRadius;
struct SMarker { const char* name; CThing* thing; C3DVector pos; };
static CThing g_things[4];
static C3DVector g_positions[4];
static SMarker g_markers[4];
static int g_markerCount;
static CThing g_queen;
static C3DVector g_queenPos;
static CThingSearchTools g_tools;
static int g_lookups, g_overflows;
const C3DVector& CThing::GetPosition()
{
    if (this == &g_queen) return g_queenPos;
    return g_positions[this - g_things];
}
CCharString::CCharString(const char* text, long) { PStringData = strdup(text); }
CCharString::CCharString(const CCharString& other) { PStringData = strdup((const char*)other.PStringData); }
CCharString::~CCharString() { free(PStringData); }
CThing* CThingSearchTools::FindFirstInArea(const C2DBoxI& area, const void* filter) const
{
    ++g_lookups;
    if (area.MinX != (long)g_queenPos.X - 60 || area.MaxY != (long)g_queenPos.Y + 60) return 0;
    const char* name = (const char*)((const CThingFilter_HasTrackName*)filter)->Name.PStringData;
    for (int i = 0; i < g_markerCount; ++i)
        if (strcmp(g_markers[i].name, name) == 0) return g_markers[i].thing;
    return 0;
}
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
CAIStateGroupBase::~CAIStateGroupBase() {}
CThingSearchTools* CAIStateGroupBase::PeekThingSearchTools() { return &g_tools; }
template <class T> T* CRetailVector<T>::erase(T* first, T* last)
{
    for (T* p = first; p != last; ++p) p->~T();
    _M_finish = first;
    return first;
}
template <class T> void CRetailVector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned long, bool)
{
    ++g_overflows;
    unsigned long n = (unsigned long)(_M_finish - _M_start);
    T* fresh = (T*)malloc(sizeof(T) * (n + 4));
    for (unsigned long i = 0; i < n; ++i) new (&fresh[i]) T(_M_start[i]);
    new (&fresh[n]) T(x);
    _M_start = fresh; _M_finish = fresh + n + 1; _M_end_of_storage = fresh + n + 4;
}
struct CAIBrainStorage { CAIBrain brain; };
static void SetMarker(int i, const char* name, float x, float y, bool dead)
{
    g_markers[i].name = name; g_markers[i].thing = &g_things[i];
    g_positions[i].X = x; g_positions[i].Y = y; g_positions[i].Z = 0;
    g_things[i].m_flags91 = dead ? 1 : 0;
}
int main()
{
    int ok = 1;
    static CAIBrain brain;
    brain.PCreature = &g_queen;
    g_queenPos.X = 100.4f; g_queenPos.Y = 200.7f;
    static CAIStateGroup_HoverHornetQueen q;
    q.PBrain = &brain;
    q.ArenaCentrePos.X = 100.0f; q.ArenaCentrePos.Y = 200.0f; q.ArenaCentrePos.Z = 0;
    // one stale entry that clear() must drop; capacity 2 so the second push_back overflows
    CIntelligentPointer* store = (CIntelligentPointer*)malloc(sizeof(CIntelligentPointer) * 2);
    new (&store[0]) CIntelligentPointer(&g_queen);
    q.CircleMarkers._M_start = store; q.CircleMarkers._M_finish = store + 1; q.CircleMarkers._M_end_of_storage = store + 1;
    SetMarker(0, "WaspQueenCirclePos1", 110.0f, 200.0f, false);   // distance 10
    SetMarker(1, "WaspQueenCirclePos2", 100.0f, 230.0f, false);   // distance 30
    SetMarker(2, "WaspQueenCirclePos3", 120.0f, 200.0f, true);    // dead: ends the scan
    g_markerCount = 3;
    q.SetUpCirclePoints();
    if (g_lookups != 3) { ok = 0; printf("lookups %d\n", g_lookups); }
    if (q.CircleMarkers._M_finish - q.CircleMarkers._M_start != 2 || q.CircleMarkers._M_start[0].m_pItem != &g_things[0] ||
        q.CircleMarkers._M_start[1].m_pItem != &g_things[1]) { ok = 0; printf("markers\n"); }
    if (fabs(g_WaspQueenCircleRadius - 42.0f) > 0.001f) { ok = 0; printf("radius %f\n", g_WaspQueenCircleRadius); }
    // a missing third marker (null result) also ends the scan
    g_lookups = 0; g_markerCount = 1;
    q.SetUpCirclePoints();
    if (g_lookups != 2 || q.CircleMarkers._M_finish - q.CircleMarkers._M_start != 1 || fabs(g_WaspQueenCircleRadius - 22.0f) > 0.001f) { ok = 0; printf("second\n"); }
    if (ok) printf("0095ab50_TEST PASS\n");
    return ok ? 0 : 1;
}