# NF237 overlay headers

None. Every change in this hand-back is an in-place respell inside a row's own HEAD local model (access, const, reference parameters,
member names) or an explicit call in the row body; no file under rebuild/include is overlaid, so `orig/` is empty, no FABLE_STATIC_ASSERT
moved (pin_asserts not needed) and no includer's preprocessed text changes (equivalence check not needed: the include trees are HEAD's).
