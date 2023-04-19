/**
 * Name: Rene Ravanan andrewid: rravanan
 *
 * @file trans.c
 * @brief Contains various implementations of matrix transpose
 *
 * Each transpose function must have a prototype of the form:
 *   void trans(size_t M, size_t N, double A[N][M], double B[M][N],
 *              double tmp[TMPCOUNT]);
 *
 * All transpose functions take the following arguments:
 *
 *   @param[in]     M    Width of A, height of B
 *   @param[in]     N    Height of A, width of B
 *   @param[in]     A    Source matrix
 *   @param[out]    B    Destination matrix
 *   @param[in,out] tmp  Array that can store temporary double values
 *
 * A transpose function is evaluated by counting the number of hits and misses,
 * using the cache parameters and score computations described in the writeup.
 *
 * Programming restrictions:
 *   - No out-of-bounds references are allowed
 *   - No alterations may be made to the source array A
 *   - Data in tmp can be read or written
 *   - This file cannot contain any local or global doubles or arrays of doubles
 *   - You may not use unions, casting, global variables, or
 *     other tricks to hide array data in other forms of local or global memory.
 *
 * TODO: fill in your name and Andrew ID below.
 * @author Your Name <andrewid@andrew.cmu.edu>
 */

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "cachelab.h"

/**
 * @brief Checks if B is the transpose of A.
 *
 * You can call this function inside of an assertion, if you'd like to verify
 * the correctness of a transpose function.
 *
 * @param[in]     M    Width of A, height of B
 * @param[in]     N    Height of A, width of B
 * @param[in]     A    Source matrix
 * @param[out]    B    Destination matrix
 *
 * @return True if B is the transpose of A, and false otherwise.
 */
#ifndef NDEBUG
static bool is_transpose(size_t M, size_t N, double A[N][M], double B[M][N]) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                fprintf(stderr,
                        "Transpose incorrect.  Fails for B[%zd][%zd] = %.3f, "
                        "A[%zd][%zd] = %.3f\n",
                        j, i, B[j][i], i, j, A[i][j]);
                return false;
            }
        }
    }
    return true;
}
#endif

/*
 * You can define additional transpose functions here. We've defined
 * some simple ones below to help you get started, which you should
 * feel free to modify or delete.
 */

/**
 * @brief A simple baseline transpose function, not optimized for the cache.
 *
 * Note the use of asserts (defined in assert.h) that add checking code.
 * These asserts are disabled when measuring cycle counts (i.e. when running
 * the ./test-trans) to avoid affecting performance.
 */
static void trans_basic(size_t M, size_t N, double A[N][M], double B[M][N],
                        double tmp[TMPCOUNT]) {
    assert(M > 0);
    assert(N > 0);

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            B[j][i] = A[i][j];
        }
    }

    assert(is_transpose(M, N, A, B));
}

/**
 * @brief A contrived example to illustrate the use of the temporary array.
 *
 * This function uses the first four elements of tmp as a 2x2 array with
 * row-major ordering.
 */
static void trans_tmp(size_t M, size_t N, double A[N][M], double B[M][N],
                      double tmp[TMPCOUNT]) {
    assert(M > 0);
    assert(N > 0);

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            size_t di = i % 2;
            size_t dj = j % 2;
            tmp[2 * di + dj] = A[i][j];
            B[j][i] = tmp[2 * di + dj];
        }
    }

    assert(is_transpose(M, N, A, B));
}

/**
 * @brief The solution transpose function that will be graded.
 *
 * You can call other transpose functions from here as you please.
 * It's OK to choose different functions based on array size, but
 * this function must be correct for all values of M and N.
 */
static void transpose_submit(size_t M, size_t N, double A[N][M], double B[M][N],
                             double tmp[TMPCOUNT]) {

    if ((M == 32 && N == 32) || (M == 1024 && N == 1024)) {
        size_t block_size = 8;

        for (size_t i = 0; i < N; i += block_size) {

            for (size_t j = 0; j < M; j += block_size) {

                for (size_t row = i; row < i + block_size; row++) {

                    for (size_t col = j; col < j + block_size; col++) {

                        if (row != col)
                            B[col][row] = A[row][col];
                    }
                    if (i == j)
                        B[row][row] = A[row][row];
                    // Double misses happen at points when row == col, so to
                    // minimize misses, perform these separately, tried this
                    // outside the loop, but that takes more cycles
                }
            }
        }
        // for(size_t i = 0; i < M; i ++) B[i][i] = A[i][i];
    }
    // highscore: 124 million but somehow this works on autolab
    // else if (M == 1024 && N == 1024){
    //     size_t block_size = 8;
    //     // size_t block_size = 8;
    //     for (size_t i = 0; i < N; i += block_size){
    //         for (size_t j = 0; j < M; j += block_size){
    //             for(size_t row = i; row < i + block_size; row++){
    //                 for (size_t col = j; col < j + block_size; col++){
    //                     B[col][row] = A[row][col];
    //                 }
    //             }
    //         }
    //     }
    // }

    else if (M == N)
        trans_basic(M, N, A, B, tmp);
    else
        trans_tmp(M, N, A, B, tmp);
}

void transpose_1024(size_t M, size_t N, double A[N][M], double B[M][N],
                    double tmp[TMPCOUNT]) {

    // size_t block_size = 8;
    for (size_t i = 0; i < 1024; i += 8) {
        for (size_t j = 0; j < 1023; j += 8) {
            for (size_t row = i; row < i + 8; row++) {
                for (size_t col = j; col < j + 8; col++) {
                    B[col][row] = A[row][col];
                }
            }
        }
    } // I was being dumb with running the 1024 test and forgot to use -l
    // for (size_t i = 0; i < N; i += block_size){
    //     for (size_t j = 0; j < M; j += block_size){
    //         double a = A[i][j];
    //         double b = A[i+1][j];
    //         double c = A[i+2][j];
    //         double d = A[i][j+1];
    //         double e = A[i+1][j+1];
    //         double f = A[i+2][j+1];
    //         double g = A[i][j+2];
    //         double h = A[i+1][j+2];
    //         double k = A[i+2][j+2];
    //         double l = A[i][j+3];
    //         double m = A[i+1][j+3];
    //         double n = A[i+2][j+3];
    //         B[j][i] = a;
    //         B[j][i+1] = b;
    //         B[j][i+2] = c;
    //         double o = A[i+3][j];
    //         double p = A[i+3][j+1];
    //         double q = A[i+3][j+2];
    //         double r = A[i+3][j+3];
    //         B[j][i+3] = o;
    //         B[j+1][i] = d;
    //         B[j+1][i+1] = e;
    //         B[j+1][i+2] = f;
    //         B[j+1][i+3] = p;
    //         B[j+2][i] = g;
    //         B[j+2][i+1] = h;
    //         B[j+2][i+2] = k;
    //         B[j+2][i+3] = q;
    //         B[j+3][i] = l;
    //         B[j+3][i+1] = m;
    //         B[j+3][i+2] = n;
    //         B[j+3][i+3] = r;

    //     }
    // }

    // for (size_t i = 0; i < N; i += block_size){
    //     for(size_t j = 0; j < M; j += 8*block_size){
    //         for (size_t row = i; row < i + block_size; row ++){
    //             for (size_t col = j; col < j + 8*block_size; col ++){
    //                 tmp[(row-i)*block_size + col-j] = A[row][col];
    //             }
    //         }
    //         for (size_t col = j; col < j + 8*block_size; col ++){
    //             for (size_t row = i; i < row + block_size; row++){
    //                 B[col][row] = tmp[(row-i)*block_size + col-j];
    //             }
    //         }
    //     }
    // }
    // for (size_t i = 0; i < N; i += block_size){
    //     for (size_t j = 0; j < M; j += block_size){

    //         for(size_t row = i; row < i + block_size; row ++){
    //             for (size_t col = j; col < j + block_size; col++){
    //                 // printf("check this index %zu", (col-j)*(block_size) +
    //                 row - i);

    //                 // if (row == col) tmp[64+row-i] = A[row][col];
    //                 // else
    //                 if (row != col) tmp[(row - i)*(block_size) + col-j] =
    //                 A[row][col];
    //             }
    //             if (i == j) B[row][row] = A[row][row];

    //         }
    //         for(size_t col = j; col < j + block_size; col ++){
    //             for (size_t row = i; row < i + block_size; row++){

    //                 if (row != col) B[col][row] = tmp[(row-i)*(block_size) +
    //                 col-j];
    //             }

    //         }
    //         // if (i == j) {
    //         //     for (size_t row = i; row < i + block_size; row ++){
    //         //         B[row][row] = tmp[64 + row - i];
    //         //     }
    //         // }
    //     }
    // }
    // for (size_t i = 0; i < M; i ++) B[i][i] = A[i][i];

    // size_t block_size = 8;
    // for (size_t i = 0; i < N; i += block_size){
    //     for (size_t j = 0; j < M; j += 8*block_size){

    //         for(size_t jj = j; jj < j + 8*block_size; jj += block_size){

    //             for(size_t row = i; row < i + block_size; row ++){
    //                 for (size_t col = jj; col < jj + block_size; col++){
    //                     // printf("check this index %zu",
    //                     (col-j)*(block_size) + row - i);
    //                     // if (row != col)
    //                     tmp[(col - jj)*(block_size) + row-i] = A[row][col];
    //                 }
    //                 // if (i == j) B[row][row] = A[row][row];

    //             }
    //         }
    //         for(size_t jj = j; jj < j + 8*block_size; jj += block_size){
    //             for(size_t col = jj; col < jj + block_size; col ++){
    //                 for (size_t row = i; row < i + block_size; row++){
    //                     // if (row != col)
    //                     B[col][row] = tmp[(col - jj)*(block_size) + row-i];
    //                 }
    //             }
    //         }

    //     }
    // }
}
/**
 * @brief Registers all transpose functions with the driver.
 *
 * At runtime, the driver will evaluate each function registered here, and
 * and summarize the performance of each. This is a handy way to experiment
 * with different transpose strategies.
 */
void registerFunctions(void) {
    // Register the solution function. Do not modify this line!
    registerTransFunction(transpose_submit, SUBMIT_DESCRIPTION);

    registerTransFunction(transpose_1024, "try");
    // Register any additional transpose functions
    registerTransFunction(trans_basic, "Basic transpose");
    registerTransFunction(trans_tmp, "Transpose using the temporary array");
}
