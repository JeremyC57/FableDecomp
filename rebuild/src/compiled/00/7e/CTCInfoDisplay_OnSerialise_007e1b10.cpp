// CTCInfoDisplay::OnSerialise(CPersistContext&, bool) -- persists the two text tags, the radius and the display time
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E1B10, 201 bytes, `ret 8`.
// Callees: CCharString::CCharString() @ 0x0099E4B0, CCharString::CCharString(const CCharString&) @ 0x0099EC30,
//          CCharString::~CCharString() @ 0x0099EAE0, CPersistContext::Transfer(const char*, CCharString&, const
//          CCharString&) @ 0x004109A0, CPersistContext::Transfer(const char*, float&, const float&) @ 0x00410620;
//          the names "TextTag" / "TextTagBack" / "Radius" / "DisplayTime".
// Shape: each string default is the inline by-value EmptyString() (built, copied into the argument temporary and
// destroyed: VC7.1 has no NRVO), the second built in the dead `context` slot once the context sits in edi; the
// float defaults are `0.0f` literals bound to const references, stored as immediates into that same slot.
// Layout (Ego_r.pdb CTCInfoDisplay, 0x20 bytes, matches retail): TextTag +0x10, TextTagBack +0x14, Radius +0x18,
// DisplayTime +0x1C -- the landed ctor 0x007E1AC0 uses the same model.
class CThing;

class CCharString { public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

inline CCharString EmptyString() { CCharString empty; return empty; }

class CPersistContext { public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);   // CPersistContext::Transfer<T> (member template, PDB)
    //   instances here: CCharString 0x004109A0; float 0x00410620
};

struct CTCInfoDisplay {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 CTCBase
    unsigned char _pad08[4];                            // +0x08 CTCBase flags
    bool          Forward;                              // +0x0c
    bool          Backward;                             // +0x0d
    CCharString   TextTag;                              // +0x10
    CCharString   TextTagBack;                          // +0x14
    float         Radius;                               // +0x18
    float         DisplayTime;                          // +0x1c
    void OnSerialise(CPersistContext& context, bool);
};

void CTCInfoDisplay::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("TextTag", TextTag, EmptyString());
    context.Transfer("TextTagBack", TextTagBack, EmptyString());
    context.Transfer("Radius", Radius, 0.0f);
    context.Transfer("DisplayTime", DisplayTime, 0.0f);
}