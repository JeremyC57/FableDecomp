// CCodaVoice::ClearField0C(long unused) -- slot 3 of the GameCODA voice base: zero the word at +0xc and succeed (0)
// Leaf: ?ClearField0C@CCodaVoice@@UAEJJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3F460, 12 bytes, thiscall `ret 4`.
// Identity: slot 3 of the CCodaVoice vtable 0x012B5C10 (landed ctor 0x00C59BA0) and of the derived voice tables 0x012B5C60,
//   0x012B5CD0, 0x012B5D70, 0x012B8AE0, which inherit it. Descriptive (GameCODA: no PDB/RTTI): the word's role and the
//   argument are not proven.
// Shape: the store, then `xor eax,eax`.
class CCodaVoice {                                      // vtable 0x012B5C10
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual long ClearField0C(long unused);             // slot 3
    void* m_pPrev;                                      // +0x04
    void* m_pNext;                                      // +0x08
    long  m_0c;                                         // +0x0c
};

long CCodaVoice::ClearField0C(long unused)
{
    m_0c = 0;
    return 0;
}