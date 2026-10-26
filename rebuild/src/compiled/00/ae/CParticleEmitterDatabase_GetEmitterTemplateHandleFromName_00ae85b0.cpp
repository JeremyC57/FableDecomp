// CParticleEmitterDatabase::GetEmitterTemplateHandleFromName(const CCharString&) -- looks the upper-cased name up in the database's symbol map and returns its handle, or 0 when the name is unknown
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE85B0, 61 bytes (`ret 4`).
// Callees: CCharString::ToUpper() const @ 0x0099EE20 (returns a CCharString through the hidden pointer),
// CSymbolMap::Lookup(const CCharString&, long*) @ 0x009E5170 (labelled "SymbolMap_Lookup"),
// CCharString::~CCharString @ 0x0099EAE0.
// Shape: the symbol map is the member at +0 (`mov ecx,esi`); the upper-cased temporary is homed in the name's dead
// argument slot (`lea ecx,[esp+14h]`), the handle in the `push ecx` slot; the lookup result survives the
// temporary's destructor in bl and selects the handle branch-free (`neg bl ; sbb ebx,ebx ; and ebx,eax`).
class CCharString { public:
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
    CCharString ToUpper() const;                     // 0x0099EE20
};

class CSymbolMap { public:
    unsigned char m_data[0x24];
    bool Lookup(const CCharString& name, long* value);   // 0x009E5170
};

struct CParticleEmitterDatabase {
    CSymbolMap    SymbolMap;                         // +0x00
    bool          UsingSymbolMap;                    // +0x24
    long GetEmitterTemplateHandleFromName(const CCharString& name);
};

long CParticleEmitterDatabase::GetEmitterTemplateHandleFromName(const CCharString& name)
{
    long handle;
    bool found = SymbolMap.Lookup(name.ToUpper(), &handle);
    return found ? handle : 0;
}