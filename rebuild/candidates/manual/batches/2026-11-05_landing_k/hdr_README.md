# Chunk 784 header overlay

Every edit sits behind a NEW opt-in macro. No landed source defines these macros, so the default path of each header is
byte-for-byte unchanged and no landed includer can change. No includer recheck is needed: the brief requires one only when a
default path changes.

| Header | Opt-in macro | What it adds | Evidence | Used by |
|---|---|---|---|---|
| fable_string.h | FABLE_RETAIL_CWIDESTRING_TEXT_EQ | `CWideStringData::operator==(const unsigned short*)`, the inline `CWideString::operator==(const wchar_t*)` (a null string compares the literal with "" through wcscmp, else the STLport text operator==), and an explicit-specialization declaration of the STLport `operator==(basic_string<unsigned short>, const unsigned short*)` (char_traits<unsigned short>::length has no body here) | FableWin ??8CWideString@@QBE_NPBG@Z, inline at 0x0049227C..0x004922A7. The out-of-line STLport text compare is 0x00404280 | parked 0x00491AD2 |
| fable_string.h | FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ | `__forceinline` on both CWideString operator== overloads (a per-TU opt-in like FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ) | The /Os CSubtitleRenderer TU expands both in place (0x00491CF6..0x00491D1B, 0x0049227C..0x004922A7) | parked 0x00491AD2 |
| fable_string.h | FABLE_RETAIL_CWIDESTRING_SUBSTRING | `CWideString GetSubString(unsigned long start, unsigned long end) const; // 0x0099BDB0` | Ego_r + FableWin ?GetSubString@CWideString@@QBE?AV1@KK@Z. Caller 0x00491B44 | parked 0x00491AD2 |
| fable_string.h | FABLE_RETAIL_CCHARSTRING_STARTSWITH | `__forceinline bool StartsWith(const CCharString&) const` (strncmp of this string's GetAsCharArray against the prefix, prefix length) | Ego_r ?StartsWith@CCharString@@QBE_NABV1@@Z, expanded at 0x004920BE..0x004920F6 (`neg; sbb; inc`) | parked 0x00491AD2 |
| engine/CFontBank.h | FABLE_RETAIL_CFONTBANK_STRING_HELPERS | `C2DExtentsF GetStringExtents(const CWideString&, bool) const; // 0x0048DAA2`, `float GetStringWidth(const CWideString&) const; // 0x0048DA62` (declarations only) | FableWin ?GetStringExtents@CFontBank@@QBE?AVC2DExtentsF@@ABVCWideString@@_N@Z, Ego_r/FableWin ?GetStringWidth@CFontBank@@QBEMABVCWideString@@@Z | parked 0x00491AD2 |
| engine/CANavQuadTreeNode.h | FABLE_RETAIL_CANAVQUADTREENODE_AREA_OUTOFLINE (nested in FABLE_RETAIL_CANAVQUADTREENODE) | out-of-line `C2DBoxF GetAreaCovering() const; // 0x00A763B0` in place of the inline body | OutputWorld calls 0x00A763B0 at all seven sites. Bound spelling ?GetAreaCovering@CANavQuadTreeNode@@QBE?AVC2DBoxF@@XZ | parked 0x00A15890 |
| engine/CNavBlockedQuadTreeNode.h | FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE_COPY (nested in FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE) | `CNavBlockedQuadTreeNode(const CNavBlockedQuadTreeNode&); // 0x00A76900` | Ego_r ??0CNavBlockedQuadTreeNode@@QAE@ABV0@@Z (landed out of line). OutputWorld `push node; call 0x00A76900` | parked 0x00A15890 |
| engine/CRenderManager2D.h | FABLE_RETAIL_CRENDERMANAGER2D_VIEWPORTBOXFLOAT | `const C2DBoxF& GetViewportBoxFloat() const; // 0x00A0AAA0` (declaration only) | Bound spelling ?GetViewportBoxFloat@CRenderManager2D@@QBEABVC2DBoxF@@XZ (reloc_bindings; the landed Add2DTri 0x009DC030 calls it). No PDB record | parked 0x00BC6DB0 |

The overlay copy of CNavBlockedQuadTreeNode.h includes CANavQuadTreeNode.h from its own directory, so both must be copied
together. Confirmed in use: the same 0x00A15890 source grades 1999 bytes with the overlay and 2047 without it. Without the overlay
the opt-in macros are simply ignored.
