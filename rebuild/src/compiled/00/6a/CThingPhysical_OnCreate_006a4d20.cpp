// CThingPhysical::OnCreate() -- adds the "CTCSoundPlayer" component (inactive, no parameters), then returns CThing::OnCreate() as a bool
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A4D20, length 60 bytes (vtable 0x01244E84 slot 8; also
// called directly from CThingGameObject::OnCreate at 0x00838A46).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CThing::AddTC(const CCharString&, bool,
// CTCParamsBase*) @ 0x004C9D60 (result dropped), CCharString::~CCharString @ 0x0099EAE0, CThing::OnCreate @ 0x004C9560.
// The literal at 0x0123A504 is "CTCSoundPlayer".
// Shape: the component name is a NAMED CCharString local in its own scope (a bare temporary
// reuses the constructor's returned `this`; retail re-derives `lea eax,[esp+0Ch]`), destroyed
// before the base call; the base's byte result is normalised with `? true : false` (`setne al`).
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
class CTCBase;
class CTCParamsBase;
class CThing { public:
    virtual void v00();
    unsigned char OnCreate();                                // 0x004C9560 (CThing's, called non-virtually)
public:
    CTCBase* AddTC(const CCharString&, bool, const CTCParamsBase*);   // 0x004C9D60
};
class CThingPhysical : public CThing { public:
    virtual bool OnCreate();
};

bool CThingPhysical::OnCreate()
{
    {
        CCharString name("CTCSoundPlayer", -1);
        AddTC(name, false, 0);
    }
    return CThing::OnCreate() ? true : false;
}