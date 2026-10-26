// CTCExpression::OnSerialise(CPersistContext& context, bool) -- persist the expression def by name: saving writes the def's instantiation name (empty without a def), loading reads the name and, when it is not empty, looks the def up again
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EF070, 303 bytes (+ padding), `ret 8`.
// Callees: CCharString() @ 0x0099E4B0, CCharString(const CCharString&) @ 0x0099EC30, ~CCharString @ 0x0099EAE0,
//   CCharString::operator= @ 0x0099EFB0, operator!=(const char*) @ 0x0099E960, the def's virtual slot 10 (+0x28,
//   GetInstantiationName, CDefString by hidden pointer), CDefStringTable::GetString(long) @ 0x009D49B0 on
//   g_DefStringTable 0x013CA828, CPersistContext::Transfer(const char*, CCharString&, const CCharString&) @ 0x004109A0
//   (manifest "Transfer<CIntelligentPointer<CThing>>" is propagated), CTCBase::GetDefinitionManager @ 0x006869D0,
//   CDefinitionManager::GetDef<CExpressionDef>(const CCharString&, CDefPointer*) @ 0x00593666 (manifest
//   "GetDef<CHeroPostcardGeneratorDef>" is a byte-twin propagation).
// Literals: "ExpressionDef" @ 0x012718D0, "" @ 0x0122D70E.
// Layout = Ego_r.pdb CTCExpression (0x10): PDef (CDefPointer<CExpressionDef const>) +0x0c; CPersistContext mode +0x18.
// Shape: the landed CTCAtmosPlayer::OnSerialise 0x007E36A0 pattern: both arms home the name in the dead `context`
//   slot, the Transfer default is an inline by-value empty string (built, copied, destroyed: no NRVO); the save name
//   is assigned through the inline `CDefString::GetString()` (table call inline in the argument: the CDefString temporary
//   and the result take the right frame words) on a named def local (the null test on ecx, which then carries the
//   vcall's `this`; testing the member loads eax); the load lookup pushes its arguments before the manager
//   getter.
class CCharString { public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator!=(const char* text) const;            // 0x0099E960
};
inline CCharString EmptyString() { CCharString empty; return empty; }
struct CDefStringTable {
public:
    CCharString GetString(long) const;   // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828
class CDefString { public:                                     // 4-byte def-name handle (user constructor: hidden-pointer return)
    long Index;
    CDefString();
    CCharString GetString() const { return g_DefStringTable.GetString(Index); }
};
class CDefClassBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetInstantiationName() const;    // slot 10 (+0x28)
};
class CExpressionDef : public CDefClassBase { public: };
template <class T> class CDefPointer { public: T* Object; };
class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CDefPointer<const CExpressionDef>* out);   // 0x00593666
};
class CPersistContext { public:
    unsigned char _pad00[0x18];
    long Mode;                                          // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);   // 0x004109A0
};
class CTCExpression {
public:
    virtual void v00();
    void*  ParentThing;                                 // +0x04
    bool   ToBeRemoved;                                 // +0x08
    bool   IsInGlobalUpdateTCs;                         // +0x09
    CDefPointer<const CExpressionDef> PDef;             // +0x0c
    CDefinitionManager* GetDefinitionManager();         // 0x006869D0 (CTCBase)
    void OnSerialise(CPersistContext& context, bool);
};

void CTCExpression::OnSerialise(CPersistContext& context, bool)
{
    if (context.IsSaving()) {
        CCharString name;
        const CExpressionDef* def = PDef.Object;
        if (def)
            name = def->GetInstantiationName().GetString();
        context.Transfer("ExpressionDef", name, EmptyString());
    } else {
        CCharString name;
        context.Transfer("ExpressionDef", name, EmptyString());
        if (name != "")
            GetDefinitionManager()->GetDef(name, &PDef);
    }
}