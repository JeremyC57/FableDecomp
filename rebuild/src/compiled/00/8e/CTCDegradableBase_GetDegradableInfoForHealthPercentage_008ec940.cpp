// CTCDegradableBase::GetDegradableInfoForHealthPercentage(float) -- the degradation stage whose health band holds the percentage (stages sorted by falling health), null at or above the first stage or with no stages
// Leaf: GetDegradableInfoForHealthPercentage
//
// Retail 0x008EC940, 119 bytes (`ret 4`). Ego_r.pdb `?GetDegradableInfoForHealthPercentage@CTCDegradableBase
// @@AAEPBVCDegradableInfo@@M@Z`. Callees: none direct; the def's virtual slot 1 when the copied def
// pointer drops its last reference.
// Layout = Ego_r.pdb CTCDegradableBase (PDefinition +0x10); CDegradableDef Degradations
// (vector<CDegradableInfo>, 0x1c-byte elements, HealthPercentage first) at +0x30 (retail 12-byte vector).
// Shape: the stage vector is a REFERENCE bound to a member of the by-value GetDefinition() temporary, so
// the copy's add-ref and release (both null-tested) run first and the def object (esi) is then read
// unprotected; the result local is zeroed first (edi), the empty case is the trailing `return 0` after the
// non-empty block (its own `xor eax,eax` exit placed last); `pct > it` gives `test ah,41h ; jne`, `pct <= prev` `test ah,41h ; jnp`.
struct CDegradableInfo {                                    // 0x1c bytes
    float         HealthPercentage;                         // +0x00
    unsigned char EngineGraphic[0x10];                      // +0x04 CEngineGraphic
    long          SmashParticleEmitter;                     // +0x14
    bool          BlocksNavigation;                         // +0x18
};

template<class T> struct CRetailVector {                    // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    const T* end() const { return _M_finish; }
};

struct CDegradableDef {
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long          RefCount;                                 // +0x04
    unsigned char _pad08[0x30 - 0x08];
    CRetailVector<CDegradableInfo> Degradations;            // +0x30
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DestroySelf(); }
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object != 0) Object->AddRef(); }
    ~CDefPointer() { if (Object != 0) Object->Release(); }
    T* operator->() const { return Object; }
};

struct CThing;
struct CTCDegradableBase {
    virtual void v00();
    CThing*                     ParentThing;                // +0x04 (CTCBase)
    bool                        ToBeRemoved;                // +0x08
    bool                        IsInGlobalUpdateTCs;        // +0x09
    long                        LastWorldFrameHit;          // +0x0c
    CDefPointer<CDegradableDef> PDefinition;                // +0x10 CDefPointer<const CDegradableDef>
    CDefPointer<CDegradableDef> GetDefinition() const { return PDefinition; }
    const CDegradableInfo* GetDegradableInfoForHealthPercentage(float healthPercentage);
};

const CDegradableInfo* CTCDegradableBase::GetDegradableInfoForHealthPercentage(float healthPercentage)
{
    const CDegradableInfo* result = 0;
    const CRetailVector<CDegradableInfo>& degradations = GetDefinition()->Degradations;
    const CDegradableInfo* it = degradations.begin();
    if (it != degradations.end())
    {
        if (healthPercentage < it->HealthPercentage)
        {
            const CDegradableInfo* previous = it;
            for (++it; it != degradations.end(); ++it, ++previous)
            {
                if (healthPercentage > it->HealthPercentage && healthPercentage <= previous->HealthPercentage)
                    break;
            }
            result = previous;
        }
        return result;
    }
    return 0;
}