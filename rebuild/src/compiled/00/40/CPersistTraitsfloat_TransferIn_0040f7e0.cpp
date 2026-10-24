// CPersistTraits<float>::TransferIn(CPersistContext&, CStringParser&, float*) -- text persist read of a float: a symbol name is resolved through the context's symbol map, anything else is parsed as a float
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040F7E0, 135 bytes (+9 int3 padding), `__fastcall`, `ret 0Ch`.
// Callees: CStringParser::PeekNextItemType @ 0x009BA900, CStringParser::ReadAsString @ 0x009BA6F0 (hidden CCharString
//   result), CSymbolMap lookup (name, long*) @ 0x009E5170, operator+(const char*, const CCharString&) @ 0x0099F690
//   (`__fastcall`, hidden result in ecx), CPersistContext::Error(const CStringParser&, const CCharString&) @ 0x009BAA90,
//   ~CCharString @ 0x0099EAE0, CStringParser::ReadAsFloat @ 0x009BA650; message literal @ 0x0122DFE8.
// Shape: the traits object (`this`, ecx) is never read; `push ecx` is the frame word of the looked-up `long value`.
// When the next item is a symbol (item type 2) the name is read into the dead parser-argument slot, looked up in
// CPersistContext::PSymbolMap (+4); a miss reports `"<msg>" + name` (the concatenation temporary lives in the dead
// context-argument slot) through CPersistContext::Error. The symbol's long value is converted to float (`fild`) before
// the name's destructor. Otherwise ReadAsFloat's x87 result is stored straight to the output.
// CStringParser uses engine/CStringParser.h; CPersistContext (no engine header) is modelled from the PDB layout.
#include "engine/CStringParser.h"

struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                            // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690 (hidden result in ecx)
extern const char g_msgUnknownSymbol_0122dfe8[];               // 0x0122DFE8

struct CStringParserMethods : CStringParser {
    long PeekNextItemType();                                   // 0x009BA900 (EParsedItemType)
    CCharString ReadAsString();                                // 0x009BA6F0
    float ReadAsFloat();                                       // 0x009BA650
};

struct CSymbolMap {
    bool Lookup(const CCharString& name, long* value) const;   // 0x009E5170
};

struct CPersistContext {                                       // Ego_r.pdb, 0x2C bytes
    void* PDefinitionManager;                                  // +0x00
    CSymbolMap* PSymbolMap;                                    // +0x04
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009BAA90
    CSymbolMap* GetSymbolMap() const { return PSymbolMap; }
};

enum { PARSED_ITEM_SYMBOL = 2 };

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, T* out) const;
};

template <> void CPersistTraits<float>::TransferIn(CPersistContext& context, CStringParser& parser, float* out) const
{
    CStringParserMethods& p = static_cast<CStringParserMethods&>(parser);
    if (p.PeekNextItemType() == PARSED_ITEM_SYMBOL) {
        CCharString name = p.ReadAsString();
        long value;
        if (!context.GetSymbolMap()->Lookup(name, &value))
            context.Error(parser, g_msgUnknownSymbol_0122dfe8 + name);
        *out = (float)value;
    }
    else
        *out = p.ReadAsFloat();
}