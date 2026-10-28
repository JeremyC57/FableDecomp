// CGameMapCell::GetAtmosBlend(long index) const -- the blend weight of one of the cell's atmosphere themes: the two stored blends, and for the third theme what is left of 255
// Leaf: ?GetAtmosBlend@CGameMapCell@@QBEEJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B74E0, 47 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?GetAtmosBlend@CGameMapCell@@QBEEJ@Z (const, unsigned char, long index); Ego_r.pdb
//   CGameMapCell is 7 bytes: AtmosThemes[3] +0, AtmosBlends[2] +3, SoundTheme +5, Music +6 -- the body reads only
//   AtmosBlends. Callers 0x0049E51C / 0x0049F365 / 0x0049F37A / 0x0081F1EF.
// Shape: the third blend sums the blends before it (`i != index`: VC7.1 propagates index == 2 into the compare, `cmp eax,2 ;
//   jne`, but does not unroll; a literal bound of 2 unrolls) and
//   returns 0xFF minus the total.
class CGameMapCell {
public:
    unsigned char AtmosThemes[3];                    // +0x00
    unsigned char AtmosBlends[2];                    // +0x03
    signed char   SoundTheme;                        // +0x05
    unsigned char Music;                             // +0x06
    unsigned char GetAtmosBlend(long index) const;
};

unsigned char CGameMapCell::GetAtmosBlend(long index) const
{
    if (index == 2) {
        long total = 0;
        for (long i = 0; i != index; ++i)
            total += AtmosBlends[i];
        return (unsigned char)(255 - total);
    }
    return AtmosBlends[index];
}