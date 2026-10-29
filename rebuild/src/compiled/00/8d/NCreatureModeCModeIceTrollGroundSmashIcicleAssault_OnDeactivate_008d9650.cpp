// NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::OnDeactivate() -- stand-mode deactivation, resume a state-1 troll's animation, then empty the icicle list: every icicle trap still alive is switched to trap type 4 and manually reset before its entry is erased
// Leaf: OnDeactivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D9650, 232 bytes (`ret`).
// Callees: CModeStandBase::OnDeactivate 0x00861D00 (qualified base call); CThingAICreature::SetSuspendAnimation(bool, long)
// 0x00833A10; CBaseIntelligentPointer::GetPItem 0x00A01B50 and ~CBaseIntelligentPointer 0x00A01C10; CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound 0x0040F020 (inlined GetInterface, CTCTrapBase = type 0x7d, bit 29 of +0x2c); CTCTrapBase::ManualReset
// 0x00787C0C; STLport __copy<CIcicleData*> 0x008E34C0 (`__fastcall`: first in ecx, last in edx; manifest "_Move<...>" is a
// byte-twin label) from the inlined vector::erase.
// Layout (retail, see the ctor 0x00743BE0): Icicles +0x84 (12-byte STLport vector of CIcicleData, 0x24 bytes: PEffect +0,
// PTrap +8, EffectTimer +0x10, TrapTimer +0x14, Position +0x18). CTCTrapBase: TrapType +0x5c (PDB +0x68 less the three
// 12-byte vectors).
// Shape: begin() is taken once into a local (ebp) and the loop runs `while (icicle != end()) { ...; erase(icicle); }`; the
// trap out pointer is declared outside the loop (a GetInterface miss leaves the previous value, as retail's ebx);
// IS_THING_ALIVE as landed 0x008C92C0; erase as the landed STLport vector::erase 0x0084F570.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                     // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);  // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingAICreature { public:                  // empty here: only its member is called (thing base at offset 0)
public:
    void SuspendAI(bool, void*);   // 0x00833A10
};

class CThing : public CThingAICreature { public:
    unsigned char   _pad00[0x10];
    unsigned char   m_state;               // +0x10 creature state byte
    unsigned char   _pad11[0x0f];
    unsigned long   m_presentBits[9];      // +0x20 std::bitset of interface types
    CInterfaceMap   m_interfaces;          // +0x44
    unsigned char   _pad4c[0x91 - 0x4c];
    unsigned char   m_flags91;             // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCTrapBase { public:                       // interface type 0x7d
    unsigned char _pad00[0x5c];
    long          TrapType;                // +0x5c (ETrapType)
    void ManualReset();                    // 0x00787C0C
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class C3DVector { public: float X, Y, Z; };

struct random_access_iterator_tag {
    random_access_iterator_tag() {}
};

namespace NCreatureMode {
struct CIcicleData {                       // 0x24 bytes
    CIntelligentPointer<CThing> PEffect;      // +0x00
    CIntelligentPointer<CThing> PTrap;        // +0x08
    long                     EffectTimer;  // +0x10
    long                     TrapTimer;    // +0x14
    C3DVector                Position;     // +0x18
};
}

NCreatureMode::CIcicleData* __fastcall __copy(NCreatureMode::CIcicleData* first, NCreatureMode::CIcicleData* last,
                                              NCreatureMode::CIcicleData* result,
                                              const random_access_iterator_tag& tag, int* distance);   // 0x008E34C0

template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class T> struct vector {         // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    bool empty() { return _M_start == _M_finish; }
    T* erase(T* position)
    {
        if (position + 1 != end())
            __copy(position + 1, _M_finish, position, random_access_iterator_tag(), (int*)0);
        --_M_finish;
        _Destroy(_M_finish);
        return position;
    }
};

struct CModeStandBase {                    // retail sizeof 0x68
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x60];
public:
    virtual void OnDeactivate();   // 0x00861D00
};

namespace NCreatureMode {

struct CModeIceTrollGroundSmashIcicleAssault : CModeStandBase {   // vtable 0x01268774
    unsigned char       _members[0x1c];    // PTarget +0x68, Filter +0x70, Timer +0x7c, PDef +0x80
    vector<CIcicleData> Icicles;           // +0x84
    vector<C3DVector>   ViablePoints;      // +0x90
    void OnDeactivate();
};

void CModeIceTrollGroundSmashIcicleAssault::OnDeactivate()
{
    CModeStandBase::OnDeactivate();
    if (ParentThing->m_state == 1)
        ParentThing->SuspendAI(false, 0);
    CIcicleData* icicle = Icicles.begin();
    CTCTrapBase* trap;
    while (icicle != Icicles.end()) {
        if (IS_THING_ALIVE(icicle->PTrap)) {
            icicle->PTrap.GetPItem()->GetInterface(&trap, 0x7d);
            trap->TrapType = 4;
            trap->ManualReset();
        }
        Icicles.erase(icicle);
    }
}

}