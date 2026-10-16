// CCombatSequence_WillForcePush::IsValid() -- valid unless the owner's will-power component (interface 0xCE) is inactive, or, for a non-enemy hero, the hero stands within 8 units
// Leaf: IsValid
// Retail: Fable.exe (TLC, VC7.1) @ 0x009098E0, 204 bytes (208 with padding), virtual IsValid.
// Callees: CVectorMap::LowerBound 0x004365B0 (inlined GetInterface, type 0xCE = bit 14 of the bitset dword +0x38),
//   CPlayerManager::GetMainPlayer 0x00449970 (landed name; manifest "Catch@0040d959" is wrong) on the main game component
//   0x013B86A0 +0x1c, CBaseIntelligentPointer::GetPItem 0x00A01B50 (the player's character pointer +0x34),
//   AreThingsEnemies 0x0076D2F0 (__fastcall), CThing::GetPosition 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is
//   propagated); 8.0f @ 0x0125916C.
// Layout: sequence owner +0x1c (landed sibling OnCreate rows); the component's active byte +0xc.
// Shape: positive nesting puts the no-component / inactive returns at the end; the hero comes through an inline
//   GetCharacter() reference; the distance is the inline (a - b).GetLength() = sqrt(X*X + (Y*Y + Z*Z)) (evaluated Z, Y, X) kept on the FPU (fsqrt); `if (d < 8)
//   return false; return true;`.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLength() const { return (float)sqrt(X * X + (Y * Y + Z * Z)); }
};

struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CThingCreatureBase : CThing { };

bool __fastcall AreThingsEnemies(const CThing* a, const CThing* b);   // 0x0076D2F0

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CPlayer {
    unsigned char _pad[0x34];
    CBaseIntelligentPointer PCharacter;         // +0x34
    const CBaseIntelligentPointer& GetCharacter() const { return PCharacter; }
};

struct CPlayerManager {
    CPlayer* GetMainPlayer();                   // 0x00449970
};

struct CMainGameComponent_Retail {
    unsigned char _pad[0x1c];
    CPlayerManager* PlayerManager;              // +0x1c
};
extern CMainGameComponent_Retail* g_pMainGameComponent;   // 0x013B86A0

struct CTCWillPower {                           // interface type 0xCE
    unsigned char _pad[0xc];
    bool Active;                                // +0x0c
};

struct CCombatSequenceBase {
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
            CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetCharacter().GetPItem();
            if (!AreThingsEnemies(hero, m_pOwner)) {
                if ((hero->GetPosition() - m_pOwner->GetPosition()).GetLength() < 8.0f)
                    return false;
            }
            return true;
        }
        return false;
    }
    return true;
}
