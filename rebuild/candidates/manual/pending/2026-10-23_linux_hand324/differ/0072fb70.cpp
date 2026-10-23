// CTCDummyVillager::FrameUpdate() -- while the villager's creature is gone and the dummy can be recreated: schedule the recreation (day or frame, from the creature def's respawn ranges) once, and when it is due recreate the villager at a random unseen GAZE_OUT_OF_BUILDING_MARKER of its village (nearest to the hero on the second, any-visibility pass; else the village centre), turning it to face the hero
// Grade: DIFFER(1280v1278) -- frame 0x60 and byte-slot packing match; residue is register allocation: VC7.1 CSEs `lea edi,[def+0xB8]` (killing the edi zero register, so thingDef spills) where retail re-reads [esi+0B8h] against edi=0, plus the markers vector / best slot order (0x34 v 0x28) and the begin reload for free()
// Leaf: ?FrameUpdate@CTCDummyVillager
//
// Retail 0x0072FB70, 1280 bytes (FPO `sub esp,60h`, `ret`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCDummyVillager::CanBeRecreated 0x0072F9E0, CTCBase::GetDefinitions
//   0x006869D0, CDefinitionManager::GetDef<T> 0x0042AF3C, CDefClassBase::GetSubDef<CCreatureDef> 0x00485815, CTCBase::GetWorld
//   0x00686A70, CTCBase::GetRandom 0x00686DA0, GFRandom 0x004CDAE0 (__fastcall), the world frame getter 0x0049D870 (static),
//   CVectorMap::LowerBound 0x0040F020, CTCVillageMember::GetPVillage 0x007EE540, CTCBase::GetPlayerManager 0x00686D40,
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetCharacterThing 0x00487DC0, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, vector<CThing*>::push_back
//   0x0062E7E0, the marker shuffle 0x007304F0 (__fastcall), the marker filter functor 0x006A8AF0, CThing::GetPosition 0x004C73D0,
//   CTCDummyVillager::CreateVillager 0x0072F750, IS_THING_ALIVE 0x004AB8F0, C3DVector::Normalise 0x00A14440, CRightHandedSet
//   0x00487050, free 0x00BFEA14.
// Shape: defs fetched into unscoped out slots and released by hand (CTCGold idiom); marker filter is a temporary of an empty
//   functor with a user ctor (packs into the byte slot beside found/second); passes are `for (bool second = false; !found &&
//   second != true; second = true)`; GetLength sums z*z first.
// Layout: Ego_r.pdb CTCDummyVillager (landed CanBeRecreated 0x0072F9E0): ParentThing +0x04, DefType +0x0C, Creature +0x30,
//   DayToBeRecreated +0x40, FrameToBeRecreated +0x44, Respawnable +0x48; retail CThing dead flag +0x91 bit 0, def index (word)
//   +0x8C, graphic +0x60 (vslot 32 sets the orientation); creature def respawn day range +0xB8/+0xBC, frame range +0xC0/+0xC4;
//   world camera +0x34 (vslot 3 point visible, vslot 6 point in view); village marker list +0x70 (node next +0, pointer +8).
#include <stddef.h>
extern "C" void __cdecl free(void*);
#pragma intrinsic(sqrt)
extern "C" double __cdecl sqrt(double);

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& b) const { return C3DVector(X - b.X, Y - b.Y, Z - b.Z); }
    float GetLength() const { return (float)sqrt(Z * Z + Y * Y + X * X); }
    void Normalise();                                   // 0x00A14440
};
struct CRightHandedSet {
    C3DVector Forward, Up;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);   // 0x00487050
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CGraphic {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04();
    virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13(); virtual void m14();
    virtual void m15(); virtual void m16(); virtual void m17(); virtual void m18(); virtual void m19();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23(); virtual void m24();
    virtual void m25(); virtual void m26(); virtual void m27(); virtual void m28(); virtual void m29();
    virtual void m30(); virtual void m31();
    virtual void SetOrientation(const CRightHandedSet& set);   // slot 32 (+0x80)
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CGraphic*      Graphic;                             // +0x60
    unsigned char  _pad64[0x8c - 0x64];
    unsigned short DefIndex;                            // +0x8C
    unsigned char  _pad8e[0x91 - 0x8e];
    unsigned char  Flags91;                             // +0x91
    const C3DVector& GetPosition() const;               // 0x004C73D0
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    void* GetPItem() const;                             // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer<CThing>& p);   // 0x004AB8F0
struct SMarkerNode {
    SMarkerNode* Next;
    SMarkerNode* Prev;
    CIntelligentPointer<CThing> Marker;                 // +0x08
};
struct CTCVillage {
    void*       _vt;
    CThing*     ParentThing;                            // +0x04
    unsigned char _pad08[0x70 - 0x08];
    struct CMarkerList {
        SMarkerNode* Head;                              // std::list sentinel
        SMarkerNode* begin() const { return Head->Next; }
        SMarkerNode* end() const { return Head; }
    } Markers;                                          // +0x70
};
struct CTCVillageMember {
    CTCVillage* GetPVillage() const;                    // 0x007EE540
};
template<class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CRetailVector() { if (_M_start) free(_M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void push_back(const T& value);                     // 0x0062E7E0
};
void __fastcall RandomShuffle(CThing** first, CThing** last);   // 0x007304F0
struct CMarkerFilter { CMarkerFilter() {} bool operator()(CThing* marker) const; };   // 0x006A8AF0
struct CCamera {
    virtual void m00(); virtual void m01(); virtual void m02();
    virtual bool IsPointVisible(const C3DVector& p);    // slot 3 (+0x0C)
    virtual void m04(); virtual void m05();
    virtual bool IsPointInView(const C3DVector& p);     // slot 6 (+0x18)
};
struct CGameClock { unsigned char _pad00[0x14]; long Day; };
struct CWorld {
    unsigned char _pad00[0x1c];
    CGameClock*   Clock;                                // +0x1C
    unsigned char _pad20[0x34 - 0x20];
    CCamera*      Camera;                               // +0x34
    static long GetFrame();                             // 0x0049D870
};
struct CRandom;
long __fastcall GFRandom(long range, CRandom* rng);    // 0x004CDAE0
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
};
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
    template<class T> bool GetSubDef(CDefPointer<T>* out) const;   // 0x00485815
};
struct CCreatureDef : CDefClassBase {
    unsigned char _pad08[0xb8 - 0x08];
    long RespawnDayMin, RespawnDayMax;                  // +0xB8
    long RespawnFrameMin, RespawnFrameMax;              // +0xC0
};
struct CThingDef : CDefClassBase {};
struct CDefinitionManager {
    template<class T> bool GetDef(long index, CDefPointer<T>* out) const;   // 0x0042AF3C
    long GetDefGlobalIndexFromName(const CCharString& name);                // 0x009AD410
};
struct CGameDefinitionManager { static CDefinitionManager* Get(); };   // 0x0044C6B0
struct CPlayer { CThing* GetCharacterThing(); };       // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };   // 0x00449970

struct CTCDummyVillager {
    virtual void v00();
    CThing*                     ParentThing;            // +0x04
    unsigned char               _pad08[0x4];
    long                        DefType;                // +0x0C
    unsigned char               _pad10[0x30 - 0x10];
    CIntelligentPointer<CThing> Creature;               // +0x30
    unsigned long               CreatureUID[2];         // +0x38
    long                        DayToBeRecreated;       // +0x40
    long                        FrameToBeRecreated;     // +0x44
    bool                        Respawnable;            // +0x48
    CDefinitionManager* GetDefinitions();               // 0x006869D0
    CWorld* GetWorld();                                 // 0x00686A70
    CRandom* GetRandom();                               // 0x00686DA0
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
    bool CanBeRecreated();                              // 0x0072F9E0
    void CreateVillager(CTCVillage* village, const C3DVector& pos, bool flag);   // 0x0072F750
    void FrameUpdate();
};

void CTCDummyVillager::FrameUpdate()
{
    if (Creature.GetPItem() && !(Creature.GetPItem()->Flags91 & 1))
        return;
    if (!CanBeRecreated())
        return;
    if (DayToBeRecreated == 0x7fffffff && FrameToBeRecreated == 0x7fffffff) {
        CDefPointer<CThingDef> thingOut;
        GetDefinitions()->GetDef(DefType, &thingOut);
        CThingDef* thingDef = thingOut.Object;
        CDefPointer<CCreatureDef> creatureOut;
        bool found = thingDef->GetSubDef(&creatureOut);
        CCreatureDef* creatureDef = creatureOut.Object;
        if (found) {
            if (creatureDef->RespawnDayMin == 0 && creatureDef->RespawnDayMax == 0) {
                DayToBeRecreated = GetWorld()->Clock->Day;
                CWorld* world = GetWorld();
                FrameToBeRecreated = GFRandom(creatureDef->RespawnFrameMax - creatureDef->RespawnFrameMin, GetRandom()) + world->GetFrame() + creatureDef->RespawnFrameMin;
            } else {
                long today = GetWorld()->Clock->Day;
                DayToBeRecreated = GFRandom(creatureDef->RespawnDayMax - creatureDef->RespawnDayMin, GetRandom()) + creatureDef->RespawnDayMin + today;
                FrameToBeRecreated = 0x7fffffff;
            }
        }
        if (creatureDef)
            creatureDef->Release();
        if (thingDef)
            thingDef->Release();
    }
    bool due;
    if (FrameToBeRecreated != 0x7fffffff)
        due = GetWorld()->GetFrame() >= FrameToBeRecreated;
    else
        due = GetWorld()->Clock->Day >= DayToBeRecreated;
    if (!due)
        return;
    CThing* parent = ParentThing;
    if (!parent->HasTC(0x23))
        return;
    CTCVillageMember* member = parent->GetTC<CTCVillageMember>(0x23);
    if (!member->GetPVillage())
        return;
    C3DVector best;
    bool found = false;
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetCharacterThing();
    long markerDef = CGameDefinitionManager::Get()->GetDefGlobalIndexFromName("GAZE_OUT_OF_BUILDING_MARKER");
    CTCVillage::CMarkerList& list = member->GetPVillage()->Markers;
    CRetailVector<CThing*> markers;
    for (SMarkerNode* node = list.begin(); node != list.end(); node = node->Next)
        markers.push_back(node->Marker.GetPItem());
    RandomShuffle(markers.begin(), markers.end());
    for (bool second = false; !found && second != true; second = true) {
        for (CThing** it = markers.begin(); it != markers.end(); ++it) {
            CThing* m = *it;
            if (!m || (m->Flags91 & 1) || m->DefIndex != markerDef || !CMarkerFilter()(m))
                continue;
            if (GetWorld()->Camera->IsPointVisible((*it)->GetPosition())) {
                if (!second)
                    continue;
                if (GetWorld()->Camera->IsPointInView((*it)->GetPosition()) == true)
                    continue;
            }
            if (found && !((best - hero->GetPosition()).GetLength() > ((*it)->GetPosition() - hero->GetPosition()).GetLength()))
                continue;
            found = true;
            best = (*it)->GetPosition();
            if (second)
                break;
        }
    }
    if (!found && member) {
        if (CTCVillage* village = member->GetPVillage())
            best = village->ParentThing->GetPosition();
    }
    if (member) {
        if (CTCVillage* village = member->GetPVillage()) {
            CreateVillager(village, best, true);
            if (IS_THING_ALIVE(Creature)) {
                C3DVector dir = hero->GetPosition() - Creature.GetPItem()->GetPosition();
                dir.Normalise();
                Creature.GetPItem()->Graphic->SetOrientation(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), dir));
            }
        }
    }
}
