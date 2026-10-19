// CEngineVideoOptionsDef::operator=(const CEngineVideoOptionsDef&) -- copy video option state and bit flags; retail 00434450 len 240
struct CEngineVideoOptionsBase { unsigned char _base[0x1c]; CEngineVideoOptionsBase& operator=(const CEngineVideoOptionsBase&); };
struct CVideoOptionName { unsigned char bytes[0x10]; CVideoOptionName& operator=(const CVideoOptionName&); };

#pragma pack(push,1)
struct VideoFlags { unsigned char bit0:1; unsigned char bit1:1; unsigned char bit2:1; unsigned char rest:5; };
struct CEngineVideoOptionsDef : CEngineVideoOptionsBase {
    unsigned long m_value1; unsigned long m_value2;
    VideoFlags m_flags;
    unsigned long m_word25;
    CVideoOptionName m_name; unsigned char _pad_name[3];
    unsigned long m_words[12];
    unsigned char m_bytes[8];
    unsigned long m_tail;
    CEngineVideoOptionsDef& operator=(const CEngineVideoOptionsDef&);
};
#pragma pack(pop)
CEngineVideoOptionsDef& CEngineVideoOptionsDef::operator=(const CEngineVideoOptionsDef& other)
{
    CEngineVideoOptionsBase::operator=(other);
    m_value1=other.m_value1; m_value2=other.m_value2;
    m_flags.bit0=other.m_flags.bit0;
    m_flags.bit1=other.m_flags.bit1;
    m_flags.bit2=other.m_flags.bit2;
    m_word25=other.m_word25;
    CVideoOptionName* srcName=(CVideoOptionName*)&other.m_name; CVideoOptionName* dstName=&m_name;
    dstName->operator=(*srcName); dstName->bytes[13]=srcName->bytes[13];
    m_words[0]=other.m_words[0]; m_words[1]=other.m_words[1]; m_words[2]=other.m_words[2]; m_words[3]=other.m_words[3];
    m_words[4]=other.m_words[4]; m_words[5]=other.m_words[5]; m_words[6]=other.m_words[6]; m_words[7]=other.m_words[7];
    m_words[8]=other.m_words[8]; m_words[9]=other.m_words[9]; m_words[10]=other.m_words[10]; m_words[11]=other.m_words[11];
    for (int i=0;i<8;i++) m_bytes[i]=other.m_bytes[i];
    m_tail=other.m_tail;
    return *this;
}
