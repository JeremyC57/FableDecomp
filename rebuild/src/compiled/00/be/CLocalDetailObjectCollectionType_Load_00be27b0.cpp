// CLocalDetailObjectCollectionType::Load(CDataInputStream& stream, bool binary) -- read the three resource ids (binary longs, or text names resolved through the engine graphics bank), the 0x34-byte parameter block, then resolve the ids to bank entries
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE27B0, length 720 bytes (+ int3 padding), `ret 8`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inlined CMemoryDataInputStream::Read slow path),
//   stream virtual slot 6 (+0x18) reading a CCharString by value, the graphics bank's FindEntry(const CCharString&) 0x009CCDC0
//   (manifest "CQuestManager::OnQuestStartScreenDisplayed" is propagated), its two index getters 0x00A24280 / 0x00A24290
//   (byte-identical `[this+0x380][i]` bodies; manifest "CTCInventoryAbilities::GetAbilityLevel" is propagated),
//   CCharString::~CCharString 0x0099EAE0. The bank is [[0x01436E98]+0x10].
// Layout: as the landed copy ctor 0x00BE26C0 -- vptr, seven longs +4..+0x1c, a 13-dword block at +0x20 (word 9 = +0x44 is the
//   primitive kind tested against 1).
// Shape: the bank is reached through an inline GetBank() accessor (the global-then-bank load sits in the call block). Each id read goes through a __forceinline helper whose local is homed in the dead stream-argument slot; the text
//   helper returns early on an empty name, so the CCharString dtor is emitted on both arms.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CCharStringData { public:
    char* Data;                             // +0
    long  Length;                           // +4
};
class CCharString {
public:
    CCharStringData* PStringData;
    ~CCharString();                         // 0x0099EAE0
    bool IsEmpty() const { return PStringData == 0 || PStringData->Length == 0; }
};

struct CGraphicsBank {
    unsigned long FindEntry(const CCharString& name);   // 0x009CCDC0
    long GetEntry(long index);                          // 0x00A24280
    long GetAnimatedEntry(long index);                  // 0x00A24290
};
struct SEngineResources {                   // engine resource record reached through the pointer at 0x01436E98
    unsigned char  _pad[0x10];
    CGraphicsBank* Bank;                    // +0x10
    CGraphicsBank* GetBank() const { return Bank; }
};
extern SEngineResources* g_pEngineResources;   // 0x01436E98

class CDataInputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual CCharString ReadString();       // slot 6 (+0x18)
    unsigned long        m_position;        // +4
    long                 StreamSize;        // +8
    const unsigned char* m_pData;           // +0xC
    long                 SourceChunkStreamPos;
    long                 m_remaining;       // +0x14
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong() { long v; Read(&v, sizeof(v)); return v; }
    __forceinline long ReadIndex()
    {
        CCharString name = ReadString();
        if (name.IsEmpty())
            return 0;
        return g_pEngineResources->GetBank()->FindEntry(name);
    }
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};

struct SCollectionBlock {                   // 0x34 bytes of plain data at +0x20
    long m_words[9];
    long m_kind;                            // +0x24 (object +0x44)
    long m_tail[3];
};

class CBaseClassNonCopyable {
public:
    virtual void v00();
};

class CLocalDetailObjectCollectionType : public CBaseClassNonCopyable {
public:
    long             m_4;                   // +0x04 resolved entry for m_14
    long             m_8;                   // +0x08 resolved entry for m_18
    long             m_c;                   // +0x0c resolved entry for m_1c
    long             m_10;                  // +0x10
    long             m_14;                  // +0x14
    long             m_18;                  // +0x18
    long             m_1c;                  // +0x1c
    SCollectionBlock m_block;               // +0x20
    void Load(CDataInputStream& stream, bool binary);
};

void CLocalDetailObjectCollectionType::Load(CDataInputStream& stream, bool binary)
{
    if (binary) {
        m_14 = stream.ReadLong();
        m_1c = stream.ReadLong();
        m_18 = stream.ReadLong();
    } else {
        m_14 = stream.ReadIndex();
        m_1c = stream.ReadIndex();
        m_18 = stream.ReadIndex();
    }
    stream.Read(&m_block, sizeof(m_block));
    if (m_14 > 0) {
        if (m_block.m_kind == 1)
            m_4 = g_pEngineResources->GetBank()->GetAnimatedEntry(m_14);
        else
            m_4 = g_pEngineResources->GetBank()->GetEntry(m_14);
    }
    if (m_1c > 0)
        m_c = g_pEngineResources->GetBank()->GetEntry(m_1c);
    if (m_18 > 0)
        m_8 = g_pEngineResources->GetBank()->GetEntry(m_18);
}