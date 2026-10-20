// CDiceGameHost_00c14f90::~CDiceGameHost_00c14f90() -- destructor of an unidentified (no-RTTI) container class: drain and delete every entry its primary pool still hands out, delete the two owned helper objects, then the member pools and the base
// Flags: /EHsc
// Grade: DIFFER(248v248) -- scheduling only: retail forms `lea ebx,[esi+40h]` (PoolB, the next object destroyed) between the second and third handle vtable stores, ours before the first; the zero stores needed the inline COwnedPointer::Delete() pin (v1 had the +0x08 zero store sunk below the handle stores); class identity unresolved (no RTTI)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14F90, 248 bytes (+ int3 padding), plain `ret`; called only by its scalar
//   deleting destructor 0x00C15A10. The manifest label "CTCDiceGame::~CTCDiceGame" is propagated (the vtables stored here,
//   0x012B29F8 / 0x012B27E0 / 0x012B29B0 / 0x012B28D8 / 0x012B1AB0 / 0x012B2828, have no RTTI locator); every class name
//   below is descriptive.
// Callees: the pool's virtual slot 11 (+0x2c, next entry), DeleteData(entry) @ 0x00C14A50 (on `this`), the helpers'
//   destructors @ 0x00C2BE10 / 0x00C2A9B0 with operator delete @ 0x00BFE9BC, the pools' map member destructor @ 0x00C19250.
// Layout (from the stores): helpers +0x08 / +0x0c, pool A (vtable 0x012B29B0, base 0x012B1AB0, map +8) +0x10, pool B
//   (vtable 0x012B28D8, same base, map +8) +0x40, three 12-byte handles (vtable 0x012B2828) +0x78 / +0x84 / +0x90.
// Shape: /EHsc destructor (the member destructors are the unwind states); `while (entry = A.Next()) DeleteData(entry)`;
//   `delete` of each helper then a zero store; members die in reverse order with their own vtable resets.
struct CMap_00c19250 {
    unsigned char _body[0x28];
    ~CMap_00c19250();                               // 0x00C19250
};
struct CPoolBase {                                  // vtable 0x012B1AB0
    virtual void v00();
    unsigned char _pad04[4];
    ~CPoolBase() { }
};
struct CPoolA : CPoolBase {                         // vtable 0x012B29B0, 0x30 bytes
    CMap_00c19250 Map;                              // +0x08
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void* Next();                           // slot 11 (+0x2c)
    ~CPoolA() { }
};
struct CPoolB : CPoolBase {                         // vtable 0x012B28D8, 0x38 bytes
    CMap_00c19250 Map;                              // +0x08
    unsigned char _pad30[8];
    virtual void v01();
    ~CPoolB() { }
};
struct CHandle {                                    // vtable 0x012B2828, 12 bytes
    virtual void v00();
    unsigned char _pad04[8];
    ~CHandle() { }
};
struct CHelper08 { ~CHelper08(); };                 // 0x00C2A9B0
struct CHelper0c { ~CHelper0c(); };                 // 0x00C2BE10
template <class T> struct COwnedPointer {          // an owning pointer deleted and cleared through an inline helper
    T* p;
    void Delete() { delete p; p = 0; }
};
struct CHostBase {                                  // vtable 0x012B27E0
    virtual void v00();
    ~CHostBase() { }
};
struct CDiceGameHost_00c14f90 : CHostBase {         // vtable 0x012B29F8
    unsigned char _pad04[4];
    COwnedPointer<CHelper08> PHelper08;             // +0x08
    COwnedPointer<CHelper0c> PHelper0c;             // +0x0c
    CPoolA     PoolA;                               // +0x10
    CPoolB     PoolB;                               // +0x40
    CHandle    Handle0;                             // +0x78
    CHandle    Handle1;                             // +0x84
    CHandle    Handle2;                             // +0x90
    virtual void v00();
    void DeleteData(void* entry);                   // 0x00C14A50
    ~CDiceGameHost_00c14f90();
};

CDiceGameHost_00c14f90::~CDiceGameHost_00c14f90()
{
    void* entry;
    while ((entry = PoolA.Next()) != 0)
        DeleteData(entry);
    PHelper0c.Delete();
    PHelper08.Delete();
}
