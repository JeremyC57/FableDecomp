// CThingFindNavigationLayerAt::operator()(CThing* thing) -- for a thing whose physics component has a physics mesh, raise Layer to the mesh's navigation level at Pos; always returns true (keep iterating)
// Leaf: ??RCThingFindNavigationLayerAt
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0063EFB0, length 81 bytes (+ int3 padding).
// Callees: CTCPhysicsBase virtual slot 42 (`call [edx+0A8h]`, has-mesh test), CTCPhysicsBase::GetPhysicsMeshObject
// @ 0x006B0850 (CPhysicsMeshObject through the hidden pointer, a 0x40-byte frame local),
// CPhysicsMeshObject::GetNavigationLevelAt @ 0x00A11D10 (`const C3DVector&`), ~CPhysicsMeshObject @ 0x00A127D0.
// Layout: PDB CThingFindNavigationLayerAt { C3DVector Pos; long Layer; }; retail CThing PhysicsTC +0x60.
// Shape: no EH frame (exceptions off in this TU); `if (level > Layer) Layer = level;` is `cmp eax,[esi+0Ch] ; jle`.
class C3DVector { public:
    float x, y, z;
};

class CPhysicsMeshObject { public:
    unsigned char _body[0x40];
    CPhysicsMeshObject(const CPhysicsMeshObject& other);
    ~CPhysicsMeshObject();                          // 0x00A127D0
    long GetNavigationLevelAt(const C3DVector& pos);   // 0x00A11D10
};

class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41();
    virtual bool HasPhysicsMeshObject();            // slot 42 (+0xa8)
    CPhysicsMeshObject GetPhysicsMeshObject();      // 0x006B0850
};

class CThing { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                      // +0x60
};

class CThingFindNavigationLayerAt { public:
    C3DVector Pos;                                  // +0x00
    long      Layer;                                // +0x0c
    bool operator()(CThing* thing);
};

bool CThingFindNavigationLayerAt::operator()(CThing* thing)
{
    CTCPhysicsBase* physics = thing->PhysicsTC;
    if (physics && physics->HasPhysicsMeshObject()) {
        CPhysicsMeshObject mesh = physics->GetPhysicsMeshObject();
        long level = mesh.GetNavigationLevelAt(Pos);
        if (level > Layer)
            Layer = level;
    }
    return true;
}