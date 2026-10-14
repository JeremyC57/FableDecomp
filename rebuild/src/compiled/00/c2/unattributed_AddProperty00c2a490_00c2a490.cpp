// CTCPropertyContainer_00c2a490::AddProperty(void*, long, long, void*, void*, void*, void*) -- EH-framed: -1 when the map at +0x68 already knows the second argument (virtual slot 17, key by reference); the record for the third argument is looked up in the map at +8 (slot 17) or, when absent, allocated (0x50 bytes, `new`, state 0) and built with `this` and the key (@ 0x00C372C0); a 0x168-byte property is then allocated (`new`, state 1) and built with the record and the first, second and fourth to seventh arguments (@ 0x00C364A0, registering itself); the flag at +4 is set and 0 returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A490, oracle length 226 bytes. Built under /EHsc.
// Retail bytes:  ... mov eax,[edi+68h] ; lea ecx,[edi+68h] ; lea edx,[esp+1Ch] ; push edx ; call [eax+44h] ; test eax,eax ; je find ; or eax,-1 ; ... ret 1Ch ; find: mov eax,[edi+8] ; push esi ; lea ecx,[edi+8] ; lea edx,[esp+24h] ; push edx ; call [eax+44h] ; mov esi,eax ; test esi,esi ; jne have ; push 50h ; call operator new ; add esp,4 ; mov [esp+8],eax ; test eax,eax ; mov [esp+14h],esi ; je null ; mov ecx,[esp+24h] ; push ecx ; push edi ; mov ecx,eax ; call CRecord::CRecord ; jmp built ; null: xor eax,eax ; built: mov dword ptr [esp+14h],-1 ; mov esi,eax ; have: push 168h ; call operator new ; add esp,4 ; mov [esp+8],eax ; test eax,eax ; mov dword ptr [esp+14h],1 ; je done ; (six arguments pushed) ; push esi ; mov ecx,eax ; call CProperty::CProperty ; done: mov ecx,[esp+0Ch] ; mov byte ptr [edi+4],1 ; ... xor eax,eax ... ret 1Ch
// The state-0 store reuses the zero in esi; the `new` temporaries share one frame slot; the
// trailing -1 store is dropped after the last constructor. The manifest label is a description;
// the classes are named by address.
struct CTCPropertyContainer_00c2a490;
struct CRecord_00c2a490 {                  // 0x50 bytes
    unsigned char _body[0x50];
    CRecord_00c2a490(CTCPropertyContainer_00c2a490* owner, long key);   // 0x00C372C0
};

struct CProperty_00c2a490 {                // 0x168 bytes
    unsigned char _body[0x168];
    CProperty_00c2a490(CRecord_00c2a490* record, void* a, long b, void* d, void* e, void* f, void* g);   // 0x00C364A0
};

struct CRecordMap_00c2a490 {               // 0x60 bytes, polymorphic
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual CRecord_00c2a490* Find(const long& key);   // slot 17
    unsigned char _pad[0x5c];
};

struct CTCPropertyContainer_00c2a490 {
    long m_0;
    bool m_dirty;                          // +4
    CRecordMap_00c2a490 m_records;             // +8
    CRecordMap_00c2a490 m_properties;          // +0x68
    long AddProperty(void* a, long b, long c, void* d, void* e, void* f, void* g);
};

long CTCPropertyContainer_00c2a490::AddProperty(void* a, long b, long c, void* d, void* e, void* f, void* g)
{
    if (m_properties.Find(b))
        return -1;
    CRecord_00c2a490* record = m_records.Find(c);
    if (!record)
        record = new CRecord_00c2a490(this, c);
    new CProperty_00c2a490(record, a, b, d, e, f, g);
    m_dirty = true;
    return 0;
}