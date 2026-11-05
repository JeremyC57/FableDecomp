// CGameScriptThing::GetDefName() const -- the definition name of the scripted thing (looked up in the global def-string table), or the empty string when the thing is gone or dead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D02C0, length 131 bytes (padded to 144).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls), CThing::GetDefName @ 0x004C7CC0 (CDefString by hidden pointer),
//          CDefStringTable::GetString @ 0x009D49B0 on the global table @ 0x013CA828 (CDefString by value, CCharString by hidden pointer),
//          CCharString copy constructor @ 0x0099EC30, CCharString::~CCharString @ 0x0099EAE0, CCharString(const char*, long) @ 0x0099EBF0
//          with the empty literal @ 0x0122D70E.
// Shape: two 4-byte frame temporaries (the CDefString and the looked-up name); the name is a NAMED local returned by value
// (VC7.1 has no NRVO: copy constructor into the hidden pointer, then the local's destructor).
// Leaf: ?GetDefName@CGameScriptThing@@UBE?AVCCharString@@XZ
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"
extern const char g_emptyString_0122d70e[];       // 0x0122D70E ("")
class CDefString { public:                                // a definition-string handle: one table index
    long m_index;
};
class CDefStringTable { public:
    CCharString GetString(CDefString name);        // 0x009D49B0
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)           // 0x013CA828

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CGameScriptThing { public:
    virtual void v00();
    unsigned char           _pad04[8];
    CIntelligentPointer<CThing> PThing;                // +0x0c CIntelligentPointer<CThing>
    virtual CCharString GetDefName() const;
};

CCharString CGameScriptThing::GetDefName() const
{
    if (PThing.GetPItem() && !PThing.GetPItem()->IsDead()) {
        CCharString name = GDefStringTable.GetString(PThing.GetPItem()->GetDefName());
        return name;
    }
    return CCharString(g_emptyString_0122d70e, -1);
}