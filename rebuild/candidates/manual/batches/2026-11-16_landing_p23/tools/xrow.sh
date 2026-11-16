# xrow.sh <addr> <targets-regex>: grade the landed row with q.py, then print its relocations whose retail target matches
a=$1; f=$(ls rebuild/src/compiled/*/*/*_$a.cpp | head -1)
fl=$(grep -m1 '^// Flags:' $f | sed 's#^// Flags:##'); lf=$(grep -m1 '^// Leaf:' $f | awk '{print $3}')
g=$(python3 $S/tools/q.py $a $f "$lf" ${fl:+--flags $fl} 2>&1 | tail -1)
echo "$a | $g"
python3 $S/fu3/xref.py $a $S/wine/q/$a/$a.obj "$lf" 2>&1 | grep -E "$2" | sed 's/^/    /'
