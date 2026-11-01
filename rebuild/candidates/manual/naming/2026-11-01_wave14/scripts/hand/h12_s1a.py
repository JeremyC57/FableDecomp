# sub-wave 1a (hand): small single-callee families. HE_OV=<overlay>
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, edit_all, resub
# ---- 0x00A14F70 ?GetNodeAt@CNavigatorManager@@QBEPBVCANavQuadTreeNode@@ABVC2DVector@@E@Z (Ego_r + FableWin; landed 0x00A15140 /
#      0x00A1AF00 define / call it so): const member, const node*, unsigned char nav layer.
GN = 'const CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer) const;   // 0x00A14F70 (Ego_r.pdb, const)'
# rows whose node type is a row-local struct: the struct is the PDB class CANavQuadTreeNode
for a in ('00a199f0', '00a19a30', '00a1a210'):
    resub(a, 'src', r'\bstruct CANavQuadTreeNode_%s \{' % a, 'class CANavQuadTreeNode { public:', 1)
    resub(a, 'src', r'\bCANavQuadTreeNode_%s\b' % a, 'CANavQuadTreeNode')
    edit(a, 'src', 'CANavQuadTreeNode* GetNodeAt(const C2DVector& position, bool layer);   // 0x00A14F70', GN)
edit('00a199f0', 'src', '    CANavQuadTreeNode* node = m_pManager->GetNodeAt(to, false);', '    const CANavQuadTreeNode* node = m_pManager->GetNodeAt(to, false);')
edit('00a1a210', 'src', '    CANavQuadTreeNode* node = m_pManager->GetNodeAt(*position, layer);', '    const CANavQuadTreeNode* node = m_pManager->GetNodeAt(*position, layer);')
# GetMyNodeAt rows (two retail copies of one PDB name: the leaf stays, see DEFERRED): the callee only
for a in ('00a1a1d0', '00a1aec0'):
    edit(a, 'src', 'struct CANavQuadTreeNode_%s {' % a, 'class CANavQuadTreeNode;                        // Ego_r.pdb node class (GetNodeAt 0x00A14F70 returns it)\nstruct CANavQuadTreeNode_%s {' % a)
    edit(a, 'src', 'CANavQuadTreeNode_%s* GetNodeAt(const C2DVector& position, bool layer);   // 0x00A14F70' % a, GN)
    edit(a, 'src', '    return m_pManager->GetNodeAt(*position, m_pOwner->GetLayerAt(position));', '    return (CANavQuadTreeNode_%s*)m_pManager->GetNodeAt(*position, m_pOwner->GetLayerAt(position));' % a)
# rows with a CANavQuadTreeNode class already: the declaration, and the non-const locals take the result through a cast
for a, old in (('00a1ab40', 'CANavQuadTreeNode* GetNodeAt(const C2DVector& position, bool layer);   // 0x00A14F70'),
               ('00a1bee0', 'CANavQuadTreeNode* GetNodeAt(const C2DVector& position, bool layer);   // 0x00A14F70'),
               ('00a1df60', 'CANavQuadTreeNode* GetNodeAt(const C2DVector& position, bool layer);   // 0x00A14F70'),
               ('00a1bca0', 'CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer);   // 0x00A14F70'),
               ('00787620', 'CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer);   // 0x00A14F70'),
               ('00a472f0', 'CANavQuadTreeNode* GetNodeAt(const C2DVector& position, unsigned char layer);   // 0x00A14F70'),
               ('00a1e370', 'CANavQuadTreeNode* GetNodeAt(const C2DVector& position, long layer);   // 0x00A14F70')):
    edit(a, 'src', old, GN)
    resub(a, 'src', r'(=\s*)((?:NavigationManager|navigator)->GetNodeAt\()', r'\1(CANavQuadTreeNode*)\2')
# ---- 0x00662820 ?IsAnimCombinationAnim@CThingCreatureBase@@QBE_NJ@Z (Ego_r + FableWin const member)
resub('00692a20', 'src', r'\bCThingCreatureBase_Calls\b', 'CThingCreatureBase')
edit('00692a20', 'src', '    bool IsAnimCombinationAnim(long animationIndex);   // 0x00662820', '    bool IsAnimCombinationAnim(long animationIndex) const;   // 0x00662820 (Ego_r.pdb, const)')
edit('00692a20', 'src', 'class CThingCreatureBase { public:         // engine/CThingCreatureBase.h exists but is not used: only this call is modelled',
     'class CThingCreatureBase { public:               // engine/CThingCreatureBase.h exists but is not used: only this call is modelled')
edit('00694d10', 'src', '    bool IsAnimCombinationAnim(long animation); // 0x00662820 (CThingCreatureBase)\n', '')
edit('00694d10', 'src', '''class CBaseObject;
class CBaseObjectPointer;''', '''class CThingCreatureBase : public CThing { public:   // the creature the action belongs to (Ego_r.pdb CThingCreatureBase derives from CThing)
    bool IsAnimCombinationAnim(long animation) const;   // 0x00662820 (Ego_r.pdb, const)
};
class CBaseObject;
class CBaseObjectPointer;''')
edit('00694d10', 'src', '        if (!creature->IsAnimCombinationAnim(entry->Animation)) {', '        if (!static_cast<CThingCreatureBase*>(creature)->IsAnimCombinationAnim(entry->Animation)) {')
# ---- 0x006A4E40 ?GetHealthPercentage@CThingPhysical@@QBEMXZ (Ego_r + FableWin; Health / MaxHealth)
CTP = 'class CThingPhysical { public: float GetHealthPercentage() const; };   // 0x006A4E40 (Ego_r.pdb CThingPhysical, const)\n'
edit('008ecac0', 'src', '    float GetHealthPercentage();                            // 0x006A4E40', '    float GetHealthPercentage() const;                      // 0x006A4E40 (Ego_r.pdb, const)')
edit('006a9d10', 'src', '    float GetNormalisedValue_006a4e40();        // 0x006A4E40\n', '')
edit('006a9d10', 'src', 'class CThingPlayerCreature {\npublic:', CTP + 'class CThingPlayerCreature : public CThingPhysical {   // (the thing is a CThingPhysical: the health fraction is its base accessor)\npublic:')
edit('006a9d10', 'src', '    float value = GetNormalisedValue_006a4e40();', '    float value = GetHealthPercentage();')
edit('0082d1d0', 'src', '    float GetNormalisedTime();                       // 0x006A4E40 (manifest label propagated)\n', '')
edit('0082d1d0', 'src', 'class CThingBuilding { public:', CTP + 'class CThingBuilding : public CThingPhysical { public:   // (the building is a CThingPhysical: the health fraction is its base accessor)')
edit('0082d1d0', 'src', '    float time = GetNormalisedTime();', '    float time = GetHealthPercentage();')
edit('008903d0', 'src', 'extern float __fastcall Sub_GetHealthPct(CThingPlayerCreature* hero);', CTP.rstrip('\n'))
edit('008903d0', 'src', '        float v = Sub_GetHealthPct(hero);', '        float v = ((CThingPhysical*)hero)->GetHealthPercentage();')
# ---- 0x0070C4E0 ?GetFirstCombinationAnimFromType@CAnimateThing@@QAEPAVCAnimSet@@J@Z (Ego_r + FableWin; FindCombinationAnim is in no PDB)
edit('00728ff0', 'src', '    CCombinationAnim* FindCombinationAnim(long type);   // 0x0070C4E0', '    CAnimSet* GetFirstCombinationAnimFromType(long type);   // 0x0070C4E0 (Ego_r.pdb)')
edit('00728ff0', 'src', 'struct CCombinationAnim {', 'class CAnimSet;\nstruct CCombinationAnim {')
edit('00728ff0', 'src', '        CCombinationAnim* blink = animate.FindCombinationAnim(0xd);', '        CCombinationAnim* blink = (CCombinationAnim*)animate.GetFirstCombinationAnimFromType(0xd);')
edit('008e9710', 'src', '    CAnimSet* FindCombinationAnim(long type);   // 0x0070C4E0', '    CAnimSet* GetFirstCombinationAnimFromType(long type);   // 0x0070C4E0 (Ego_r.pdb)')
edit_all('008e9710', 'src', 'animator.FindCombinationAnim(', 'animator.GetFirstCombinationAnimFromType(', 2)
edit('008e9a30', 'src', 'struct AnimateThingMethods {CAnimSet*GetFirstCombinationAnimFromType(long);void RemoveCombinationAnimsOfType(long);}; //70C4E0,70D8C0',
     'class CAnimateThing { public: CAnimSet* GetFirstCombinationAnimFromType(long); void RemoveCombinationAnimsOfType(long); }; //70C4E0,70D8C0 (Ego_r.pdb CAnimateThing)')
edit('008e9a30', 'src', 'struct AnimationComponentMethods {AnimateThingMethods&GetAnimation();}; //70B460, actual CTCAnimationComplex +C',
     'class CTCAnimationComplex { public: CAnimateThing& GetAnimation(); }; //70B460 (Ego_r.pdb CTCAnimationComplex), +C')
resub('008e9a30', 'src', r'\bAnimateThingMethods\b', 'CAnimateThing')
resub('008e9a30', 'src', r'\bAnimationComponentMethods\b', 'CTCAnimationComplex')
