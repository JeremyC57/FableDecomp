// CThingCreatureBase::UseAbility(long abilityDefIndex, float buttonPressure) -- pass the button pressure to the ranged-attacks component (if any), fetch the ability's def and hand it to the creature's virtual ability handler
// Grade: DIFFER(133v135) -- lane gap: retail loads the component through `mov eax,[eax+4] ; mov ecx,eax` (ours `mov ecx,[eax+4]`) and keeps this in edi / the map and def in esi (ours swap esi/edi); the dead-argument-slot def pointer and the holder release already match
// Retail: Fable.exe (TLC, VC7.1) @ 0x00665510, 135 bytes (`ret 8`).
// Callees: CInterfaceMap::LowerBound @ 0x0040F020, CTCHeroRangedAttacks::SetButtonPressure(float) @ 0x007899E0,
//   CThing::GetDefinitionManager @ 0x004C7990 (label "CViewBase::GetDisplayEngine" propagated),
//   CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) @ 0x0043FFA0 (byte-twin label
//   GetDef<CThingPhysicalSwitchDef>), CThingCreatureBase virtual slot 72 (+0x120, 0x0066A4E0, manifest "vfunc_72").
// Layout: retail CThing: interface bitset +0x20 (TC 0x81 = CTCHeroRangedAttacks: word +0x30 bit 1), CVectorMap +0x44.
//   The def is ref counted (count +4, virtual Destroy slot 1).
// Shape: the def pointer lives in the dead pressure argument slot; its release is the inline CDefPointer dtor
//   (`dec [esi+4] ; jne ; call [vtbl+4]`).
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCHeroRangedAttacks {
    void SetButtonPressure(float pressure);     // 0x007899E0
};
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
};
struct CHeroAbilityDef : CDefClassBase { };
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template<class T> struct CDefHolder {          // releases a def taken out of a CDefPointer slot (kept in one register)
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --((CDefClassBase*)Object)->RefCount == 0)
            ((CDefClassBase*)Object)->Destroy();
    }
};
struct CDefinitionManager {
    template<class T> bool GetDef(long index, CDefPointer<const T>* out);   // 0x0043FFA0
};

struct CThing {
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    CDefinitionManager* GetDefinitionManager(); // 0x004C7990
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};

struct CThingCreatureBase : CThing {
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60();
    virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
    virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void UseAbilityDef(const CHeroAbilityDef* def);    // slot 72 (+0x120) 0x0066A4E0
    void UseAbility(long abilityDefIndex, float buttonPressure);
    __forceinline void UseAbilityByIndex(long abilityDefIndex)
    {
        CDefPointer<const CHeroAbilityDef> out;
        GetDefinitionManager()->GetDef(abilityDefIndex, &out);
        CDefHolder<const CHeroAbilityDef> def(out.Object);
        UseAbilityDef(def.Object);
    }
};

void CThingCreatureBase::UseAbility(long abilityDefIndex, float buttonPressure)
{
    if (HasTC(0x81)) {
        CTCHeroRangedAttacks* ranged = GetTC<CTCHeroRangedAttacks>(0x81);
        ranged->SetButtonPressure(buttonPressure);
    }
    UseAbilityByIndex(abilityDefIndex);
}
