// CFrameData::ResetTexture  (retail 0x009fe440)

class CGraphicFrame { public:
    void UnInitialise();   // 0x00A00B90 (Ego_r.pdb ?UnInitialise@CGraphicFrame@@QAEXXZ)
};
struct SubA {  // member at CFrameData+0x1c
    int x;
};

class CManagedTexture { public:
    bool Release();   // 0x00A69AD0 (Ego_r.pdb ?Release@CManagedTexture@@QAE_NXZ)
};
struct SubB {  // member at CFrameData+0x20
    int x;
};

struct CFrameData {
    char pad[0x1c];         // 0x00..0x1b
    SubA a;                 // 0x1c
    SubB b;                 // 0x20
    char pad2[0x38 - 0x24]; // 0x24..0x37
    unsigned char f38;      // 0x38
    unsigned char f39;      // 0x39
    unsigned char f3a;      // 0x3a
    void ResetTexture();
};

void CFrameData::ResetTexture()
{
    ((CManagedTexture*)&(this->b))->Release();
    ((CGraphicFrame*)&(this->a))->UnInitialise();
    this->f38 = 0xff;
    this->f3a = 0x7f;
}