// _global::__default_constructed<CPersistInfo>(CPersistInfo* p) -- STLport's default-value helper (_STLP_DEFAULT_CONSTRUCTED(_Tp) = __default_constructed((_Tp*)0)): returns `_Tp()` by value; here the 24-byte mapped value of a map<unsigned __int64, CPersistInfo>, whose words are left uninitialised and copied out through temporaries
// Grade: DIFFER(62v111) -- retail builds the value through a 0x30-byte frame (two 24-byte temporaries): the first 12-byte member is copied word by word store/reload (F+0 -> F+0 -> F+0x18 -> result), the other three words straight from the frame; the lane keeps one 0x18 temporary and copies each word once
// Leaf: __default_constructed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082F5C0, length 111 bytes (`ret 4`: only the hidden result pointer is on the
// stack; the `_Tp*` argument arrives in ecx -- /Gr algorithm TU, the caller passes `xor ecx,ecx`).
// Label correction: the manifest's "CDateAndTime::CDateAndTime" is propagated. The only caller, 0x0082FB90 (a map operator[] on
// an unsigned __int64 key: `cmp [esi+4],[edi+14h]` high word then low word), calls it right before building value_type(key,
// value): this is `_STLP_DEFAULT_CONSTRUCTED(_Tp)` of the mapped type (the manifest's pair<unsigned __int64,
// NScript::CRuntimeScriptedEntityPersist::CPersistInfo> instantiations sit next to it).
// Callees: none.
// Tried: (a) a CPersistInfo local copied twice (56 bytes); (b) a POD CPersistInfo through __default_constructed_aux (the lane
// zero-fills `_Tp()`); (c) a user default constructor (NRVO collapses it to 5 bytes); (d) this form -- a 12-byte first member
// with an empty default constructor and a memberwise copy constructor (62 bytes, one temporary). Next: the real CPersistInfo
// layout (Ego_r.pdb NScript::CRuntimeScriptedEntityPersist::CPersistInfo) with its member types' copy constructors, and the
// STLport `_HasDefaultZeroValue` tag argument passed by value.
struct S3 {
    unsigned long a, b, c;
    S3() { }
    S3(const S3& o) : a(o.a), b(o.b), c(o.c) { }
};
struct CPersistInfo {
    S3 first;
    unsigned long x, y, z;
};
struct __false_type { };
template <class _Tp> inline _Tp __default_constructed_aux(_Tp*, const __false_type&) { return _Tp(); }
template <class _Tp> _Tp __fastcall __default_constructed(_Tp* __p) { return __default_constructed_aux(__p, __false_type()); }
template CPersistInfo __fastcall __default_constructed<CPersistInfo>(CPersistInfo*);
