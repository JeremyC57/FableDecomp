// CAIStateGroup_SleepOnFloor::IsRunnable() -- gather the free sleep points of this creature in the 20-unit box round it and latch a random one; runnable when there was one
// Leaf: ?IsRunnable@CAIStateGroup_SleepOnFloor@@UAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00969E70, 311 bytes, thiscall `ret`. Ego_r.pdb ?IsRunnable@CAIStateGroup_SleepOnFloor@@UAE_NXZ
//   (0x0098E950); members PSleepPoint (CIntelligentPointer<CThing>) +0x14, Asleep +0x1c.
// Callees: CThing::GetPos 0x004C73D0, C2DBoxF::ToC2DBoxI 0x00A55A30 (Ego_r.pdb ?ToC2DBoxI@C2DBoxF@@QBE?AVC2DBoxI@@XZ),
//   CAIStateGroupBase::PeekThingSearchTools 0x008FD000, CThingSearchTools::GetAllThingsInRoughArea<CThing, vector<CThing*>,
//   CFilter_Bind2<CThingFilter_IsMySleepPoint, unary_negate<CThingFilter_IsOccupiableOccupied> > > 0x00969FB0 (the Ego_r.pdb instance
//   0x0098E650; its body calls CThingFilter_IsMySleepPoint::operator() 0x006A8930 and CThingFilter_IsOccupiableOccupied::operator()
//   0x006A8A30), CAIStateGroupBase::GetPWorldSeed 0x008FCFD0, GFROR13 0x00497890, CBaseIntelligentPointer ctor / SetPItem /
//   GetPItem / dtor (CIntelligentPointer<CThing> temporary, vtable 0x01238C6C), free 0x00BFEA14. Constant 20.0f at 0x0129669C.
// Shape: the debug build's GFFilter_Bind2(CThingFilter_IsMySleepPoint(creature), std::not1(CThingFilter_IsOccupiableOccupied()))
//   (FableWin.pdb) is the inline Bind2 of two references (the empty negated filter in the dead byte at esp+0xf). The 2D centre is a
//   named C2DVector (FableWin ??0C2DVector@@QAE@ABVC3DVector@@@Z, inline): GetPos and its X/Y loads come first, the creature is re-read
//   for the filter, and the float box (FableWin ??0C2DBoxF@@QAE@ABVC2DVector@@M@Z, inline) is built among the argument pushes. The
//   rough-area body is visible in this TU (FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY, noinline; retail compiles 0x00969FB0 right
//   after this function), so VC7.1 knows the result vector is not captured: its start stays in ebx and the zero in edi across the
//   later calls. The random index is the inline GFRandom (Ego_r.pdb ?GFRandom@@YIKKPAK@Z, landed 0x009104D0 model).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CENTRE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
class CTCMapwho;                                                    // engine/CTCMapwho.h names it in NThingMap::CMapwhoNode first
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CFilter_Bind2.h"
#include "stlport/fable_containers.h"

long __fastcall GFROR13(unsigned long value);                       // 0x00497890 ?GFROR13@@YIJK@Z
__forceinline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

class CThingFilter_IsMySleepPoint {
public:
    CThingFilter_IsMySleepPoint(const CThing& sleeper) : Sleeper(const_cast<CThing*>(&sleeper)) { }
    bool operator()(const CThing& thing) const;                    // 0x006A8930 (Ego_r.pdb 0x005A48A0)
    CThing* Sleeper;
};
class CThingFilter_IsOccupiableOccupied {
public:
    typedef CThing argument_type;
    typedef bool result_type;
    CThingFilter_IsOccupiableOccupied() { }
    bool operator()(const CThing& thing) const;                    // 0x006A8A30 (Ego_r.pdb 0x005A49E0)
};

namespace std {
template <class _Predicate> class unary_negate {                    // STLport: the predicate held by value
protected:
    _Predicate _M_pred;
public:
    explicit unary_negate(const _Predicate& x) : _M_pred(x) { }
    bool operator()(const typename _Predicate::argument_type& x) const { return !_M_pred(x); }
};
template <class _Predicate> inline unary_negate<_Predicate> not1(const _Predicate& pred) { return unary_negate<_Predicate>(pred); }
}

class CAIStateGroup_SleepOnFloor : public CAIStateGroupBase {
public:
    virtual bool IsRunnable();
    CIntelligentPointer<CThing> PSleepPoint;                        // +0x14
    bool Asleep;                                                    // +0x1c
};

bool CAIStateGroup_SleepOnFloor::IsRunnable()
{
    std::vector<CThing*> sleepPoints;
    C2DVector centre(GetCreature().GetPos());
    PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(C2DBoxF(centre, 20.0f).ToC2DBoxI(),
        CFilter_Bind2<CThingFilter_IsMySleepPoint, std::unary_negate<CThingFilter_IsOccupiableOccupied> >(CThingFilter_IsMySleepPoint(GetCreature()), std::not1(CThingFilter_IsOccupiableOccupied())), sleepPoints);
    if (sleepPoints.empty())
        return false;
    PSleepPoint = CIntelligentPointer<CThing>(sleepPoints[GFRandom(sleepPoints.size(), GetPWorldSeed())]);
    return true;
}