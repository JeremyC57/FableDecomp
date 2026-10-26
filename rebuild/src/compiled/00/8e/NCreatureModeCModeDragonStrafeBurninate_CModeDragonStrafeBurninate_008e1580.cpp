// NCreatureMode::CModeDragonStrafeBurninate::CModeDragonStrafeBurninate(CThing*, const vector<C3DVector>&, const C3DVector&, float, float, const C3DVector&, const C3DVector&) -- fly the given path, then burn from FlameStartPos to FlameEndPos between the two path percentages
// Leaf: ??0CModeDragonStrafeBurninate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E1580, 234 bytes (`ret 1Ch`). Vtable 0x01268718 = NCreatureMode::CModeDragonStrafeBurninate.
// Callees: CModeDragonFlying::CModeDragonFlying(CThing*, const vector<C3DVector>&, const C3DVector&, float,
// CCountedPointer<CCreatureActionBase>) 0x008E1150 (speed 30.0f, an empty end action passed by value);
// CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10; CCharString(const char*, long) 0x0099EBF0,
// operator= 0x0099EFB0, ~CCharString 0x0099EAE0; CRC 0x004014A0.
// Layout (retail): CModeDragonFlying 0x94 (Filter +0x38: Group +0x3c, GroupCRC +0x40), Timer +0x94, PBeamWeaponEffect +0x98,
// LastExplodeFrame +0xa0, FlameStartPos +0xa4, FlameEndPos +0xb0, FlameActive +0xbc, FlameStartPercentage +0xc0,
// FlameStopPercentage +0xc4.
// Shape: the overload of 0x00743990 for a scripted path: Timer and LastExplodeFrame are left alone; the C3DVector
// copies are POD memberwise copies; the body hands the literal's implicit CCharString conversion to the inherited
// filter's inline SetGroup (the CRC's null arm is laid out after the return).
class CThing;
class CThingCreatureBase;
class CBaseObject;

class CCharStringData { public:
    const char*   m_pData;                 // +0  (CBasicString<char>::PData)
    unsigned long m_length;                // +4  (StringLength)
};

class CCharString { public:
    CCharStringData* m_pData;
    CCharString();                         // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};

class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0: CRC

class CAnimFilterOfGroup { public:                // vtable 0x01267568
    virtual void v00();
    CCharString   Group;                   // +4
    unsigned long GroupCRC;                // +8
    CAnimFilterOfGroup(const CCharString& group)
        : Group()
    {
        Group = group;
        GroupCRC = CCRC::Calc(0, group.GetPString(), group.GetLength());
    }
    void SetGroup(const CCharString& group)
    {
        Group = group;
        GroupCRC = CCRC::Calc(0, group.GetPString(), group.GetLength());
    }
};

struct CBaseIntelligentPointer {           // 8 bytes: vptr + the pointed-to item
    virtual void v00();
    CBaseObject* PItem;                    // +4
    CBaseIntelligentPointer();             // 0x00A01B10
    void SetPItem(void* item);             // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing() { }
    CIntelligentPointerThing(CThing* item) { SetPItem(item); }
};

struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointerCreature() { }
};

template <class T> struct CAllocator {     // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {    // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector& operator=(const C3DVector& other) { X = other.X; Y = other.Y; Z = other.Z; return *this; }
};

template <class T> class CCountedPointer { public:   // 8 bytes: object + shared count block
    T*    Object;
    void* Count;
    CCountedPointer() : Object(0), Count(0) { }
    CCountedPointer(const CCountedPointer& other);   // user copy: a by-value argument is built in its stack slot
    ~CCountedPointer();
};

template <class T> struct vector;          // STLport vector (only passed by reference)
class CCreatureActionBase;

namespace NCreatureMode {

struct CModeDragonFlying {                 // vtable 0x0126868C, retail sizeof 0x94
    virtual void v00();
    unsigned char      _base[0x34];        // CModeBase + CModeDragonFlyingBase
    CAnimFilterOfGroup Filter;             // +0x38
    unsigned char      _rest[0x50];        // Destination .. UsePreviousPos
    CModeDragonFlying(CThing* thing, const vector<C3DVector>& path, const C3DVector& destination, float speed,
                      CCountedPointer<CCreatureActionBase> endAction);   // 0x008E1150
};

struct CModeDragonStrafeBurninate : CModeDragonFlying {   // vtable 0x01268718
    long                     Timer;                 // +0x94
    CIntelligentPointerThing PBeamWeaponEffect;     // +0x98
    long                     LastExplodeFrame;      // +0xa0
    C3DVector                FlameStartPos;         // +0xa4
    C3DVector                FlameEndPos;           // +0xb0
    bool                     FlameActive;           // +0xbc
    float                    FlameStartPercentage;  // +0xc0
    float                    FlameStopPercentage;   // +0xc4
    virtual void v00();
    CModeDragonStrafeBurninate(CThing* thing, const vector<C3DVector>& path, const C3DVector& destination,
                               float flameStartPercentage, float flameStopPercentage,
                               const C3DVector& flameStartPos, const C3DVector& flameEndPos);
};

CModeDragonStrafeBurninate::CModeDragonStrafeBurninate(CThing* thing, const vector<C3DVector>& path, const C3DVector& destination,
                                                       float flameStartPercentage, float flameStopPercentage,
                                                       const C3DVector& flameStartPos, const C3DVector& flameEndPos)
    : CModeDragonFlying(thing, path, destination, 30.0f, CCountedPointer<CCreatureActionBase>())
{
    FlameStartPos = flameStartPos;
    FlameEndPos = flameEndPos;
    FlameActive = false;
    FlameStartPercentage = flameStartPercentage;
    FlameStopPercentage = flameStopPercentage;
    Filter.SetGroup("STRAFE_BURNINATE");
}

}