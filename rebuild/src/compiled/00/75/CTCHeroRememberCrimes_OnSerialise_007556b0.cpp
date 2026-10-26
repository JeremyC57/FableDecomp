// CTCHeroRememberCrimes::OnSerialise(CPersistContext& context, bool loading) -- persists the region -> (crime duration, fine) map and the HasPaidTheFine flag
// Leaf: ?OnSerialise@CTCHeroRememberCrimes@@MAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007556B0, 82 bytes. CTCHeroRememberCrimes vtable 0x0126980C slot 1.
// Identity: Ego_r name set ?OnSerialise@CTCHeroRememberCrimes@@MAEXAAVCPersistContext@@_N@Z (protected virtual).
// Callees: CPersistContext::TransferMap<std::pair<long, long>, CCharString>(const char*, std::map<CCharString,
//          std::pair<long, long> >&, const std::pair<long, long>&) @ 0x00756D00 (PDB name set: TransferMap<V, K> takes
//          map<K, V>; the sibling 2-argument overload is 0x00757160),
//          CPersistContext::Transfer<signed char>(const char*, signed char&, const signed char&) @ 0x004045C0 (the bool
//          member goes through the folded one-byte instantiation).
// Strings: "RegionNamesToCrimeDurationAndFine" @ 0x0126988C, "HasPaidTheFine" @ 0x0126987C.
//
// Shape: the 8-byte frame is the {0, 0} pair default (STLport `pair(const _T1&, const _T2&)`), filled AFTER the three
// pushes (a temporary bound to the const reference in the call); the one-byte zero default of the second call is homed
// in the dead `context` argument slot (`lea edx,[esp+14h]` ... `mov byte ptr [esp+20h],0`); the second member's address
// is formed by advancing `this` (`add esi,18h`); the second parameter is unused (`ret 8`).
// Layout: HasPaidTheFine is +0x18 in retail (PDB +0x1C: the STLport map is 12 bytes, not 16); a local model.
class CCharString;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {                                    // STLport 4.6: header node, count, compare (12 bytes)
public:
    void*         _M_header;
    unsigned long _M_node_count;
    _Compare      _M_key_compare;
};
}

class CPersistContext {
public:
    template <class _Tp, class _Key> void TransferMap(const char* name, std::map<_Key, _Tp>& map, const _Tp& defaultValue);   // 0x00756D00 for <pair<long, long>, CCharString>
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);            // 0x004045C0
};

class CTCHeroRememberCrimes {
public:
    virtual void v00();
    unsigned char _pad_0x04[0x8];              // +0x04 CTCBase
    std::map<CCharString, std::pair<long, long> > RegionNamesToCrimeDurationAndFine;   // +0x0C
    signed char   HasPaidTheFine;              // +0x18 (PDB +0x1C, bool)

protected:
    virtual void OnSerialise(CPersistContext& context, bool loading);
};

void CTCHeroRememberCrimes::OnSerialise(CPersistContext& context, bool loading)
{
    context.TransferMap("RegionNamesToCrimeDurationAndFine", RegionNamesToCrimeDurationAndFine,
                        std::pair<long, long>(0, 0));
    context.Transfer("HasPaidTheFine", HasPaidTheFine, 0);
}
