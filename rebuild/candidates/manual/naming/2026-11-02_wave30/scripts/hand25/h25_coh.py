# h25_coh.py: naming wave 25, family coh -- three CTCCreatureOpinionOfHero rows one function off their PDB names, found through the
#  0x00686A80 PeekWorld binder 0x00681980 (DECISIONS_W24_RESOLVED item 7). Ego_r.pdb orders IsWillingToStartFollowingHero /
#  IsWillingToContinueFollowingHero / GetToleranceToBeingHit / GetToleranceToBeingHitRemaining / OnHit (0x0058F950 .. 0x0058FAA0) and
#  FableWin.pdb the same five (0x01F488D0 .. 0x01F48AE0); retail 0x006818A0 / 0x006818F0 (landed as the two IsWilling..) / 0x00681940 /
#  0x00681980 / 0x006819D0 follow that order, and the bodies agree:
#  0x00681940 (override, else manager tolerance + personality) = ?GetToleranceToBeingHit@CTCCreatureOpinionOfHero@@QBEMXZ
#    (landed as GetToleranceToBeingHitRemaining);
#  0x00681980 (the same minus NumberOfTimesHit, `fsub [esi+10h]` on both paths) = ?GetToleranceToBeingHitRemaining@...@@QBEMXZ (landed as
#    CViewBase::CalculateDisplayDistance on a CViewBase / CDisplayEngine model of the same fields, with 0x00686A80 as GetDisplayEngine);
#  0x006819D0 (bool heavy -> bool: count the hit, push the decay frame, report the tolerance used up) = ?OnHit@...@@QAE_N_N@Z (landed as
#    UpdateHitTolerance). No landed row binds any of the three.
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
A = '00681940'
he.edit(A, 'src', '''// CTCCreatureOpinionOfHero::GetToleranceToBeingHitRemaining() const -- the override when set (>= 0), else the reaction manager's
// tolerance for this thing plus the personality's ToleranceToBeingHit
''', '''// CTCCreatureOpinionOfHero::GetToleranceToBeingHit() const -- the override when set (>= 0), else the reaction manager's
// tolerance for this thing plus the personality's ToleranceToBeingHit
// Leaf: ?GetToleranceToBeingHit@CTCCreatureOpinionOfHero@@QBEMXZ
// Naming wave 25: Ego_r.pdb / FableWin.pdb order GetToleranceToBeingHit before GetToleranceToBeingHitRemaining (the latter subtracts
//   NumberOfTimesHit: retail 0x00681980) and OnHit (0x006819D0); this body is the former (was GetToleranceToBeingHitRemaining).
''')
he.edit(A, 'src', '''    float GetToleranceToBeingHitRemaining() const;
};

float CTCCreatureOpinionOfHero::GetToleranceToBeingHitRemaining() const''', '''    float GetToleranceToBeingHit() const;
};

float CTCCreatureOpinionOfHero::GetToleranceToBeingHit() const''')
he.put('00681980', 'src', '''// CTCCreatureOpinionOfHero::GetToleranceToBeingHitRemaining() const -- the tolerance to being hit (the override when set, else the
//   reaction manager's tolerance for this thing plus the personality's) minus the hits counted so far
// Leaf: ?GetToleranceToBeingHitRemaining@CTCCreatureOpinionOfHero@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00681980, 65 bytes (padded to 80).
// Identity (naming wave 25): Ego_r.pdb / FableWin.pdb order GetToleranceToBeingHit / GetToleranceToBeingHitRemaining / OnHit, as retail
//   0x00681940 / 0x00681980 / 0x006819D0; the body is GetToleranceToBeingHit (0x00681940) inlined with `fsub [esi+10h]`
//   (NumberOfTimesHit) on both paths. Was CViewBase::CalculateDisplayDistance over a CViewBase / CDisplayEngine model of the same fields
//   (m_pThing +4 = ParentThing, m_nearOffset +0x10 = NumberOfTimesHit, m_distance +0x14 = ToleranceToBeingHitOverride, m_pTarget +0x20 =
//   PPersonalityDef with +0x50 ToleranceToBeingHit) and 0x00686A80 as CViewBase::GetDisplayEngine: it is
//   ?PeekWorld@CTCBase@@IBEABVCWorld@@XZ (DECISIONS_W24_RESOLVED item 7) and the manager is CWorld +0x74 POpinionReactionManager.
// Callees: PeekWorld 0x00686A80, COpinionReactionManager::GetToleranceToBeingHit(const CThing*) const 0x00701940.
// Shape: as 0x00681940 -- ParentThing is a named local read before the PeekWorld call (edi), the manager a local (its call precedes the
//   push); the personality's tolerance is added only when the def is present.
class CThing;
class COpinionReactionManager { public:
    float GetToleranceToBeingHit(const CThing* thing) const;        // 0x00701940
};
class CWorld { public:
    unsigned char _pad00[0x74];
    COpinionReactionManager* POpinionReactionManager;   // +0x74 CCountedPointer<COpinionReactionManager> data
};
class COpinionPersonalityDef { public:
    unsigned char _pad00[0x50];
    float ToleranceToBeingHit;                     // +0x50
};
class CTCBase
{
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};
class CTCCreatureOpinionOfHero : public CTCBase { public:
    void*   __vftable;                             // +0x00 (CTCBase)
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    long    LastOpinionReactionFrame;              // +0x0c
    float   NumberOfTimesHit;                      // +0x10
    float   ToleranceToBeingHitOverride;           // +0x14
    long    FrameToDecayNumberOfTimesHit;          // +0x18
    void*   PDef;                                  // +0x1c CDefPointer<COpinionOfHeroDef const>
    COpinionPersonalityDef* PPersonalityDef;       // +0x20 CDefPointer<COpinionPersonalityDef const>
    float GetToleranceToBeingHit() const           // 0x00681940 (landed; inline here)
    {
        if (ToleranceToBeingHitOverride >= 0.0f)
            return ToleranceToBeingHitOverride;
        CThing* thing = ParentThing;
        COpinionReactionManager* manager = PeekWorld().POpinionReactionManager;
        float tolerance = manager->GetToleranceToBeingHit(thing);
        if (PPersonalityDef)
            tolerance += PPersonalityDef->ToleranceToBeingHit;
        return tolerance;
    }
    float GetToleranceToBeingHitRemaining() const;
};
float CTCCreatureOpinionOfHero::GetToleranceToBeingHitRemaining() const
{
    return GetToleranceToBeingHit() - NumberOfTimesHit;
}
''')
A = '006819d0'
he.edit(A, 'src', '''// CTCCreatureOpinionOfHero::UpdateHitTolerance(bool heavy) -- count one more hit''', '''// CTCCreatureOpinionOfHero::OnHit(bool heavy) -- count one more hit''')
he.edit(A, 'src', '''// Leaf: UpdateHitTolerance
''', '''// Leaf: ?OnHit@CTCCreatureOpinionOfHero@@QAE_N_N@Z
// Naming wave 25: Ego_r.pdb / FableWin.pdb order GetToleranceToBeingHit / GetToleranceToBeingHitRemaining / OnHit(bool) as retail
//   0x00681940 / 0x00681980 / this row: OnHit (was UpdateHitTolerance); the tail is GetToleranceToBeingHitRemaining 0x00681980 inlined,
//   over GetToleranceToBeingHit 0x00681940 (was spelled GetToleranceToBeingHitRemaining).
''')
he.edit(A, 'src', '''// the world accessor @ 0x00686A80''', '''// CTCBase::PeekWorld @ 0x00686A80''')
he.edit(A, 'src', '''    float GetToleranceToBeingHitRemaining() const  // 0x00681940 (landed; inline here)''', '''    float GetToleranceToBeingHit() const           // 0x00681940 (landed; inline here)''')
he.edit(A, 'src', '''    bool UpdateHitTolerance(bool heavy);
};
bool CTCCreatureOpinionOfHero::UpdateHitTolerance(bool heavy)''', '''    float GetToleranceToBeingHitRemaining() const  // 0x00681980 (landed; inline here)
    {
        return GetToleranceToBeingHit() - NumberOfTimesHit;
    }
    bool OnHit(bool heavy);
};
bool CTCCreatureOpinionOfHero::OnHit(bool heavy)''')
he.edit(A, 'src', '''    return GetToleranceToBeingHitRemaining() - NumberOfTimesHit <= 0.0f;''', '''    return GetToleranceToBeingHitRemaining() <= 0.0f;''')
