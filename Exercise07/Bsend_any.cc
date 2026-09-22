#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long TOTAL_POINTS = 10000000;
    double start_time = MPI_Wtime();

    long long base = TOTAL_POINTS / size;
    long long extra = TOTAL_POINTS % size;
    long long my_points = base + (rank < extra ? 1 : 0);

    unsigned int seed = static_cast<unsigned int>(time(NULL)) ^ (rank * 7919 + 1);
    srand(seed);

    long long local_count_inside = 0;
    for (long long i = 0; i < my_points; i++) {
        double x = static_cast<double>(rand()) / RAND_MAX;
        double y = static_cast<double>(rand()) / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            local_count_inside++;
        }
    }

    int buffer_size = sizeof(long long) + MPI_BSEND_OVERHEAD;
    char* buffer = new char[buffer_size];

    if (rank != 0) {
        MPI_Buffer_attach(buffer, buffer_size);
        MPI_Bsend(&local_count_inside, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);

        void* detached_buffer;
        int detached_size;
        MPI_Buffer_detach(&detached_buffer, &detached_size);
    } else {
        long long total_count_inside = local_count_inside;
        for (int received = 0; received < size - 1; received++) {
            long long worker_count;
            MPI_Status status;
            MPI_Recv(&worker_count, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 0,
                     MPI_COMM_WORLD, &status);
            std::cout << "Rank 0 received from rank " << status.MPI_SOURCE
                      << ": " << worker_count << "\n";
            total_count_inside += worker_count;
        }

        double end_time = MPI_Wtime();
        double pi_estimate = 4.0 * static_cast<double>(total_count_inside)
                                  / static_cast<double>(TOTAL_POINTS);

        std::cout << "----\n";
        std::cout << "Processes used : " << size << "\n";
        std::cout << "Estimated Pi   : " << pi_estimate << "\n";
        std::cout << "Elapsed time   : " << (end_time - start_time) << " seconds\n";
    }

    delete[] buffer;

    MPI_Finalize();
    return 0;
}
