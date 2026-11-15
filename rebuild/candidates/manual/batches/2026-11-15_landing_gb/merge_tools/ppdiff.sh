#!/bin/bash
S=${S:?set S to the lane scratch root}
i=0; for t in "$1" "$2"; do i=$((i+1)); cpp -P -nostdinc -undef -D_MSC_VER=1310 -D_M_IX86=600 -D_WIN32 -x c++ -I $t -I $S/stubinc_equiv "$3" 2>&1 | tr -s ' \t\n' '\n' > $S/c22/pp_$i.txt; done
diff $S/c22/pp_1.txt $S/c22/pp_2.txt
