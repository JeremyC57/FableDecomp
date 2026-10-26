// MD5(const CCharString& text) -- (anonymous namespace) the lower-case hex MD5 digest of a narrow string: md5_init / md5_append over the string's bytes / md5_finish, then the 16 digest bytes appended two hex digits at a time
// Leaf: ?MD5@
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055E5F0, 185 bytes (192 with padding), plain `ret` (/Gr __fastcall: the hidden
//   CCharString result in ecx, &text in edx); /GS cookie 0x0139C8A8 checked by 0x00BFE9F9.
// Callers: 0x00560DE9, 0x00560F2A (CTCHeroOnlineScoreboard::ProcessLogin 0x00560D00; the scoreboard TU owns this
//   anonymous namespace).
// Callees: CCharString() 0x0099E4B0, md5_init 0x00A3DF00 / md5_append 0x00A3DF30 / md5_finish 0x00A3E010 (Ego_r.pdb
//   `@md5_init@4`, `@md5_append@12`, `@md5_finish@8`: L. Peter Deutsch's md5.c built extern "C" __fastcall),
//   _snprintf 0x00BFEB96 ("%02x" @ 0x0124C5C0), CCharString::operator+=(const char*) 0x0099F100, copy ctor 0x0099EC30,
//   ~CCharString 0x0099EAE0.
// Label correction: the catalogue's "MD5" is right but unscoped; Ego_r.pdb `?MD5@?A0xf010c0d0@@YI?AVCCharString@@ABV2@@Z`
//   (anonymous namespace, __fastcall; our object's namespace hash differs, so the leaf is the `?MD5@` prefix).
//   Callee 0x00A3DF00 is catalogued "COpinionTransientOffset::COpinionTransientOffset" -- it is md5_init.
// Layout: md5_state_t { count[2], abcd[4], buf[64] } = 0x58 bytes; CCharString { CCharStringData* } with the inline
//   length / text accessors ("" = 0x0129AAF4 without a data block). Local model (fable_string.h has no inline accessors).
// Shape: the length argument is evaluated before the text pointer (right to left); `hex[2] = 0` is scheduled after the
//   push of the buffer address; `return result;` copies into the hidden result and destroys the local.
extern "C" int __cdecl _snprintf(char* buffer, unsigned int count, const char* format, ...);   // 0x00BFEB96

typedef unsigned char md5_byte_t;
typedef unsigned int md5_word_t;
typedef struct md5_state_s {
    md5_word_t count[2];                    // +0x00 message length in bits, lsw first
    md5_word_t abcd[4];                     // +0x08 digest buffer
    md5_byte_t buf[64];                     // +0x18 accumulate block
} md5_state_t;                              // 0x58 bytes
extern "C" void __fastcall md5_init(md5_state_t* pms);                                           // 0x00A3DF00
extern "C" void __fastcall md5_append(md5_state_t* pms, const md5_byte_t* data, int nbytes);     // 0x00A3DF30
extern "C" void __fastcall md5_finish(md5_state_t* pms, md5_byte_t digest[16]);                  // 0x00A3E010

class CCharStringData { public:
    const char* m_pData;                    // +0x00
    long m_length;                          // +0x04
};
class CCharString {
public:
    CCharStringData* m_pData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator+=(const char* text);    // 0x0099F100
    long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetString() const { return m_pData == 0 ? "" : m_pData->m_pData; }   // "" = 0x0129AAF4
};

namespace {
CCharString __fastcall MD5(const CCharString& text)
{
    CCharString result;
    md5_state_t state;
    md5_init(&state);
    md5_append(&state, (const md5_byte_t*)text.GetString(), text.GetLength());
    md5_byte_t digest[16];
    md5_finish(&state, digest);
    for (int i = 0; i < 16; ++i) {
        char hex[3];
        _snprintf(hex, 3, "%02x", digest[i]);
        hex[2] = 0;
        result += hex;
    }
    return result;
}
}