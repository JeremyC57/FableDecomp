// CAIStateGroup_HoverHornetQueen::SetUpCirclePoints() -- rebuild the circle markers from the live "WaspQueenCirclePos<n>" track things (n = 1, 2, ... until one is missing) within 60 units of the queen, and set the circle radius to the farthest marker's distance from the arena centre plus 12
// Leaf: ?SetUpCirclePoints@CAIStateGroup_HoverHornetQueen@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
//   template instance, manifest <CThingFilter_HasDefGlobalIndex, CThing>, in fact <CThingFilter_HasScriptName, CThing>) as a same-TU `template <class TFilter, class T>
//   __declspec(noinline)` body: VC7.1 then knows only that call writes `marker`, keeps the one marker load in esi across the
//   GetPos call and drops the null half of the loop test. The body is a STAND-IN (the real search walks the thing-map
//   cells; FindFirstInArea is a placeholder declaration) -- flag it for review. (2) The loop is
//   `do { lookup; if (IS_THING_ALIVE(marker)) { ... } } while (IS_THING_ALIVE(marker));` with marker declared outside and
//   nulled per iteration; retail's loop-bottom `test [esi+91h],1` is that while test with the null half folded.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095AB50, length 448 bytes (+ int3 padding; /GS: the 32-byte name buffer).
// Callees: vector::erase(first, last) @ 0x004AC0F0 (the inline clear), sprintf @ 0x00BFEB10 ("WaspQueenCirclePos%d" @ 0x01294ECC),
// CCharString(const char*, long) @ 0x0099EBF0 / copy ctor @ 0x0099EC30 / ~CCharString @ 0x0099EAE0, CThing::GetPos
// @ 0x004C73D0, __ftol2 @ 0x00BFEA70, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::
// GetPFirstThingInRoughArea @ 0x007FD110 (manifest instance <CThingFilter_HasDefGlobalIndex, CThing>; here the filter carries
// 0x007FD110 is the <CThingFilter_HasScriptName, CThing> instance (filter operator 0x006A7E30 compares the ScriptName; naming wave 19)
// a track name), CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C), the vector's _M_insert_overflow @ 0x004AC940, __security_check_cookie
// @ 0x00BFE9F9 (cookie @ 0x0139C8A8).
// Globals: 12.0f @ 0x01294EC8, the circle radius float @ 0x0138D2D8 (.bss, unnamed in the PDB dump; descriptive name).
// Flags: /GS
// Layout: HoverHornetQueen from Ego_r.pdb (base 0x14): ArenaCentrePos +0x28, CircleMarkers +0x50 (PDB +0x54: the Things vector
// at +0x40 is 12 bytes in STLport).
#include <math.h>
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThing.h"
#define IS_THING_ALIVE(p) ((p) != 0 && !(p)->IsDead())
class C2DBoxI { public:
    long MinX, MinY, MaxX, MaxY;
    C2DBoxI(long minX, long minY, long maxX, long maxY) : MinX(minX), MinY(minY), MaxX(maxX), MaxY(maxY) { }
};

class CThingFilter_HasScriptName { public:
    CCharString Name;
    CThingFilter_HasScriptName(const CCharString& name) : Name(name) { }
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
extern "C" int __cdecl sprintf(char* buffer, const char* format, ...);   // 0x00BFEB10
// Name fix (agentNF54): the local CBaseIntelligentPointer / CIntelligentPointer / CRetailVector stand-ins replaced by the shared
//   engine/CIntelligentPointer.h (retail branch, FABLE_RETAIL_CINTELLIGENTPOINTER_COPY) and the STLport 4.6 vector of stlport/fable_containers.h
//   (erase(first, last) out of line, FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE): the overflow call 0x004AC940 binds
//   ?_M_insert_overflow@?$vector@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@IAEXPAV?$CIntelligentPointer@VCThing@@@@ABV3@ABU__false_type@2@I_N@Z
//   (the landed row's own leaf; was ?_M_insert_overflow@?$CRetailVector@UCIntelligentPointer@@@@...) and clear()'s erase 0x004AC0F0 the
//   STLport vector<CIntelligentPointer<CThing> >::erase.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#include "stlport/fable_containers.h"
#include "engine/CIntelligentPointer.h"
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThing* GetCreature() const { return PBrain->PCreature; }
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000 (Ego_r.pdb ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ, protected)  // 0x008FD000
public:
};
extern float g_WaspQueenCircleRadius;           // 0x0138D2D8
struct CAIStateGroup_HoverHornetQueen : CAIStateGroupBase {
    unsigned char _pad14[0x1c - 0x14];
    C3DVector StartPos;                         // +0x1c
    C3DVector ArenaCentrePos;                   // +0x28
    bool bOneTime;                              // +0x34
    CThing* PEmitter;                           // +0x38
    CThing* PEmitter2;                          // +0x3c
    std::vector<CIntelligentPointer<CThing> > Things;  // +0x40
    long CurrentCirclePoint;                    // +0x4c
    std::vector<CIntelligentPointer<CThing> > CircleMarkers;   // +0x50
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
            CThingFilter_HasScriptName filter(track);
            const C3DVector& pos = GetCreature()->GetPos();
            long x = (long)pos.X;
            long y = (long)pos.Y;
            PeekThingSearchTools().GetPFirstThingInRoughArea(C2DBoxI(x - 60, y - 60, x + 60, y + 60), filter, &marker);
        }
        if (IS_THING_ALIVE(marker)) {
            const C3DVector& pos = marker->GetPos();
            float dx = pos.X - ArenaCentrePos.X;
            float dy = pos.Y - ArenaCentrePos.Y;
            float distance = (float)sqrt(dx * dx + dy * dy);
            if (distance > maxDistance)
                maxDistance = distance;
            CircleMarkers.push_back(CIntelligentPointer<CThing>(marker));
        }
    } while (IS_THING_ALIVE(marker));
    g_WaspQueenCircleRadius = maxDistance + 12.0f;
}