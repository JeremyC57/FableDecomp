// CTCTavernGameCoinGolf::vfunc_32() -- start-round override: coin-base hook, fetch the golf definition, reset the score, copy the definition's par (+0x58) to +0x17C, set up the round and enter state 16
// Retail: Fable.exe @ 0x007D3510, 80 bytes (vtable 0x0126F49C slot 32).
// Callees: CTCTavernGameCoinBase::vfunc_32 0x008EF180 (non-virtual base call; a jmp to 0x005E9A90),
//          CDefClassBase::GetSubDef<T>(CDefPointer<T>*) 0x007D4BD0 (bool, out-parameter; result unused),
//          CTCTavernGameCoinGolf::SetupForNewRound 0x007D2EF0, CTCTavernGame::SetState(long) 0x005E9B20.
// The definition comes from the parent thing (+4) -> +0x70 (the thing's definition object); the
// CDefPointer is one counted word (object: vptr, ref count +4, virtual slot 1 = destroy).
// Shape: the out pointer is null-initialised from the zero register that also clears Score (+0x2C,
// CTCTavernGame::Score -- PDB +0x34, retail 8 bytes lower). The definition pointer is taken into a
// named local right after the getter (kept in edi across the two calls) and released by hand at the
// end without a null test (`dec [edi+4] ; jne ; call [vt+4]`).
struct CDefObject {
    virtual void v00();
    virtual void Destroy();                         // slot 1 (+0x04)
    long m_refCount;                                // +0x04
};

class CTavernGameCoinGolfDef : public CDefObject { public:
    unsigned char _pad[0x50];
    long m_par;                                     // +0x58
};

struct CDefPointer {                                // CDefPointer<const CTavernGameCoinGolfDef>
    CTavernGameCoinGolfDef* m_p;
    CDefPointer() : m_p(0) {}
};

class CDefClassBase { public:
    bool GetSubDef_007d4bd0(CDefPointer* out);               // 0x007D4BD0
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_007d4bd0(CDefPointer* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char _pad[0x70];
    CDefClassBase* m_pDefinition;                   // +0x70
    CDefClassBase* GetDefinition() { return m_pDefinition; }
};

struct CTCTavernGameCoinBase {
    void* __vftable;
    CThing* m_pParentThing;                         // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x24];
    float Score;                                    // +0x2c
    unsigned char _pad_0x30[0x14c];
    long m_par;                                     // +0x17c (golf)
    void vfunc_32();                                // 0x008EF180
    void SetState(long state);                      // 0x005E9B20
};

class CTCTavernGameCoinGolf : public CTCTavernGameCoinBase { public:
    void SetupForNewRound();                        // 0x007D2EF0
    void vfunc_32();
};

void CTCTavernGameCoinGolf::vfunc_32()
{
    CTCTavernGameCoinBase::vfunc_32();
    CDefPointer defSlot;
    m_pParentThing->GetDefinition()->GetSubDef_007d4bd0(&defSlot);
    Score = 0;
    CTavernGameCoinGolfDef* def = defSlot.m_p;
    m_par = def->m_par;
    SetupForNewRound();
    SetState(16);
    if (--def->m_refCount == 0)
        def->Destroy();
}