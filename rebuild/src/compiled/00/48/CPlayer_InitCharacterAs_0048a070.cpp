// CPlayer::InitCharacterAs(const CCharString& defName) -- refresh the world's thing manager and map pointers, (re)create the player character when missing or dead, report progress, take control of the character, then point the camera at it (main player) or register it with the main player's character (co-op player)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048A070, length 297 bytes, `ret 4`.
// Callees: the world's virtual slots 3 (+0xc, the thing manager) and 12 (+0x30, the world map), CBaseIntelligentPointer::
// GetPItem 0x00A01B50, CPlayer::CreateCharacter(const CCharString&) 0x00489D40, CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0, DisplayProgress(const CCharString&, bool, float, bool) 0x009E9F40
// (`__fastcall`), CPlayer::SetControlledCreature 0x00487CF0, CPlayerManager::GetMainPlayerNumber 0x00449700 and
// GetMainPlayer 0x00449970, the world's +0x34 object's slot 9 GetCameraManager (+0x24), CGameCameraManager slot 27
// (+0x6c, the followed thing) and ResetCamera 0x0069A990, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// 0x0040F020 (TC 0x7a, word 3 bit 26), the TC's 0x006AEDC0 (propagated label "InsertIntelligentPointerInList").
// Literal "CPlayer::InitCharacterAs" 0x012380F4.
// Layout: engine/CPlayer.h (PlayerManager +0x10, World +0x1C, ThingManager +0x20, PWorldMap +0x24, Number +0x28,
// ControlledCreature +0x2c, PlayerCharacter +0x34) through the CPlayer_Methods carrier.
// Shape: the progress text is a temporary; the player number is a named local read before the GetMainPlayerID call
// (ebx, pushed/popped around its short live range); the camera out pointer reuses the dead argument slot; GetMainPlayer is called twice.
#include "engine/CPlayer.h"

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void __fastcall DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2);   // 0x009E9F40

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> CTCEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    CTCEntry* _M_start;
    CTCEntry* _M_finish;
    CTCEntry* _M_end_of_storage;
    CTCEntry* LowerBound(const ETCInterfaceType& type);   // 0x0040F020
    __forceinline CTCBase* Find(long type)
    {
        CTCEntry* it = LowerBound((ETCInterfaceType)type);
        if (it == _M_finish || type < it->first)
            it = _M_finish;
        return it->second;
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[8];                  // +0x20
    unsigned char _pad40[4];
    CTCMap        Components;                          // +0x44
    unsigned char _pad50[0x41];
    unsigned char Dead : 1;                            // +0x91 bit 0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type) { return Components.Find(type); }
};
struct CTCPlayerCharacterLinks {                       // TC type 0x7a
    void AddThing(CThing* thing);                      // 0x006AEDC0
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CGameCameraManager { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26();
    virtual void SetFollowedThing(CThing* thing);      // slot 27 (+0x6c)
    void ResetCamera();                                // 0x0069A990
};
struct CCameraManagerSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
struct CWorld_Retail {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager* GetThingManager();          // slot 3 (+0x0c)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                  // slot 12 (+0x30)
    unsigned char         _pad04[0x30];
    CCameraManagerSource* CameraSource;                // +0x34
};

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

struct CPlayer_Methods : CPlayer {
    CIntelligentPointer<CThing>& ControlledCreaturePtr() { return *(CIntelligentPointer<CThing>*)ControlledCreature; }
    CIntelligentPointer<CThing>& PlayerCharacterPtr() { return *(CIntelligentPointer<CThing>*)PlayerCharacter; }
    bool CreateCharacter(const CCharString& defName);  // 0x00489D40
    void SetControlledCreature(CThing* creature);      // 0x00487CF0
    void InitCharacterAs(const CCharString& defName);
};

void CPlayer_Methods::InitCharacterAs(const CCharString& defName)
{
    CWorld_Retail* world = (CWorld_Retail*)World;
    ThingManager = world->GetThingManager();
    PWorldMap = ((CWorld_Retail*)World)->GetWorldMap();
    if (PlayerCharacterPtr().GetPItem() == 0 || PlayerCharacterPtr().GetPItem()->Dead)
        CreateCharacter(defName);
    DisplayProgress("CPlayer::InitCharacterAs", false, -1.0f, false);
    SetControlledCreature(PlayerCharacterPtr().GetPItem());
    long number = Number;
    if (number == ((CPlayerManager*)PlayerManager)->GetMainPlayerNumber()) {
        CGameCameraManager* camera;
        ((CWorld_Retail*)World)->CameraSource->GetCameraManager(&camera);
        camera->SetFollowedThing(PlayerCharacterPtr().GetPItem());
        camera->ResetCamera();
        return;
    }
    if (static_cast<CPlayer_Methods&>(((CPlayerManager*)PlayerManager)->GetMainPlayer()).ControlledCreaturePtr().GetPItem() != 0) {
        CThing* creature = static_cast<CPlayer_Methods&>(((CPlayerManager*)PlayerManager)->GetMainPlayer()).ControlledCreaturePtr().GetPItem();
        if (creature->HasTC(0x7a))
            ((CTCPlayerCharacterLinks*)creature->GetTC(0x7a))->AddThing(PlayerCharacterPtr().GetPItem());
    }
}