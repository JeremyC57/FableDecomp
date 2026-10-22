// Grade: DIFFER(541v541) -- frame and slots now as retail (same-TU noinline LowerBound stand-ins, advisor A3); left: object/item ebx-ebp register swap
// CGameScriptInterface::MakeHeroCarryItemInHand(const CScriptThing& thing, bool makeActiveWeapon, bool e) const -- put a valid inventory item (component 0x1a, carryable 0x47) into the live hero's hand: add it to the hero's inventory of its category when it has no inventory yet (else it must already be the hero's), optionally make it the active weapon, take the inventory's instance when it was just added, clear the target carry slot, take it out of any other slot and put it into the slot, then bring it out of limbo
// Leaf: ?MakeHeroCarryItemInHand@CGameScriptInterface@@UBEXABVCScriptThing@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00897FA0, length 0x21D bytes (+ int3 padding), `ret 0xc`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetHeroCreature 0x00487DC0, the component-map LowerBound 0x0040F020 and
//   its const instantiation 0x004365B0, CTCInventoryItem::GetInventoryType 0x005D9030 / AddToInventory 0x005D8D50 /
//   GetInventoryOwner 0x005D8E90, puts 0x00BFEAF2 + abort 0x00BFEAEC (bitset<274>::test), CTCInventoryWeapons::SetThingAsActiveWeapon
//   0x005CB2B0, the inventory's virtual slot 47 (+0xbc), the carryable's carry-slot getter 0x00705CC0 (manifest
//   "CTCTavernGame::GetBanter" is propagated), CTCCarrying::IsSlotFree 0x006A1230 (manifest "InventoryFindItemByIDLowerBound")
//   / RemoveThingFromPrimarySlot 0x006A1E00 / IsCarryingThing 0x006A1060 / RemoveThing 0x006A19B0 / PutThingInCarrySlot 0x006A1F80,
//   CThing::SetInLimbo 0x004C8CF0.
// Layout: the landed CGameScriptInterface::SetHeroHandLampAsLit 0x00898EC0 model (PlayerManager +0x14, component mask +0x20,
//   component map +0x44, dead bit +0x91); CThing +0x8c is a 16-bit id; the inventory item's owning inventory at +0x10.
extern "C" int __cdecl puts(const char* text);          // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort();   // 0x00BFEAEC
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key in a stack temporary), end when the key is absent
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = a creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[9];                    // +0x20: which component kinds the thing holds (bitset<274>)
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x3c];
    unsigned short m_id;                           // +0x8c
    unsigned char  _pad8e[3];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    void SetInLimbo(bool limbo);                   // 0x004C8CF0
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word
    __forceinline bool TestComponent(unsigned long position) const    // STLport bitset<274>::test with its range check
    {
        if (position >= 274) {
            puts("bitset");
            abort();
        }
        return (m_tcMask[position >> 5] & (1UL << (position & 31))) != 0;
    }
};
__declspec(noinline) STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind)
{
    STCEntry_Retail* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind)
        ++it;
    return it;
}
__declspec(noinline) STCEntry_Retail* CTCMap_Retail::LowerBound(const unsigned long& kind) const
{
    STCEntry_Retail* it = m_pFirst;
    while (it != m_pLast && (unsigned long)it->m_kind < kind)
        ++it;
    return it;
}
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;       // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
    CScriptThing();                                // 0x008CFF40 (the null script thing)
};
struct CPlayer_Retail {
    CThing_Retail* GetHeroCreature();              // 0x00487DC0
    CThing_Retail* GetControlledCreature();        // 0x00487DD0
};
struct CPlayerManager_Retail {
    CPlayer_Retail* GetMainPlayer();               // 0x00449970
};
struct CCharString;
struct CDefinitionManager_Retail {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CWorld; struct CMainGameComponent; struct CDisplayEngine; struct CThingSearchTools;
__forceinline CTCBase_Retail* GetTCIfPresent(CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;                            // deliberately unset when the component is absent (retail reads a dead slot)
    if (thing->HasTC(kind))
        tc = GetTC(thing, kind);
    return tc;
}
__forceinline CTCBase_Retail* GetTCIfPresentConst(const CThing_Retail* thing, long kind)
{
    CTCBase_Retail* tc;
    if (thing->HasTC(kind))
        tc = GetTCConst(thing, kind);
    return tc;
}
template <class T> __forceinline bool GetTCOut(CThing_Retail* thing, long kind, T** out)   // the out-parameter lookup: the component stays in eax
{
    if (thing->HasTC(kind)) {
        *out = (T*)GetTC(thing, kind);
        return true;
    }
    return false;
}
struct CCharString {                               // fable_string.h layout: one data-block pointer
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
__forceinline bool IsThingAlive(const CScriptThing& thing)   // the thing exists and is not dead
{
    CThing_Retail* t = thing.GetThing();
    return t && !t->m_dead;
}
struct CTCInventoryContainer {
    void SetThingAsActiveWeapon(long id, bool flag);     // 0x005CB2B0
    virtual void i00(); virtual void i01(); virtual void i02(); virtual void i03(); virtual void i04(); virtual void i05(); virtual void i06(); virtual void i07();
    virtual void i08(); virtual void i09(); virtual void i10(); virtual void i11(); virtual void i12(); virtual void i13(); virtual void i14(); virtual void i15();
    virtual void i16(); virtual void i17(); virtual void i18(); virtual void i19(); virtual void i20(); virtual void i21(); virtual void i22(); virtual void i23();
    virtual void i24(); virtual void i25(); virtual void i26(); virtual void i27(); virtual void i28(); virtual void i29(); virtual void i30(); virtual void i31();
    virtual void i32(); virtual void i33(); virtual void i34(); virtual void i35(); virtual void i36(); virtual void i37(); virtual void i38(); virtual void i39();
    virtual void i40(); virtual void i41(); virtual void i42(); virtual void i43(); virtual void i44(); virtual void i45(); virtual void i46();
    virtual CThing_Retail* GetThingFromID(long id);      // slot 47 (+0xbc)
};
struct CTCInventoryItem {
    unsigned char          _pad00[0x10];
    CTCInventoryContainer* PInventory;                             // +0x10
    long GetInventoryType();                                       // 0x005D9030
    bool AddToInventory(CTCBase_Retail* inventory, bool a, bool b, long c, bool d, bool e);   // 0x005D8D50
    CThing_Retail* GetInventoryOwner();                            // 0x005D8E90
};
struct CTCCarryable {
    long GetCarrySlot();                                           // 0x00705CC0
};
struct CTCCarrying {
    bool IsSlotFree(long slot);                                    // 0x006A1230
    CThing_Retail* RemoveThingFromPrimarySlot(bool drop);          // 0x006A1E00
    bool IsCarryingThing(CThing_Retail* thing);                    // 0x006A1060
    void RemoveThing(CThing_Retail* thing);                        // 0x006A19B0
    void PutThingInCarrySlot(CThing_Retail* thing, long slot, bool flag);   // 0x006A1F80
};
struct CWorld; struct CMainGameComponent; struct CDisplayEngine; struct CThingSearchTools;
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                     World;             // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent*         Component;         // +0x08
    CDisplayEngine*             DisplayEngine;     // +0x0c
    CDefinitionManager_Retail*  DefinitionManager; // +0x10
    CPlayerManager_Retail*      PlayerManager;     // +0x14
    CThingSearchTools*          ThingSearchTools;  // +0x18
    virtual void MakeHeroCarryItemInHand(const CScriptThing& thing, bool makeActiveWeapon, bool e) const;
};

void CGameScriptInterface::MakeHeroCarryItemInHand(const CScriptThing& thing, bool makeActiveWeapon, bool e) const
{
    CThing_Retail* hero = PlayerManager->GetMainPlayer()->GetHeroCreature();
    if (!hero || hero->m_dead)
        return;
    if (!thing.IsValid())
        return;
    CThing_Retail* object = thing.GetThing();
    bool added = false;
    if (!object->HasTC(0x1a))
        return;
    CTCInventoryItem* item = (CTCInventoryItem*)GetTC(object, 0x1a);
    if (!object->HasTC(0x47))
        return;
    CTCCarryable* carryable = (CTCCarryable*)GetTCConst(object, 0x47);
    if (item->PInventory == 0) {
        long type = item->GetInventoryType();
        if (!hero->TestComponent(type))
            return;
        item->AddToInventory(GetTC(hero, type), false, false, 0, true, e);
        added = true;
    }
    else if (item->GetInventoryOwner() != hero)
        return;
    if (item->PInventory == 0)
        return;
    if (makeActiveWeapon && object->HasTC(0x48))
        item->PInventory->SetThingAsActiveWeapon(object->m_id, false);
    if (added)
        object = item->PInventory->GetThingFromID(object->m_id);
    long slot = carryable->GetCarrySlot();
    CTCCarrying* carrying = 0;
    if (hero->HasTC(0x46))
        carrying = (CTCCarrying*)GetTC(hero, 0x46);
    if (!carrying->IsSlotFree(slot))
        carrying->RemoveThingFromPrimarySlot(false);
    if (carrying->IsCarryingThing(object))
        carrying->RemoveThing(object);
    carrying->PutThingInCarrySlot(object, slot, true);
    object->SetInLimbo(false);
}
