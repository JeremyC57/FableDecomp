lock () 
{ 
    for a in "$@";
    do
        o=$(cat $S/w22/nflocks/$a/owner 2> /dev/null);
        if [ -z "$o" ] || [ "$o" = released ] || [ "$o" = NF433 ]; then
            mkdir -p $S/w22/nflocks/$a;
            echo NF433 > $S/w22/nflocks/$a/owner;
            echo "$a locked";
        else
            echo "$a SKIP owner=$o";
        fi;
    done
}
