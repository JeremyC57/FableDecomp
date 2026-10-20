// CTCCameraPointShapeActivationBase::vfunc_9() -- per-frame activation: while the shape test says the subject is inside, run the inside hook; register the camera point with the camera manager when it becomes inside (and its angle is valid), unregister it when it leaves
//
// Retail 0x0075C5F0, 121 bytes (plain `ret`). Vtable 0x0123AEDC (CTCCameraPointShapeActivationBase)
// slot 9. Callees: CTCBase::GetWorld() @ 0x00686A70 (manifest "CViewBase::GetDisplayEngine",
// propagated), virtual slot 9 (+0x24) of the world's +0x34 object (fetch the camera manager through
// an out pointer, bool result), this's virtual slots 29 (+0x74, the shape test) and 34 (+0x88, the
// inside hook), CTCCameraPointDefinitionBase::IsAngleValid @ 0x0075C530, and two camera-manager
// members taking the parent thing: remove @ 0x006984A0 / add @ 0x006991F0 (catalog labels
// "std::list_erase_value" / "FindInListAndInsertIfMatches").
// Layout: ParentThing +0x04 (CTCBase), Activated +0x3c (CTCCameraPointDefinitionBase, landed ctor
// 0x0075C180).
// Shape: an early return when no manager; the shape result is a bool local (bl); `if (Activated
// && !inside) {...} else if (!Activated && inside && IsAngleValid()) {...}` re-tests Activated in
// the else arm, and `Activated = false` stores the known-zero bl.
// Shape: the camera source is reached by naming the GetWorld() CALL result and chaining the member in the call expression
//   (`CWorld* world = GetWorld(); world->m_pCameraSource->GetCameraManager(&manager)`, retail `mov ecx,[eax+34h]`).
struct CThing;

struct CCameraPointManager {
    void RemoveCameraPoint(CThing* thing);          // 0x006984A0
    void AddCameraPoint(CThing* thing);             // 0x006991F0
};

struct CCameraManagerSource {                       // the world's +0x34 object
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CCameraPointManager** out);   // slot 9 (+0x24)
};

struct CWorld {
    unsigned char         _pad00[0x34];
    CCameraManagerSource* m_pCameraSource;          // +0x34
};

struct CTCCameraPointShapeActivationBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void vfunc_9();                                                         // slot 9
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual bool IsSubjectInside();                                                 // slot 29 (+0x74)
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void OnSubjectInside();                                                 // slot 34 (+0x88)

    CThing*       ParentThing;                      // +0x04 (CTCBase)
    unsigned char _pad08[0x3c - 0x08];
    bool          Activated;                        // +0x3c

    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)
    bool IsAngleValid();                            // 0x0075C530 (CTCCameraPointDefinitionBase)
};

void CTCCameraPointShapeActivationBase::vfunc_9()
{
    CCameraPointManager* manager;
    CWorld* world = GetWorld();
    if (!world->m_pCameraSource->GetCameraManager(&manager))
        return;
    bool inside = IsSubjectInside();
    if (inside)
        OnSubjectInside();
    if (Activated && !inside)
    {
        manager->RemoveCameraPoint(ParentThing);
        Activated = false;
    }
    else if (!Activated && inside && IsAngleValid())
    {
        manager->AddCameraPoint(ParentThing);
        Activated = true;
    }
}