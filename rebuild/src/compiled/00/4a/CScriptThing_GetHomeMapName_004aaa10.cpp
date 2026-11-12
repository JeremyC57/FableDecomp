
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): 0x0143E8EC is CScriptThing::NullString (Ego_r.pdb + FableWin.pdb `dynamic initializer
//   for 'CScriptThing::NullString'', initializer 0x01224C50 landed by agentFAM09); the null branch copies it with the out-of-line
//   CCharString(const CCharString&) 0x0099EC30 (a public static of the row's CScriptThing model, spelled as engine/CScriptThing.h declares
//   it). Was kHomeMapName (a char array) through CCharString(const char*), neither of which retail references.
class CCharString { public:
    char* p;
    CCharString(const CCharString& other);   // copy 0x0099EC30
};

struct CInner {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual CCharString GetName();
};

class CScriptThing { public:
    unsigned char _pad_0x0[0x4];
    CInner* inner;
    CCharString GetHomeMapName();
    static CCharString NullString;           // 0x0143E8EC ?NullString@CScriptThing@@2VCCharString@@A
};

CCharString CScriptThing::GetHomeMapName()
{
    CInner* inner = this->inner;
    if (inner == 0)
        return NullString;
    return inner->GetName();
}