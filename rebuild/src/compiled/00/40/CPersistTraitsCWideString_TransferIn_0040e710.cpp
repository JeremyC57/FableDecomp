// CPersistTraits<CWideString>::TransferIn(CPersistContext&, CStringParser&, CWideString&) -- parse a quoted string from the text save and store it as a wide string; report a missing opening or closing quote
// Grade: RELOCATION_MATCH (237v237) -- 1 grade: each Error message is a named block local (its address lea'd and pushed, not the constructor's eax), which keeps retail's fall-through opening-quote arm and the cross-jump at the context load; parameters by reference per Ego_r.pdb ?TransferIn@?$CPersistTraits@VCWideString@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040E710, 237 bytes (+ int3 padding), `ret 0Ch` (__fastcall, the empty traits `this` is unused).
// Callees: CStringParser::ReadAsSymbol() @ 0x009BA790 (manifest "ReadAsString"), CCharString::operator!=(const char*)
//   @ 0x0099E960, CCharString::CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0,
//   CStringParser::ReadAsStringUntilString(const CCharString&) @ 0x009BA330, CCharString::ToWideString() @ 0x0099E480,
//   CWideString::operator= @ 0x0099B7D0, ~CWideString @ 0x0099B510, CPersistContext::Error(const CStringParser&,
//   const CCharString&) @ 0x009BAA90 (manifest "CDefinitionManager::Error"; its `this` is the context argument).
//   Strings: "\"" 0x0122DF24, "Unable to find closing quote" 0x0122DF04.
// Shape: if/else -- the opening-quote error is the fall-through arm; the else arm holds the read/convert/assign chain
//   (its temporaries destroyed in reverse order) and the closing-quote check; VC7.1 cross-jumps the two Error calls into one tail. The symbol
//   and message temporaries are homed in the dead parser-argument slot. Speed-optimised (default) TU.
class CCharStringData;
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0 ??4CWideString@@QAEABV0@ABV0@@Z
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    bool operator!=(const char* text) const;            // 0x0099E960
    CWideString ToWideString() const;                   // 0x0099E480
};
class CStringParser { public:
    CCharString ReadAsSymbol();                                     // 0x009BA790
    CCharString ReadAsStringUntilString(const CCharString& end);    // 0x009BA330
};
class CPersistContext { public:
    void Error(const CStringParser& parser, const CCharString& message);   // 0x009BAA90
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CWideString> {
    void TransferIn(CPersistContext& context, CStringParser& parser, CWideString& value);
};

void CPersistTraits<CWideString>::TransferIn(CPersistContext& context, CStringParser& parser, CWideString& value)
{
    if (parser.ReadAsSymbol() != "\"") {
        CCharString message("Unable to find closing quote");
        context.Error(parser, message);
    } else {
        {
            CCharString quote("\"");
            value = parser.ReadAsStringUntilString(quote).ToWideString();
        }
        if (parser.ReadAsSymbol() != "\"") {
            CCharString message("Unable to find closing quote");
            context.Error(parser, message);
        }
    }
}