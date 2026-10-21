// CTCActionUseWindow::OnActionUse(CThing& user) -- base OnActionUse; give a live creature user the look-through-window action; then, when the current game camera reports a view, touch the owner's position and let the user's physics fill a vector
// Leaf: ?OnActionUse@CTCActionUseWindow@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EBAB0, length 122 bytes (+6 int3 padding). Vtable slot 28 of CTCActionUseWindow
// (0x0123F5EC). `ret 4`.
// Callees: CTCActionUseBase::OnActionUse @ 0x007EAB90 (qualified), CCreatureAction_LookThroughWindow::CCreatureAction_
// LookThroughWindow(CThingAICreature&) @ 0x007EBA20 (stores vtable 0x01270FBC = CCreatureAction_LookThroughWindow; catalog
// label "CCreatureAction_AlertToPlayer" propagated), CThingCreatureBase::SetCurrentAction @ 0x006644F0 (result dropped),
// ~CCreatureActionBase @ 0x00693EF0, CTCBase::GetWorld @ 0x00686A70, the camera's virtual slot 9 (+0x24, bool, out pointer),
// CThing::GetPosition @ 0x004C73D0 (result unused), the user's physics virtual slot 70 (+0x118, out pointer).
// Shape: the action temporary (0xA8 bytes, no own members: only the out-of-line base dtor runs) is passed by reference straight
// from the ctor's eax; the null/dead test guards only the action; the second block reads user.+0x60 unconditionally. The camera
// is reached through an inline CWorld accessor (GetCurrentGameCamera, stand-in name): it lands straight in ecx for the virtual
// call (`mov ecx,[eax+34h] ; mov eax,[ecx]`) and also gives retail's this=esi / user=edi colouring.
// Layout: PDB CWorld PCurrentGameCamera +0x34; retail CThing +0x60 physics pointer, +0x91 bit 0 Dead.
struct C3DVector { float X, Y, Z; };
struct CThingPhysics {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void GetVector(C3DVector* out);    // slot 70 (+0x118)
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CThingPhysics* Physics;                    // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                     // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    const C3DVector& GetPosition() const;      // 0x004C73D0
};
class CCreatureActionBase;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CThingAICreature : public CThingCreatureBase {};
class CCreatureActionBase {                    // retail 0xA8 bytes
public:
    virtual ~CCreatureActionBase();            // 0x00693EF0
    unsigned char _pad04[0xa8 - 4];
};
class __declspec(novtable) CCreatureAction_LookThroughWindow : public CCreatureActionBase {
public:
    CCreatureAction_LookThroughWindow(CThingAICreature& creature);   // 0x007EBA20
};
struct CAIGameCameraBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetView(long* out);           // slot 9 (+0x24)
};
struct CWorld {
    unsigned char _pad00[0x34];
    CAIGameCameraBase* PCurrentGameCamera;     // +0x34
    CAIGameCameraBase* GetCurrentGameCamera() const { return PCurrentGameCamera; }
};

inline CThingAICreature* CastToCreature(CThing* thing) { return (CThingAICreature*)thing; }

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
    CWorld* GetWorld();                        // 0x00686A70
};
class CTCActionUseBase : public CTCBase {
public:
    virtual void OnActionUse(CThing& user);    // 0x007EAB90
};
class CTCActionUseWindow : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);
    void UpdateView(CThing& user)
    {
        long view;
        if (GetWorld()->GetCurrentGameCamera()->GetView(&view)) {
            ParentThing->GetPosition();
            C3DVector v;
            user.Physics->GetVector(&v);
        }
    }
};

void CTCActionUseWindow::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    CThingAICreature* creature = CastToCreature(&user);
    if (creature && !creature->IsDead())
        creature->SetCurrentAction(CCreatureAction_LookThroughWindow(*creature));
    UpdateView(user);
}