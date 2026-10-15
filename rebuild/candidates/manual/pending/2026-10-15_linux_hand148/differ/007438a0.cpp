// NCreatureMode::CModeDragonStrafe::CModeDragonStrafe(CThing*) -- CModeDragonFlyingBase(thing), own vtable, the "STRAFE" filter, Timer = 0, Speed = 0
// Grade: DIFFER(145v145) -- same `push -1` placement as 007437c0
// Leaf: ??0CModeDragonStrafe
// Retail: Fable.exe (TLC, VC7.1) @ 0x007438A0 (`ret 4`). Vtable 0x012686C8 = NCreatureMode::CModeDragonStrafe (rtti-vtables.tsv); the manifest label CModeDragonFlying is propagated.
// Callees: CModeBase::CModeBase(CThing*) @ 0x0073EAC0, CCharString @ 0x0099EBF0 / 0x0099E4B0 / operator= 0x0099EFB0 /
// ~ 0x0099EAE0, CRC @ 0x004014A0.
// Shape: the stores before the derived vtable store (+0x1c..+0x2c: the spline's CArray zeroed, Tension 0.5, the bool)
// are the inline constructor of the `__declspec(novtable)` intermediate base CModeDragonFlyingBase (its own vtable
// 0x01268658 is never stored); the filter (vtable 0x01267568) is built from an implicit CCharString conversion of the
// literal; its CRC is one `p ? .. : ..` guard pair that VC folds into one null test.
// Layout: retail CArray is 0x0c (PDB 0x10), so every PDB offset after the spline sits 4 bytes lower.
struct CThing;
struct CBaseObject;

struct CCharStringData {
    const char*   m_pData;                 // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
};

struct CCharString {
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0 (default argument: the literal converts implicitly)
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC

struct CAnimFilterOfGroup {                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = Calc_004014a0(0, group.m_pData == 0 ? "" : group.m_pData->m_pData, group.m_pData == 0 ? 0 : group.m_pData->m_length);   // one shared null test (CSE of the two guards)
    }
};

struct CBaseIntelligentPointer {           // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* m_pItem;                  // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    void SetPItem(CBaseObject* item);      // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* item) { SetPItem((CBaseObject*)item); }
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, sizeof 0x1c (vtable 0x012673CC)
    virtual void v00();
    unsigned char _body[0x18];             // ParentThing, WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    CModeBase(CThing* thing);              // 0x0073EAC0
};

struct C3DVector { float x, y, z; };

template <class T> struct CArray {         // retail CArray is 0x0c: data, size, capacity
    T*   m_pData;
    long m_size;
    long m_capacity;
    CArray() : m_pData(0), m_size(0), m_capacity(0) { }
};

struct CCardinalSpline {                   // retail 0x14 (PDB 0x18 with the 16-byte CArray)
    CArray<C3DVector> ControlPoints;       // +0
    float             Tension;             // +0xc
    bool              EndPointsConnected;  // +0x10
    CCardinalSpline() : Tension(0.5f), EndPointsConnected(false) { }
};

struct CCreatureActionBase;
struct SCountBlock;
template <class T> struct CCountedPointer {
    T*           m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

namespace NCreatureMode {

struct __declspec(novtable) CModeDragonFlyingBase : CModeBase {   // vtable 0x01268658 (never stored here: novtable)
    CCardinalSpline FlightSpline;          // +0x1c
    float           SplinePercentageComplete;   // +0x30
    float           LegAnimAlpha;          // +0x34
    CModeDragonFlyingBase(CThing* thing) : CModeBase(thing) { }
};

struct CModeDragonStrafe : CModeDragonFlyingBase {   // vtable 0x012686C8
    CAnimFilterOfGroup Filter;                       // +0x38
    long               Timer;                        // +0x44
    long               BurninateStartTime;           // +0x48
    long               BurninateDuration;            // +0x4c
    bool               Burninated;                   // +0x50
    float              Speed;                        // +0x54
    virtual void v00();
    CModeDragonStrafe(CThing* thing);
};

CModeDragonStrafe::CModeDragonStrafe(CThing* thing)
    : CModeDragonFlyingBase(thing)
    , Filter("STRAFE")
    , Timer(0)
    , Speed(0)
{
}

}
