// CAIStateGroup_GoToPub::WalktoSeatAndSit(bool) -- walk to the chosen seat and turn to face along the seat's forward vector while the pub and table live (and, when leaving at cooking time is requested, it is cooking time), then sit down on a new tavern stool mode and mark the seat as taken; terminate on any failure
// Grade: DIFFER(808v808) nd=71 (was 122; size now equal) -- levers: HasTC through a named unsigned long word local (retail keeps the shr-dword bit test; the one-expression form narrows bit 24 to a byte load), and a named forward copy before the look sum. Tried: !=0 / const-ref HasTC forms (122), assigned look (86), named-return operator+ (77). Residue: frame 0xd8 v 0xd4 (one extra slot) shifting every frame offset, look X/Y/Z slot scatter. Next: find the extra 4-byte local (the villager out-pointer path).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00949580, length 806 bytes (+ int3 padding), `ret 4`.
// Callees: CVectorMap::LowerBound @ 0x0040F020, GFGetPVillageInterface @ 0x007EE540 (`__fastcall`), CBaseIntelligentPointer::
// GetPItem @ 0x00A01B50, CTCVillage::IsCookTime @ 0x00658C0A, AIAction_MoveToPos @ 0x0097EC00, CThingCreatureBase::
// GetPosition @ 0x006A4D00, AIAction_LookAtPos @ 0x0097D580, CCharString ctor @ 0x0099EBF0 / dtor @ 0x0099EAE0,
// CThingCreatureBase::GetAnimFromName @ 0x00662A00, operator new @ 0x00BFEA1A, CModeSitTavernStool ctor @ 0x0094B530,
// CCountedPointer<CModeSitBase> ctor @ 0x0094B3A0 / copy ctor @ 0x008AA6A0 / dtor @ 0x008AB320, CCreatureAction_SitDown ctor
// @ 0x008AB9D0 / dtor @ 0x008ABAA0, AIAction_PerformActionAndWait @ 0x0097DB70, CTCTavernTable::SetSeatTaken @ 0x007B8530,
// Terminate @ 0x008FCF40.
// Layout: Ego_r.pdb CAIStateGroup_GoToPub PPub +0x14, PTable +0x24, SeatIndex +0x3c, Pos +0x40, RHSet +0x4c (Forward
// +0x58), Sitting +0x70.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet {
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool HasTC(long type) const { unsigned long word = m_presentBits[type >> 5]; return ((word >> (type & 31)) & 1) != 0; }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool has = (m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return has;
    }
};
struct CThingCreatureBase : CThing {
    const C3DVector& GetPosition() const;       // 0x006A4D00
    long GetAnimFromName(const CCharString& name, bool b) const;   // 0x00662A00
};
struct CTCVillager;                             // interface 0x23
struct CTCVillage {
    bool IsCookTime();                          // 0x00658C0A
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);   // 0x007EE540
struct CTCTavernTable {                         // interface 0xb8
    void SetSeatTaken(long seat, bool taken);   // 0x007B8530
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CModeSitBase;
struct CModeSitTavernStool {
    unsigned char _data[0x38];
    CModeSitTavernStool(CThingCreatureBase* creature);   // 0x0094B530
};
struct CModeSitPointer {                        // CCountedPointer<CModeSitBase>
    void* Object;
    void* Count;
    CModeSitPointer(CModeSitTavernStool* mode); // 0x0094B3A0
    CModeSitPointer(const CModeSitPointer& o);  // 0x008AA6A0
    ~CModeSitPointer();                         // 0x008AB320
};
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _body[0xac];
};
struct CCreatureAction_SitDown : CCreatureActionBase {
    CCreatureAction_SitDown(CThingCreatureBase* creature, long anim, CModeSitPointer mode, bool b);   // 0x008AB9D0
    ~CCreatureAction_SitDown();                 // 0x008ABAA0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool& moving, long anim, bool lookAround, bool b6,
                            const CThing* ignoreThing, bool b8, float speedScale);   // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long lookFlags, float tolerance);   // 0x0097D580
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CBaseIntelligentPointer PPub;               // +0x14
    CBaseIntelligentPointer PBar;               // +0x1c
    CBaseIntelligentPointer PTable;             // +0x24
    CBaseIntelligentPointer PBarman;            // +0x2c
    CBaseIntelligentPointer PTankard;           // +0x34
    long SeatIndex;                             // +0x3c
    C3DVector Pos;                              // +0x40
    CRightHandedSet RHSet;                      // +0x4c
    long ActionPointIndex;                      // +0x64
    long NextFramePub;                          // +0x68
    long FrameLeavePub;                         // +0x6c
    bool Sitting;                               // +0x70
    bool WalktoSeatAndSit(bool leaveAtCookTime);
};

bool CAIStateGroup_GoToPub::WalktoSeatAndSit(bool leaveAtCookTime)
{
    CTCVillager* villager;
    CThingCreatureBase* creature = GetCreature();
    bool flag = creature->HasTC(0x23);
    if (flag)
        villager = creature->GetTC<CTCVillager>(0x23);
    CTCVillage* village = GFGetPVillageInterface(villager);
    CTCTavernTable* table;
    CThing* tableThing = PTable.GetPItem();
    flag = tableThing->HasTC(0xb8);
    if (flag)
        table = tableThing->GetTC<CTCTavernTable>(0xb8);
    long result = 3;
    do {
        if (!IS_THING_ALIVE(PPub) || (!village->IsCookTime() && leaveAtCookTime) || !IS_THING_ALIVE(PTable) || result == 1) {
            Terminate();
            return false;
        }
        result = AIAction_MoveToPos(2, Pos, flag, 3, true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2) {
        Terminate();
        return false;
    }
    C3DVector forward = RHSet.Forward;
    C3DVector look = forward + GetCreature()->GetPosition();
    do {
        if (!IS_THING_ALIVE(PPub) || (!village->IsCookTime() && leaveAtCookTime) || !IS_THING_ALIVE(PTable)) {
            Terminate();
            return false;
        }
        result = AIAction_LookAtPos(2, look, 0x11, 0.01f);
    } while (result == 4);
    if (result == 1) {
        Terminate();
        return false;
    }
    long anim = GetCreature()->GetAnimFromName(CCharString("SIT_AT_TABLE_INTO"), false);
    CModeSitPointer mode(new CModeSitTavernStool(GetCreature()));
    bool failed = AIAction_PerformActionAndWait(&CCreatureAction_SitDown(GetCreature(), anim, mode, false), false) != 2;
    if (failed) {
        Terminate();
        return false;
    }
    table->SetSeatTaken(SeatIndex, true);
    Sitting = true;
    return true;
}
