// CTCWillLightning::GetTargets(std::list<CCountedPointer<CLightningBeam> >&, long, CThing*, std::list<CThing*>&, float, float, float, bool) -- pick the lightning targets around a source thing (the player's targeted or override thing, else the living, lightningable, hostile creatures in range sorted nearest first) and either retarget every beam of the list at each new target or add one new beam per target up to the beam limit; each target is appended to the targeted list; true when any beam was set
// Leaf: ?GetTargets@CTCWillLightning@@AAE_NAAV?$list@V?$CCountedPointer@VCLightningBeam@CTCWillLightning@@@@V?$allocator@V?$CCountedPointer@VCLightningBeam@CTCWillLightning@@@@@std@@@std@@JPAVCThing@@AAV?$list@PAVCThing@@V?$allocator@PAVCThing@@@std@@@3@MMM_N@Z
// Inline-budget: `#pragma inline_depth(0)` around two statements of the new-beam arm -- retail CALLS GFRandom (0x004CDAE0) and
//   list<CCountedPointer<CLightningBeam> >::push_back (0x007A3870) there (0x007A12FA / 0x007A1310) while it EXPANDS GFRandom in the retarget
//   loop (0x007A11B9..0x007A11D8) and list<CThing*>::push_back in both arms, and expands vector<CThing*>::push_back at its first site
//   (0x007A0EFC calls _M_insert_overflow) but calls it at the second (0x007A0F20). c2 inline trace (tmp/inl, c2.dll 0x1078D1AA): the
//   new-beam sites sit at static frequency 0.536 (GFRandom c1 42 sc 47, CCountedPointer push_back c1 51 sc 57, list<CThing*> push_back c1 37
//   sc 41) and the first vector push_back at 0.199 (c1 39 sc 61, expanded in retail): no uniform /d2inlT<N> splits them (N > 61 and N <= 47).
//   Plain-C++ shapes traced first (18 variants, no grade spent): for / while / for(;;) / do-while / flag-condition target searches, `||` and
//   nested-if forms of the skip tests, if / else / continue forms of the two arms, both list size() spellings -- the new-beam sites only ever
//   took 0.536 or 0.0715 (retail's split needs 0.125 < f <= 0.158). The expression's sum is formed inside the region (`call ; add eax,
//   [edi+0DCh]`, as retail).
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x007A0D90, 1517 bytes, `ret 20h`, 8-aligned ebp frame (the 8-byte CIntelligentPointer<CThing> and
//   CCountedPointer locals of the loops). Ego_r.pdb / FableWin.pdb ?GetTargets@CTCWillLightning@@AAE_NAAV?$list@..@std@@JPAVCThing@@AAV?$list@PAVCThing@@..@3@MMM_N@Z
//   (private); callers InitialiseBeams 0x007A16F0 and MakeRandomBeam 0x007A1870 (landed, same TU).
// Callees (each checked against retail with tmp/rel.py): CThing::GetPos 0x004C73D0, __ftol2 0x00BFEA70, CBaseIntelligentPointer ctor 0x00A01B10 /
//   GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CVectorMap::LowerBound 0x0040F020 (GetTC<CTCTargetingBase>, interface 8),
//   CTCTargetingBase::GetPTargetedThing (vtable slot 33, +0x84), NTargeting::IsThingVulnerableToMeleeStrikesFromThing 0x007F59F0,
//   NCreatureMode::IsLightningable 0x0085FAA0 / IsUnderground 0x0085F120, CTCEnemy::AreThingsEnemies 0x0076D2F0, vector<CThing*>
//   _M_insert_overflow 0x00436AC0 / push_back 0x00436C80, CTCBase::GetThingSearchTools 0x00686D80 / GetWorld 0x00686A70,
//   CThingSearchTools::GetAllThingsInRoughArea<CThingCreatureBase, vector<CThing*>, CFilter_Bind4<IsNot, IsNot, IsTargetable, In2DRange> >
//   0x007A3CA0 (same TU), std::sort<CThing**, CThingCompare_NearestToPos> 0x007A4260 (__fastcall), CWorld::GetWorldSeed 0x0049D810,
//   GFFloatRandom 0x004978A0, GFROR13 0x00497890, GFRandom 0x004CDAE0, malloc 0x00BFEA0E / free 0x00BFEA14, operator new 0x00BFEA1A,
//   CLightningBeam(CThing*, CThing*) 0x007A0630, CCountedPointer<CLightningBeam>(CLightningBeam*) 0x007A4210 / ~CCountedPointer 0x007A3200,
//   list<CCountedPointer<CLightningBeam> >::push_back 0x007A3870. Vtable CIntelligentPointer<CThing> 0x01238C6C.
// Shape: `float area = m_fAreaEffect;` copied first (the integer copy to the frame, 0x007A0D9E), the box from GetPos().ToC2DCoordI() (X then Y)
//   and (long)area; `else return false` when neither the override nor the targeted thing is alive (the one shared `xor al,al` exit);
//   the filters as GFFilter_Bind4 temporaries; CThing type 2 (the player creature) read through CClass::GetType; the CIntelligentPointer
//   member assignments through the ASSIGN opt-in (GetPItem then SetPItem); the counted pointer's ctor / dtor visible but called
//   (VISIBLE_CALLED_CTOR / _DTOR: retail calls the same-TU COMDATs).
// Layout: shared headers plus the chunk 2015 overlay (hdr/README.md): the retail CTCWillLightning class (CTCWILLLIGHTNING_CLASS), new
//   CThingFilter_IsTargetable / CFilter_Bind4 headers.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_DTOR
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_CTCWILLLIGHTNING_CLASS
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_NCREATUREMODE_LIGHTNINGABLE
#define FABLE_RETAIL_NTARGETING_MELEE
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorld.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_IsTargetable.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/CFilter_Bind4.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCWillLightning.h"
#include "engine/ThingAlive.h"
#include "engine/GFRandom.h"
#include "engine/GFFloatRandom.h"
#include "engine/CTCEnemy.h"
#include "engine/NCreatureMode.h"
#include "engine/NTargeting.h"
#include "engine/CThingCompare_NearestToPos.h"


bool CTCWillLightning::GetTargets(std::list<CCountedPointer<CLightningBeam> >& beams, long max_beams, CThing* source,
                                  std::list<CThing*>& targets, float damage_per_hit, float decapitation_time, float delay,
                                  bool retarget_beams)
{
    float area = m_fAreaEffect;
    C2DBoxI box(source->GetPos().ToC2DCoordI(), (long)area);
    std::vector<CThing*> things;
    CThing* target = 0;
    if (ParentThing->GetType() == 2 && source == m_cbpPlayer.Get()) {
        CTCTargetingBase* targeting;
        m_cbpPlayer->GetTC(&targeting);
        CThing* targeted = targeting->GetPTargetedThing();
        if (!IS_THING_ALIVE(targeted))
            return false;
        if ((targeted->GetTypeFlags() & 8) && !NTargeting::IsThingVulnerableToMeleeStrikesFromThing(*targeted, *m_cbpPlayer.Get()))
            return false;
        if (!NCreatureMode::IsLightningable(*targeted))
            return false;
        target = targeted;
    }
    if (source == m_cbpPlayer.Get()) {
        if (IS_THING_ALIVE(PTargetOverride))
            things.push_back(PTargetOverride.Get());
        else if (IS_THING_ALIVE(target))
            things.push_back(target);
        else
            return false;
    } else {
        GetThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(box,
            GFFilter_Bind4(CThingFilter_IsNot(*source), CThingFilter_IsNot(*m_cbpPlayer.Get()), CThingFilter_IsTargetable(false, true),
                           CThingFilter_In2DRange(source->GetPos(), area)), things);
    }
    std::sort(things.begin(), things.end(), CThingCompare_NearestToPos(source->GetPos()));
    bool result = false;
    for (std::vector<CThing*>::iterator it = things.begin(); it != things.end(); ++it) {
        CThing* thing = *it;
        bool already_targeted = false;
        for (std::list<CThing*>::iterator t = targets.begin(); t != targets.end(); ++t) {
            if (*t == thing) {
                already_targeted = true;
                break;
            }
        }
        if (NCreatureMode::IsUnderground(*thing))
            continue;
        if (!NCreatureMode::IsLightningable(*thing))
            continue;
        if (thing != target && !CTCEnemy::AreThingsEnemies(*ParentThing, *thing))
            continue;
        if (IS_THING_ALIVE(target) && target != thing && CTCEnemy::AreThingsEnemies(*target, *thing))
            continue;
        if (already_targeted)
            continue;
        if (retarget_beams) {
            for (std::list<CCountedPointer<CLightningBeam> >::iterator b = beams.begin(), end = beams.end(); b != end; ++b) {
                result = true;
                (*b)->m_tcpTarget = thing;
                (*b)->m_tcpHome = source;
                (*b)->m_fDamagePerHit = damage_per_hit;
                (*b)->m_fDecapitationTime = GFFloatRandom(1.0f, GetWorld().GetWorldSeed()) + decapitation_time;
                (*b)->m_fDelay = delay;
                (*b)->m_fActivationTime = m_fActivationTime;
                (*b)->m_fDeActivationTime = m_fDeActivationTime;
                (*b)->PDef = m_lspDef;
                (*b)->m_OnhitEveryFrame = m_OnhitEveryFrame + GFRandom(m_OnhitEveryFrame, GetWorld().GetWorldSeed());
                (*b)->m_bTemporary = false;
                (*b)->m_nFrameCount = -1;
                targets.push_back(thing);
            }
        } else if (beams.size() < (unsigned long)max_beams) {
            CCountedPointer<CLightningBeam> beam(new CLightningBeam(source, thing));
            beam->m_fDamagePerHit = damage_per_hit;
            beam->m_fDecapitationTime = GFFloatRandom(1.0f, GetWorld().GetWorldSeed()) + decapitation_time;
            beam->m_fDelay = delay;
            beam->m_fActivationTime = m_fActivationTime;
            beam->m_fDeActivationTime = m_fDeActivationTime;
            beam->PDef = m_lspDef;
#pragma inline_depth(0)
            long onhit_every_frame = GFRandom(m_OnhitEveryFrame, GetWorld().GetWorldSeed()) + m_OnhitEveryFrame;
#pragma inline_depth()
            beam->m_OnhitEveryFrame = onhit_every_frame;
#pragma inline_depth(0)
            beams.push_back(beam);
#pragma inline_depth()
            targets.push_back(thing);
            result = true;
        }
    }
    return result;
}
