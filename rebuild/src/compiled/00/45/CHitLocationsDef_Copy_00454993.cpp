class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
class CHitLocationsDef { public:
    unsigned char _tail[0x34];
};
struct CSub {
    void SubCopy(const CSub* src);
};

void __fastcall CHitLocationsDef_Copy(CHitLocationsDef* self, int /*edx*/, const CHitLocationsDef* src)
{
    ((CDefClassBase*)(self))->operator=(*(const CDefClassBase*)(src));
    ((CSub*)((unsigned char*)self + 0x28))->SubCopy((const CSub*)((const unsigned char*)src + 0x28));
}