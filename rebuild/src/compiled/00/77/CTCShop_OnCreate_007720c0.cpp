// Clear the member at this+0x24. __fastcall this=ecx.
#pragma pack(push,1)
class CTCShop { public:
    char pad_0[0x24];
    char field;
    void Clear();
};
#pragma pack(pop)
void CTCShop::Clear() { this->field = 0; }