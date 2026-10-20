// Grade: DIFFER(319v315) -- only the def release: retail reloads the def into ecx (`mov ecx,[esp+34h] ; cmp ecx,ebp ; pop edi ; je ; dec [ecx+4]`), ours into eax and re-reads it into ecx for the virtual call (the CreateObject 0x00894820 residue)
// CGameScriptInterface::CreateObjectOnEntity(const CCharString& defName, const CScriptThing& entity, const CCharString& scriptName) const -- when the entity is alive, create a thing object from the named def at the entity's position, attach it to the entity's attachment when both have one, and return its script thing (the null script thing otherwise)
// Leaf: ?CreateObjectOnEntity@CGameScriptInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008946E0, length 315 bytes (+ int3 padding), `ret 10h`.
// Signature from Ego_r.pdb: ?CreateObjectOnEntity@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@ABV2@0@Z.
// Callees: the CreateObject 0x00894820 set (GetDefGlobalIndexFromName 0x009AD410, GetDef<CThingObjectDef> 0x0042B040,
//   CThingObject::Create 0x00703210, GetScriptThing 0x004C7E50, CScriptThing::operator= 0x008AB980) plus the out-of-line
//   ~CScriptThing @ 0x004AA840 for the GetScriptThing temporary; the entity's CScriptThing slots 75 IsAlive, 6 GetPosition,
//   11 GetThing; the attachment holder at CThing+0x60 (slot 32 +0x80 on the new object's, slot 72 +0x120 on the
//   entity's).
// Layout: as CreateObject; CThing +0x60 is a component pointer (name unproven).
// Shape: the result local is built first; the def pointer lives in the dead `defName` slot; the inline-once band puts
//   the GetScriptThing temporary's destructor out of line and the result's inline (block end first).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
struct CCharString {
    void* PStringData;
};
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;

struct CBaseClass {                                     // the polymorphic root (vptr at +0)
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
struct CCPPointerInfo {
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CCountedScriptImp {                              // CCountedPointer<CScriptThingImp>
    void* Data;
    CCPPointerInfo* Info;
    CCountedScriptImp() : Data(0), Info(0) {}
    CCountedScriptImp(const CCountedScriptImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedScriptImp()
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
struct CThing;
struct CScriptThing : CBaseClass {                      // vtable 0x01238C8C
    CCountedScriptImp PImp;                             // +0x04
    CScriptThing() {}
    virtual ~CScriptThing() {}
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const;
    virtual void v05() const;
    virtual const C3DVector& GetPosition() const;       // slot 6 (+0x18)
    virtual void v07() const; virtual void v08() const; virtual void v09() const; virtual void v10() const;
    virtual CThing* GetThing() const;                   // slot 11 (+0x2c)
    virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const; virtual void v17() const; virtual void v18() const; virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const; virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const; virtual void v33() const; virtual void v34() const; virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const; virtual void v41() const; virtual void v42() const; virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const; virtual void v49() const; virtual void v50() const; virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const; virtual void v57() const; virtual void v58() const; virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const; virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const; virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
    CScriptThing& operator=(const CScriptThing& other); // 0x008AB980
};

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CThingObjectDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
struct CDefPointerThingObjectDef {
    CThingObjectDef* Object;
    CDefPointerThingObjectDef() : Object(0) {}
    ~CDefPointerThingObjectDef() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
};
struct CGameDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);                     // 0x009AD410
    bool GetDef(long globalIndex, CDefPointerThingObjectDef* out);               // 0x0042B040
};
struct CAttachmentHolder {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void Attach(void* attachment);              // slot 32 (+0x80)
};
struct CThingVirtuals {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void* GetAttachment();   // slot 72 (+0x120)
};
struct CThing {
    unsigned char _pad00[0x60];
    CAttachmentHolder* Attachments;                     // +0x60
    CThingVirtuals* GetTargetAttachments() const { return (CThingVirtuals*)Attachments; }
    CScriptThing GetScriptThing();                      // 0x004C7E50
};
struct CThingObject : CThing {
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                           const CRightHandedSet* orientation, unsigned long uid,
                                           const CCharString& scriptName);   // 0x00703210
};

struct CGameScriptInterface {
    void*                   _vptr;                      // +0x00
    void*                   World;                      // +0x04
    void*                   Component;                  // +0x08
    void*                   DisplayEngine;              // +0x0c
    CGameDefinitionManager* DefinitionManager;          // +0x10
    CScriptThing CreateObjectOnEntity(const CCharString& defName, const CScriptThing& entity, const CCharString& scriptName) const;
};

CScriptThing CGameScriptInterface::CreateObjectOnEntity(const CCharString& defName, const CScriptThing& entity,
                                                        const CCharString& scriptName) const
{
    CScriptThing result;
    if (entity.IsAlive()) {
        long index = DefinitionManager->GetDefGlobalIndexFromName(defName);
        CDefPointerThingObjectDef def;
        DefinitionManager->GetDef(index, &def);
        CThingObject* object = CThingObject::Create(index, entity.GetPosition(), 4, 0, 0, scriptName);
        if (object) {
            CAttachmentHolder* holder = object->Attachments;
            if (holder) {
                CThingVirtuals* target = entity.GetThing()->GetTargetAttachments();
                if (target)
                    holder->Attach(target->GetAttachment());
            }
            result = object->GetScriptThing();
        }
    }
    return result;
}
