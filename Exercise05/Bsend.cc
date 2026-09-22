#include <mpi.h>
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int NUM_SENDS = 3;
    int buffer_size = NUM_SENDS * (sizeof(int) + MPI_BSEND_OVERHEAD);
    char* buffer = new char[buffer_size];

    if (rank == 0) {
        MPI_Buffer_attach(buffer, buffer_size);
    }

    int number;
    for (int i = 0; i < 3; i++) {
        if (rank == 0) {
            number = i * 10;
            MPI_Bsend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            std::cout << "Process 0 buffered-sent " << number << "\n";
        } else if (rank == 1) {
            MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Process 1 received " << number << "\n";
        }
    }

    if (rank == 0) {
        void* detached_buffer;
        int detached_size;
        MPI_Buffer_detach(&detached_buffer, &detached_size);
    }

    delete[] buffer;

    MPI_Finalize();
    return 0;
}
