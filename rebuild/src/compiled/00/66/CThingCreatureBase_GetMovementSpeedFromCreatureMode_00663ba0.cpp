// CThingCreatureBase::GetMovementSpeedFromCreatureMode(const C3DVector& target, long movementType) -- ask the creature's current mode for the movement speed towards a target, given the offset from the creature and its facing
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup written as the ternary entry pick (`((it == last || type < it->type) ? last : it)->component`, the landed 0x00584157 idiom): the component loads straight into ecx
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663BA0, 151 bytes (`ret 8`).
// Callees: CInterfaceMap::LowerBound @ 0x004365B0, CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 (label
//   "CPixelShader::CountReferences" propagated), CThing::GetPosition @ 0x004C73D0 (label "CVertexBufferWin32::DoSizeof"
//   propagated), CTCPhysicsBase::GetFacing slot 70 (+0x118, C3DVector by hidden pointer),
//   CTCCreatureModeManager::CModeBase virtual slot 7 (+0x1c; vtable 0x012673CC, 0x00739E70 for the base).
// Layout: retail CThing: interface bitset +0x20 (TC 0x31 = CTCCreatureModeManager: word +0x24 bit 17), CVectorMap +0x44,
//   physics component +0x60.
// Shape: `T* p = 0; if (HasTC(k)) p = GetTC(k);` in a `__forceinline` accessor used as `this` (the preloaded
//   `xor ecx,ecx`, the component loaded straight into ecx), the mode fetched before the position;
//   offset = target - position through an inline operator- building C3DVector(x, y, z) (Z computed first), the facing
//   a by-value virtual result; the mode's result is returned directly.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};
struct CModeBase {                              // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual float GetMovementSpeed(const C3DVector& offset, const C3DVector& facing, long movementType);    // slot 7 (+0x1c)
};
struct CTCCreatureModeManager {
    CModeBase* GetCurrentMode();                // 0x0073A6E0
};
struct CTCPhysicsBase {
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
    virtual C3DVector GetFacing() const;        // slot 70 (+0x118)
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};

struct CThingCreatureBase : CThing {
    float GetMovementSpeedFromCreatureMode(const C3DVector& target, long movementType);
    __forceinline CTCCreatureModeManager* GetModeManager()
    {
        CTCCreatureModeManager* modes = 0;
        if (HasTC(0x31))
            modes = GetTC<CTCCreatureModeManager>(0x31);
        return modes;
    }
};

float CThingCreatureBase::GetMovementSpeedFromCreatureMode(const C3DVector& target, long movementType)
{
    CModeBase* mode = GetModeManager()->GetCurrentMode();
    C3DVector offset = target - GetPosition();
    C3DVector facing = PhysicsTC->GetFacing();
    return mode->GetMovementSpeed(offset, facing, movementType);
}