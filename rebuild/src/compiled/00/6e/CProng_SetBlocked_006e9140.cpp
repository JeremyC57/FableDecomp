// CProng::SetBlocked(bool blocked) -- mark the prong blocked: start a three-second blockage timer and release the occupier unless it is dead; unblocking clears the timer
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E9140, length 141 bytes.
// Callees: _ftol @ 0x00BFEA70, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, GetPItem @ 0x00A01B50,
// SetPItem @ 0x00A01B90, ~CBaseIntelligentPointer @ 0x00A01C10; the frame rate @ 0x01375550 and 3.0f @ 0x0122DED4.
// Layout: Ego_r.pdb CProng (0x1c): Blocked +0, Prong +1, ProngBlockageTimer +0x10, POccupier +0x14.
// Shape: the occupier release is the landed SetPTarget temporary-pointer shape (0x007A7150) with a null item; the
// occupier's dead bit is tested through a second GetPItem.
#include "fable_compiler.h"
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4 (an 8-byte temporary: `sub esp,8`)
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                             // 0x00A01C10
    CBaseObject* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x0123E7D8 (CIntelligentPointer<CThingCreatureBase>)
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CThingCreatureBase { public:                                 // only the dead bit is read here
    unsigned char _pad00[0x91];
    unsigned char m_dead : 1;                               // +0x91 bit 0
    unsigned char m_91_1 : 7;
};

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

struct CProng {
    bool                    Blocked;                        // +0x00
    unsigned char           Prong[0xc];                     // +0x01 C3DVector (packed in the PDB)
    unsigned char           _pad0d[3];
    long                    ProngBlockageTimer;             // +0x10
    CBaseIntelligentPointer POccupier;                      // +0x14 CIntelligentPointer<CThingCreatureBase>
    FABLE_FORCEINLINE void SetPOccupierItem(CBaseObject* item)  // the landed SetPTarget shape: a temporary pointer set to the
    {                                                       // item, read back into the member, destroyed
        CIntelligentPointer pointer;
        pointer.SetPItem(item);
        POccupier.SetPItem(pointer.GetPItem());
    }
    CThingCreatureBase* GetPOccupier() { return (CThingCreatureBase*)POccupier.GetPItem(); }
    void SetBlocked(bool blocked);
    void SetPOccupyingAttacker(CThingCreatureBase* attacker);
};

void CProng::SetBlocked(bool blocked)
{
    Blocked = blocked;
    if (blocked) {
        ProngBlockageTimer = (long)(CMainGameComponent::GetConstantFPS() * 3.0f);
        if (GetPOccupier() && !GetPOccupier()->m_dead)
            SetPOccupierItem(0);
    } else {
        ProngBlockageTimer = 0;
    }
}