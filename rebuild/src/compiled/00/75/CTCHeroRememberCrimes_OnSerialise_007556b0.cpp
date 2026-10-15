// CTCHeroRememberCrimes::OnSerialise(CPersistContext*, long) -- persists the region -> (crime duration, fine) map and the HasPaidTheFine flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007556B0, 82 bytes. CTCHeroRememberCrimes vtable 0x0126980C slot 1.
// Callees: CPersistContext::TransferMap(const char*, map<CCharString,pair<long,long>>&, const pair<long,long>&) @ 0x00756D00,
//          CPersistContext::Transfer<signed char>(const char*, signed char&, const signed char&) @ 0x004045C0 (the bool
//          member goes through the folded one-byte instantiation).
// Strings: "RegionNamesToCrimeDurationAndFine" @ 0x0126988C, "HasPaidTheFine" @ 0x0126987C.
//
// Shape: the 8-byte frame is the {0, 0} pair default, filled AFTER the three pushes (a temporary bound to the const
// reference in the call); the one-byte zero default of the second call is homed in the dead `context` argument slot
// (`lea edx,[esp+14h]` ... `mov byte ptr [esp+20h],0`); the second member's address is formed by advancing `this`
// (`add esi,18h`); the second parameter is unused (`ret 8`).
// Layout: HasPaidTheFine is +0x18 in retail (PDB +0x1C: the map is 12 bytes, not 16); a local model.

struct SCrimeDurationAndFine {                 // std::pair<long,long>
    long Duration;
    long Fine;
    SCrimeDurationAndFine(long duration, long fine) : Duration(duration), Fine(fine) {}
};

struct CCrimeMap {                             // map<CCharString, pair<long,long>>: header, count, compare
    void*         m_pHeader;
    unsigned long m_count;
    unsigned char m_compare[4];
};

struct CPersistContext {
    void TransferMap(const char* name, CCrimeMap& value, const SCrimeDurationAndFine& defaultValue);   // 0x00756D00
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);            // 0x004045C0
};

struct CTCHeroRememberCrimes {
    virtual void v00();
    unsigned char _pad_0x04[0x8];              // +0x04 CTCBase
    CCrimeMap     RegionNamesToCrimeDurationAndFine;   // +0x0C
    signed char   HasPaidTheFine;              // +0x18 (PDB +0x1C, bool)

    virtual void OnSerialise(CPersistContext* context, long version);
};

void CTCHeroRememberCrimes::OnSerialise(CPersistContext* context, long version)
{
    context->TransferMap("RegionNamesToCrimeDurationAndFine", RegionNamesToCrimeDurationAndFine,
                         SCrimeDurationAndFine(0, 0));
    context->Transfer("HasPaidTheFine", HasPaidTheFine, 0);
}