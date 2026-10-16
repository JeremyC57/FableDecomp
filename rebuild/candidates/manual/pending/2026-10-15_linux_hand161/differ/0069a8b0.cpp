// CTCCombat::AddMeleeFlourishAbility() -- catalog label; the body is a CGameCameraManager member: drop the main player's mode 0x2f, switch the camera manager def to the camera set's melee entry and mark the observed thing as in melee
// Grade: DIFFER(108v100) -- (1) retail pushes &def, THEN loads World->DefinitionManager into ecx, then pushes the index (zero store after both pushes); ours pushes &def and the index back to back; (2) the CDefPointer release: retail `mov ecx,[esp+4] ; test ; dec [ecx+4] ; jne ; mov eax,[ecx] ; call [eax+4]`, our inline dtor re-reads the object after the dec (a hand-written pointer-local release gives the `add eax,4 ; dec esi` counted form instead)
//
// Retail 0x0069A8B0, 100 bytes (plain `ret`). Callees: CPlayerManager::GetMainPlayer @ 0x00449970
// (labelled "Catch@0040d959", propagated), CPlayer::RemoveMode(long) @ 0x00634190,
// CDefinitionManager::GetDef<CCameraManagerDef>(long, CDefPointer<const CCameraManagerDef>*) @ 0x0069CE80
// (labelled GetDef<CThingPhysicalSwitchDef>, one of the propagated GetDef labels),
// CGameCameraManager::SetCameraManagerDef(const CDefPointer<const CCameraManagerDef>&) @ 0x0069A850
// (landed), the def's virtual slot 1 (release at zero). Global 0x013B86A0 (its +0x1c player manager).
// Label: `this` has World +0x74, ObservedThingInMelee +0xa9 and PCameraManagerSet +0x130 and it calls
// SetCameraManagerDef on itself, so this is CGameCameraManager (retail layout of the landed
// 0x0069CEF0 / 0x0069A850 sources), not CTCCombat; named here under the catalog leaf.
// Shape: RemoveMode's argument is pushed before the GetMainPlayer call; the melee def index
// (+0x48 of the camera set def) and the definition manager (World +0x10) are read before the
// zeroed CDefPointer local is declared, so VC sinks its zero store below the pushes; the local's
// inline destructor releases after the flag store (no null store).
struct CPlayer {
    void RemoveMode(long mode);                     // 0x00634190
};

struct CPlayerManager {
    CPlayer* GetMainPlayer();                       // 0x00449970
};

struct CGlobalGame {                                // the object behind 0x013B86A0
    unsigned char   _pad[0x1c];
    CPlayerManager* PlayerManager;                  // +0x1c
};
extern CGlobalGame* g_pGame_013b86a0;               // 0x013B86A0

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +4
};

template <class T>
struct CDefPointer {
    CDefPointeeBase* m_pObject;
    CDefPointer() : m_pObject(0) {}
    ~CDefPointer()
    {
        if (m_pObject) {
            if (--m_pObject->m_refCount == 0)
                m_pObject->Destroy();
        }
    }
};

struct CCameraManagerDef;

struct CCameraManagerSetDef : CDefPointeeBase {
    unsigned char _pad08[0x48 - 0x08];
    long          MeleeCameraManager;               // +0x48 global def index
};

struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<const CCameraManagerDef>* out);   // 0x0069CE80
};

struct CWorld {
    unsigned char       _pad[0x10];
    CDefinitionManager* DefinitionManager;          // +0x10
};

struct CGameCameraManager {
    void*         __vftable;                        // +0x00
    unsigned char _pad04[0x70];                     // +0x04 Camera, OldCamera
    CWorld*       World;                            // +0x74
    unsigned char _pad78[0xa9 - 0x78];
    bool          ObservedThingInMelee;             // +0xa9
    unsigned char _padaa[0x130 - 0xaa];
    CCameraManagerSetDef* PCameraManagerSet;        // +0x130 CDefPointer<const CCameraManagerSetDef>

    void SetCameraManagerDef(const CDefPointer<const CCameraManagerDef>& def);   // 0x0069A850
    void AddMeleeFlourishAbility();
};

void CGameCameraManager::AddMeleeFlourishAbility()
{
    g_pGame_013b86a0->PlayerManager->GetMainPlayer()->RemoveMode(0x2f);
    long index = PCameraManagerSet->MeleeCameraManager;
    CDefPointer<const CCameraManagerDef> def;
    World->DefinitionManager->GetDef(index, &def);
    SetCameraManagerDef(def);
    ObservedThingInMelee = true;
}
