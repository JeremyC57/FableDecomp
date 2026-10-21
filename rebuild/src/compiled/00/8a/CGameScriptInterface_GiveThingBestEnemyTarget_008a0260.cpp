// CGameScriptInterface::GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const -- point a valid thing's AI targeting (TC 8, when it is a CTCTargetingAI) at the target creature, add the target to its enemy list (TC 0x49), and when the target changed for the player creature notify it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A0260, length 397 bytes (`ret 8`).
// Callees: the script things' slots 75 (+0x12C IsValid) and 11 (+0x2C GetThing), the component-map LowerBound @ 0x0040F020,
//   the targeting component's slots 22 (+0x58 GetClassTypeName, CCharString by value) and 29 (+0x74, current target),
//   CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0, the string compare @ 0x00411570, CTCTargetingAI::SetScriptTargetOverride
//   @ 0x008ABBA0 (manifest "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated; name from the landed
//   ClearThingBestEnemyTarget 0x008A03F0), the TC 0x49 enemy-list add @ 0x0076D1B0, the player-creature notify @ 0x006D5AB0.
// Layout: CThing type byte +0x10, UsingComponents +0x20, component map +0x44, flags +0x6C (bit 3 = creature).
// Shape: a same-TU LowerBound stand-in packs the key, literal and class-name result into the dead argument slots as retail;
//   if/else SetScriptTargetOverride calls; the class-name compare is the landed IsActionRunning 0x00712D70 operator== (nested SameText), its bool in bl
//   across the two string dtors; the previous target is a frame word zeroed before the first GetThing.
struct CThing_Retail;
struct CCharStringData {
    char*         PData;                    // +0x00
    unsigned long StringLength;             // +0x04
};
long __fastcall CompareStrings(const char* a, const char* b);   // 0x00411570
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    bool SameText(const CCharString& other) const
    {
        if (PStringData->StringLength != other.PStringData->StringLength)
            return false;
        return CompareStrings(PStringData->PData, other.PStringData->PData) == 0;
    }
    __forceinline bool operator==(const CCharString& other) const     // the landed IsActionRunning 0x00712D70 form
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return SameText(other);
        return false;
    }
};
struct CTCBase_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual CCharString GetClassTypeName() const;       // slot 22 (+0x58)
    virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing_Retail* GetCurrentTarget();          // slot 29 (+0x74)
};
struct CInterfaceMap_Retail {
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (STLport lower_bound over the 8-byte entries):
// compiled above the caller so VC7.1 knows the key's address does not escape and packs the frame words as retail
// (the chunk 242 / landed GetItemDefNamesFromContainer 0x008A8F20 idiom).
CInterfaceMap_Retail::SEntry* CInterfaceMap_Retail::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing_Retail {
    unsigned char         _pad00[0x10];
    unsigned char         m_type;                   // +0x10 (1 = the player creature)
    unsigned char         _pad11[0xf];
    unsigned long         m_presentBits[9];         // +0x20
    CInterfaceMap_Retail  m_interfaces;             // +0x44
    unsigned char         _pad4c[0x20];
    unsigned char         m_flags;                  // +0x6c (bit 3 = creature)
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase_Retail* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;        // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
};
struct CTCTargetingAI : CTCBase_Retail { void SetScriptTargetOverride(CThing_Retail* target); };   // 0x008ABBA0
struct CTCEnemyList { void AddEnemy(CThing_Retail* enemy); };                                       // 0x0076D1B0
struct CThingPlayerCreature_Retail { void OnBestEnemyTargetChanged(); };                            // 0x006D5AB0
struct CGameScriptInterface {
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const;
};

void CGameScriptInterface::GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const
{
    if (!thing.IsValid())
        return;
    CThing_Retail* previous = 0;
    CThing_Retail* owner = thing.GetThing();
    if (owner->HasInterface(8)) {
        CTCBase_Retail* targeting = owner->GetInterface(8);
        bool isAI;
        {
            CCharString aiClass("CTCTargetingAI");
            isAI = targeting->GetClassTypeName() == aiClass;
        }
        if (isAI) {
            previous = targeting->GetCurrentTarget();
            if (target.IsValid() && (target.GetThing()->m_flags & 8))
                static_cast<CTCTargetingAI*>(targeting)->SetScriptTargetOverride(target.GetThing());
            else
                static_cast<CTCTargetingAI*>(targeting)->SetScriptTargetOverride(0);
        }
    }
    if (target.IsValid()) {
        CThing_Retail* owner2 = thing.GetThing();
        if (owner2->HasInterface(0x49))
            ((CTCEnemyList*)owner2->GetInterface(0x49))->AddEnemy(target.GetThing());
    }
    if (previous != target.GetThing() && thing.GetThing()->m_type == 1)
        ((CThingPlayerCreature_Retail*)thing.GetThing())->OnBestEnemyTargetChanged();
}