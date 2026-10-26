class CTCShop;
class CTCShopKeeper {
public:
    CTCShop* GetShopTC() const;   // 0x00798F50
};
struct S;  extern void __fastcall od2(S* p);
struct S { int x; void M(); };
void S::M(){ S* p=((S*)(((CTCShopKeeper*)(this))->GetShopTC())); if(p) od2(p); }