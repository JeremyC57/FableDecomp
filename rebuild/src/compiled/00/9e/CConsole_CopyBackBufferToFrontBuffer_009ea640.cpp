class CCharString { public:
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct CConBuf {                                         // 12 bytes: implicit copy assignment
    CCharString text;                                    // +0
    bool        b;                                       // +4
    int         d;                                       // +8
};
class CConsole { public:
    char pad20[0x20];
    CConBuf front;                                       // +0x20
    char pad2c[4];
    CConBuf back;                                        // +0x30
    void CopyBackBufferToFrontBuffer();
};
void CConsole::CopyBackBufferToFrontBuffer()
{
    CConsole* const& self = this;
    front.text = back.text;
    self->front.b = self->back.b;
    self->front.d = self->back.d;
}
