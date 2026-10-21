// Grade: DIFFER(436v442) -- argument order + registers in the camera branch: retail evaluates the open-chest module's ctor arguments as object (edi), creature (esi), environment (eax) after the allocation, keeping &PCreature in esi and the module in ebp; ours evaluates right to left (environment, object, creature) with &PObjectToUse in ebp, and loads the camera owner through eax before the slot-9 call (retail: straight into ecx).
// CCreatureAction_UseLamp::Initialise() -- pick a "LIGHT_LAMP" animation filtered by the lamp's action-use animation group, play it and, for
// a player-controlled creature with a camera manager, switch the camera to an open-chest scripted module on the lamp; then the base
// initialisation.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00847DF0, length 442 bytes.
// Callees: CCharString 0x0099E4B0 / 0x0099EBF0 / 0x0099EFB0 / 0x0099EAE0; CBaseIntelligentPointer::GetPItem 0x00A01B50; LowerBound 0x004365B0
// (interface 0xd, bit 13 of +0x20); CTCActionUseBase::GetActionUseAnimationGroupName 0x007EABA0; CCRC::Calc 0x004014A0; CThingCreatureBase::
// GetRandomAnimFromNameWithFilter 0x00662EF0; CCreatureActionBase::SetAnimation 0x00693030; creature slot 77 (+0x134); 0x00692940 game
// context (manifest "_Dest_val" is propagated) whose +0x34 object's slot 9 (+0x24) yields the camera manager; operator new 0x00BFEA1A;
// 0x006925B0 (manifest "CWorld::DrawGetEnvironment"); CScriptModuleOpenChest 0x00853120; CCountedPointer(T*) 0x005ED270;
// CGameCameraManager::SetScriptedCameraMode 0x00699E60; CCreatureActionBase::Initialise 0x00694D10. Filter vtables 0x01267568 / 0x0122FFCC.
// Layout: PCreature +8, PObjectToUse +0xa8 (landed 0x00847FB0 model); group-name filter {vptr, CCharString +4, CRC +8}.
// Shape: the filter ctor assigns the name then CRCs it (null-first GetString/GetLength); "LIGHT_LAMP" is an implicit conversion; the
// interface lookup is its own statement (sharing the key slot with the returned name temp keeps the frame at 0x14); the module goes to
// SetScriptedCameraMode through an implicit T* -> CCountedPointer conversion (built in the argument slot).
// Tried: explicit CCountedPointer temp (frame +8), lookup chained into the name call (frame +4).
// Leaf: Initialise
struct CCharStringData { char* Data; long Length; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->Data; }
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
};
struct CCRC { static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
struct CAnimFilterBase {                               // vtable 0x0122FFCC
    virtual ~CAnimFilterBase() {}
    virtual bool Accept(const void* entry) const;
};
struct CAnimFilterGroupName : CAnimFilterBase {        // vtable 0x01267568
    CCharString GroupName;                             // +4
    unsigned long GroupCRC;                            // +8
    CAnimFilterGroupName(const CCharString& groupName)
    {
        GroupName = groupName;
        GroupCRC = CCRC::Calc(0, groupName.GetString(), groupName.GetLength());
    }
    virtual bool Accept(const void* entry) const;
};
struct CAnimationEntry;
struct CTCBase;
struct CTCEntry { long Type; CTCBase* TC; };
struct CTCMap {
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const unsigned long& type) const;   // 0x004365B0
};
struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); 
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); 
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); 
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); 
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); 
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); 
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); 
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); 
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); 
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); 
    virtual bool IsPlayerControlled();                 // slot 77 (+0x134)
    unsigned char _pad04[0x20 - 4];
    unsigned long m_presentBits[9];                    // +0x20
    CTCMap Components;                                 // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char m_flags91;                           // +0x91
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long type) const
    {
        unsigned long key = type;
        const CTCMap* map = &Components;
        CTCEntry* it = map->LowerBound(key);
        if (it == map->_M_finish || type < it->Type)
            it = map->_M_finish;
        return (T*)it->TC;
    }
};
struct CThingCreatureBase : CThing {
    CAnimationEntry* GetRandomAnimFromNameWithFilter(const CCharString& name, CAnimFilterBase* filter);   // 0x00662EF0
};
struct CTCActionUseBase {
    CCharString GetActionUseAnimationGroupName();      // 0x007EABA0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                   // +4
    CThing* GetPItem() const;                          // 0x00A01B50
};
template <class T> struct CCountedPointer {
    T* m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                        // 0x005ED270
};
struct CScriptModuleBase { virtual void v00(); };
struct CScriptModuleOpenChest : CScriptModuleBase {
    unsigned char _body[0xa0 - 4];
    CScriptModuleOpenChest(CThing* creature, CThing* chest, long environment);   // 0x00853120
};
struct CGameCameraManager {
    void SetScriptedCameraMode(CCountedPointer<CScriptModuleBase> module);   // 0x00699E60
};
struct CCameraOwner {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** out);   // slot 9 (+0x24)
};
struct CActionGameContext { unsigned char _pad[0x34]; CCameraOwner* PCameraOwner; };   // +0x34
struct CCreatureActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();                         // 0x00694D10
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    void SetAnimation(CAnimationEntry* anim, float a, float b);   // 0x00693030
    CActionGameContext* GetGameContext();              // 0x00692940
    long GetCameraEnvironment();                       // 0x006925B0
};
struct CCreatureAction_UseLamp : CCreatureActionBase {
    CBaseIntelligentPointer PObjectToUse;              // +0xa8
    virtual void Initialise();
};

void CCreatureAction_UseLamp::Initialise()
{
    CCharString animGroup;
    if (PObjectToUse.GetPItem() != 0 && !(PObjectToUse.GetPItem()->m_flags91 & 1)) {
        CThing* object = PObjectToUse.GetPItem();
        if (object->HasTC(0xd)) {
            CTCActionUseBase* use = object->GetTC<CTCActionUseBase>(0xd);
            animGroup = use->GetActionUseAnimationGroupName();
        }
    }
    CAnimFilterGroupName filter(animGroup);
    CAnimationEntry* anim = ((CThingCreatureBase*)PCreature.GetPItem())->GetRandomAnimFromNameWithFilter("LIGHT_LAMP", &filter);
    SetAnimation(anim, -1.0f, 1.0f);
    if (PCreature.GetPItem()->IsPlayerControlled()) {
        CGameCameraManager* cameraManager;
        if (GetGameContext()->PCameraOwner->GetCameraManager(&cameraManager))
            cameraManager->SetScriptedCameraMode(new CScriptModuleOpenChest(PCreature.GetPItem(), PObjectToUse.GetPItem(), GetCameraEnvironment()));
    }
    CCreatureActionBase::Initialise();
}
