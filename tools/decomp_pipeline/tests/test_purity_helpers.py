"""crawl/purity.py: rows that run retail inline asm through a shared helper header stay `retail_asm`."""
import sys
import tempfile
import unittest
from pathlib import Path

HERE = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(HERE))
from crawl import purity  # noqa: E402

HELPER = """// Retail-asm-helper: GFFloatToLongNear
// Retail-asm: 0x005BC41A `fld dword ptr [ebp+0Ch]` + 0x005BC41D `fistp dword ptr [ebp+8]`
#ifndef H
#define H
#ifdef FABLE_PORTABLE
inline long GFFloatToLongNear(float v) { return (long)v; }
#else
__forceinline long __fastcall GFFloatToLongNear(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
#endif
#endif
"""
UNDOCUMENTED_HELPER = HELPER.replace(HELPER.splitlines()[1], "// Retail-asm: the engine's rounding helper")
ROW = """// Retail-asm: 0x0052F46A `fld [esp+14h] ; fistp dword ptr [esp]` (x4)
#include "engine/GFFloatToLongNear.h"
long f(float x) { return GFFloatToLongNear(x * 255.0f); }
"""


class HelperHeaderGrades(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.inc = Path(self.tmp.name)
        (self.inc / "engine").mkdir()
        (self.inc / "engine" / "GFFloatToLongNear.h").write_text(HELPER)
        (self.inc / "engine" / "Wrapper.h").write_text('#include "GFFloatToLongNear.h"\n')
        (self.inc / "engine" / "Bad.h").write_text(UNDOCUMENTED_HELPER)
        purity._header_cache.clear()

    def tearDown(self):
        self.tmp.cleanup()
        purity._header_cache.clear()

    def grade(self, text):
        return purity.classify(text, include_dirs=[self.inc])

    def test_row_calling_helper_is_retail_asm(self):
        self.assertEqual("retail_asm", self.grade(ROW))
        self.assertTrue(purity.is_genuine_kind(self.grade(ROW)))

    def test_address_taken_emission_counts_as_a_use(self):
        row = ROW.replace("long f(float x) { return GFFloatToLongNear(x * 255.0f); }",
                          "long (__fastcall *g_keep)(float) = &GFFloatToLongNear;")
        self.assertEqual("retail_asm", self.grade(row))

    def test_helper_reached_through_another_header(self):
        self.assertEqual("retail_asm", self.grade(ROW.replace("engine/GFFloatToLongNear.h", "engine/Wrapper.h")))

    def test_row_without_its_own_evidence_is_naked_asm(self):
        row = "\n".join(l for l in ROW.splitlines() if "Retail-asm:" not in l)
        self.assertEqual("naked_asm", self.grade(row))

    def test_evidence_must_name_an_instruction_of_the_helper(self):
        self.assertEqual("naked_asm", self.grade(ROW.replace("`fld [esp+14h] ; fistp dword ptr [esp]`", "the rounding")))

    def test_helper_header_failing_the_evidence_rule_is_naked_asm(self):
        self.assertEqual("naked_asm", self.grade(ROW.replace("engine/GFFloatToLongNear.h", "engine/Bad.h")))

    def test_including_without_calling_stays_genuine(self):
        row = ROW.replace("return GFFloatToLongNear(x * 255.0f);", "return (long)x;")
        self.assertEqual("genuine", self.grade(row))

    def test_commented_include_is_ignored(self):
        row = ROW.replace('#include "engine/GFFloatToLongNear.h"', '// #include "engine/GFFloatToLongNear.h"')
        self.assertEqual("genuine", self.grade(row))

    def test_plain_row_and_asm_row_unchanged(self):
        self.assertEqual("genuine", self.grade("int f() { return 1; }\n"))
        own = "// Retail-asm: 0x00497894 `ror dword ptr [esp],0Dh`\nlong f(unsigned long v) { __asm ror v, 13\n return v; }\n"
        self.assertEqual("retail_asm", self.grade(own))

    def test_helper_header_itself_classifies_retail_asm(self):
        self.assertEqual("retail_asm", purity.classify(HELPER))

    def test_repo_helper_headers_pass_the_evidence_rule(self):
        repo = HERE.parents[1] / "rebuild" / "include"
        found = 0
        for h in sorted(repo.rglob("*.h")):
            text = h.read_text(encoding="utf-8", errors="ignore")
            if purity.RETAIL_ASM_HELPER_TAG in text:
                found += 1
                self.assertEqual("retail_asm", purity.classify(text), h.name)
        self.assertGreater(found, 0)


if __name__ == "__main__":
    unittest.main()
