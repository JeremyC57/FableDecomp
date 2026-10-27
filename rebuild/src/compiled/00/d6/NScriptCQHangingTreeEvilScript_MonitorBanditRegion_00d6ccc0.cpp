// NScript::CQ_HangingTreeEvilScript::MonitorBanditRegion() -- track the freedom fighters: refill CurrentBanditVector with every HTE_FreedomFighter and wait (frame by frame, refilling whenever a bandit was killed) until BanditsAtNextRegion reaches the bandit count, then flag that the bandits left the region
// Leaf: ?MonitorBanditRegion@CQ_HangingTreeEvilScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6CCC0, 247 bytes, thiscall, plain `ret`.
// Identity (proven): CQ_HangingTreeEvilScript::SpawnParty 0x00D6C4A0 builds a CSpawnedFunc named "MonitorBanditRegion" (0x012D2374) whose member-function pointer is this address (store at 0x00D6C76D); ?MonitorBanditRegion@CQ_HangingTreeEvilScript@NScript@@QAEXXZ is in Ego_r.pdb and FableWin.pdb.
// Label: the manifest's "sub_00d6ccc0" is a gap-scan placeholder.
// Callees: std::vector<CScriptThing>::erase 0x008AC9D0 (the inline clear()), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940; interface slots 75 GetAllThingsWithScriptName, 7 NewScriptFrame.
// Data: "HTE_FreedomFighter" @ 0x012D2050.
// Layout: retail CScriptBase is 0x40 bytes (Ego_r.pdb 0x54), then the interface +0x40 and the master data +0x44; the quest's own members sit 0x14 below their Ego_r.pdb offsets, 4 lower again after each Dinkumware container. BanditKilled +0x4a, BanditsLeftRegion +0x4e, BanditsAtNextRegion +0x68, CurrentBanditVector +0xb8 (Ego_r.pdb +0x5e/+0x62/+0x7c/+0xd0).
// Shape: size() is the signed /12 of the byte span compared unsigned against the long count.
class CScriptThing;
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6 layout (0x0C): start, finish, end of storage
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    typedef T* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    T* erase(T* first, T* last);                         // 0x008AC9D0 for CScriptThing
    void clear() { erase(begin(), end()); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
}
class CCharString {                                      // 4 bytes: the counted string-data pointer
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const char* text);       // 0x0099EFE0
};
class C3DVector;                                         // 1-aligned 12-byte vector (only referenced here)
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    long  RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T>
class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 (CScriptThing instance)
};
class CBaseClass {                                       // vtable 0x01231710
public:
    CBaseClass() { }
    CBaseClass(const CBaseClass&) { }
    virtual ~CBaseClass();                                // 0x0099A2E0
};
class CScriptThing : public CBaseClass {                 // 12 bytes, vtable 0x01238C8C (slot names: rtti-vtables.tsv / Ego_r.pdb)
public:
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
    virtual ~CScriptThing();                              // 0x004AA840
    virtual const CCharString& GetName() const;           // slot 1
    virtual CCharString GetDefName() const;               // slot 2
    virtual CCharString GetDataString() const;            // slot 3 (+0x0c)
    virtual void SetDataString(const CCharString& data);  // slot 4
    virtual void GetFocalPos() const;                     // slot 5 (signatures of 5, 7..17, 19 simplified: not called here)
    virtual const C3DVector& GetPos() const;              // slot 6 (+0x18)
    virtual void GetHomePos() const;                      // slot 7
    virtual void GetCurrentMapName() const;               // slot 8
    virtual void GetHomeMapName() const;                  // slot 9
    virtual float GetAngleXY() const;                     // slot 10
    virtual void GetPThing() const;                       // slot 11
    virtual void GetPThingUniqueID() const;               // slot 12
    virtual void GetCurrentStateGroupType() const;        // slot 13
    virtual void GetCurrentScriptPriority() const;        // slot 14
    virtual void IsSneaking() const;                      // slot 15
    virtual void IsAwareOfHero() const;                   // slot 16
    virtual void MsgIsTriggeredBy() const;                // slot 17
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    virtual void t019() const;                            // slot 19
    virtual bool MsgIsUsedBy(const CCharString& user) const;       // slot 20 (+0x50)
    CCountedPointer<CScriptThing> PImp;                   // +0x04 Data, +0x08 Info
};
class CGameScriptInterfaceBase {                     // the script interface (vtable of CGameScriptInterface 0x01260F0C;
public:                                               //   named slots: ghidra_out/cgsi_slots_aligned.tsv, the rest are fillers)
    virtual ~CGameScriptInterfaceBase();
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& things) const;   // slot 75 (+0x12c)
};
class CPersistContext;
class CScriptBase {                                      // retail 0x40 bytes (Ego_r.pdb 0x54 with Dinkumware containers)
public:
    virtual ~CScriptBase();                               // slot 0
protected:
    virtual void RegisterMain();                          // slot 1
    virtual void Main();                                  // slot 2
    virtual void Init();                                  // slot 3 (+0x0c)
public:
    virtual void OnPersist(CPersistContext& context);     // slot 4 (+0x10)
    bool IsActiveThreadTerminating();                     // 0x00CB7940
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                       // +0x40
    TData*                    MasterScriptData;           // +0x44
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HangingTreeEvilScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned char _pad048[0x2];
    bool BanditKilled;                                  // +0x4a (Ego_r.pdb +0x5e)
    unsigned char _pad04b[0x3];
    bool BanditsLeftRegion;                             // +0x4e (Ego_r.pdb +0x62)
    unsigned char _pad04f[0x19];
    long BanditsAtNextRegion;                           // +0x68 (Ego_r.pdb +0x7c)
    unsigned char _pad06c[0x40];                        // +0x74 Executioner .. +0xac CurrentEnemyVector
    std::vector<CScriptThing> CurrentEnemyVector;       // +0xac
    std::vector<CScriptThing> CurrentBanditVector;      // +0xb8 (Ego_r.pdb +0xd0)
    void MonitorBanditRegion();
};
}

void NScript::CQ_HangingTreeEvilScript::MonitorBanditRegion()
{
    BanditsLeftRegion = false;
    CurrentBanditVector.clear();
    Game->GetAllThingsWithScriptName("HTE_FreedomFighter", CurrentBanditVector);
    while ((unsigned int)BanditsAtNextRegion < CurrentBanditVector.size()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (BanditKilled) {
            if (IsActiveThreadTerminating()) return;
            CurrentBanditVector.clear();
            Game->GetAllThingsWithScriptName("HTE_FreedomFighter", CurrentBanditVector);
            BanditKilled = false;
        }
    }
    if (IsActiveThreadTerminating()) return;
    BanditsLeftRegion = true;
}