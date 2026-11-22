# Native VC7.1 source experiments

Received baseline DIFFER at 1783 bytes.

| Probe | Source change | Result | Aligned retail instructions |
| --- | --- | --- | ---: |
| `r_font_distinct_wide_conversions` | Use the string conversion operator for the initial extents and GetAsCharArray for the glyph loop. | DIFFER 1594/1910 | 283 |
| `r_font_empty_buffer_guard_typed` | Check the empty batch before the final unlock. | DIFFER 1589/1910 | 273 |
| `r_font_hoist_line_flag_typed` | Combine line-start and flag tests before loading extents. | DIFFER 1591/1910 | 271 |
| `r_font_scoped_vertex_assignment` | Ablate constness on the vertex reciprocal homogeneous W. | DIFFER 1598/1910 | 277 |
| `r_font_shared_masks_typed` | Use the actual virtual signature, glyph type, RGBA word, texture mask array and buffer types. | DIFFER 1594/1910 | 283 |
| `r_font_specific_floor_typed` | Evaluate the centering halves as double operands. | DIFFER 1596/1910 | 280 |
| `r_font_vector_temporaries` | Pass C3DVector position and C2DVector UV temporary to vertex assembly. | DIFFER 1598/1910 | 277 |
| `r_font_vertices_indexed_typed` | Write six vertices with explicit indices instead of sequenced pointer increments. | DIFFER 1548/1910 | 223 |

Selected `r_font_distinct_wide_conversions`. Corrected the virtual const signature, texture mask, buffer ownership and six-vertex order. The font temporaries and x87 evaluation still change the frame and vertex copies.

The alignment column is diagnostic. It is not a byte or relocation grade.

## Continued main-chat grades

The full 1,910-byte leaf matched retail. The donor calls the free InitFont2DVertex helper. Its true aggregate result and vector argument restore the six vertex copies. Re-evaluating the corner expressions and grouping the UV multiplication reproduce the x87 stream. All 22 relocated operands were verified and the mapped inline copy matched. The native test runs the reconstructed renderer and checks six vertices, selected texture and masked mode, spacing, justification, empty input and the batch threshold.

| Probe | Compiled bytes | Aligned instructions | Mapped copies equal / unequal |
|---|---:|---:|---:|
| round2_font_f32 | 1906 | 401 | 0 / 0 |
| round2_font_indexed | 1869 | 268 | 1 / 0 |
| round2_font_indexed_f32 | 1869 | 268 | 0 / 0 |
| round2_font_sdk | 1906 | 401 | 1 / 0 |
| round2_font_value | 1906 | 401 | 1 / 0 |
| round2_fontmath_YX | 1910 | 441 | 1 / 0 |
| round2_fontmath_YX_double | 1905 | 300 | 1 / 0 |
| round2_fontmath_YX_natural | 1905 | 299 | 1 / 0 |
| round2_fontmath_x | 1912 | 413 | 1 / 0 |
| round2_fontmath_x_double | 1910 | 391 | 1 / 0 |
| round2_fontmath_x_natural | 1910 | 390 | 1 / 0 |
| round2_fontmath_xy | 1910 | 441 | 1 / 0 |
| round2_fontmath_xy_double | 1905 | 300 | 1 / 0 |
| round2_fontmath_xy_natural | 1905 | 299 | 1 / 0 |
| round2_fontmath_y | 1904 | 423 | 1 / 0 |
| round2_fontmath_y_double | 1906 | 414 | 1 / 0 |
| round2_fontmath_y_natural | 1906 | 413 | 1 / 0 |
| round2_fontsch_count | 1910 | 442 | 1 / 0 |
| round2_fontsch_count_inc | 1910 | 442 | 1 / 0 |
| round2_fontsch_count_pos | 1910 | 442 | 1 / 0 |
| round2_fontsch_count_pvert | 1910 | 442 | 1 / 0 |
| round2_fontsch_count_rhwp | 1910 | 442 | 1 / 0 |
| round2_fontsch_count_rom | 1910 | 442 | 1 / 0 |
| round2_fontsch_count_uv0 | 1910 | 464 | 1 / 0 |
| round2_fontsch_count_uv1 | 1910 | 464 | 1 / 0 |
| round2_fontsch_count_uv2 | 1910 | 464 | 1 / 0 |
| round2_fontsch_count_uv3 | 1922 | 383 | 1 / 0 |
| round2_fontsch_count_uv4 | 1910 | 461 | 1 / 0 |
| round2_fontsch_sdk_body | 1910 | 442 | 1 / 0 |
| round2_fontsch_sdk_body_inc | 1910 | 442 | 1 / 0 |
| round2_fontsch_sdk_body_pos | 1910 | 442 | 1 / 0 |
| round2_fontsch_sdk_body_pvert | 1910 | 442 | 1 / 0 |
| round2_fontsch_sdk_body_rhwp | 1910 | 442 | 1 / 0 |
| round2_fontsch_sdk_body_rom | 1910 | 442 | 1 / 0 |
| round2_fontsch_sdk_body_uv0 | 1910 | 464 | 1 / 0 |
| round2_fontsch_sdk_body_uv1 | 1910 | 464 | 1 / 0 |
| round2_fontsch_sdk_body_uv2 | 1910 | 464 | 1 / 0 |
| round2_fontsch_sdk_body_uv3 | 1922 | 383 | 1 / 0 |
| round2_fontsch_sdk_body_uv4 | 1910 | 461 | 1 / 0 |
