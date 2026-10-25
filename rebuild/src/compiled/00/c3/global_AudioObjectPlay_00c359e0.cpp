// _global::AudioObject_Play(CAudioObject* object, long a, long b, long c) -- libpk audio API: forwards three words to the object's bool member and returns its result as a BOOL
// Leaf: AudioObject_Play
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C359E0, 33 bytes, __stdcall `ret 0x10`. Descriptive name (no PDB covers
//   libpk): the catalog's `CExclusiveAssert_OnQuestItemUsed_Bool` is propagated. Caller: 0x00C49A36.
// Callee: the object's member @ 0x00C43EC0 (__fastcall, three words, returns a bool).
// Shape: `r ? 1 : 0` gives `neg al ; sbb eax,eax ; neg eax`.
struct CAudioObject {
    bool Play(long a, long b, long c);       // 0x00C43EC0
};

int __stdcall AudioObject_Play(CAudioObject* object, long a, long b, long c)
{
    return object->Play(a, b, c) ? 1 : 0;
}