// CAIStateGroup_MinionPutOutFires::IsRunnable() -- with perception: clear the fire target, find the nearest burning thing within the creature's extended sight radius (2D) and latch it; runnable when that fire is alive
// Leaf: ?IsRunnable@CAIStateGroup_MinionPutOutFires@@UAE_NXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the +0x91 dead test through CThing::IsDead() const (Ego_r.pdb
//   ?IsDead@CThing@@QBE_NXZ) keeps retail's branchy test/jne exit instead of a folded flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00935AD0, length 368 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x34 = CTCPerceiveThing, bit 20 of +0x24),
//          CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (temporary CIntelligentPointer<CThing>, vtable 0x01244344),
//          CTCPerceiveThing::GetExtendedSightRadius @ 0x007448F0, CThing::GetPos @ 0x004C73D0,
//          CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000, CThingSearchTools::GetPBestLeafThing<CFilter_Bind2<
//          burning, CThingFilter_In2DRange>, CThingCompare_Nearest, CThing> @ 0x00936070 (the manifest instance name is
//          propagated from an identical body).
// Shape: the radius is a named float read before the search; arguments right to left: the nearest comparator copies
//          the position first, then the 2D range filter copies it again with the radius; the bind holds both filters
//          by reference; the target pointer is assigned through frame temporaries.
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04) with the fire
//          intelligent pointer at +0x14.
// Name fix (agentNF23): SetPItem takes the PDB CBaseObject* (Ego_r.pdb / FableWin.pdb ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z, 0x00A01B90); the argument is cast to CBaseObject* as CIntelligentPointer<T>::SetPointer does.
#include "fable_compiler.h"
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#include "engine/C3DVector.h"

class CTCPerceiveThing { public:                       // interface 0x34
    float GetExtendedSightRadius();             // 0x007448F0
};

struct CThingFilter_IsBurning {                 // descriptive: the empty first filter
    CThingFilter_IsBurning() { }
};
class CThingFilter_In2DRange { public:
    C3DVector Centre;
    float Range;
    CThingFilter_In2DRange(const C3DVector& centre, float range) : Centre(centre), Range(range) { }
};
template <class A, class B> class CFilter_Bind2 { public:
    const A& First;
    const B& Second;
    CFilter_Bind2(const A& a, const B& b) : First(a), Second(b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
class CThingCompare_Nearest { public:
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
class CThingSearchTools { public:
    bool GetPBestLeafThing(const CFilter_Bind2<CThingFilter_IsBurning, CThingFilter_In2DRange>& filter,
                           const CThingCompare_Nearest& compare, CThing** out) const;   // 0x00936070 (stand-in below)
};
long NearSink_PutOutFires(const void* self, long seen);   // stand-in: the search the real body runs
#pragma auto_inline(off)
// Same-TU stand-in for GetPBestLeafThing (0x00936070, an out-of-line COMDAT template instance): it reads its inputs and
// writes the out pointer only, so VC7.1 sees the filter/comparator/out addresses do not escape (landed 0x00936A60 idiom).
__declspec(noinline) bool CThingSearchTools::GetPBestLeafThing(const CFilter_Bind2<CThingFilter_IsBurning, CThingFilter_In2DRange>& filter,
                                                              const CThingCompare_Nearest& compare, CThing** out) const
{
    *out = 0;
    long best = NearSink_PutOutFires(this, *(const long*)&compare + *(const long*)&filter.Second.Range);
    if (best)
        *(long*)out = best;
    return *out != 0;
}
#pragma auto_inline(on)

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();   // slot 0 (retail scalar deleting dtor; was placeholder v00); 0x00A01C10
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    void SetPItem(CBaseObject* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    FABLE_FORCEINLINE CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    FABLE_FORCEINLINE CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem((CBaseObject*)item); return *this; }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
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

struct CAIStateGroup_MinionPutOutFires : CAIStateGroupBase {
    CIntelligentPointerThing PFire;             // +0x14
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionPutOutFires::IsRunnable()
{
    CTCPerceiveThing* perceive;
    if (GetCreature()->GetTC(TCI_PERCEIVE_THING, &perceive)) {
        CThing* fire = 0;
        PFire = CIntelligentPointerThing(0);
        float radius = perceive->GetExtendedSightRadius();
        if (PeekThingSearchTools().GetPBestLeafThing(
                CThingFilter_IsBurning() && CThingFilter_In2DRange(((CThing*)(GetCreature()))->GetPos(), radius),
                CThingCompare_Nearest(((CThing*)(GetCreature()))->GetPos()), &fire)) {
            PFire = CIntelligentPointerThing(fire);
            if (IS_THING_ALIVE(PFire))
                return true;
        }
    }
    return false;
}