// CThing::IsUnderScriptSupervision() -- true when the quest manager's script-thing manager knows this thing and the script thing it hands back carries a positive script counter
// Grade: DIFFER(ours v retail) -- 165v169: the virtual call on the CreateScriptThing temporary goes through the returned hidden pointer (`mov edx,[eax] ; mov ecx,eax`) where retail re-addresses the frame temporary (`mov eax,[esp+10h] ; lea ecx,[esp+10h]`); the Info reload for operator delete lands in eax instead of ecx
// Leaf: IsUnderScriptSupervision
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7E90, 169 bytes (+7 int3). `ret`, bool in al.
// Callees: CGameScriptThingManager::HasScriptThing(CThing*) 0x00704790 and CreateScriptThing(CThing*) 0x00704860 (CScriptThing
//          by hidden pointer) on g_pQuestManager->ScriptThingManager (+0x04, global 0x013B89FC), CScriptThing virtual slot 74
//          (+0x128, GetScriptCounter), operator delete 0x00BFE9BC, CBase::~CBase 0x0099A2E0.
// Shape: the `&&` with a temporary in its right operand: a constructed flag (ebx, 0 then 1) decides the temporary's
//        destruction after the full expression; the result byte is stored to the frame from both arms (bl / 0); the
//        inlined ~CScriptThing is the counted script-pointer release (RefCount at Info+0, DeleteFunc(Data) through
//        Info+4/+8, delete Info) then the vtable store, both pointers nulled and the out-of-line CBase destructor.
// Layout: CScriptThing (12 bytes, vtable 0x01238C8C): CBase vptr, Data +0x04, Info +0x08 (CCPPointerInfo: RefCount +0,
//         DeleteFunc +4, Data +8).
struct CThing;
void __cdecl operator delete(void* block);            // 0x00BFE9BC
struct CBase {
    virtual ~CBase();                                 // 0x0099A2E0
};
struct CCPPointerInfo {
    long RefCount;                                    // +0x00
    void (__fastcall* DeleteFunc)(void* data);        // +0x04
    void* Data;                                       // +0x08
};
struct CScriptThing : CBase {                         // vtable 0x01238C8C
    void*           Data;                             // +0x04
    CCPPointerInfo* Info;                             // +0x08
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73();
    virtual long GetScriptCounter() const;            // slot 74 (+0x128) 0x004AB110
    bool IsSupervised() const { return GetScriptCounter() > 0; }   // inline on the temporary: addressed in its frame slot
    ~CScriptThing()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CGameScriptThingManager {
    bool HasScriptThing(CThing* thing);               // 0x00704790
    CScriptThing CreateScriptThing(CThing* thing);    // 0x00704860
};
struct CQuestManager {
    unsigned char _pad00[4];
    CGameScriptThingManager* ScriptThingManager;      // +0x04
};
extern CQuestManager* g_pQuestManager;                // 0x013B89FC
struct CThing {
    bool IsUnderScriptSupervision();
};
bool CThing::IsUnderScriptSupervision()
{
    bool supervised = g_pQuestManager->ScriptThingManager->HasScriptThing(this)
        && g_pQuestManager->ScriptThingManager->CreateScriptThing(this).IsSupervised();
    return supervised;
}
