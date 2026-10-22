// CChunkedFileRawDataChunk::WriteToFile @ 0x00aa7830

struct CDataOutputStream {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void Write(void* buf, unsigned int len);  // slot4 @ vtbl+0x10
};

struct CChunkedFileSaver;

struct CChunkedFileRawDataChunk {
    char pad[0x10];
    void* Data;          // +0x10 buffer
    unsigned int ByteSize;   // +0x14 length
    void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver);
};

void CChunkedFileRawDataChunk::WriteToFile(CDataOutputStream& stream,
                                           CChunkedFileSaver& saver)
{
    stream.Write(this->Data, this->ByteSize);
}