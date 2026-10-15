// CGameCameraManager::PlayCameraSound(const CCharString& sound) -- play `sound` through the observed thing (PObservedThing at +0x100) with the camera-manager def (PDef at +0x134) (0x00698A30)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00698B20, length 36 bytes (48 with padding).
// Leaf: PlayCameraSound
// The chunk label (CSpellContainerList::AddChild) is propagated garbage. The offsets fit the PDB
// CGameCameraManager (PObservedThing +0x110, PDef +0x14c) shifted to the retail container sizes (the
// AvailableCamPoints list and the EarthQuakes / CameraList / SpecialCameraList vectors are narrower).
// Callees: 0x00A01B50 (CBaseIntelligentPointer::GetPItem, on +0x100), 0x00698A30
// (PlayCameraSoundThroughObservedThing, __fastcall: thing in ecx, def in edx, the sound on the stack).
// Shape: arguments evaluated right to left -- the sound pushed first, the def read into esi, then GetPItem;
// `mov edx,esi ; mov ecx,eax` and a plain call (esi is restored after it), `ret 4`.
// The def pointer goes through an inline accessor of the 4-byte CDefPointer.
class CCharString;
struct CThing;
struct CBaseObject;
struct CCameraManagerDef;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                       // +0x04
    CBaseObject* GetPItem() const;              // 0x00A01B50
};

template <class T>
struct CDefPointer {
    T* m_pDef;
    T* Get() const { return m_pDef; }
};

void __fastcall PlayCameraSoundThroughObservedThing(CThing* thing, const CCameraManagerDef* def, const CCharString& sound);   // 0x00698A30

struct CGameCameraManager {
    unsigned char                         _00[0x100];
    CBaseIntelligentPointer               PObservedThing;   // +0x100 (CIntelligentPointer<CThingGameObject const>)
    unsigned char                         _108[0x2c];       // +0x108..+0x133
    CDefPointer<const CCameraManagerDef>  PDef;             // +0x134
    void PlayCameraSound(const CCharString& sound);
};

void CGameCameraManager::PlayCameraSound(const CCharString& sound)
{
    PlayCameraSoundThroughObservedThing((CThing*)PObservedThing.GetPItem(), PDef.Get(), sound);
}