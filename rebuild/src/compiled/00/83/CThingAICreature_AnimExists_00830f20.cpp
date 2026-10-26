// CThingAICreature::AnimExists(const CCharString& name, bool currentModeOnly) const -- looks the animation name up in the creature def's animation table with the current mode's filter; failing that, unless restricted to the current mode, with the no-group filter
// Leaf: ?AnimExists@CThingAICreature@@MBE_NABVCCharString@@_N@Z
// Identity (sweep3, FINDINGS chunk 481 "AnimExists"; was the descriptive HasAnimation_slot73, manifest vfunc_73): Ego_r.pdb /
//   FableWin.pdb ?AnimExists@CThingAICreature@@MBE_NABVCCharString@@_N@Z -- the only CThingAICreature (and CThingCreatureBase)
//   virtual with this (const CCharString&, bool) -> bool signature -- overriding the public
//   ?AnimExists@CThingCreatureBase@@UBE_NABVCCharString@@_N@Z: slot 73 is this body in the CThingAICreature vtable 0x0127293C and
//   the base body 0x00662CC0 in the CThingPlayerCreature vtable 0x012457FC (CThingPlayerCreature has no AnimExists of its own).
//   Protected virtual const as in the PDB; the vptr shifts the modelled padding (PBaseDef stays at +0x70). The table
//   search 0x005DC2E0 is Ego_r ?AnimExists@CAnimationSet@@QBE_NABVCCharString@@ABVCAnimFilterBase@@@Z (its label; was the
//   descriptive CAnimationTable::Find taking the filter by pointer).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830F20, 181 bytes (+11 int3). `ret 8`, bool in al. CThingAICreature vtable 0x0127293C slot 73 (+0x124).
//         Catalog name CThingAICreature::vfunc_73.
// Callees: CDefClassBase::GetSubDef(CDefPointer<T>*) 0x0042B0A2 on the base def (manifest "GetSubDef<CAbilityDef>" is a propagated
//          instantiation), the mode manager's current mode 0x0073A6E0 (manifest "CPixelShader::CountReferences" propagated; landed
//          as GetCurrentMode), CMode virtual slot 4 (+0x10, the animation filter), the def table's find 0x005DC2E0 (manifest
//          "CAnimationDef_FindAnimationByName" propagated), CDefClassBase virtual slot 1 (Delete) on the def copy's last reference.
//         Vtables: CAnimFilterNoGroup 0x0122FFDC, CAnimFilterBase 0x0122FFCC.
// Shape: the def holder (frame word, zeroed first) is released at each of the three exits (null test, folded decrement, Delete);
//        the no-group filter temporary is built in the second argument's dead home slot; the table address (def+0x34) stays in edi.
//        GetSubDef gets a same-TU `auto_inline(off)` stand-in body that writes only the out slot: VC7.1 then knows the
//        holder does not escape and keeps the def in esi across the mode/filter/find calls (an extern declaration
//        re-reads the holder for each release). The fallback is an `if (!currentModeOnly) { ... }` block so the false
//        return is laid out last, as retail.
// Layout: CThing PBaseDef +0x70; the +0x134 pointer is the mode manager (descriptive); the sub-def's animation table at +0x34.
class CCharString { public: void* PStringData; };
class CAnimFilterBase {
public:                             // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() { }
    virtual void v01();
    virtual void v02();
};
class CAnimFilterNoGroup : public CAnimFilterBase {
public:        // vtable 0x0122FFDC
    CAnimFilterNoGroup() { }
};
class CAnimationSet {                                // Ego_r CAnimationSet: the creature def's animation set
public:
    bool AnimExists(const CCharString& name, const CAnimFilterBase& filter) const;   // 0x005DC2E0
};
struct CDefClassBase {
    virtual void v00();
    virtual void Delete();                           // slot 1 (+0x04)
    long RefCount;                                   // +0x04
    bool GetSubDef(void* out) const;                 // 0x0042B0A2
};
struct CCreatureDef : CDefClassBase {                // descriptive
    unsigned char   _pad08[0x34 - 0x08];
    CAnimationSet   Animations;                      // +0x34
};
struct CDefPointer {                                 // CDefPointer<const CCreatureDef>
    CCreatureDef* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        CCreatureDef* old = Object;
        if (old) {
            if (--old->RefCount == 0)
                old->Delete();
        }
    }
};
void* FindSubDef_CDefClassBase_GetSubDef(const CDefClassBase* def);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(void* out) const
{
    void* def = FindSubDef_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

struct CMode {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual const CAnimFilterBase* GetAnimFilter_slot4();   // slot 4 (+0x10)
};
struct CModeManager {                                // descriptive
    CMode* GetCurrentMode();                         // 0x0073A6E0
};
class CThingAICreature {
public:
    // vptr +0x00 (AnimExists is vtable slot 73)
    unsigned char _pad04[0x70 - 4];
    CDefClassBase* PBaseDef;                         // +0x70 CDefPointer<const CThingBaseDef>::Object
    unsigned char _pad74[0x134 - 0x74];
    CModeManager* m_pModeManager;                    // +0x134 (descriptive)
protected:
    virtual bool AnimExists(const CCharString& name, bool currentModeOnly) const;   // slot 73
};
bool CThingAICreature::AnimExists(const CCharString& name, bool currentModeOnly) const
{
    CDefPointer def;
    PBaseDef->GetSubDef(&def);
    if (def.Object->Animations.AnimExists(name, *m_pModeManager->GetCurrentMode()->GetAnimFilter_slot4()))
        return true;
    if (!currentModeOnly) {
        CAnimFilterNoGroup noGroup;
        return def.Object->Animations.AnimExists(name, noGroup);
    }
    return false;
}