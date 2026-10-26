// CCombatAction_GenericStrikeResponseBase::IsOverrideableByActionOfSamePriority() -- (label propagated) opinion-reaction condition: the reaction's target exists, is alive, has interface 81 and stands more than 3 m from the owner's creature
// Leaf: IsTargetValidAndDistant
// Retail: Fable.exe (TLC, VC7.1) @ 0x00984B40, 158 bytes. Not a combat-action method: the address is only referenced by the
// static initialiser of the opinion-reaction type table (`mov esi,984B40h` at 0x01222924, stored into the
// OPINION_REACTION_TYPE_COMMENT_TO_SELF row @ 0x013BBC30), so it is a condition callback on a reaction record; the
// record and method are named descriptively.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the target pointer at the unaligned +0x6b, four times through
//          one `lea esi,[edi+6Bh]`), CThingPhysical::GetPosition @ 0x004C73D0 (twice).
// Global: the float 3.0f @ 0x0122DED4.
// Shape: the three guards are NESTED ifs so they share the trailing `xor al,al` (early `return false`s put a tail
//          inline); the owner's creature (record +4 -> +0x18) is read before the target is fetched; the difference
//          `target - owner` is built in a named local by an inline operator- and returned through a memberwise user copy
//          constructor, which keeps the three differences on the FPU stack (a POD vector spills two 12-byte frame
//          temporaries); its length is an inline sqrt of x*x + y*y + z*z (the FPU top squared first) and the
//          `> 3.0f` result comes back as `mov eax,1` / `xor eax,eax`.
#include <math.h>
#pragma intrinsic(sqrt)

class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector() { }
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
    float Length() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

inline C3DVector operator-(const C3DVector& a, const C3DVector& b)
{
    C3DVector result;
    result.X = a.X - b.X;
    result.Y = a.Y - b.Y;
    result.Z = a.Z - b.Z;
    return result;
}

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    unsigned char _pad44[0x91 - 0x44];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CReactionOwner {
    unsigned char _pad00[0x18];
    CThing* m_pCreature;                        // +0x18
};

#pragma pack(push, 1)
struct COpinionReactionRecord {
    void* m_pVtable;                            // +0x00
    CReactionOwner* m_pOwner;                   // +0x04
    unsigned char _pad08[0x6b - 8];
    CBaseIntelligentPointer m_target;           // +0x6b CIntelligentPointer<CThing>
    bool IsTargetValidAndDistant();
};
#pragma pack(pop)

bool COpinionReactionRecord::IsTargetValidAndDistant()
{
    if (m_target.GetPItem() != 0) {
        if (!m_target.GetPItem()->IsDead()) {
            if (m_target.GetPItem()->m_presentBits[2] & 0x20000) {
                CThing* creature = m_pOwner->m_pCreature;
                CThing* target = m_target.GetPItem();
                return (target->GetPosition() - creature->GetPosition()).Length() > 3.0f;
            }
        }
    }
    return false;
}