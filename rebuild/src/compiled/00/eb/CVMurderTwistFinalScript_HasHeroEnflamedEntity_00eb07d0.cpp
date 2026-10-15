// CV_MurderTwistFinalScript::HasHeroEnflamedEntity(CScriptThing* thing) -- whether the thing was set on fire by the hero ("SCRIPT_NAME_HERO", source kind 3); false when the thread is terminating
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EB07D0, length 87 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (the implicit conversion of the literal
// 0x0125D1C8 "SCRIPT_NAME_HERO" with the defaulted -1), the thing's virtual slot 41 (+0xa4: (long, const
// CCharString&)), CCharString::~CCharString @ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating @ 0x00CB7940.
// Shape: one frame word for the conversion temporary (re-addressed with `lea` for the push: implicit, not an
// explicit temporary), destroyed right after the call (so not a named local); the result rides in bl across
// the destructor; the two exits both call IsActiveThreadTerminating: the true arm returns `!terminating`
// (`neg ; sbb ; inc`), the false arm calls it for its side effect and returns `xor al,al` (the
// `if (IsActiveThreadTerminating()) return false; return result;` form is folded to `not ; and` by the lane); `ret 4`.
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};

struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040();
    virtual bool IsEnflamedBy(long sourceKind, const CCharString& name);   // slot 41 (+0xa4)
};

struct CV_MurderTwistFinalScript {
    virtual void v00();
    unsigned char IsActiveThreadTerminating();     // 0x00CB7940 (CScriptBase)
    unsigned char HasHeroEnflamedEntity(CScriptThing* thing);
};

unsigned char CV_MurderTwistFinalScript::HasHeroEnflamedEntity(CScriptThing* thing)
{
    bool enflamed = thing->IsEnflamedBy(3, "SCRIPT_NAME_HERO");
    if (enflamed)
        return !IsActiveThreadTerminating();
    IsActiveThreadTerminating();
    return 0;
}