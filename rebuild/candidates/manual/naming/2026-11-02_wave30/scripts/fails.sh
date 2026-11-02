#!/bin/bash
# fails.sh <recheck log>: rows whose recheck is not MATCH/RELOCATION_MATCH + PASS (row result)
grep -E "^[0-9a-f]{8} +[A-Z_]+" $1 | grep -v " WARN " | grep -vE "MATCH(\[[^]]*\])? +PASS" | awk '{print $1, $2, $3}'
