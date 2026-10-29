import unittest

from tools.update_readme_progress import (
    replace_landed_sentence,
    replace_snapshot_row,
)


class LandedSentenceTests(unittest.TestCase):
    def test_updates_count_denominator_and_byte_share(self):
        original = (
            "Landed byte-matching C++ covers **89.76%** of the 49,568-function catalog by count\n"
            "and **37.58%** of retail function bytes: the functions still to do are the large ones."
        )
        self.assertEqual(
            replace_landed_sentence(original, "90.01%", 49570, "38.00%"),
            (
                "Landed byte-matching C++ covers **90.01%** of the 49,570-function catalog by count\n"
                "and **38.00%** of retail function bytes: the functions still to do are the large ones."
            ),
        )

    def test_rejects_missing_sentence(self):
        with self.assertRaisesRegex(RuntimeError, "landed coverage sentence not found"):
            replace_landed_sentence("no sentence here", "1.00%", 1, "1.00%")


class SnapshotRowTests(unittest.TestCase):
    def test_updates_two_column_snapshot_row(self):
        original = (
            "| Snapshot | Current result |\n"
            "|---|---:|\n"
            "| Landed byte-matching C++ (genuine) | "
            "**44,494 · 89.76%** |\n"
        )
        self.assertEqual(
            replace_snapshot_row(
                original,
                "Landed byte-matching C++ (genuine)",
                "**44,500 · 89.77%**",
            ),
            (
                "| Snapshot | Current result |\n"
                "|---|---:|\n"
                "| Landed byte-matching C++ (genuine) | "
                "**44,500 · 89.77%** |\n"
            ),
        )

    def test_rejects_missing_snapshot_row(self):
        with self.assertRaisesRegex(
                RuntimeError, "snapshot row not found exactly once"):
            replace_snapshot_row(
                "| Snapshot | Current result |\n",
                "Retail function bytes reproduced",
                "**2,900 · 5.85%**",
            )


if __name__ == "__main__":
    unittest.main()
