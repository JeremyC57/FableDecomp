// CManagedTexture::Sizeof() -- this object's size (8) plus, when the texture data (+4) exists, its block (0x4c + AllocSize) and the texture's (ByteLength + 0xc when a CTexture is held, 8 otherwise)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69E60, length 51 bytes.
// Shape: `mov eax,8` before the null test (the early return value), the inner size in eax and the `lea eax,[ecx+eax+54h]` tail duplicated into both arms, `ret`. Layout from engine/CManagedTextureData.h (Texture_Data +0x20, AllocSize +0x40) and engine/CTexture.h (ByteLength: 28 bits at +4).
class CTexture { public:
    void*         PD3DTexture;                     // +0x00
    unsigned long ByteLength : 28;                 // +0x04
    unsigned long AllocationSource : 4;
};
class CManagedTextureData { public:
    unsigned char _pad[0x20];
    CTexture*     Texture_Data;                    // +0x20 (CCountedPointer<CTexture>::Data)
    unsigned char _pad24[0x1c];
    unsigned long AllocSize;                       // +0x40
};
class CManagedTexture { public:
    void*                __vftable;                // +0x00
    CManagedTextureData* m_pData;                  // +0x04
    long Sizeof();
};

long CManagedTexture::Sizeof()
{
    long size = sizeof(CManagedTexture);
    if (m_pData)
        size = m_pData->AllocSize + (m_pData->Texture_Data ? m_pData->Texture_Data->ByteLength + 0xc : sizeof(CTexture)) + 0x54;
    return size;
}