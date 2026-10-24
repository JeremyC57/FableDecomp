// CPersistTraits<EInputKey>::TransferIn(CPersistContext*, CStringParser*, EInputKey*) -- parse the key as a long through the global long TransferIn and store it as the enum
// Retail: Fable.exe (TLC, VC7.1) @ 0x00679910, 31 bytes, `ret 0Ch` (__fastcall, the empty traits `this` is unused).
// Callees: TransferIn(CPersistContext*, CStringParser*, long*) @ 0x009E2590 (__fastcall: context in ecx, parser in edx).
// Shape: the long temporary is homed in the parser-argument slot ([esp+8]) once the parser is in edx; the enum store
//   re-reads it. Speed-optimised (default) TU.
struct CPersistContext;
struct CStringParser;
enum EInputKey { INPUT_KEY_NONE = 0 };

void __fastcall TransferIn(CPersistContext* context, CStringParser* parser, long* value);   // 0x009E2590

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<EInputKey> {
    void TransferIn(CPersistContext* context, CStringParser* parser, EInputKey* value);
};

void CPersistTraits<EInputKey>::TransferIn(CPersistContext* context, CStringParser* parser, EInputKey* value)
{
    long key;
    ::TransferIn(context, parser, &key);
    *value = (EInputKey)key;
}