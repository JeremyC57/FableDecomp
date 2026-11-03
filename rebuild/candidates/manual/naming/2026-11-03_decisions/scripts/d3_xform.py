# d3_xform.py <files...>: D3 (owner 2026-11-03) mechanical respell of the GameCODA EAX class / type / GUID spellings.
# Sources: class + struct-tag renames, mangled-name substrings (Leaf lines, comments), C++ GUID externs -> SDK names
# (extern "C" for the EAX 4.0 DEFINE_GUID constants). Tests (*_test.cpp): class renames only. Idempotent.
import re, sys
WORD = [("CEaxSourceNode", "CEAXBufferList"), ("CEaxContext", "CEAXListener"), ("CEaxSource", "CEAXBuffer"),
        ("CEAXContext", "CEAXListener"), ("CPKObject", "CCodaObjectBase"), ("SEaxEvent", "_CHANGES")]
SRC_ONLY_WORD = [("SGuid", "_GUID")]
MANGLED = [("CEaxSourceNode@", "CEAXBufferList@"), ("CEaxContext@", "CEAXListener@"), ("CEaxSource@", "CEAXBuffer@"),
           ("CEAXContext@", "CEAXListener@"), ("CPKObject@", "CCodaObjectBase@"), ("USEaxEvent@", "U_CHANGES@"),
           ("USGuid@", "U_GUID@")]
GUIDS = {"EAX_NULL_GUID": "EAX_NULL_GUID", "g_NullGuid_012b9438": "EAX_NULL_GUID",
         "EAX_REVERB_EFFECT": "EAX_REVERB_EFFECT", "g_EAXReverbEffectGuid": "EAX_REVERB_EFFECT",
         "g_ReverbEffectGuid": "EAX_REVERB_EFFECT", "g_EffectGuid_012b9458": "EAX_PrimaryFXSlotID",
         "g_EffectGuid_012b9468": "EAXPROPERTYID_EAX40_FXSlot0", "g_EffectGuid_012b9478": "EAXPROPERTYID_EAX40_FXSlot1",
         "g_EffectGuid_012b9488": "EAXPROPERTYID_EAX40_FXSlot2", "g_EffectGuid_012b9498": "EAXPROPERTYID_EAX40_FXSlot3"}
for i in range(4): GUIDS["EAXPROPERTYID_EAX40_FXSlot%d" % i] = "EAXPROPERTYID_EAX40_FXSlot%d" % i
for p in sys.argv[1:]:
    t0 = t = open(p, encoding="utf-8", errors="surrogateescape").read()
    test = p.endswith("_test.cpp")
    for a, b in MANGLED: t = t.replace(a, b)
    for a, b in WORD + ([] if test else SRC_ONLY_WORD): t = re.sub(r"\b%s\b" % a, b, t)
    if not test:
        # C++ `extern const _GUID X;` -> extern "C" SDK name; then every use of the old name.
        def ext(m):
            return 'extern "C" const %s %s;' % (m.group(1), GUIDS[m.group(2)])
        t = re.sub(r'(?<!"C" )extern const (_GUID|GUID) (%s)\s*;' % "|".join(GUIDS), ext, t)
        for a, b in GUIDS.items():
            if a != b: t = re.sub(r"\b%s\b" % a, b, t)
    if t != t0:
        open(p, "w", encoding="utf-8", errors="surrogateescape").write(t); print("changed", p)
