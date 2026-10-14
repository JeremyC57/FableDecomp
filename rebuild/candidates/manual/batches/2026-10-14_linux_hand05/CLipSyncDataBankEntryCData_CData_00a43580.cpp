// CLipSyncDataBankEntry::CData::CData(void* data, unsigned long size) -- base CResource(), own vtable, four null members, data pointer and size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A43580, length 48 bytes.
// Callees: CResource::CResource @ 0x009FBEC0. This class's vtable is 0x0129D3A4
// (rtti-vtables.tsv: CLipSyncDataBankEntry::CData). CResource spans +0..+0x1c (engine/CResource.h).
// The vtable store precedes the four register-zeroed stores at +0x1c..+0x28, so those pointers
// are an embedded member object with an inline constructor; the two arguments are assigned in
// the body (second argument loaded into ecx up front, stored last).
struct CResource {
    virtual void v00();
    unsigned char _body[0x18];              // +0x04..+0x1c
    CResource();                            // 0x009FBEC0
};

struct SLipSyncBuffers {                    // four raw pointers, nulled by the inline constructor
    void* m_pPhonemes;                      // +0x00
    void* m_pTimes;                         // +0x04
    void* m_pWeights;                       // +0x08
    void* m_pOwner;                         // +0x0c
    SLipSyncBuffers() : m_pPhonemes(0), m_pTimes(0), m_pWeights(0), m_pOwner(0) { }
};

struct CLipSyncDataBankEntry {
    struct CData : CResource {              // vtable 0x0129D3A4
        SLipSyncBuffers m_buffers;          // +0x1c
        void*           m_pData;            // +0x2c
        unsigned long   m_size;             // +0x30
        virtual void v00();
        CData(void* data, unsigned long size);
    };
};

CLipSyncDataBankEntry::CData::CData(void* data, unsigned long size)
    : CResource()
    , m_buffers()
{
    m_pData = data;
    m_size = size;
}
