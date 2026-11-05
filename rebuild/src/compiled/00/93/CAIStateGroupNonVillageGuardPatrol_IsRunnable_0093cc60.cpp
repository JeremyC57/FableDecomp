// CAIStateGroup_NonVillageGuardPatrol::IsRunnable() -- runnable when a "MoveTrack" track node lies in the 10m square around the creature: latch the nearest one as the patrol node
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093CC60, length 336 bytes (+ int3 padding).
// Callees: CThing::GetPos @ 0x004C73D0 (five times), __ftol2 @ 0x00BFEA70, CCharString(const char*, long)
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
#include "engine/C3DVector.h"
class CThing { public:
    const C3DVector& GetPos() const;             // 0x004C73D0
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
                                  const CThingCompare_Nearest& compare, CThingTrackNode** out) const;   // 0x0093CDB0
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the work GetPBestThingInRoughArea does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for CThingSearchTools::GetPBestThingInRoughArea @ 0x0093CDB0 (compiled in retail's TU): reads its const reference / pointer-to-const
// arguments, updates its out-parameters, and lets no argument escape.
__declspec(noinline) bool CThingSearchTools::GetPBestThingInRoughArea(const C2DBoxI& area, const CThingFilter_HasTrackName& filter, const CThingCompare_Nearest& compare, CThingTrackNode** out) const
{
    *out = 0;
    long best = NearSink_s7_0(this, *(const long*)&area + *(const long*)&filter + *(const long*)&compare);
    if (best)
        *(long*)out = best;
    return *out != 0;
}
#pragma auto_inline(on)
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CIntelligentPointerTrackNode : CBaseIntelligentPointer {   // CIntelligentPointer<CThingTrackNode>, vtable 0x0125A894
    virtual void v00();
    __forceinline CIntelligentPointerTrackNode(CThingTrackNode* const& item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)((CThing*)item)); }
    __forceinline CIntelligentPointerTrackNode& operator=(const CIntelligentPointerTrackNode& other) { CThing* item = ((CThing*)other.GetPItem()); SetPItem((CBaseObject*)item); return *this; }
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
protected:
    const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000 (Ego_r.pdb ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ, protected)  // 0x008FD000
public:
};
struct CAIStateGroup_NonVillageGuardPatrol : CAIStateGroupBase {
    CIntelligentPointerTrackNode PNode;         // +0x14 (descriptive)
    virtual bool IsRunnable();
};

bool CAIStateGroup_NonVillageGuardPatrol::IsRunnable()
{
    C2DBoxI area((long)(GetCreature()->GetPos().X - 5.0f), (long)(GetCreature()->GetPos().Y - 5.0f),
                 (long)(GetCreature()->GetPos().X + 5.0f), (long)(GetCreature()->GetPos().Y + 5.0f));
    CThingTrackNode* node = 0;
    bool found;
    {
        CCharString track("MoveTrack");
        found = PeekThingSearchTools().GetPBestThingInRoughArea(area, CThingFilter_HasTrackName(track),
            CThingCompare_Nearest(GetCreature()->GetPos()), &node);
    }
    if (found) {
        PNode = CIntelligentPointerTrackNode(node);
        return true;
    }
    return false;
}