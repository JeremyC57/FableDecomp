// Behaviour test for 00c21330: src/00c21330.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CPlayer::IsFirstPersonTargeting(long unused)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CPlayer::IsFirstPersonTargeting(long unused) -- false when the object's long at +0x48 (getter 0x00C20EE0) is zero, else byte +0xC of the 16-byte record copied out of the member at +0x38 (getter 0x00C34600)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21330, length 42 bytes.
// Identity: the catalog label is doubtful: Ego_r.pdb `?IsFirstPersonTargeting@CPlayer@@QBE_NXZ` takes
// no argument while this body ends in `ret 4`, and its callees belong to the 0x00C2xxxx module
// (0x00C20EE0 = `push -1 ; call 0x00C20720 ; mov eax,[ecx+48h] ; ret`, manifest label
// "CCombatActionBase::GetAnimationTransitionInTime"; 0x00C34600 copies four dwords from `this` into
// its hidden result, manifest label "GFToScreen"). The class is not established; the label is kept.
// Callees: the +0x48 getter @ 0x00C20EE0, the record getter @ 0x00C34600 (`__fastcall`, hidden result).
// Shape: the record is a 16-byte local returned by value through the hidden pointer (`lea eax,[esp]
// ; push eax`), its byte +0xC is the result; the early `xor al,al` exit. Retail keeps `this` in ecx across the
// 0x00C20EE0 call: both that getter and its callee 0x00C20720 are compiled in the same TU above the caller and never
// touch ecx (VC7.1 callee register-usage knowledge), so their bodies are written out here.
struct SRecord_c34600 {                            // 16 bytes, copied whole by the getter
    long          m_a;
    long          m_b;
    long          m_c;
    bool          m_flag;                          // +0x0c (the result)
    unsigned char _pad[3];
    SRecord_c34600() { }
};
struct CRecordHolder_c34600 {
    SRecord_c34600 GetRecord() const;              // 0x00C34600
};

struct CPlayer {
    unsigned char        _pad00[0x38];
    CRecordHolder_c34600 m_record;                 // +0x38
    unsigned char        _pad39[0x48 - 0x39];
    long                 m_value48;                // +0x48
    unsigned char        _pad4c[0xac - 0x4c];
    unsigned short       m_state;                  // +0xac
    long GetValue48() const;                       // 0x00C20EE0
    void CheckState(long state) const;             // 0x00C20720
    bool IsFirstPersonTargeting(long unused) const;
};

// Same-TU bodies of the two callees (retail's TU): 0x00C20720 switches on the word at +0xAC with every case
// returning at once; 0x00C20EE0 calls it with -1 and returns the long at +0x48. Neither touches ecx.
__declspec(noinline) void CPlayer::CheckState(long state) const
{
    switch (m_state) { case 0: case 1: case 2: case 3: case 4: case 5: case 6: break; }
}
__declspec(noinline) long CPlayer::GetValue48() const
{
    CheckState(-1);
    return m_value48;
}

bool CPlayer::IsFirstPersonTargeting(long unused) const
{
    if (!GetValue48())
        return false;
    SRecord_c34600 record = m_record.GetRecord();
    return record.m_flag;
}

// ---- stubs and driver ----
static int g_records = 0; static const CRecordHolder_c34600* g_holder = 0; static bool g_flag = false;
SRecord_c34600 CRecordHolder_c34600::GetRecord() const
{
    ++g_records; g_holder = this;
    SRecord_c34600 r; r.m_a = 1; r.m_b = 2; r.m_c = 3; r.m_flag = g_flag; return r;
}
int main()
{
    static CPlayer player; memset(&player, 0, sizeof(player));
    CHECK((char*)&player.m_state - (char*)&player == 0xac && (char*)&player.m_value48 - (char*)&player == 0x48);
    player.m_state = 3;
    CHECK(!player.IsFirstPersonTargeting(0) && g_records == 0);        // +0x48 zero: early false
    player.m_value48 = 5; g_flag = true;
    CHECK(player.IsFirstPersonTargeting(7) && g_records == 1 && g_holder == &player.m_record);
    CHECK((const unsigned char*)g_holder - (const unsigned char*)&player == 0x38);
    g_flag = false; player.m_state = 9;                                 // a state outside the switch changes nothing
    CHECK(!player.IsFirstPersonTargeting(0) && g_records == 2);
    if (fails == 0) printf("00c21330_TEST PASS\n");
    return fails;
}
