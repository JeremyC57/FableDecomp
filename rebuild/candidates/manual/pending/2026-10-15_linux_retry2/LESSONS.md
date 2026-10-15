# Retry batch 2 lessons
- STLport `vector::operator[]` must be modelled as `*(begin() + n)` (begin() an inline accessor), not `first[n]`: under the size pragma the former keeps the scaled index in its own register and re-reads the element per use (00581cfd, 00581fa4, 00581d8e).
