// CGameScriptInterface::EntityDecapitate(const CScriptThing& thing) const -- for a valid thing holding interface 0xd1 and the right flags, make a decapitation action its current action
// Leaf: ?EntityDecapitate@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Retail: Fable.exe @ 0x008A8E40, 98 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: if thing.IsValid() (slot 75, +0x12c): t = thing.GetThing() (slot 11); if t holds interface 0xd1 (bit 17 of
// UsingComponents word 6, +0x38), any of bits 0..3 of the +0x6c flag byte, and bit 1 of the +0xbc flag byte:
// t->SetCurrentAction(<action>(t)) (0x006644F0) on a 0x114-byte temporary built by 0x0066A560, destroyed by
// ~CCombatActionBase 0x0062E900. No null test on t.
// Layout: CThing flag bytes +0x6c / +0x91 and CCombatActionBase as the landed 0x008C92C0 model; UsingComponents +0x20 as
// the matched 0x0089A330 row. Interface 0xd1 (CTCDecapitation is the likely class), the +0xbc flag and the action class
// (ctor 0x0066A560) are not confirmed; the action is modelled as CCreatureAction_Decapitate.
// Shape: /O2 (16-aligned entry).
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];           // +0x20 bitset<274>
    unsigned char _pad44[0x6c - 0x44];
    unsigned char m_flags6c;                    // +0x6c bit 3
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char _pad92[0xbc - 0x92];
    unsigned char m_flagsbc;                    // +0xbc bit 1
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
};
class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x114 - 4];
    ~CCombatActionBase();                       // 0x0062E900
};
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
struct CCreatureAction_Decapitate : CCombatActionBase {       // retail sizeof 0x114 (name not recovered)
    CCreatureAction_Decapitate(CThing* victim);          // 0x0066A560
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                        // slot 75 (+0x12c)
};
class CGameScriptInterface {
public:
    virtual void EntityDecapitate(const CScriptThing& thing) const;
};
void CGameScriptInterface::EntityDecapitate(const CScriptThing& thing) const
{
    if (thing.IsValid())
    {
        CThing* t = thing.GetThing();
        if (t->HasTC(0xd1) && (t->m_flags6c & 0xf) && (t->m_flagsbc & 2))
        {
            CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(t);
            creature->SetCurrentAction(CCreatureAction_Decapitate(t));
        }
    }
}