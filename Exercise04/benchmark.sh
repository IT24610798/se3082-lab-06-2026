set -e

PROC_COUNTS="1 2 4 "   # adjust to however many cores your machine/cluster has
OUT=timings.csv

echo "program,processes,time_seconds" > "$OUT"

for prog in sum pi_montecarlo; do
    for p in $PROC_COUNTS; do
        echo "Running $prog with $p process(es)..."
        # grep the "Elapsed time" line the programs print and pull out the number
        line=$(mpirun -np "$p" ./"$prog" | grep "Elapsed time")
        t=$(echo "$line" | grep -oE "[0-9]+\.[0-9]+")
        echo "$prog,$p,$t" >> "$OUT"
    done
done

echo "Done. Results saved to $OUT"
