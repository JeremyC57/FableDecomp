// CTCCombat::AddMeleeFlourishAbility() -- catalog label; the body is a CGameCameraManager member: drop the main player's mode 0x2f, switch the camera manager def to the camera set's melee entry and mark the observed thing as in melee
// Leaf: AddMeleeFlourishAbility
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
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CPlayer { public:
    void RemoveMode(long mode);                     // 0x00634190
};

class CPlayerManager { public:
    CPlayer* GetMainPlayer();                       // 0x00449970
};

struct CGlobalGame {                                // the object behind 0x013B86A0
    unsigned char   _pad[0x1c];
    CPlayerManager* PlayerManager;                  // +0x1c
};
extern CGlobalGame* g_pGame_013b86a0;               // 0x013B86A0

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long m_refCount;                                // +4
};

template <class T>
class CDefPointer { public:
    CDefPointeeBase* m_pObject;
    CDefPointer() : m_pObject(0) {}
    ~CDefPointer()
    {
        if (m_pObject) {
            if (DecRefToZero(m_pObject->m_refCount))
                m_pObject->Destroy();
        }
    }
};

class CCameraManagerDef;

class CCameraManagerSetDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x48 - 0x08];
    long          MeleeCameraManager;               // +0x48 global def index
};

class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<const CCameraManagerDef>* out);   // 0x0069CE80
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const CCameraManagerDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CWorld { public:
    unsigned char       _pad[0x10];
    CDefinitionManager* DefinitionManager;          // +0x10
};

class CGameCameraManager { public:
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
void* FindRef_CGameCameraManager_SetCameraManagerDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void CGameCameraManager::SetCameraManagerDef(const CDefPointer<const CCameraManagerDef>& def)
{
    void* p = FindRef_CGameCameraManager_SetCameraManagerDef_0();
    if (!p)
        return;
    if (*(void**)&def != p)
        return;
}
#pragma auto_inline(on)

void CGameCameraManager::AddMeleeFlourishAbility()
{
    g_pGame_013b86a0->PlayerManager->GetMainPlayer()->RemoveMode(0x2f);
    long index = PCameraManagerSet->MeleeCameraManager;
    CDefPointer<const CCameraManagerDef> def;
    World->DefinitionManager->GetDef(index, &def);
    SetCameraManagerDef(def);
    ObservedThingInMelee = true;
}