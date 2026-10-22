// CFactory::Create(type, source) -- static factory: build the object for one of twelve packed type codes (0x10101..0x20203:
//   byte 0 kind 1..3, byte 1 variant 1..2, byte 2 family 1..2) from the values the source hands out (slot 0 for kind 1,
//   slot 1 for kind 2, slot 1 plus slot 2 for kind 3), then pass the source's slot-5 value to the new object's slot-5
//   target (no null check: an unknown code dereferences 0, as retail does). The catalog label
//   CCreatureAction_UnsheatheWeapons::Create is a forge autoname; the class is not confirmed.
// Leaf: Create
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1EFC0, length 853 bytes, __fastcall (type in ecx, source in edx), /EHsc.
// Callees: operator new 0x00BFEA1A, twelve constructors (ret 4 / ret 8 in retail bytes) listed per class below,
//   0x00C34870 (thiscall, ret 4) on the object's slot-5 result.
// Layout: object sizes are the operator new sizes in retail bytes (0x10/0x14/0x18/0x1c); every class is polymorphic
//   with slot 5 (+0x14) returning the target.
struct CFactoryTarget {
    void Set(long value);                                    // 0x00C34870
};
struct CFactorySource {
    virtual const long* Value0();                            // slot 0
    virtual const long* Value1();                            // slot 1
    virtual long        Value2();                            // slot 2
    virtual void v3(); virtual void v4();
    virtual long        Value5();                            // slot 5 (+0x14)
};
struct CFactoryProduct {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual CFactoryTarget* GetTarget();                     // slot 5 (+0x14)
};
struct CFactoryProduct_10101 : CFactoryProduct {
    unsigned char _pad04[0x10 - 4];
    CFactoryProduct_10101(long a);   // 0x00C1F720
};
struct CFactoryProduct_20101 : CFactoryProduct {
    unsigned char _pad04[0x18 - 4];
    CFactoryProduct_20101(long a);   // 0x00C1F7E0
};
struct CFactoryProduct_10201 : CFactoryProduct {
    unsigned char _pad04[0x18 - 4];
    CFactoryProduct_10201(long a);   // 0x00C1F8A0
};
struct CFactoryProduct_20201 : CFactoryProduct {
    unsigned char _pad04[0x18 - 4];
    CFactoryProduct_20201(long a);   // 0x00C1F960
};
struct CFactoryProduct_10102 : CFactoryProduct {
    unsigned char _pad04[0x10 - 4];
    CFactoryProduct_10102(long a);   // 0x00C1F640
};
struct CFactoryProduct_20102 : CFactoryProduct {
    unsigned char _pad04[0x18 - 4];
    CFactoryProduct_20102(long a);   // 0x00C1FA20
};
struct CFactoryProduct_10202 : CFactoryProduct {
    unsigned char _pad04[0x18 - 4];
    CFactoryProduct_10202(long a);   // 0x00C1FB00
};
struct CFactoryProduct_20202 : CFactoryProduct {
    unsigned char _pad04[0x18 - 4];
    CFactoryProduct_20202(long a);   // 0x00C1FBE0
};
struct CFactoryProduct_10103 : CFactoryProduct {
    unsigned char _pad04[0x14 - 4];
    CFactoryProduct_10103(long a, long b);   // 0x00C20230
};
struct CFactoryProduct_20103 : CFactoryProduct {
    unsigned char _pad04[0x1c - 4];
    CFactoryProduct_20103(long a, long b);   // 0x00C1FCC0
};
struct CFactoryProduct_10203 : CFactoryProduct {
    unsigned char _pad04[0x1c - 4];
    CFactoryProduct_10203(long a, long b);   // 0x00C1FD60
};
struct CFactoryProduct_20203 : CFactoryProduct {
    unsigned char _pad04[0x1c - 4];
    CFactoryProduct_20203(long a, long b);   // 0x00C1FE00
};
struct CFactory {
    static CFactoryProduct* __fastcall Create(unsigned long type, CFactorySource* source);
};

CFactoryProduct* __fastcall CFactory::Create(unsigned long type, CFactorySource* source)
{
    CFactoryProduct* product = 0;
    switch (type) {
    case 0x10101:
        product = new CFactoryProduct_10101(*source->Value0());
        break;
    case 0x20101:
        product = new CFactoryProduct_20101(*source->Value0());
        break;
    case 0x10201:
        product = new CFactoryProduct_10201(*source->Value0());
        break;
    case 0x20201:
        product = new CFactoryProduct_20201(*source->Value0());
        break;
    case 0x10102:
        product = new CFactoryProduct_10102(*source->Value1());
        break;
    case 0x20102:
        product = new CFactoryProduct_20102(*source->Value1());
        break;
    case 0x10202:
        product = new CFactoryProduct_10202(*source->Value1());
        break;
    case 0x20202:
        product = new CFactoryProduct_20202(*source->Value1());
        break;
    case 0x10103:
        product = new CFactoryProduct_10103(source->Value2(), *source->Value1());
        break;
    case 0x20103:
        product = new CFactoryProduct_20103(source->Value2(), *source->Value1());
        break;
    case 0x10203:
        product = new CFactoryProduct_10203(source->Value2(), *source->Value1());
        break;
    case 0x20203:
        product = new CFactoryProduct_20203(source->Value2(), *source->Value1());
        break;
    }
    product->GetTarget()->Set(source->Value5());
    return product;
}
