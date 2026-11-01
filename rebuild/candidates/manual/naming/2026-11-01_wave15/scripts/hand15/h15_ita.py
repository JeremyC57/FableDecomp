# family decisions (hand): DECISIONS_W14_RESOLVED item 1 -- 0x0097A630 CAIStateGroup_CreatureTakeCover::Update tests its target
# with IS_THING_ALIVE<CIntelligentPointer<CThingCreatureBase> > (Ego_r CAIStateGroup_EnemyCreatureBase +20 PTargetThing is a
# CIntelligentPointer<CThingCreatureBase>); retail folded the identical IS_THING_ALIVE<T> instantiations into 0x006961F0 (ICF),
# so the reloc_bindings row for this spelling carries an `ICF` note. The member takes its proven type; the by-value hand-off
# to AIAction_PerformActionTargetFaceAndWait converts through the template converting constructor 0x0091F990 (the spelling
# 0x009378F0 already binds: CIntelligentPointer<const CThing>(const CIntelligentPointer<CThingCreatureBase>&)).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
A = '0097a630'
edit(A, 'src', """namespace N_0056c860 { struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(CThing* item);                         // 0x0056C860
    CIntelligentPointerConstThing(const CIntelligentPointerConstThing& other);   // 0x0091F990
}; } using N_0056c860::CIntelligentPointerConstThing;""",
"""class CThingCreatureBase;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };   // CIntelligentPointer<T> (no members of its own)
namespace N_0056c860 { struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(CThing* item);                         // 0x0056C860
    template <class U> CIntelligentPointerConstThing(const CIntelligentPointer<U>& other);   // 0x0091F990 (from CIntelligentPointer<CThingCreatureBase>)
}; } using N_0056c860::CIntelligentPointerConstThing;""")
edit(A, 'src', "bool __fastcall IS_THING_ALIVE(const CIntelligentPointerConstThing* pointer);   // 0x006961F0",
     "template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x006961F0 (ICF: the identical IS_THING_ALIVE<T> instantiations share one body)")
edit(A, 'src', "    CIntelligentPointerConstThing PTargetThing; // +0x14",
     "    CIntelligentPointer<CThingCreatureBase> PTargetThing;   // +0x14 (Ego_r CAIStateGroup_EnemyCreatureBase +20)")
edit(A, 'src', "    if (!IS_THING_ALIVE(PTargetThing)) {", "    if (!IS_THING_ALIVE(static_cast<const CBaseIntelligentPointer&>(PTargetThing))) {")
edit(A, 'src', "        if (!IS_THING_ALIVE(&PTargetThing) || !IS_THING_ALIVE(((CThing*)PCoverThing.GetPItem()))) {",
     "        if (!IS_THING_ALIVE<CIntelligentPointer<CThingCreatureBase> >(PTargetThing) || !IS_THING_ALIVE(((CThing*)PCoverThing.GetPItem()))) {")
edit(A, 'src', "//   CIntelligentPointer<const CThing>(CThing*) 0x0056C860 / (const CIntelligentPointer<const CThing>&) 0x0091F990 / dtor",
     "//   CIntelligentPointer<const CThing>(CThing*) 0x0056C860 / (const CIntelligentPointer<CThingCreatureBase>&) 0x0091F990 / dtor")
edit(A, 'src', "GFROR13 0x00497890, IS_THING_ALIVE 0x006961F0 (__fastcall),",
     "GFROR13 0x00497890, IS_THING_ALIVE<CIntelligentPointer<CThingCreatureBase> > 0x006961F0 (__fastcall, ICF-folded),")
