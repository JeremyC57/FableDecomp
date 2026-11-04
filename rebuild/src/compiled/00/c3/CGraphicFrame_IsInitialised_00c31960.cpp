// `return this->ptr != 0;` (xor/test/setne). __fastcall this=ecx.
// Vendor naming wave V1 (one address, one name): 0x00C31960 ?IsReading@PKSerialiseCParser@@QAE_NXZ (was ?IsSet@T@@QAE_NXZ)
#pragma pack(push,1)
struct PKSerialiseCParser {
    int ptr;
    bool IsReading();
};
#pragma pack(pop)
bool PKSerialiseCParser::IsReading() { return this->ptr != 0; }