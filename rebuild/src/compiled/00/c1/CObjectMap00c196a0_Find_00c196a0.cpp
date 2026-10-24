// CObjectMap_00c196a0::Find(const SObjectKey_00c196a0* key) -- resolves a lookup key: picks the scope (the key's own scope record, else the map owner's default at +4 +0x3c), then looks the object up by name when the key carries one, else by its value through the scope's id finder (type 0) or the scope's second finder (type 1); 0 for any other type
// Leaf: ?Find@CObjectMap_00c196a0@
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C196A0, 186 bytes (+ int3 padding), __fastcall `ret 4`, EH-framed.
// Identity: the libpk map lookup the landed Resolve rows 0x00C19AD0 / 0x00C19B70 / 0x00C19C10 call as their Find
//   (also 0x00C11B00, 0x00C1340C, 0x00C199D7). Callees: GetScope @ 0x00C19680 (same TU: `key ? key + 0xc : m_4 +
//   0x3c`), the name finder @ 0x00C101D0 (const string&, 0), the two id finders @ 0x00C10140 / 0x00C10120 (each
//   forwards the id by address to a virtual slot 17), the MFC-style string @ 0x00C0BFE0 / 0x00C0BA40.
// Shape: ONE result variable initialised to 0 (retail zeroes eax before the name test, so the name lands in ecx);
//   per-path `return`s instead give a fourth epilogue with its own `xor eax,eax` (grade 1). The implicit string
//   temporary is built in the dead key slot (EH state 0 only around the name finder); the type `switch` tests case 0
//   first and lays it out last, sharing the final epilogue with the default; GetScope's body sits above (the retail TU
//   holds it right before this function).
// Label correction: the catalog's `TemplateConstruct_AndFindAnimation` is propagated; descriptive names.
struct CString_00c196a0 {                  // MFC-style string: one word
    char* m_pchData;
    CString_00c196a0(const char* text);    // 0x00C0BFE0
    ~CString_00c196a0();                   // 0x00C0BA40
};

struct CObject_00c196a0;

struct CScope_00c196a0 {
    CObject_00c196a0* FindByName(const CString_00c196a0& name, long flags);   // 0x00C101D0
    CObject_00c196a0* FindById(long id);           // 0x00C10140
    CObject_00c196a0* FindByAltId(long id);        // 0x00C10120
};

struct SObjectKey_00c196a0 {
    long m_0;
    long m_type;                           // +0x04: 0 id, 1 alternate id
    long m_value;                          // +0x08
    const char* m_pName;                   // +0x0c
    unsigned char* m_pScope;               // +0x10: a scope record (the scope sits at its +0xc)
};

struct CObjectMapOwner_00c196a0 {
    unsigned char _pad[0x3c];
    unsigned char m_defaultScope[4];       // +0x3c
};

struct CObjectMap_00c196a0 {
    long m_0;
    CObjectMapOwner_00c196a0* m_pOwner;    // +0x04
    CScope_00c196a0* GetScope(unsigned char* record);
    CObject_00c196a0* Find(const SObjectKey_00c196a0* key);
};

#pragma auto_inline(off)
CScope_00c196a0* CObjectMap_00c196a0::GetScope(unsigned char* record)   // 0x00C19680
{
    unsigned char* scope = m_pOwner->m_defaultScope;
    if (record)
        scope = record + 0xc;
    return (CScope_00c196a0*)scope;
}
#pragma auto_inline(on)

CObject_00c196a0* CObjectMap_00c196a0::Find(const SObjectKey_00c196a0* key)
{
    CScope_00c196a0* scope = GetScope(key->m_pScope);
    CObject_00c196a0* result = 0;
    if (key->m_pName) {
        result = scope->FindByName(key->m_pName, 0);
    } else {
        switch (key->m_type) {
        case 0:
            result = scope->FindById(key->m_value);
            break;
        case 1:
            result = scope->FindByAltId(key->m_value);
            break;
        }
    }
    return result;
}