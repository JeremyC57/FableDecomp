// CCardPositionsDef::Copy 0x004ebebc reconstruction
class CDefClassBase;

// member at +0x3c has its own Copy (0x46eabd) taking a src pointer on stack
struct SubMember {
    void CopySub(void const* src);   // __fastcall, src on stack
};

class CDefClassBase { public: char pad[0x3c]; };

class CCardPositionsDef { public:
    // base copy (0x431ed0): __fastcall, src on stack
    void BaseCopy(CDefClassBase const* src);
    char pad[0x3c];
    SubMember sub;
    void Copy(CDefClassBase const* src);
};

void CCardPositionsDef::Copy(CDefClassBase const* src)
{
    BaseCopy(src);
    sub.CopySub((char const*)src + 0x3c);
}