// CUserProfileManager::GetMaxShadowDetail() const -- the command-line cap (g_MaxDetailB) when it is set, else CConfigOptionsDefaultsDef::MaxShadowDetail, 0 without the def
// Grade: DIFFER(84v67) -- lane gap: retail cmp [cap],-1/fild [cap] and the def release scheduled with the field read into the shared push-ecx slot; 14 forms tried (RemoveReference variants, def-local Release, /Os, optimize s/sy, QFE)
// Leaf: ?GetMaxShadowDetail@CUserProfileManager@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00408550, 67 bytes, thiscall, plain `ret`, float in st0.
// Identity (proven): FableWin.pdb / Ego_r.pdb ?GetMaxShadowDetail@CUserProfileManager@@QBEMXZ; the four GetMax*Detail getters sit in the
//   PDB's address order (Effects, Texture, Shadow, Mesh -- FableWin 0x00406DC0) as 0x004084B0 / 0x00408500 / 0x00408550 / 0x004085A0
//   do, each reading the matching Max*Detail field (retail CConfigOptionsDefaultsDef +0x58 / +0x60 / +0x68 / +0x70); the one
//   caller is CSlider::GetValueFromProfile 0x0054BBB0.
// Callees: CUserProfileManager::GetConfigOptionsDefaultsDef() const 0x004083E0 (CDefPointer by hidden pointer; `this` passes
//   through in ecx), the def's virtual Delete (slot 1) when the last reference goes.
// Data: the cap 0x01375078 (-1 = none; the landed binding name g_MaxDetailB is kept).
// Shape (family K072, representative 0x004084B0): the int cap through fild; the field copied into the def pointer's own
//   frame slot before the inline release; the null path returns the 0.0f constant 0x0122DEDC.
extern long g_MaxDetailB;                                      // 0x01375078
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                        // slot 1
    mutable unsigned long IVRefCount;                   // +0x04
    void RemoveReference() const { if (--IVRefCount == 0) Delete(); }
};
class CDefClassBase : public CIVCountedPointeeBase { };
class CConfigOptionsDefaultsDef : public CDefClassBase { // retail 0x74 bytes (engine/_quarantine/CConfigOptionsDefaultsDef.h)
public:
    unsigned char _pad08[0x54 - 0x08];
    float TextureDetail;                                // +0x54
    float MaxTextureDetail;                             // +0x58
    float ShadowDetail;                                 // +0x5c
    float MaxShadowDetail;                              // +0x60
    float MeshDetail;                                   // +0x64
    float MaxMeshDetail;                                // +0x68
    float EffectsDetail;                                // +0x6c
    float MaxEffectsDetail;                             // +0x70
};
template <class T> class CDefPointer {
public:
    T* PObject;
    ~CDefPointer() { if (PObject) PObject->RemoveReference(); }
    T* operator->() const { return PObject; }
    bool IsValid() const { return PObject != 0; }
};
class CUserProfileManager {
public:
    CDefPointer<const CConfigOptionsDefaultsDef> GetConfigOptionsDefaultsDef() const;   // 0x004083E0
    float GetMaxShadowDetail() const;
};
float CUserProfileManager::GetMaxShadowDetail() const
{
    if (g_MaxDetailB != -1)
        return (float)g_MaxDetailB;
    CDefPointer<const CConfigOptionsDefaultsDef> def = GetConfigOptionsDefaultsDef();
    if (def.IsValid())
        return def->MaxShadowDetail;
    return 0.0f;
}
