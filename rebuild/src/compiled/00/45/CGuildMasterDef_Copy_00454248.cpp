#pragma optimize("s",on)
class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
struct CBase {  unsigned char _b[0x30]; };
class CGuildMasterDef : public CBase { public: void Copy(const CGuildMasterDef* other); };
void CGuildMasterDef::Copy(const CGuildMasterDef* other) {
    ((CDefClassBase*)(this))->operator=(*(const CDefClassBase*)other);
    *(unsigned long*)((char*)this + 0x28) = *(const unsigned long*)((const char*)other + 0x28);
    *(unsigned long*)((char*)this + 0x2c) = *(const unsigned long*)((const char*)other + 0x2c);
}