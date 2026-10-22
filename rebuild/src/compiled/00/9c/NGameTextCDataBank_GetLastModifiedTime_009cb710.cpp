// CStreamingFontDataBank::GetLastModifiedTime(CCharString*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CB710, oracle length 32 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,[esp+8] ; push 0 ; push 0 ; push 1 ; push 1 ; push 1 ; push 1 ; push 1 ; mov ecx,esi ; call CDateAndTime::CDateAndTime ; mov eax,esi ; pop esi ; ret 8
// Streaming font banks have no per-entry timestamps: the query ignores the entry name and
// constructs a fixed CDateAndTime(1,1,1,1,1,0,0) in the caller's hidden result slot (the first
// stack slot; the name pointer is the second), returning that pointer in eax. The seven-argument
// constructor at 0x009E9A70 is modelled as (year, month, day, hour, minute, second, millisecond);
// the parameter names are inferred.

struct CCharString;

struct CDateAndTime {
    unsigned char _raw[0x10];       // opaque; built in place by the constructor
    CDateAndTime(long year, long month, long day, long hour, long minute, long second, long millisecond);   // 0x009E9A70
};

struct CDataBank_Methods {
    CDateAndTime GetLastModifiedTime(CCharString* entryName);
};

CDateAndTime CDataBank_Methods::GetLastModifiedTime(CCharString* entryName)
{
    return CDateAndTime(1, 1, 1, 1, 1, 0, 0);
}