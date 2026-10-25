// _global::AudioSystem_Query(long a, long b) -- libpk audio API wrapper: forwards both words to the global audio system's bool query and returns it as a BOOL (1/0)
// Leaf: AudioSystem_Query
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C335C0, 29 bytes, __stdcall `ret 8`. Descriptive name (no PDB covers libpk):
//   the catalog's `operator!=<char,char>` is propagated from a byte-identical body. Callers: the libpk front end
//   0x00C1C66D / 0x00C1C776.
// Callee: the global audio system (0x0143BB58)'s member @ 0x00C3DA00 (__fastcall, two words, returns a bool: it
//   hands the words to the device at +4 and records an error code through the system's error hook on failure).
// Shape: `query ? 1 : 0` on a bool gives retail's `neg al ; sbb eax,eax ; neg eax` (a plain `return query;` is movzx).
struct CAudioSystem {
    bool Query(long a, long b);       // 0x00C3DA00
};
extern CAudioSystem g_audioSystem;    // 0x0143BB58

int __stdcall AudioSystem_Query(long a, long b)
{
    return g_audioSystem.Query(a, b) ? 1 : 0;
}