// CConsole::CopyBackBufferToFrontBuffer  retail 0x009ea620

class CCharString { public:
    void* Data;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct ConBuffer {
    CCharString   data;    // +0x00
    unsigned char flag;    // +0x04
    int           extra;   // +0x08
    int           pad;     // +0x0C
};

class CConsole { public:
    char      pad0[0x20];  // +0x00
    ConBuffer back;        // +0x20
    ConBuffer front;       // +0x30
    void CopyBackBufferToFrontBuffer();
};

void CConsole::CopyBackBufferToFrontBuffer()
{
    this->front.data = this->back.data;
    this->front.flag = this->back.flag;
    int* pd = &this->front.extra;
    int* ps = &this->back.extra;
    *pd = *ps;
}