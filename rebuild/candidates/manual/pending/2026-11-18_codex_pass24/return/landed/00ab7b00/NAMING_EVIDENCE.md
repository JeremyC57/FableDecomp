# Naming evidence

- Ego_r donor 0x00C581F0 and FableWin donor 0x034BB3A0 name `?GetStringExtents@CStaticFontBank@@UBE?AVC2DExtentsF@@PB_W_N@Z`. The retail CStaticFontBank vtable at 0x0129EAD4 points slot 1 at 0x00AB7B00, and Ego_d places it in lib_font_bank.obj. The source uses the existing FableFontWideChar ABI alias for the 16-bit code unit.
- Ego_d gives `string`, `first_line_only`, `extents`, `sx`, `sy`, and the loop's `pglyph` and `letter`. The code-unit stride, signed glyph metrics and running X/Y maxima corroborate their types and uses.
- FableWin donor 0x034BB530 gives protected `?FindGlyph@CStaticFontBank@@IBEPAVCGlyph@@G@Z`. Ego_d places it immediately after GetStringExtents and gives `ch`, `bankIndex`, `bank`, and `index`. The same native body expands bank selection by character / 64 at 0x00AB7B87, followed by unsigned range testing and the 0x18-byte glyph stride.
- Float literals are compiler literals at 0x0122DEDC and 0x0122DCB4. The first is the documented shared 0.0 pool value and the second is the compiler's unsigned-conversion bias. Both were checked as retail data.

## Behavioral fixture

The exact selected method uses the shared extents type. Fixture glyph banks supply Latin, extended, missing and out-of-range characters, plus multiline and unsigned height cases. VC7.1 RTM /Od /W3 and GNU C++03 both passed.
