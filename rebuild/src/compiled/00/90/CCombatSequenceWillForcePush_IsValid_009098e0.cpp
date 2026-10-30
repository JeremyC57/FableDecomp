// CCombatSequence_WillForcePush::IsValid() -- valid unless the owner's will-power component (interface 0xCE) is inactive, or, for a non-enemy hero, the hero stands within 8 units
// Leaf: IsValid
// Retail: Fable.exe (TLC, VC7.1) @ 0x009098E0, 204 bytes (208 with padding), virtual IsValid.
// Callees: CVectorMap::LowerBound 0x004365B0 (inlined GetInterface, type 0xCE = bit 14 of the bitset dword +0x38),
//   CPlayerManager::GetMainPlayer 0x00449970 (landed name; manifest "Catch@0040d959" is wrong) on the main game component
//   0x013B86A0 +0x1c, CBaseIntelligentPointer::GetPItem 0x00A01B50 (the player's character pointer +0x34),
//   AreThingsEnemies 0x0076D2F0 (__fastcall), CThing::GetPos 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is
//   propagated); 8.0f @ 0x0125916C.
// Layout: sequence owner +0x1c (landed sibling OnCreate rows); the component's active byte +0xc.
// Shape: positive nesting puts the no-component / inactive returns at the end; the hero comes through an inline
//   GetCharacter() reference; the distance is the inline (a - b).GetLength() = sqrt(X*X + (Y*Y + Z*Z)) (evaluated Z, Y, X) kept on the FPU (fsqrt); `if (d < 8)
//   return false; return true;`.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return (float)sqrt(X * X + (Y * Y + Z * Z)); }
};

class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;       // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPos() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing { public: };

bool __fastcall AreThingsEnemies(const CThing* a, const CThing* b);   // 0x0076D2F0

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

class CPlayer { public:
    unsigned char _pad[0x34];
    CIntelligentPointer<CThing> PCharacter;         // +0x34
    const CIntelligentPointer<CThing>& GetCharacter() const { return PCharacter; }
};

// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"

struct CMainGameComponent_Retail {
    unsigned char _pad[0x1c];
    CPlayerManager* PlayerManager;              // +0x1c
};
extern CMainGameComponent_Retail* g_pMainGameComponent;   // 0x013B86A0

struct CTCWillPower {                           // interface type 0xCE
    unsigned char _pad[0xc];
    bool Active;                                // +0x0c
};

class CCombatSequenceBase { public:
    virtual void v00();
    unsigned char _pad[0x18];
    CThingCreatureBase* m_pOwner;               // +0x1c
};

struct CCombatSequence_WillForcePush : CCombatSequenceBase {
    virtual bool IsValid();
};

bool CCombatSequence_WillForcePush::IsValid()
{
    CTCWillPower* willPower;
    if (m_pOwner->GetInterface(&willPower, 0xce)) {
        if (willPower->Active) {
            CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetCharacter().GetPItem();
            if (!AreThingsEnemies(hero, m_pOwner)) {
                if ((hero->GetPos() - m_pOwner->GetPos()).GetLength() < 8.0f)
                    return false;
            }
            return true;
        }
        return false;
    }
    return true;
}