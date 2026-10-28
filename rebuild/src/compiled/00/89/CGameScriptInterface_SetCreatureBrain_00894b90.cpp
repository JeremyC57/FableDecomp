// CGameScriptInterface::SetCreatureBrain(const CScriptThing& creature, const CCharString& brainName) const -- script call: looks the brain def up by name and installs it on the creature (thing type 1) through CThingAICreature::SetNewBrain
// Leaf: ?SetCreatureBrain@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z
// Retail: Fable.exe @ 0x00894B90, 95 bytes, thiscall `ret 8`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 589 holds this body; slot 589 is
//   ?SetCreatureBrain@CGameScriptInterface@@UBEXABVCScriptThing@@ABVCCharString@@@Z (ghidra_out/cgsi_slots_aligned.tsv).
// Body: CDefPointer<const CBrainDef> brain (null); if DefinitionManager (+0x10)->GetDef<CBrainDef>(brainName, &brain)
//   (0x006D3E80): t = creature.GetThing() (CScriptThing slot 11); if t and its type byte (+0x10) is 1:
//   ((CThingAICreature*)t)->SetNewBrain(brain) (0x00833010, the def pointer copied into its stack slot with an add-ref);
//   then ~brain (release at +4, the def's virtual slot 1 at zero).
// Callee names (PDB, both sets): 0x006D3E80 ??$GetDef@VCBrainDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCBrainDef@@@@@Z
//   (the manifest's GetDef<CHeroPostcardGeneratorDef> is a propagated label: the def pointer feeds SetNewBrain), 0x00833010
//   ?SetNewBrain@CThingAICreature@@QAEXV?$CDefPointer@$$CBVCBrainDef@@@@@Z. The thing-type byte value 1 is not named in
//   the name sets (descriptive comparison).
class CCharString;
class CBrainDef {
public:
    virtual void v00();
    virtual void Destroy();                              // slot 1 (+0x04)
    long RefCount;                                       // +0x04
};
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++const_cast<CBrainDef*>(Object)->RefCount;
    }
    ~CDefPointer()
    {
        if (Object && --const_cast<CBrainDef*>(Object)->RefCount == 0)
            const_cast<CBrainDef*>(Object)->Destroy();
    }
};
class CDefinitionManager {
public:
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* out) const;   // 0x006D3E80 <CBrainDef>
};
// Same-TU stand-in body for the out-of-line GetDef (the landed 0x008338D0 idiom): it writes the out slot only, so VC7.1
// knows the def pointer's address does not escape and keeps brain.Object in esi across the calls, as retail does.
const void* FindDef_SetCreatureBrain_StandIn(const CCharString& name);   // stand-in callee of the stand-in body
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<const T>* out) const
{
    const T* def = (const T*)FindDef_SetCreatureBrain_StandIn(name);
    if (!def)
        return false;
    out->Object = def;
    return true;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char Type;                                  // +0x10 (1 here: an AI creature)
};
class CThingAICreature : public CThing {
public:
    void SetNewBrain(CDefPointer<const CBrainDef> brain);   // 0x00833010
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
};
class CGameScriptInterface {
public:
    virtual void SetCreatureBrain(const CScriptThing& creature, const CCharString& brainName) const;
    void*               World;                           // +0x04
    void*               Component;                       // +0x08
    void*               DisplayEngine;                   // +0x0c
    CDefinitionManager* DefinitionManager;               // +0x10
};
void CGameScriptInterface::SetCreatureBrain(const CScriptThing& creature, const CCharString& brainName) const
{
    CDefPointer<const CBrainDef> brain;
    if (DefinitionManager->GetDef(brainName, &brain))
    {
        CThing* t = creature.GetThing();
        if (t && t->Type == 1)
            static_cast<CThingAICreature*>(t)->SetNewBrain(brain);
    }
}