// Grade: DIFFER(22v1081) -- first-pass stub only (one grade); the body is not reconstructed yet, see the header for the decoded shape
// CIconText::InitIcons() -- lay out the inline icons of an icon-text string: copy the text into a wide buffer, replace
//   every icon marker with spaces sized to the icon (floor of the scaled width), and add one icon primitive per marker
//   to the engine primitive renderer at its x87-computed position
// Leaf: InitIcons
// Stub: x87-heavy (floor, scaled widths) frame-pointer body; first-pass skeleton only, not reconstructed yet.
struct CWideString {
    void* PStringData;
    long GetLength() const;                            // 0x0099B220
};
class CIconText {
public:
    unsigned char _pad00[0x164];
    CWideString   Text;                                // +0x164 (descriptive)
    long          TextLength;                          // +0x168 (descriptive)
    void InitIcons();
};
#pragma optimize("s", on)
#pragma optimize("y", off)
void CIconText::InitIcons()
{
    TextLength = Text.GetLength();
}
