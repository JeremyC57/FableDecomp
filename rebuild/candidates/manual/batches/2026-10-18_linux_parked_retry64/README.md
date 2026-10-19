# Shadow pass completion and texture display

00b4d2c0 (162 bytes): actual shadow/camera/render-state/texture headers and native box type. Audited global addresses and render helper identities; technique/layer guard restores back screen and transforms, clears capture state before restore, then reads the optional draw flag. Exact-source fixture covers all technique/layer/display combinations, call order, capture state, complete box/colour/texture arguments, and restore callback enabling display. Observed new3 grades plus UNCERTAIN historical reservation4 (reserved7/10).
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.
