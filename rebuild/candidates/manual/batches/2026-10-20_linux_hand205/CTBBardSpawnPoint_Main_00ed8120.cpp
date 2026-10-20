// CTB_BardSpawnPoint::Main() -- spawn the Bowerstone bard at the spawn point and hand him to the parent quest as stBard
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ED8120, length 240 bytes (entity-script vtable 0x012F34C0 slot 1).
// Callees: CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0, ~CBaseClass @ 0x0099A2E0,
// operator delete @ 0x00BFE9BC, CScriptThing::GetPos (virtual slot 6, +0x18) on Me, and
// CGameScriptInterface::CreateCreature (virtual slot 91, +0x16C).
// Layout = Ego_r.pdb CTB_BardSpawnPoint (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8},
// ParentClass +0x14 (CV_TalentlessBardScript*), PMasterData +0x18. In the parent, stBard sits at PDB +0x68,
// retail +0x54 (the quest script base is 0x14 bytes smaller in retail; see CQ_HangingTreeGoodScript 0x00D74F80).
// Shape: `ParentClass->stBard = Game->CreateCreature(def, Me.GetPos(), name, false);` -- the two literals are named
// locals ("TB_Bard" built first, so it sits above the def name), the returned CScriptThing is a frame temporary whose
// inline destructor (vtable store, counted release, zero stores, ~CBaseClass) runs after the inlined PImp
// assignment of the compiler-generated CScriptThing::operator=.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

struct C3DVector { float X, Y, Z; };

struct CCPPointerInfo {                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};

struct CScriptThing;
struct CCountedScriptImp {                               // CCountedPointer<CScriptThing>
    CScriptThing*   Data;
    CCPPointerInfo* Info;
    CCountedScriptImp& operator=(const CCountedScriptImp& other)
    {
        CCPPointerInfo* info = other.Info;
        CScriptThing*   data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedScriptImp()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CBaseClass {                                      // polymorphic root of the script handles
    virtual ~CBaseClass();                               // 0x0099A2E0
};

struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C
    CCountedScriptImp PImp;                              // +0x04 Data, +0x08 Info
    virtual ~CScriptThing() {}
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const;
    virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18) 0x004AA980
};

#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
struct CGameScriptInterface {                            // retail vtable 0x01260F0C
    VF10(00) VF10(01) VF10(02) VF10(03) VF10(04) VF10(05) VF10(06) VF10(07) VF10(08)
    VF(090)
    virtual CScriptThing CreateCreature(const CCharString& defName, const C3DVector& position,
                                        const CCharString& scriptName, bool persistent) const;  // slot 91 (+0x16C) 0x008A9100
};

struct CV_TalentlessBardScript {
    unsigned char _base[0x54];                           // quest script base and earlier members
    CScriptThing  stBard;                                // +0x54 (PDB +0x68)
};

struct CEntityScriptBase {
    virtual void v00();
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
};

struct CTB_BardSpawnPoint : CEntityScriptBase {
    CV_TalentlessBardScript* ParentClass;                // +0x14
    void*                    PMasterData;                // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

void CTB_BardSpawnPoint::Main()
{
    CCharString scriptName("TB_Bard");
    CCharString defName("CREATURE_BOWERSTONE_BARD");
    ParentClass->stBard = Game->CreateCreature(defName, Me.GetPos(), scriptName, false);
}
