// CThingHolySite::SerialiseGameState(CPersistContext& context) -- base CThingPhysical serialisation; unless the context mode is 1 or 3, re-fetches the def by index and re-reads PlayerStart from it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083BB40, length 90 bytes (vtable 0x012452DC slot 3).
// Callees: CThingPhysical::SerialiseGameState @ 0x006A5AF0 (called non-virtually),
// CThing::GetDefinitionManager() @ 0x004C7990 (manifest "CViewBase::GetDisplayEngine" is propagated: the body
// reads the world's +0x20 manager and never uses `this`), CDefinitionManager::GetDef<CThingHolySiteDef>(long, CDefPointer&) @ 0x0083BD10.
// Shape: the context pointer is kept in edi for the mode test and then edi is reused for &Def
// (+0xd0); the GetDef arguments (the 16-bit def index at +0x8c, &Def) are pushed before the
// manager call (VC evaluates arguments before the object expression); PlayerStart (+0xd4) is
// written `Def->PlayerStart != 0` from the def's byte (`test al,al ; setne dl`). The catalog
// label "vfunc_3" is the vtable slot (the base call 0x006A5AF0 is CThingPhysical::SerialiseGameState).
class CPersistContext { public:
    unsigned char _pad00[0x18];
    long          m_mode;                                  // +0x18
};
class CThingHolySiteDef { public:
    unsigned char _pad00[0x7e];
    unsigned char PlayerStart;                             // +0x7e
};
struct CDefPointerHolySiteDef {
    CThingHolySiteDef* m_pDef;
    CThingHolySiteDef* operator->() const { return m_pDef; }
};
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointerHolySiteDef& out);   // 0x0083BD10
};
class CThingPhysical { public:
    virtual void v00();
    unsigned char  _pad04[0x88];
    unsigned short m_defIndex;                             // +0x8c
    unsigned char  _pad8e[0x42];
    CDefinitionManager* GetDefinitionManager();            // 0x004C7990
    virtual bool SerialiseGameState(CPersistContext& context);   // 0x006A5AF0
};
class CThingHolySite : public CThingPhysical { public:
    CDefPointerHolySiteDef Def;                            // +0xd0
    bool                   PlayerStart;                    // +0xd4
    virtual bool SerialiseGameState(CPersistContext& context);
};

bool CThingHolySite::SerialiseGameState(CPersistContext& context)
{
    if (CThingPhysical::SerialiseGameState(context)) {
        if (context.m_mode != 1 && context.m_mode != 3) {
            GetDefinitionManager()->GetDef(m_defIndex, Def);
            PlayerStart = Def->PlayerStart != 0;
        }
        return true;
    }
    return false;
}