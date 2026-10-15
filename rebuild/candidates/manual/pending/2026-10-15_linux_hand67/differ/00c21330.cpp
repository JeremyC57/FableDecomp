// CPlayer::IsFirstPersonTargeting(long unused) -- false when the object's long at +0x48 (getter 0x00C20EE0) is zero, else byte +0xC of the 16-byte record copied out of the member at +0x38 (getter 0x00C34600)
// Grade: DIFFER(48v42) -- retail keeps `this` in ecx across the call to 0x00C20EE0 (no `push esi ; mov esi,ecx`, then `add ecx,38h` for the member): whole-program register knowledge of the 0x00C2xxxx callee; the rest (16-byte record in the frame, `mov al,[esp+0Ch]`) matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C21330, length 42 bytes.
// Identity: the catalog label is doubtful: Ego_r.pdb `?IsFirstPersonTargeting@CPlayer@@QBE_NXZ` takes
// no argument while this body ends in `ret 4`, and its callees belong to the 0x00C2xxxx module
// (0x00C20EE0 = `push -1 ; call 0x00C20720 ; mov eax,[ecx+48h] ; ret`, manifest label
// "CCombatActionBase::GetAnimationTransitionInTime"; 0x00C34600 copies four dwords from `this` into
// its hidden result, manifest label "GFToScreen"). The class is not established; the label is kept.
// Callees: the +0x48 getter @ 0x00C20EE0, the record getter @ 0x00C34600 (`__thiscall`, hidden result).
// Shape: the record is a 16-byte local returned by value through the hidden pointer (`lea eax,[esp]
// ; push eax`), its byte +0xC is the result; the early `xor al,al` exit.
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
    long GetValue48() const;                       // 0x00C20EE0
    bool IsFirstPersonTargeting(long unused) const;
};

bool CPlayer::IsFirstPersonTargeting(long unused) const
{
    if (!GetValue48())
        return false;
    SRecord_c34600 record = m_record.GetRecord();
    return record.m_flag;
}
