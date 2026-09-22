#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long N = 10000000;

    double start_time = MPI_Wtime();

    long long chunk = N / size;
    long long remainder = N % size;

    long long my_start = rank * chunk + 1 + (rank < remainder ? rank : remainder);
    long long my_count  = chunk + (rank < remainder ? 1 : 0);
    long long my_end    = my_start + my_count - 1;

    long long local_sum = 0;
    for (long long i = my_start; i <= my_end; i++) {
        local_sum += i;
    }

    long long total_sum = 0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    std::cout << "Rank " << rank << " summed [" << my_start << ", " << my_end
              << "] = " << local_sum << "\n";

    if (rank == 0) {
        std::cout << "----\n";
        std::cout << "Total sum (1.." << N << ") = " << total_sum << "\n";
        std::cout << "Expected (N*(N+1)/2)       = " << (N * (N + 1)) / 2 << "\n";
        std::cout << "Processes used: " << size << "\n";
        std::cout << "Elapsed time: " << (end_time - start_time) << " seconds\n";
    }

    MPI_Finalize();
    return 0;
}
